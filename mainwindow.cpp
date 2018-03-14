#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QApplication>
#include <QFileDialog>
#include <QDebug>
#include <QString>

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
    QString file_name = QFileDialog::getExistingDirectory(NULL,"caption",".");
    ui->Project_Path_textEdit->setPlainText(file_name);
}
