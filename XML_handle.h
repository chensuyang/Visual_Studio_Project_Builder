#ifndef XML_HANDLE_H
#define XML_HANDLE_H

#ifdef WIN32
#pragma execution_character_set("utf-8")
#endif

#include <QString>
#include <QVector>
#include <QtXml>


bool Create_VS_filters_File(QString File,QDomDocument vs_filters_file_XML);//创建VS的filters文件
bool Create_VS_Vcxproj_File(QString File,QDomDocument vs_vcxproj_file_XML);//创建VS的vcxproj文件
bool Create_VCxproj_Filters_XML(QDomDocument* vs_filters_file_XML,QString RootPath,QVector<QString> &File_Array,QVector<QString> &Path_Array,bool EliminateRootPathFile);//创建XML文件
bool Create_VCxproj_XML(QDomDocument* vs_vcxproj_file_XML,QString Project_Name,QVector<QString> Define_Array,QVector<QString> Include_Dir_Array,QVector<QString> H_File_Array,QVector<QString> C_File_Array);//创建XML文件

#endif // XML_HANDLE_H
