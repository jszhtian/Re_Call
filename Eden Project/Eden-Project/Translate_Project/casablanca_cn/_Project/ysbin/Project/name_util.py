file = '人名对照.txt'
fs = open(file,"rb")
ls = fs.read().decode('u8').split('\r\n')
fs.close()

for l in ls:
    l=l[l.find('亂'):]
    jp=l[:l.find('乽')]
    cn=l[l.find("【"):l.find("「")]
    #string = 'else if(!strcmp(Name, "%s"))\n\tstrcpy(Name, "%s");' %\
    #         (jp,cn)
    string = 'AymReplaceText(Name, "%s", "%s");' % (jp,cn)
    print(string)
