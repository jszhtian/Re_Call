#pragma once
#ifndef PROCFILE_H
#define PROCFILE_H
#include<qstring.h>
#include<qstringlist.h>
#include<QFile>
#include"typedef.h"
#include<qmessagebox.h>
#include<QTextCodec>
#include <QTextStream>
#include "zlib.h"
#include "zconf.h"
#include <Windows.h>

QStringList *CSTOpen(QString file,QString cp);
bool CSTSave(QString file, QStringList* list);
void ExportUTF8(QStringList *RAW, QStringList *NEW, QString FilePath);
QStringList *utf8textproc(QString file);
#endif // PROCFILE_H
