#ifndef DBCONNECTION_H
#define DBCONNECTION_H

#include <QSqlDatabase>
#include <QString>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

static bool connectiondatabase()
{
    QSqlDatabase db=QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    db.setDatabaseName("foodorder");
    db.setUserName("root");
    db.setPassword("lm?");
    if(!db.open())
    {
        //----------------------输出数据库连接错误信息--------------------//
        qDebug()<<"Failed to open FoodOrder database:\n\t"+db.lastError().driverText();
        qDebug()<<"\t"+db.lastError().databaseText();
        return false;
    }
    return true;
}

#endif // DBCONNECTION_H
