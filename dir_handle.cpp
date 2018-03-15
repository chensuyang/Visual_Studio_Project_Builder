#include "dir_handle.h"
#include <QDir>
#include<QVector>
#include <stdio.h>
#include "Qdebug"


bool FindFile(const QString &path,QVector<QString> &FilePath_Array,bool OnlyDir)//遍历文件or目录
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
            if(OnlyDir)//如果当前只需要目录
            {
                FilePath_Array.append(fileInfo.path()+"/"+fileInfo.fileName());//添加目录到QVector
            }
            FindFile(fileInfo.filePath(),FilePath_Array,OnlyDir);
        }
        else
        {
            if(!OnlyDir)//如果当前只需要文件
            {
                FilePath_Array.append(fileInfo.path()+"/"+fileInfo.fileName());//添加文件目录到QVector
            }
        }
        i++;
    }
    while(i<list.size());
    return true;
}
