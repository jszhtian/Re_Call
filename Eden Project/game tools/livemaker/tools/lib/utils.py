# -*- coding: utf-8 -*-
from __future__ import print_function
from codecs import open
import os


def sanitize_html(html):
    html = html.replace('<tr><tr height=20"></tr>', '</tr>')
    html = html.replace('<tr height=100"></tr>', '</tr>')
    html = html.replace('</td><tr ', '</td></tr><tr ')
    return html


def sanitize_translations():
    """Sanitizes the HTML of imported scripts"""
    os.chdir('tls\\text')
    for fn in os.listdir('.'):
        print('Converting', fn)
        page = open(fn, 'r', encoding='utf8').read()
        valid = sanitize_html(page)
        open(fn, 'w', encoding='utf8').write(valid)
    os.chdir('..\\..')


def sanitize_jp(text):
    """I'm going to use this. I can feel it in my bones."""
    text = text.strip()
    return text