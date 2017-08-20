#ifndef DATABASE_H
#define DATABASE_H

#include <QSqlDatabase>

class database
{
public:
    database();

    static QSqlDatabase db;
    static bool Open();
    static void Close();
};

#endif // DATABASE_H
