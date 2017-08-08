#ifndef CONNECTDATABASE_H
#define CONNECTDATABASE_H

#endif // CONNECTDATABASE_H
#include <QSqlDatabase>
#include <QString>
#include <QDebug>
#include <QSqlQuery>
#include <QVariant>
#include <QSqlRecord>
#include <QSqlError>

bool connectiondatabase()
{
    //注意：先要在MySQL中创建数据库：create database student;
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    db.setDatabaseName("student");
    db.setUserName("root");
    db.setPassword("lm?");
    if(db.open())
    {
        qDebug()<<"database is established!";
    }
    else
    {
        qDebug()<<"build error:"<<db.lastError().text();
        return false;
    }

    QSqlQuery query;
    query.exec("drop table student");
    query.exec("create table student (sno varchar(10) primary key,sname varchar(20) ,sclass varchar(10) )");
    query.exec("insert into student(sno,sname,sclass) values('2009001','lilei','0901')");
    query.exec("insert into student(sno,sname,sclass) values('2009002','lucy','0902')");
    query.exec("insert into student(sno,sname,sclass) values('2009003','mike','0903')");

    query.exec("select * from student");
    qDebug()<<"sno	sname	sclass";
    while(query.next())
    {
        qDebug()<<query.value(0).toString()<<"	"<<query.value(1).toString()<<"	"<<query.value(2).toString();
    }
    return true;
}
