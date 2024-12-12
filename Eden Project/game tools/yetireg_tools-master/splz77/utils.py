#!/usr/bin/env python

import os, errno
import sys
import subprocess

def is_executable(path):
    return os.path.isfile(path) and os.access(path, os.X_OK)

def makedir(dirname):
    try:
        os.makedirs(dirname)
    except OSError as exc: # Python >2.5
        if exc.errno == errno.EEXIST and os.path.isdir(dirname):
            pass
        else: raise

def custom_popen(cmd):
    # needed for py2exe
    creationflags = 0x08000000 if sys.platform == 'win32' else 0 # CREATE_NO_WINDOW
    return subprocess.Popen(cmd, bufsize=0, stdout=subprocess.PIPE,
                            stdin=subprocess.PIPE, stderr=subprocess.STDOUT,
                            creationflags=creationflags)


