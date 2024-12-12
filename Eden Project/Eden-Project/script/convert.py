import os, sys, re

path1 = input('Input:\n\t')
path2 = input('Output:\n\t')
FileAfter=input('FileAfterName:\n\t')
O_decode = input('Decode:\n\t')
O_encode = input('Encode:\n\t')
a=O_decode
if len(path2) == 0 or len(path1) == 0 or len(O_decode) == 0 or len(O_encode) == 0:
    print('Has Error.')
    sys.exit(-1)
for f in os.listdir(path1):
    if f.find(FileAfter)!=-1:
        print('File:' + f)
        fs1 = open(os.path.join(path1, f.replace(FileAfter, FileAfter)), 'rb')
        try:
            ls = fs1.read().decode(O_decode).split('\r\n')
        except UnicodeDecodeError:
            print('UnicodeDecodeError.Set decode')
            O_decode=input('')
            ls = fs1.read().decode(O_decode).split('\r\n')
            O_decode=a
        fs2 = open(os.path.join(path2, f.replace(FileAfter, FileAfter)), 'wb')
        fs2.write('\n'.join(ls).encode(O_encode))
        fs1.close()
        fs2.close()
