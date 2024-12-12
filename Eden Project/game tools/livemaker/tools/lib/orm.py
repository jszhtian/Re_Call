# coding=utf-8
import sqlite3
import logging

__author__ = 'TinFoil'


class DBManager(object):
    def __init__(self, filename=None, db='lines.db', timeout=20):
        # use the connection if handed one
        if isinstance(db, sqlite3.Connection):
            self.connection = db
        else:
            self._init_db(db, timeout)

        self.filename = filename
        self.lazy_statements = []
        self.speakers = self._get_speakers()
        self._speaker_cache = {}
        self._linked_cache = self._get_speakers()

    def _init_db(self, db, timeout):
        self.connection = sqlite3.connect(db, timeout=timeout)
        logging.debug(u'Creating/connecting to speakers and lines table')
        with self.connection as con:
            con.execute(u'''CREATE TABLE IF NOT EXISTS speakers(id INTEGER, related_id INTEGER,
                                                               lang TEXT, name TEXT UNIQUE)''')
            con.execute(u'''CREATE TABLE IF NOT EXISTS lines(location INTEGER NOT NULL,
                                                            filename TEXT NOT NULL, script_file TEXT,
                                                            jp_speaker INTEGER, en_speaker INTEGER,
                                                            jp TEXT NOT NULL, en TEXT,
                                                            FOREIGN KEY(jp_speaker) REFERENCES speakers(id),
                                                            FOREIGN KEY(en_speaker) REFERENCES speakers(id),
                                                            PRIMARY KEY(location, jp)
                                                            )''')

    def add_speaker(self, lang, name):
        logging.debug(u'Adding speaker with language {} and name {}'.format(lang, name))
        with self.connection as con:
            con.execute(u'INSERT OR IGNORE INTO speakers(lang, name) VALUES (?,?)', (lang, name))
        return

    def get_speaker(self, lang, name):
        logging.debug(u'Querying for speaker {} and name {}'.format(lang, name))
        c = self.connection.cursor()
        c.execute(u'SELECT id, related_id, lang, name from speakers where lang = ? and name = ?', (lang, name))
        result = c.fetchone()
        logging.debug(u'Got result: {}'.format(result))
        self._speaker_cache[(lang, name)] = result
        c.close()
        return result

    def link_speakers(self, id1, id2):
        # try not to hit the database
        try:
            return self._linked_cache[(id1, id2)] and self._linked_cache[(id2, id1)]
        except KeyError:
            pass
        logging.debug(u'Linking speaker IDs {} and {}'.format(id1, id2))
        with self.connection as con:
            con.execute(u'UPDATE OR FAIL speakers SET related_id = ? where id = ?', (id1, id2))
            con.execute(u'UPDATE OR FAIL speakers SET related_id = ? where id = ?', (id2, id1))

        # cache links
        self._linked_cache[(id1, id2)] = True
        self._linked_cache[(id2, id1)] = True

    def get_or_add_speaker(self, lang, name):
        try:
            return self._speaker_cache[(lang, name)]
        except KeyError:
            logging.debug(u'Missed speaker key cache.')
        self.add_speaker(lang, name)
        return self.get_speaker(lang, name)

    def commit(self):
        """Executes statements in bulk with one transaction."""
        c = self.connection.cursor()
        c.execute(u'begin')
        logging.debug(u'Executing {} lazy statements'.format(len(self.lazy_statements)))
        for statement, params in self.lazy_statements:
            c.execute(statement, params)

        self.connection.commit()
        self.lazy_statements = []

    def add_jp_line(self, location, fn, line, speaker_id=None, commit=True):
        logging.debug(u'Adding JP line with loc, fn, line, speaker_id: {}'.format(location, fn,
                                                                                  line, speaker_id))
        if speaker_id is not None:
            statement = (u'INSERT OR IGNORE INTO lines(location, filename, jp_speaker, jp) VALUES (?,?,?,?)',
                         (location, fn, speaker_id, line)
                         )
        else:
            statement = (u'INSERT OR IGNORE INTO lines(location, filename, jp) VALUES (?,?,?)',
                         (location, fn, line)
                         )

        if commit:
            logging.debug(u'Executing previous statement')
            c = self.connection.cursor()
            c.execute(statement[0], statement[1])
            self.connection.commit()
        else:
            logging.debug(u'Putting off previous statement')
            self.lazy_statements.append(statement)

    def translate_line(self, jp, en, jp_id=None, en_id=None, location=None, commit=True):
        logging.debug(u'Updating EN line with location, jp, en: {}'.format((location, jp, en)))
        statement = 'UPDATE OR FAIL lines SET en = ?'

        # update english speaker
        if en_id is not None:
            statement += ', en_speaker = ?'

        if self.filename is not None:
            statement += ', script_file = ?'

        # japanese line
        statement += ' WHERE jp = ?'

        # japanese speaker
        if jp_id is not None:
            statement += 'and jp_speaker = ?'

        # location
        if location:
            statement += ' AND location = ?'

        values = tuple(x for x in (en, en_id, self.filename, jp, jp_id, location) if x is not None)
        logging.debug(u'Query template and variables: {} | {}'.format(statement, values))
        if commit:
            logging.debug(u'Executing previous statement')
            c = self.connection.cursor()
            c.execute(statement, values)
            if c.rowcount == 0:
                raise ValueError(u'Line could not be updated')
            self.connection.commit()
        else:
            logging.debug(u'Putting off previous statement')
            self.lazy_statements.append((statement, values))

    def get_line(self, jp=None, en=None, location=None):
        if jp is None and en is None and location is None:
            raise TypeError(u'No arguments specified.')

        statement = 'SELECT location, filename, jp_speaker, en_speaker, jp, en FROM lines WHERE'
        need_and = False
        if jp is not None:
            statement += ' jp like ?'
            need_and = True
            jp = '%' + jp + '%'

        if en is not None:
            if need_and:
                statement += ' and'
            statement += ' en like ?'
            en = '%' + en + '%'

        if location is not None:
            if need_and:
                statement += ' and'
            statement += ' location = ?'

        with self.connection as con:
            results = con.execute(statement, tuple(x for x in (jp, en, location) if x is not None))

        return list(row for row in results)

    def _get_speakers(self):
        speaker_ids = {}
        c = self.connection.cursor()
        c.execute(u'select name,id from speakers')

        for row in speaker_ids:
            speaker_ids[row[0]] = row[1]

        return speaker_ids

