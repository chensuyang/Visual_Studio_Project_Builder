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
     QVector<QString> File_Array;
     QVector<QString> Path_Array;
     FindFile(Project_Path,File_Array,Path_Array);
     Create_XML(vs_filters_file,Project_Path,File_Array,Path_Array,true);
     Create_VS_filters_File(Project_Path+"/test.xml",vs_filters_file);



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
