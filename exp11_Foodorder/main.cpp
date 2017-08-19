#include "mainwindow.h"
#include <QApplication>
#include "mainwindow.h"
#include "dbconnection.h"
#include <QMessageBox>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //------------------打开数据库--------------------------//
    if(!connectiondatabase())
    {
        return 1;
    }
    qDebug()<<"foodorder database is connected!";

    MainWindow w;
    w.setWindowTitle("主界面");
    w.show();
    return a.exec();
}
