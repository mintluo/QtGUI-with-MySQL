#ifndef DATABASECONNECTION_H
#define DATABASECONNECTION_H
#include <QSqlDatabase>

static bool connectiondatabase()
{
    QSqlDatabase db=QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("localhost");
    db.setDatabaseName("hotel");
    db.setUserName("root");
    db.setPassword("lm?");
    if(!db.open())
    {
        return false;
    }
    return true;
}
#endif // DATABASECONNECTION_H
