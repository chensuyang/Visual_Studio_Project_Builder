#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QApplication>
#include <QFileDialog>
#include <QDebug>
#include <QString>
#include <QStandardItemModel>
#include <QMessageBox>
#include <QtXml>
#include "dir_handle.h"
#include "XML_handle.h"

QVector<QString> File_Array;//文件路径数组
QVector<QString> Path_Array;//目录路径数组
QVector<QString> External_Path_Array;//外部目录数组

QStandardItemModel  *Project_Add_Path_tableView_model = new QStandardItemModel();//项目额外添加目录表格
QStandardItemModel  *Project_Remove_Path_tableView_model = new QStandardItemModel();//项目额外添加目录表格

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_Project_Path_Button_clicked()//项目根目录选择按钮被点击
{
    QFileInfo fi("C:/");
    QString Project_Path = QFileDialog::getExistingDirectory(NULL,"caption",".");
    ui->Project_Path_textEdit->setPlainText(Project_Path);
    fi=ui->Project_Path_textEdit->toPlainText();
    if(fi.isDir())//当前设置的项目目录有效
    {
     Project_Add_Path_tableView_Init();
     Project_Remove_Path_tableView_Init();//初始化两个列表框
     QDomDocument vs_filters_file("vs_filters_file");
     FindFile(Project_Path,File_Array,Path_Array);


    }
    else
    {
        QMessageBox::warning(NULL,QString("警告"),QString("当前目录无效"),QMessageBox::Yes);
    }
}


void MainWindow::Project_Add_Path_tableView_Init(void)//项目添加目录的列表框初始化
{
    Project_Add_Path_tableView_model->setColumnCount(2);
    Project_Add_Path_tableView_model->setHeaderData(0,Qt::Horizontal,QString("目录"));
    Project_Add_Path_tableView_model->setHeaderData(1,Qt::Horizontal,QString("递归搜索"));
    ui->Project_Add_Path_tableView->setModel(Project_Add_Path_tableView_model);
    ui->Project_Add_Path_tableView->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);//表头信息显示居左
    ui->Project_Add_Path_tableView->setColumnWidth(0,530);
    ui->Project_Add_Path_tableView->setColumnWidth(1,100);
    ui->Project_Remove_Path_tableView->verticalHeader()->hide();//隐藏序号
}

void MainWindow::Project_Remove_Path_tableView_Init(void)//项目排除目录的列表框初始化
{
    Project_Remove_Path_tableView_model->setColumnCount(2);
    Project_Remove_Path_tableView_model->setHeaderData(0,Qt::Horizontal,QString("目录"));
    Project_Remove_Path_tableView_model->setHeaderData(1,Qt::Horizontal,QString("递归搜索"));
    ui->Project_Remove_Path_tableView->setModel(Project_Remove_Path_tableView_model);
    ui->Project_Remove_Path_tableView->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);//表头信息显示居左
    ui->Project_Remove_Path_tableView->setColumnWidth(0,530);
    ui->Project_Remove_Path_tableView->setColumnWidth(1,100);
    ui->Project_Remove_Path_tableView->verticalHeader()->hide();//隐藏序号
}

void MainWindow::on_Project_Add_Path_Button_clicked()//项目添加目录按钮被点击
{
    QFileInfo fi("C:/");
    QString file_name = QFileDialog::getExistingDirectory(NULL,"caption",".");
    fi=file_name;
    if(fi.isDir())//当前设置的项目目录有效
    {
        long row_cnt=Project_Add_Path_tableView_model->rowCount();
        Project_Add_Path_tableView_model->setItem(row_cnt,0,new QStandardItem(file_name));
        Project_Add_Path_tableView_model->item(row_cnt,0)->setForeground(QBrush(QColor(0, 0, 0))); //设置字符颜色
        Project_Add_Path_tableView_model->item(row_cnt,0)->setTextAlignment(Qt::AlignCenter);//设置字符位置
        Project_Add_Path_tableView_model->setItem(row_cnt,1,new QStandardItem("Yes"));


    }
    else
    {
        QMessageBox::warning(NULL,QString("警告"),QString("当前目录无效"),QMessageBox::Yes);
    }
}

void MainWindow::on_Project_Remove_Path_Button_clicked(void)//项目排除目录按钮被点击
{
    QFileInfo fi("C:/");
    QString file_name = QFileDialog::getExistingDirectory(NULL,"caption",".");
    fi=file_name;
    if(fi.isDir())//当前设置的项目目录有效
    {
        long row_cnt=Project_Remove_Path_tableView_model->rowCount();
        Project_Remove_Path_tableView_model->setItem(row_cnt,0,new QStandardItem(file_name));
        Project_Remove_Path_tableView_model->item(row_cnt,0)->setForeground(QBrush(QColor(0, 0, 0))); //设置字符颜色
        Project_Remove_Path_tableView_model->item(row_cnt,0)->setTextAlignment(Qt::AlignCenter);//设置字符位置
        Project_Remove_Path_tableView_model->setItem(row_cnt,1,new QStandardItem("Yes"));
    }
    else
    {
        QMessageBox::warning(NULL,QString("警告"),QString("当前目录无效"),QMessageBox::Yes);
    }
}

void MainWindow::on_pushButton_5_clicked()
{
    External_Path_Array.clear();//清空外部文件目录
    for(int i=0;i<(Project_Add_Path_tableView_model->rowCount());i++)//将要添加的外部路径列表框中所有项目添加至External_Path_Array
    {
        QVector<QString> Tmp_File_Array;//临时文件路径数组
        QModelIndex index=Project_Add_Path_tableView_model->index(i,1,QModelIndex());//获取每一行第二列的数据
        QString isFind= index.data().toString();
        if(isFind=="是")//判断需不需要递归搜索
        {
            index=Project_Add_Path_tableView_model->index(i,0,QModelIndex());//获取每一行第一列的数据
            QString External_Path= index.data().toString();
            FindFile(External_Path,Tmp_File_Array,External_Path_Array);//遍历文件,并直接把目录添加到External_Path_Array中
        }
        else
        {
            index=Project_Add_Path_tableView_model->index(i,0,QModelIndex());//获取每一行第一列的数据
            QString External_Path= index.data().toString();
            External_Path_Array.append(External_Path);//添加进External_Path_Array
        }
    }
}

void MainWindow::on_pushButton_clicked()
{
    qDebug("%d",this->frameGeometry().width());
    if(this->frameGeometry().width()==706)//如果窗口没有伸展
    {
        this->setMinimumSize(970, 600);
        this->setMaximumSize(970, 600);
    }
    else
    {
        this->setMinimumSize(690, 600);
        this->setMaximumSize(690, 600);
    }
}
