#include <QApplication>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlError>
#include "mainwindow.h"

static bool createConnection()
{
    QSqlDatabase db =QSqlDatabase::addDatabase("QMYSQL");
    db.setUserName("root");//用户名
    db.setPassword("lm?");//密码
    db.setHostName("localhost");
    db.setDatabaseName("StationHelper");//数据库名
    db.setPort(3306);//端口
    if(!db.open())
    {
        qDebug()<<"open failed\n"<<db.lastError().driverText()<<"\n";
        return false;
    }
    else
    {
        qDebug()<<"open database success\n";
    }
    return true;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    if(!createConnection())
        return 0;
    MainWindow w;
    w.show();

    return a.exec();
}
