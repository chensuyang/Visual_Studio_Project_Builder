﻿#include "mainwindow.h"
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
QString Project_Path;//项目目录

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
    Project_Path = QFileDialog::getExistingDirectory(NULL,"caption",".");
    ui->Project_Path_textEdit->setPlainText(Project_Path);
    fi=ui->Project_Path_textEdit->toPlainText();
    if(fi.isDir())//当前设置的项目目录有效
    {
     QDomDocument vs_filters_file("vs_filters_file");
     FindFile(Project_Path,File_Array,Path_Array);


    }
    else
    {
        QMessageBox::warning(NULL,QString("警告"),QString("当前目录无效"),QMessageBox::Yes);
    }
}

void MainWindow::on_Project_Add_Path_Button_clicked()//项目添加目录按钮被点击
{
    QFileInfo fi("C:/");
    QString file_name = QFileDialog::getExistingDirectory(NULL,"caption",".");
    fi=file_name;
    if(fi.isDir())//当前设置的项目目录有效
    {
        ui->Project_Add_Path_listWidget->addItem(file_name);


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
         ui->Project_Remove_Path_listWidget->addItem(file_name);
    }
    else
    {
        QMessageBox::warning(NULL,QString("警告"),QString("当前目录无效"),QMessageBox::Yes);
    }
}

void MainWindow::on_pushButton_5_clicked()//构建VS项目按钮被点击
{
    /*
    QVector<QString> Tmp_File_Array;//临时文件路径数组
    for(int i=0;i<(ui->Project_Add_Path_listWidget->count());i++)//将要添加的外部路径列表框中所有项目添加至External_Path_Array
    {
        QVector<QString> Tmp_File_Array;//临时文件路径数组
        QString External_Path=ui->Project_Add_Path_listWidget->item(i)->text();//获取列表框中的目录

        FindFile(External_Path,Tmp_File_Array,External_Path_Array);//遍历文件,并直接把目录添加到External_Path_Array中
        QVector<QString>::iterator File_iter;
        for (File_iter=External_Path_Array.begin();File_iter!=External_Path_Array.end();File_iter++)
        {
            qDebug()<<*File_iter;
        }
    }
    */
    QDomDocument vs_vcxproj_file_XML;
    QVector<QString> Define_Array;
    QVector<QString> Include_Dir_Array;
    QVector<QString> H_File_Array;
    QVector<QString> C_File_Array;

    Define_Array.append("NRF52");
    Define_Array.append("STM32F103");

    Include_Dir_Array.append(".\\AAC1");
    Include_Dir_Array.append(".\\AAC2");

    C_File_Array.append("Boot\\main_aac1.c");
    C_File_Array.append("Boot\\main_aac2.c");
    C_File_Array.append("Boot\\main_aac3.c");

    H_File_Array.append("Boot\\main_aac1.h");
    H_File_Array.append("Boot\\main_aac2.h");
    H_File_Array.append("Boot\\main_aac3.h");

    Create_VCxproj_XML(&vs_vcxproj_file_XML,"AAC",Define_Array,Include_Dir_Array,H_File_Array,C_File_Array);

    qDebug()<<QString(vs_vcxproj_file_XML.toString());

    QFile f(Project_Path+"/test.vcxproj");
    if (!f.open(QFile::Truncate | QFile::ReadWrite))return;
    f.write(QString(vs_vcxproj_file_XML.toString()).toUtf8());
    f.close();

}

void MainWindow::on_pushButton_clicked()
{
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
