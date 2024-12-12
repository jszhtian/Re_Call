#ifndef DEF_H
#define DEF_H
#include <QtGlobal>

struct CS2Head
{
    char Magic[8];
    quint32 SZcompress;
    quint32 SZuncompress;
};

struct CS2InfoHead
{
    quint32 totalsize;
    quint32 UnknownInt32;
    quint32 index1Size;
    quint32 opcodeoffset;
};
#endif // DEF_H
