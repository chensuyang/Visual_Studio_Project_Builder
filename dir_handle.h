#ifndef DIR_HANDLE_H
#define DIR_HANDLE_H

#ifdef WIN32
#pragma execution_character_set("utf-8")
#endif

#include <QVector>
#include <QString>
bool FindFile(const QString & path,QVector<QString> &FilePath_Array,bool OnlyDir);//遍历文件or目录


#endif // DIR_HANDLE_H
