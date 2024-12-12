#!/usr/bin/env python
'''
OWASP ZSC
https://www.owasp.org/index.php/OWASP_ZSC_Tool_Project
https://github.com/zscproject/OWASP-ZSC
http://api.z3r0d4y.com/
https://groups.google.com/d/forum/owasp-zsc [ owasp-zsc[at]googlegroups[dot]com ]
'''
import binascii
import random
import string
from core.compatible import version
_version = version()


def encode(f):
    val_name = ''.join(
        random.choice(string.ascii_lowercase + string.ascii_uppercase)
        for i in range(50))
    data = ''
    eval = '$' + val_name
    if _version is 2:
        data = '$' + val_name + ' = "' + str(binascii.b2a_base64(f)).replace(
            '\n', '') + '";\n'

    if _version is 3:
        data = '$' + val_name + ' = "' + str(binascii.b2a_base64(f.encode(
            'latin-1')).decode('latin-1').replace('\n', '')) + '";\n'

    var_str = '$' + ''.join(
        random.choice(string.ascii_lowercase + string.ascii_uppercase)
        for i in range(50))
    var_data = '$' + ''.join(
        random.choice(string.ascii_lowercase + string.ascii_uppercase)
        for i in range(50))
    func_name = ''.join(
        random.choice(string.ascii_lowercase + string.ascii_uppercase)
        for i in range(50))
    func_argv = '$' + ''.join(
        random.choice(string.ascii_lowercase + string.ascii_uppercase)
        for i in range(50))
    f = '''
use MIME::Base64 qw(decode_base64);
%s
sub %s {
    %s = shift;
    %s = decode_base64(%s);
    return %s;
}
%s = %s;
eval %s(%s);
''' % (data, func_name, func_argv, var_str, func_argv, var_str, var_data, eval,
       func_name, var_data)
    return f


def start(content,cli):
    return str(str('=begin\n') + str(content.replace(
        '=begin', '#=begin').replace('=cut', '#=cut')) + str('\n=cut') + str(
            encode(content)) + str('\n'))
