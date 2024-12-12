#include "procfile.h"

#if _MSC_VER >= 1600
#pragma execution_character_set("utf-8")
#endif
char cs2magic[]="CatScene";

wchar_t* c2w(const char *str,int cp)
{
	if (cp==-1)
	{
		return (wchar_t*)*str;
	}
	wchar_t* buffer;
	if (str)
	{
		size_t nu = strlen(str);
		size_t n = (size_t)MultiByteToWideChar(cp, 0, (const char *)str, int(nu), NULL, 0);
		buffer = 0;
		buffer = new wchar_t[n+1];
		memset(buffer, 0, sizeof(wchar_t)*(n + 1));
		MultiByteToWideChar(cp, 0, (const char *)str, int(nu), buffer, int(n));
	}
	return buffer;
}

QStringList *CSTOpen(QString file,QString cp)
{
	int codepage = CP_ACP;
	if (cp=="Shift-JIS")
	{
		codepage = 932;
	}
	if (cp == "GB18030-0")
	{
		codepage = 936;
	}
	if (cp == "UTF-8")
	{
		codepage = CP_UTF8;
	}
	if (cp=="UTF-16")
	{
		codepage = -1;
	}

    QStringList* result=new QStringList;
    QFile cstfile(file);
    if(!cstfile.open(QIODevice::ReadOnly))
    {
        QMessageBox::critical(NULL, "IO Error", "打开文件失败！");
        return nullptr;
    }
    qint64 filesize=cstfile.size();
    auto buffer=new char[filesize];
    cstfile.read(buffer,filesize);
    cstfile.close();
    CS2Head *head;
    head=(CS2Head*)buffer;
    if (memcmp(head->Magic,cs2magic,8)!=0)
    {
        QMessageBox::critical(NULL, "Error", "文件不匹配!");
        return nullptr;
    }
    if(filesize!=0x10+head->SZcompress)
    {
        QMessageBox::critical(NULL, "Error", "文件损坏!");
        return nullptr;
    }
    auto uncompressbuffer=new char[head->SZuncompress+100];
	memset(uncompressbuffer, 0, head->SZuncompress + 100);
    uLongf uncompsize= head->SZuncompress + 100;
    int zstatus = uncompress((unsigned char*)uncompressbuffer, &uncompsize, (const Bytef*)buffer + 0x10, head->SZcompress);
    if (Z_OK!= zstatus)
    {
        QMessageBox::critical(NULL, "Error", "Zlib展开失败!");
		QMessageBox::critical(NULL, "Error Code", QString::number(zstatus));
        return nullptr;
    }
    if (uncompsize!=head->SZuncompress)
	{
		QMessageBox::critical(NULL, "Error", "Zlib展开错误!");
		return nullptr;
	}
	CS2InfoHead* info;
	info = (CS2InfoHead*)uncompressbuffer;
    if (uncompsize- sizeof(CS2InfoHead) !=info->totalsize)
	{
		QMessageBox::critical(NULL, "Error", "文件损坏!");
		return nullptr;
	}
	char *opcodeptr;
	opcodeptr = uncompressbuffer + sizeof(CS2InfoHead) + info->opcodeoffset;
	qint32 entrys = (info->opcodeoffset - info->index1Size) / 4;
	qint32 *index = (qint32*)(uncompressbuffer + sizeof(CS2InfoHead) + info->index1Size);
	for (int i = 0; i < entrys; i++)
	{
		qint32 offset=index[i];
		
		if (opcodeptr[offset]==0x1 && opcodeptr[offset+1]==0x20)
		{

			/*
            qint32 j = offset;
			for (j; j < info->totalsize - info->opcodeoffset; j++)
			{
			if (opcodeptr[j] == 0)
			{
			break;
			}
			}
			qint32 comlength = j - offset + 1;
            */
            if (opcodeptr[offset+2]!=0x0)
            {
                wchar_t* tmpwc = c2w((const char*)&opcodeptr[offset + 2],codepage);
                QString TmpString = QString::fromWCharArray(tmpwc);
				delete tmpwc;
				tmpwc = nullptr;
                result->append(TmpString);
            }

		}
		if (opcodeptr[offset] == 0x1 && opcodeptr[offset + 1] == 0x21)
		{

			/*
            qint32 j = offset;
			for (j; j < info->totalsize - info->opcodeoffset; j++)
			{
			if (opcodeptr[j] == 0)
			{
			break;
			}
			}
			qint32 comlength = j - offset + 1;
			*/
            if (opcodeptr[offset+2]!=0x0)
            {
                wchar_t* tmpwc = c2w((const char*)&opcodeptr[offset + 2],codepage);
                QString TmpString = QString::fromWCharArray(tmpwc);
				delete tmpwc;
				tmpwc = nullptr;
                result->append(TmpString);
            }
		}

        if (opcodeptr[offset] == 0x1 && opcodeptr[offset + 1] == 0x30)
        {
            if (opcodeptr[offset+2]==0x30||opcodeptr[offset+2]==0x31||opcodeptr[offset+2]==0x32||opcodeptr[offset+2]==0x33||opcodeptr[offset+2]==0x34||opcodeptr[offset+2]==0x35||opcodeptr[offset+2]==0x36||opcodeptr[offset+2]==0x37||opcodeptr[offset+2]==0x38||opcodeptr[offset+2]==0x39)
            {
                wchar_t* tmpwc = c2w((const char*)&opcodeptr[offset + 2],codepage);
                QString TmpString = QString::fromWCharArray(tmpwc);
                delete tmpwc;
                tmpwc = nullptr;
                result->append(TmpString);
            }

        }
		

	}
    delete buffer;
    delete uncompressbuffer;
    return result;
}


bool CSTSave(QString file,QStringList* list)
{
	QFile cstfile(file);
	QString outputfile = file + ".new";
	if (!cstfile.open(QIODevice::ReadOnly))
	{
		QMessageBox::critical(NULL, "IO Error", "打开文件失败！");
		return false;
	}
	qint64 filesize = cstfile.size();
	auto buffer = new char[filesize];
	cstfile.read(buffer, filesize);
	cstfile.close();
	CS2Head *head;
	head = (CS2Head*)buffer;
	if (memcmp(head->Magic, cs2magic, 8) != 0)
	{
		QMessageBox::critical(NULL, "Error", "文件不匹配!");
		return false;
	}
	if (filesize != 0x10 + head->SZcompress)
	{
		QMessageBox::critical(NULL, "Error", "文件损坏!");
		return false;
	}
	auto uncompressbuffer = new char[head->SZuncompress+100];
	memset(uncompressbuffer, 0, head->SZuncompress + 100);
	uLongf sz= head->SZuncompress + 100;
	int zstatus = uncompress((unsigned char*)uncompressbuffer, &sz, (const Bytef*)buffer + 0x10, head->SZcompress);
	if (Z_OK != zstatus)
	{
		QMessageBox::critical(NULL, "Error", "Zlib展开失败!");
		QMessageBox::critical(NULL, "Error Code", QString::number(zstatus));
		return false;
	}
	if (sz != head->SZuncompress)
	{
		QMessageBox::critical(NULL, "Error", "Zlib展开错误!");
		return false;
	}
	CS2InfoHead* info;
	info = (CS2InfoHead*)uncompressbuffer;
	if (sz - sizeof(CS2InfoHead) != info->totalsize)
	{
		QMessageBox::critical(NULL, "Error", "文件损坏!");
		return false;
	}
	char *opcodeptr;
	opcodeptr = uncompressbuffer + sizeof(CS2InfoHead) + info->opcodeoffset;
	qint32 entrys = (info->opcodeoffset - info->index1Size) / 4;
	qint32 *index = (qint32*)(uncompressbuffer + sizeof(CS2InfoHead) + info->index1Size);
	auto index1buf = new char[info->index1Size];
	memcpy(index1buf, uncompressbuffer + sizeof(CS2InfoHead), info->index1Size);
	auto index2buf = new qint32[entrys];
	auto opbuf = new char[655350];
	qint32 bytetotransfer = 0;
	qint32 order = 0;

	for (int i = 0; i < entrys; i++)
	{
		qint32 offset = index[i];

		if (opcodeptr[offset] == 0x1 && opcodeptr[offset + 1] == 0x20)
		{
			index2buf[i] = bytetotransfer;
			if (opcodeptr[offset + 2] != 0x0)
			{
				if (order>=list->size())
				{
					QMessageBox::critical(NULL, "Error", "超出范围!");
					return false;
				}
				QByteArray tmp = list->at(order).toLocal8Bit();
				char* tmpc = tmp.data();
				qint32 strlen = list->at(order).toLocal8Bit().size();
				memcpy(opbuf + bytetotransfer, &opcodeptr[offset], 2);
				memcpy(opbuf + bytetotransfer+2, tmpc, strlen);
				memset(opbuf + bytetotransfer + 2 + strlen, 0, 1);
				bytetotransfer += strlen + 3;
				order += 1;
			}
			else
			{
				quint32 j = offset;
				for (j; j < info->totalsize - info->opcodeoffset; j++)
				{
					if (opcodeptr[j] == 0)
					{
						break;
					}
				}
				qint32 comlength = j - offset + 1;
				memcpy(opbuf + bytetotransfer, &opcodeptr[offset], comlength);
				bytetotransfer += comlength;
			}
			continue;

		}
		if (opcodeptr[offset] == 0x1 && opcodeptr[offset + 1] == 0x21)
		{
			index2buf[i] = bytetotransfer;
			if (opcodeptr[offset + 2] != 0x0)
			{
				if (order >= list->size())
				{
					QMessageBox::critical(NULL, "Error", "超出范围!");
					return false;
				}
				QByteArray tmp = list->at(order).toLocal8Bit();
				char* tmpc = tmp.data();
				qint32 strlen = list->at(order).toLocal8Bit().size();
				memcpy(opbuf + bytetotransfer, &opcodeptr[offset], 2);
				memcpy(opbuf + bytetotransfer + 2, tmpc, strlen);
				memset(opbuf + bytetotransfer + 2 + strlen, 0, 1);
				bytetotransfer += strlen + 3;
				order += 1;
			}
			else
			{
				quint32 j = offset;
				for (j; j < info->totalsize - info->opcodeoffset; j++)
				{
					if (opcodeptr[j] == 0)
					{
						break;
					}
				}
				qint32 comlength = j - offset + 1;
				memcpy(opbuf + bytetotransfer, &opcodeptr[offset], comlength);
				bytetotransfer += comlength;
			}
			continue;

		}
        if (opcodeptr[offset] == 0x1 && opcodeptr[offset + 1] == 0x30)
        {
            index2buf[i] = bytetotransfer;
            if (opcodeptr[offset+2]==0x30||opcodeptr[offset+2]==0x31||opcodeptr[offset+2]==0x32||opcodeptr[offset+2]==0x33||opcodeptr[offset+2]==0x34||opcodeptr[offset+2]==0x35||opcodeptr[offset+2]==0x36||opcodeptr[offset+2]==0x37||opcodeptr[offset+2]==0x38||opcodeptr[offset+2]==0x39)
            {
                if (order>=list->size())
                {
                    QMessageBox::critical(NULL, "Error", "超出范围!");
                    return false;
                }
                QByteArray tmp = list->at(order).toLocal8Bit();
                char* tmpc = tmp.data();
                qint32 strlen = list->at(order).toLocal8Bit().size();
                memcpy(opbuf + bytetotransfer, &opcodeptr[offset], 2);
                memcpy(opbuf + bytetotransfer+2, tmpc, strlen);
                memset(opbuf + bytetotransfer + 2 + strlen, 0, 1);
                bytetotransfer += strlen + 3;
                order += 1;
            }
            else
            {
                quint32 j = offset;
                for (j; j < info->totalsize - info->opcodeoffset; j++)
                {
                    if (opcodeptr[j] == 0)
                    {
                        break;
                    }
                }
                qint32 comlength = j - offset + 1;
                memcpy(opbuf + bytetotransfer, &opcodeptr[offset], comlength);
                bytetotransfer += comlength;
            }
            continue;

        }
		index2buf[i] = bytetotransfer;
		quint32 j = offset;
		for (j; j < info->totalsize - info->opcodeoffset; j++)
		{
			if (opcodeptr[j] == 0)
			{
				break;
			}
		}
		qint32 comlength = j - offset + 1;
		memcpy(opbuf + bytetotransfer, &opcodeptr[offset], comlength);
		bytetotransfer += comlength;

	}
	CS2InfoHead newinfo;
	newinfo.index1Size = info->index1Size;
	newinfo.UnknownInt32 = info->UnknownInt32;
	newinfo.opcodeoffset = info->opcodeoffset;
	qint32 tot = info->opcodeoffset + bytetotransfer;
	newinfo.totalsize = tot;
	char* outbuf = new char[tot+sizeof(CS2InfoHead)];
	memcpy(outbuf, &newinfo, sizeof(CS2InfoHead));
	memcpy(outbuf + sizeof(CS2InfoHead), index1buf, newinfo.index1Size);
	memcpy(outbuf + sizeof(CS2InfoHead) + newinfo.index1Size, index2buf, entrys*sizeof(qint32));
	memcpy(outbuf + sizeof(CS2InfoHead) + newinfo.opcodeoffset, opbuf, bytetotransfer);
	CS2Head newhead;
	memcpy(&newhead.Magic, cs2magic, 8);
	newhead.SZuncompress = tot + sizeof(CS2InfoHead);
	char* compbuf = new char[tot + sizeof(CS2InfoHead)];
	uLong compsize;
	int zstatus2 = compress((unsigned char*)compbuf, &compsize, (const Bytef*)outbuf, tot + sizeof(CS2InfoHead));
	if (zstatus2!=Z_OK)
	{
		QMessageBox::critical(NULL, "Error", "Zlib压缩失败!");
		QMessageBox::critical(NULL, "Error Code", QString::number(zstatus2));
		return false;
	}
	newhead.SZcompress = compsize;
	QFile outfile(outputfile);
	if (!outfile.open(QIODevice::WriteOnly))
	{
		QMessageBox::critical(NULL, "IO Error", "打开文件失败！");
		return false;
	}
	outfile.write((char*)&newhead, sizeof(CS2Head));
	outfile.write(compbuf, newhead.SZcompress);
    return true;
}


void ExportUTF8(QStringList *RAW, QStringList *NEW, QString FilePath)
{
    QFile utf8file(FilePath);
    if(!utf8file.open(QIODevice::WriteOnly|QIODevice::Text))
    {
        QMessageBox::information(NULL, "IO操作", "无法写入！");
    }
    QTextStream textStream(&utf8file);
    textStream.setEncoding(QStringEncoder::Utf8);
    int count=RAW->size();
    for(int i=0;i<count;++i)\
    {
        textStream<<"@"<<QString::number(i);
        textStream<<Qt::endl;
        textStream<<RAW->at(i);
        textStream<<Qt::endl;
        textStream<<NEW->at(i);
        textStream<<Qt::endl;
        textStream<<Qt::endl;
    }
    utf8file.close();
}

QStringList *utf8textproc(QString file)
{
    QStringList* result=new QStringList;
       QFile utf8file(file);
       if(!utf8file.open(QIODevice::ReadOnly|QIODevice::Text))
       {
           QMessageBox::information(NULL, "打开", "读取文件失败！");
           return NULL;
       }
       QTextStream textStream(&utf8file);
       textStream.setEncoding(QStringDecoder::Utf8);
       while(!textStream.atEnd())
       {
           QString line=textStream.readLine();
           result->append(line);
       }
       utf8file.close();
       return result;
}
