#ifndef XML_HANDLE_H
#define XML_HANDLE_H

#ifdef WIN32
#pragma execution_character_set("utf-8")
#endif

#include <QString>
#include <QVector>
#include <QtXml>


bool Create_VS_filters_File(QString File,QDomDocument vs_filters_file_XML);//创建VS的filters文件
bool Create_XML(QDomDocument vs_filters_file_XML,QVector<QString> &Path_Array,QVector<QString> &File_Array);//创建XML文件


#endif // XML_HANDLE_H
