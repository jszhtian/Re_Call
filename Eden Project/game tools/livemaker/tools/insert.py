#! /usr/bin/env python
# -*- coding: utf-8 -*-

import cProfile
import logging
import multiprocessing
import os
import re
import sqlite3
from codecs import open

from bs4 import BeautifulSoup as BSoup
from lib.orm import DBManager
from lib.utils import sanitize_html, sanitize_jp


replacement_speakers = {
    u'フィフティファイヴ': u'課長',
    u'ハレン': u'ハロン',
    u'女性スタッフA': u'女子スタッフ１',
    u'女性スタッフB': u'女子スタッフ２',
    u'MP': u'ＭＰ',
    u'ランチパイロット': u'軍人ランチ',
}

# speakers that don't exist in-game but do in the translation files
ignored_speakers = {
    u'スコット少尉': True,
}


def parse_spoken_jp(line):
    match = re.search(ur'(?P<speaker>.+?)\t?(?P<text>[「『].+?[」』])', line)
    try:
        speaker = match.group('speaker').strip()
        speaker = replacement_speakers.get(speaker, speaker)
        text = match.group('text').strip()
    except (IndexError, AttributeError):
        print line
        raise IndexError(u'Could not parse spoken Japanese line')
    return speaker, text


def parse_spoken_en(line):
    match = re.search(ur'(?P<speaker>[\w\s\?]+)[:\s]+(?P<text>.+)', line)
    try:
        speaker = match.group('speaker').strip()
        text = match.group('text').strip()
    except (IndexError, AttributeError):
        print line
        raise IndexError('Could not parse spoken English line')
    return speaker, text


# basically we get all of the lines and hash them
# and then we get all of the translations and hash them
# and then we slam them together into one file
def insert_spoken(en_speaker, en_text, jp_speaker, jp_text, orm):
    # get and/or prompt for the speaker translation
    try:
        jp_id = orm.speakers[jp_speaker]
        en_id = orm.speakers[en_speaker]
    except KeyError:
        # speaker doesn't exist
        try:
            jp_id, en_id, i, j = orm.get_speaker('jp', u'【名前_{0}】'.format(jp_speaker))
        except TypeError:
            byte_string = raw_input(u'Speaker {} not found. Please input its equivalent: '.format(jp_speaker))
            old_jp_speaker = jp_speaker
            try:
                jp_speaker = byte_string.decode('utf8')
            except UnicodeDecodeError:
                jp_speaker = byte_string.decode('cp932')

                # ignore speaker from now on
                if jp_speaker == 'ignore':
                    print('Ignoring speaker.')
                    ignored_speakers[old_jp_speaker] = True
                    orm.translate_line(jp_text, en_text, commit=False)

            print 'Adding replacement.'
            replacement_speakers[old_jp_speaker] = jp_speaker
            jp_id, en_id, i, j = orm.get_speaker('jp', u'【名前_{0}】'.format(jp_speaker))

    # english speaker does not exist
    try:
        en_speaker = orm.get_speaker('en', en_id)[3]
    except TypeError:
        # add and link speakers
        en_id = orm.get_or_add_speaker('en', en_speaker)[0]
        orm.link_speakers(en_id, jp_id)

    # add translation
    orm.translate_line(jp_text, en_text, jp_id=jp_id, en_id=en_id, commit=False)


def is_line(row):
    # skips over header anchor
    if len(row.find_all('td')) != 2:
        return False

    # is a previous/next line
    # this works because we already found the choices, which have anchors too
    if row.find('a') is not None:
        return False

    # passed scrutiny
    return True


def insert_from_html(html_file, filename):
    soup = BSoup(html_file)
    orm = DBManager(filename)

    # get the giant table
    tbl = soup.find('table')

    # iterate over HTML rows
    for row in tbl.findChildren('tr'):

        # commit choice selections
        choices = row.find_all('ul')
        if choices:
            jp_choices = [c.text for c in choices[0].find_all('li')]
            en_choices = [c.text for c in choices[1].find_all('li')]

            # try to commit
            for jp, en in zip(jp_choices, en_choices):
                orm.translate_line(sanitize_jp(jp), en, commit=False)

        # get rows and guess validity
        if not is_line(row):
            continue
        tds = row.find_all('td')

        # we should be safe now.
        jp_text = sanitize_jp(tds[0].text).strip()
        en_text = tds[1].text.strip()

        # is a spoken line
        # and isn't a line with just quotes
        if u'「' in jp_text and '"' in en_text and len(jp_text.split(u'「')[0]) > 0:
            # get and hash speakers
            jp_speaker, jp_text = parse_spoken_jp(jp_text)
            en_speaker, en_text = parse_spoken_en(en_text)

            # ignore or proceed
            if jp_speaker in ignored_speakers:
                logging.debug(u'Ignoring speaker {}'.format(jp_speaker))
                orm.translate_line(jp_text, en_text, commit=False)
            else:
                insert_spoken(en_speaker, en_text, jp_speaker, jp_text, orm)

        # is not a spoken line
        else:
            orm.translate_line(jp_text, en_text, commit=False)

    orm.commit()
    return filename


def search_for_translation(jp, script):
    # proper parsing is way expensive in comparison to the time saved with this
    if not jp in script:
        return None

    # make a soup of lines
    soup = BSoup(script)
    tbl = soup.find('table')

    # look inside table rows
    for row in tbl.findChildren('tr'):
        tds = row.find_all('td')

        # look in each first cell (the japanese)
        if len(tds) != 2:
            continue
        if tds and jp in tds[0].text:
            en_text = tds[1].text
            # throw the speaker away
            try:
                en_text = parse_spoken_en(en_text)[1]
            except AttributeError:
                pass
            return en_text


def insert_orphan(conn, current_file, en, filename, jp):
    statement = u'UPDATE lines SET en = ?, script_file = ? WHERE filename = ? and jp = ?'
    values = (en, current_file, filename, jp)
    c = conn.cursor()
    c.execute(statement, values)
    conn.commit()
    c.close()


def repatriate_orphans():
    """All-file orphan JP line search"""
    all_scripts = []
    for fn in os.listdir('..\\tls\\text'):
        contents = open('..\\tls\\text\\' + fn, encoding='utf8').read()
        all_scripts.append((fn, contents))

    # get all blank lines
    conn = sqlite3.connect('lines.db')
    c = conn.cursor()
    c.execute('select filename, jp from lines where en is null')
    rows = c.fetchall()
    c.close()

    for filename, jp in rows:
        print u'Looking for {}'.format(jp)
        found = False
        # do a full search in all scripts
        for current_file, contents in all_scripts:
            if jp in contents:
                en = search_for_translation(jp, sanitize_html(contents))
                if en is not None:
                    insert_orphan(conn, current_file, en, filename, jp)
                    print u'Found in file {}!'.format(current_file)
                    found = True
                    break

        if not found:
            print u'Line from {} does not exist'.format(filename)


def adopt_orphans(desperate=False):
    """Same-file orphan JP line search"""
    conn = sqlite3.connect('lines.db')

    current_file = 'dongs'

    # get literally every line
    c = conn.cursor()
    c.execute('select filename, jp, script_file from lines')
    rows = c.fetchall()
    c.close()

    # iterate through all lines
    for filename, jp, script_file in rows:

        # scripts with no insertions
        if script_file is None:
            if current_file == 'dongs':
                continue
            # open script file
            print u'Searching for {}'.format(jp)
            script = open(os.path.join('..', 'tls', 'text', current_file), encoding='utf8').read()
            script = sanitize_html(script)
            en = search_for_translation(jp, script)

            # update line w/ parameters
            if en is not None:
                # insert!
                print u'Inserting: {}'.format(en)
                insert_orphan(conn, current_file, en, filename, jp)

            # failed to get a replacement line
            else:
                print u'No match in file {}'.format(current_file)

                # if we're desperate for a match.
                if desperate:
                    prefix, num = re.search(r'(\w+)([\d]+)', current_file).groups()

                    # try next file
                    for i in [1, -2, 101]:
                        num = int(num) + i
                        fn = '{}{}.html'.format(prefix, num)
                        try:
                            script = open(os.path.join('..', 'tls', 'text', fn), encoding='utf8').read()
                            script = sanitize_html(script)
                            en = search_for_translation(jp, script)

                            if en is not None:
                                print u'Paid off - inserting: {}'.format(en)
                                insert_orphan(conn, current_file, en, filename, jp)
                                continue
                        except IOError:
                            pass

                        print 'No match in file {}'.format(fn)

        # moved on to a new file
        elif current_file != script_file:
            line_cache = None
            current_file = script_file


def main(synchronous=False):
    logging.basicConfig(filename='insert.log', level=logging.WARNING)

    if synchronous:
        logging.debug('Launching {} child processes'.format(multiprocessing.cpu_count()))
        pool = multiprocessing.Pool(multiprocessing.cpu_count())
        results = []

    # iterate over translation files
    for fn in os.listdir('..\\tls\\text'):

        logging.debug(u'Inserting from {}'.format(fn))
        path = os.path.join('..\\', 'tls', 'text', fn)

        # open and clean
        page = open(path, encoding='utf8').read()
        page = sanitize_html(page)
        if synchronous:
            result = pool.apply_async(insert_from_html, (page, fn))
            results.append(result)
        else:
            insert_from_html(page, fn)

    if synchronous:
        pool.close()
        for result in results:
            print u'Finished {}'.format(result.get())
        pool.join()

if __name__ == '__main__':
    # cProfile.run('main(True)', 'insert.profile')
    repatriate_orphans()