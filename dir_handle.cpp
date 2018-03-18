#include "dir_handle.h"
#include <QDir>
#include<QVector>
#include <stdio.h>
#include "Qdebug"
#include <QTextCodec>

QString Chinese_processing(QString text)
{
    return text;
}

bool FindFile(const QString &path,QVector<QString> &File_Array,QVector<QString> &Path_Array)//遍历文件and目录
{
    QDir dir(path);
    if (!dir.exists())
    return false;
    dir.setFilter(QDir::Dirs|QDir::Files);
    dir.setSorting(QDir::DirsFirst);
    QFileInfoList list = dir.entryInfoList();
    int i=0;
    do
    {
        QFileInfo fileInfo = list.at(i);
        if(fileInfo.fileName()=="."|fileInfo.fileName()=="..")//筛除本目录以及父目录
        {
            i++;
            continue;
        }
        bool bisDir=fileInfo.isDir();
        if(bisDir)//判断当前是不是目录
        {
            Path_Array.append(fileInfo.path()+"/"+fileInfo.fileName());//添加目录到QVector
            FindFile(fileInfo.filePath(),File_Array,Path_Array);
        }
        else
        {

            File_Array.append(fileInfo.path()+"/"+fileInfo.fileName());//添加文件目录到QVector
        }
        i++;
    }
    while(i<list.size());
    return true;
}
