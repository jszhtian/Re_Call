import sys

chunk=[
    (b'\x07\x00\x00\x74',b'\x07\x00\x00\xEB'),
    (b'\x02\x3B\xCB\x75\x59',b'\x02\x3B\xCB\xEB\x53'),
    (b'\xFE\x18\x0F\x84',b'\xFE\x18\x0F\x83'),
    (b'\xF6\x01\x00\x00\x0F\x84',b'\xF6\x01\x00\x00\x0F\x83')
    ]

def Crack():
    #exe=sys.argv[1]
    exe = 'pieces_cn.exe'
    fs=open(exe,'rb')
    stm=fs.read()
    fs.close()
    IsSuccess = 0
    for i in range(len(chunk)):
        IsSuccess = 0
        if stm.find(chunk[i][0]):
            stm=stm.replace(chunk[i][0],chunk[i][1])
            IsSuccess = 1
        else:
            continue
    fs=open(exe+'.exe','wb')
    fs.write(stm)
    fs.close()
    if (IsSuccess==1):
        print('success')
    else:
        print('error')
    sa=input()


print('Whirlpool_Crack Ver0.9\nMade by AyamiKaze.\n2020.02.21\n\n')
Crack()
