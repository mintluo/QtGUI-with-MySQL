#include "database.h"
#include <QSqlError>
#include <QDebug>

database::database()
{

}

QSqlDatabase database::db = QSqlDatabase::addDatabase("QMYSQL");
bool database::Open()
{
    db.setHostName("localhost");
    db.setUserName("root");
    db.setPassword("lm?");
    db.setDatabaseName("qqdb");
    if(!db.open())
    {
        //----------------------输出数据库连接错误信息--------------------//
        qDebug()<<"Failed to open FoodOrder database:\n\t"+db.lastError().text();
        return false;
    }
    //db.exec("SET NAMES 'gbk'");
    return true;
}
void database::Close()
{
    db.close();
}
