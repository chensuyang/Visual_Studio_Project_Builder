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


bool Create_VS_Vcxproj_File(QString File,QDomDocument vs_vcxproj_file_XML)//创建VS的vcxproj文件
{
    QFile file(File);
    if (!file.open(QIODevice::Truncate | QIODevice::ReadWrite))//要求重写and读写权限打开文件
    {
        return false;//打开文件失败
    }

    if(!file.write(vs_vcxproj_file_XML.toByteArray()))//将XML转换为QString并写入文件
    {
        return false;//写入文件失败
    }
    return true;
}


bool Create_VCxproj_Filters_XML(QDomDocument* vs_filters_file_XML,QString RootPath,QVector<QString> &File_Array,QVector<QString> &Path_Array,bool EliminateRootPathFile)//创建XML文件
{
    vs_filters_file_XML->appendChild(vs_filters_file_XML->createProcessingInstruction("xml","version=\"1.0\" encoding=\"UTF-8\""));//创建文件头

    QDomElement project = vs_filters_file_XML->createElement("project");//创建project根节点

    project.setAttribute("ToolsVersion","4.0");//为project节点添加信息
    project.setAttribute("xmlns","http://schemas.microsoft.com/developer/msbuild/2003");//为project节点添加信息

    QDomElement ItemGroup_1 = vs_filters_file_XML->createElement("ItemGroup");//创建ItemGroup节点(第一个)
    QDomElement ItemGroup_2 = vs_filters_file_XML->createElement("ItemGroup");//创建ItemGroup节点(第二个)
    QVector<QString>::iterator Path_iter;

    for (Path_iter=Path_Array.begin();Path_iter!=Path_Array.end();Path_iter++)
    {
        QDomElement Filter=vs_filters_file_XML->createElement("Filter");//创建Filter节点
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
            QDomElement ClCompile=vs_filters_file_XML->createElement("ClCompile");//创建ClCompile节点
            ClCompile.setAttribute("Include", QString(*File_iter).mid(RootPath.length()+1));//ClCompile节点添加信息

            QDomElement ClCompile_Filter=vs_filters_file_XML->createElement("Filter");//创建ClCompile节点(绝对路径去除工程根目录后得到相对路径)

            QDomText ClCompile_Filter_FilePath = vs_filters_file_XML->createTextNode(fileinfo.absolutePath().mid(RootPath.length()+1));//为ClCompile节点下的Filter节点添加信息(文件的路径,不含文件名)
            ClCompile_Filter.appendChild(ClCompile_Filter_FilePath);

            ClCompile.appendChild(ClCompile_Filter);//添加Filter节点到ClCompile

            ItemGroup_2.appendChild(ClCompile);//添加ClCompile节点到ItemGroup_2
        }

    }


    project.appendChild(ItemGroup_1);//添加ItemGroup节点到project
    project.appendChild(ItemGroup_2);//添加ItemGroup节点到project

    vs_filters_file_XML->appendChild(project);//添加project节点到XML
    return true;
}





bool Create_VCxproj_XML(QDomDocument* vs_vcxproj_file_XML,QString Project_Name,QVector<QString> Define_Array,QVector<QString> Include_Dir_Array,QVector<QString> H_File_Array,QVector<QString> C_File_Array)//创建XML文件
{
    QByteArray res_file_data;

    QFile file(":/Project.vcxproj");
    if (!file.open(QIODevice::ReadOnly))//要求读权限打开文件
    {
        return false;//打开文件失败
    }
    //读取资源文件内容
    res_file_data=file.readAll();

    // 关闭资源文件
    file.close();

    // 用资源文件的内容填充QDomDocument
    if(!(vs_vcxproj_file_XML->setContent(res_file_data,false)))
    {
       return false;//填充数据失败
    }

    QDomElement root = vs_vcxproj_file_XML->documentElement(); //获取根节点

    // 获取TagName为PropertyGroup的节点组
    QDomNodeList PropertyGroup_List = root.elementsByTagName("PropertyGroup");


    //获取TagName为PropertyGroup的节点(第一个)
    QDomNode PropertyGroup = PropertyGroup_List.at(0);

    // 修改ProjectName
    QDomNodeList nodeList = PropertyGroup.toElement().elementsByTagName("ProjectName");
    if (nodeList.count() >0 )
    {
        // 获取Tag为ProjectName的节点
        QDomElement el = nodeList.at(0).toElement();

        // 标签之间的内容作为节点的子节点出现，得到原来的子节点
        QDomNode oldnode = el.firstChild();

        // 用提供的Project_Name值来设置子节点的内容
        el.firstChild().setNodeValue(Project_Name);

        // 值修改过后的子节点
        QDomNode newnode = el.firstChild();

        // 调用节点的replaceChild方法实现修改功能
        el.replaceChild(newnode,oldnode);
    }


    // 获取TagName为ItemDefinitionGroup的节点组
    QDomNodeList ItemDefinitionGroup_List = root.elementsByTagName("ItemDefinitionGroup");

    //获取TagName为ItemDefinitionGroup的节点
    QDomElement ItemDefinitionGroup = ItemDefinitionGroup_List.at(0).toElement();

    // 获取TagName为ClCompile的节点组
    QDomNodeList ClCompile_List = ItemDefinitionGroup.elementsByTagName("ClCompile");

    //获取TagName为ClCompile的节点
    QDomNode ClCompile = ClCompile_List.at(0);

    // 修改PreprocessorDefinitions
    nodeList = ClCompile.toElement().elementsByTagName("PreprocessorDefinitions");
    if (nodeList.count() >0 )
    {
        // 获取Tag为PreprocessorDefinitions的节点
        QDomElement el = nodeList.at(0).toElement();

        // 标签之间的内容作为节点的子节点出现，得到原来的子节点
        QDomNode oldnode = el.firstChild();

        // 先处理一下Define_Array,从数组转成符合要求的QString

        QString define_text="";

        // 遍历容器,并添加到临时QString(最后一个不添加,要做另外处理)
        for(int i=0;i<Define_Array.length()-1;i++)
        {
            define_text=define_text+Define_Array[i]+";";
        }

        // 最后一个不加引号
        define_text=define_text+Define_Array[Define_Array.length()-1];

        // 用提供的Project_Name值来设置子节点的内容
        el.firstChild().setNodeValue(define_text);

        // 值修改过后的子节点
        QDomNode newnode = el.firstChild();

        // 调用节点的replaceChild方法实现修改功能
        el.replaceChild(newnode,oldnode);
    }


    //获取TagName为PropertyGroup的节点(第四个)
    PropertyGroup = PropertyGroup_List.at(3);

    // 修改IncludePath
    nodeList = PropertyGroup.toElement().elementsByTagName("IncludePath");
    if (nodeList.count() >0 )
    {
        // 获取Tag为IncludePath的节点
        QDomElement el = nodeList.at(0).toElement();

        // 标签之间的内容作为节点的子节点出现，得到原来的子节点
        QDomNode oldnode = el.firstChild();

        // 先处理一下Include_Dir_Array,从数组转成符合要求的QString

        QString define_text="";

        // 遍历容器,并添加到临时QString(最后一个不添加,要做另外处理)
        for(int i=0;i<Include_Dir_Array.length()-1;i++)
        {
            define_text=define_text+Include_Dir_Array[i]+";";
        }

        // 最后一个不加引号
        define_text=define_text+Include_Dir_Array[Include_Dir_Array.length()-1];

        // 用提供的Project_Name值来设置子节点的内容
        el.firstChild().setNodeValue(define_text);

        // 值修改过后的子节点
        QDomNode newnode = el.firstChild();

        // 调用节点的replaceChild方法实现修改功能
        el.replaceChild(newnode,oldnode);
    }


    // 获取TagName为ItemGroup的节点组
    QDomNodeList ItemGroup_List = root.elementsByTagName("ItemGroup");


    //获取TagName为ItemGroup的节点(第二个)
    QDomElement ItemGroup = ItemGroup_List.at(1).toElement();


    // 添加所有的H文件
    for(int i=0;i<H_File_Array.length();i++)
    {
        QDomElement ClInclude=vs_vcxproj_file_XML->createElement("ClInclude");//创建ClInclude节点
        ClInclude.setAttribute("Include", H_File_Array[i]);//ClInclude节点添加信息
        ItemGroup.appendChild(ClInclude);//添加ClInclude节点到ItemGroup
    }

    // 添加所有的C文件
    for(int i=0;i<C_File_Array.length();i++)
    {
        QDomElement ClCompile=vs_vcxproj_file_XML->createElement("ClCompile");//创建CClCompile节点
        ClCompile.setAttribute("Include", C_File_Array[i]);//ClCompile节点添加信息
        ItemGroup.appendChild(ClCompile);//添加ClCompile节点到ItemGroup
    }


}
