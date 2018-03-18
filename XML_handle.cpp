#include "XML_handle.h"
#include <QDebug>
#include <QTextCodec>

QTextCodec *utf8 = QTextCodec::codecForName("system");

bool Create_VS_filters_File(QString File,QDomDocument vs_filters_file_XML)//创建VS的filters文件
{
    QFile file(File);
    if (!file.open(QIODevice::Truncate | QIODevice::ReadWrite))//要求重写and读写权限打开文件
    {
        return false;//打开文件失败
    }

    if(!file.write(vs_filters_file_XML.toByteArray()))//将XML转换为QString并写入文件
    {
        return false;//写入文件失败
    }
    return true;
}


bool Create_XML(QDomDocument vs_filters_file_XML,QString RootPath,QVector<QString> &File_Array,QVector<QString> &Path_Array,bool EliminateRootPathFile)//创建XML文件
{
    vs_filters_file_XML.appendChild(vs_filters_file_XML.createProcessingInstruction("xml","version=\"1.0\" encoding=\"UTF-8\""));//创建文件头

    QDomElement project = vs_filters_file_XML.createElement("project");//创建project根节点

    project.setAttribute("ToolsVersion","4.0");//为project节点添加信息
    project.setAttribute("xmlns","http://schemas.microsoft.com/developer/msbuild/2003");//为project节点添加信息

    QDomElement ItemGroup_1 = vs_filters_file_XML.createElement("ItemGroup");//创建ItemGroup节点(第一个)
    QDomElement ItemGroup_2 = vs_filters_file_XML.createElement("ItemGroup");//创建ItemGroup节点(第二个)
    QVector<QString>::iterator Path_iter;

    for (Path_iter=Path_Array.begin();Path_iter!=Path_Array.end();Path_iter++)
    {
        QDomElement Filter=vs_filters_file_XML.createElement("Filter");//创建Filter节点
        Filter.setAttribute("Include", QString(*Path_iter).mid(RootPath.length()+1));//Filter节点添加信息(绝对路径去除工程根目录后得到相对路径)
        ItemGroup_1.appendChild(Filter);//添加Filter节点到ItemGroup_1
    }

    QVector<QString>::iterator File_iter;
    for (File_iter=File_Array.begin();File_iter!=File_Array.end();File_iter++)
    {
        QFileInfo fileinfo;
        fileinfo=*File_iter;//将文件绝对地址放入QFileInfo以提取路径
        if(!EliminateRootPathFile || fileinfo.absolutePath().mid(RootPath.length()+1).length()!=0)//如果不需要排除project根目录文件或者当前文件不是根目录下的文件
        {
            QDomElement ClCompile=vs_filters_file_XML.createElement("ClCompile");//创建ClCompile节点
            ClCompile.setAttribute("Include", QString(*File_iter).mid(RootPath.length()+1));//ClCompile节点添加信息

            QDomElement ClCompile_Filter=vs_filters_file_XML.createElement("Filter");//创建ClCompile节点(绝对路径去除工程根目录后得到相对路径)

            QDomText ClCompile_Filter_FilePath = vs_filters_file_XML.createTextNode(fileinfo.absolutePath().mid(RootPath.length()+1));//为ClCompile节点下的Filter节点添加信息(文件的路径,不含文件名)
            ClCompile_Filter.appendChild(ClCompile_Filter_FilePath);

            ClCompile.appendChild(ClCompile_Filter);//添加Filter节点到ClCompile

            ItemGroup_2.appendChild(ClCompile);//添加ClCompile节点到ItemGroup_2
        }

    }


    project.appendChild(ItemGroup_1);//添加ItemGroup节点到project
    project.appendChild(ItemGroup_2);//添加ItemGroup节点到project

    vs_filters_file_XML.appendChild(project);//添加project节点到XML
    return true;
}
