#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#ifdef WIN32
#pragma execution_character_set("utf-8")
#endif

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_Project_Path_Button_clicked();

    void on_Project_Add_Path_Button_clicked();

    void on_Project_Remove_Path_Button_clicked();

    void Project_Add_Path_tableView_Init(void);//项目添加目录的列表框初始化

    void Project_Remove_Path_tableView_Init(void);//项目排除目录的列表框初始化
    void on_pushButton_5_clicked();

    void on_pushButton_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
