#include "mainwindow.h"
#include <QApplication>

#include "mainwindow.h"
#include "databaseconnection.h"
#include "login.h"
#include "chgpwd.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    if(!connectiondatabase())
    {
        return false;
    }
    login log;
    log.show();
    return a.exec();
}
