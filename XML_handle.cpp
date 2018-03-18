#include "XML_handle.h"
#include <QDebug>
#include <QTextCodec>

QTextCodec *utf8 = QTextCodec::codecForName("system");

bool Create_VS_filters_File(QString File,QDomDocument vs_filters_file_XML)//创建VS的filters文件
{
    QFile file(File);
    if (!file.open(QIODevice::ReadWrite))//要求读写权限打开文件
    {
        return false;//打开文件失败
    }

    if(!file.write(vs_filters_file_XML.toByteArray()))//将XML转换为QString并写入文件
    {
        return false;//写入文件失败
    }
    return true;
}


bool Create_XML(QDomDocument vs_filters_file_XML,QVector<QString> &Path_Array,QVector<QString> &File_Array)//创建XML文件
{


    vs_filters_file_XML.appendChild(vs_filters_file_XML.createProcessingInstruction("xml","version=\"1.0\" encoding=\"UTF-8\""));//创建文件头

    QDomElement project = vs_filters_file_XML.createElement("project");//创建project根节点

    project.setAttribute("ToolsVersion","4.0");//为project节点添加信息
    project.setAttribute("xmlns","http://schemas.microsoft.com/developer/msbuild/2003");//为project节点添加信息

    QDomElement ItemGroup_1 = vs_filters_file_XML.createElement("ItemGroup");//创建ItemGroup节点(第一个)
    QDomElement ItemGroup_2 = vs_filters_file_XML.createElement("ItemGroup");//创建ItemGroup节点(第二个)
    QVector<QString>::iterator iter;
    for (iter=Path_Array.begin();iter!=Path_Array.end();iter++)
    {
        QDomElement Filter=vs_filters_file_XML.createElement("Filter");//创建Filter节点
        Filter.setAttribute("Include", *iter);//Filter节点添加信息
        ItemGroup_1.appendChild(Filter);//添加Filter节点到ItemGroup_1
    }


    project.appendChild(ItemGroup_1);//添加ItemGroup节点到project
    project.appendChild(ItemGroup_2);//添加ItemGroup节点到project

    vs_filters_file_XML.appendChild(project);//添加project节点到XML
    return true;
}
