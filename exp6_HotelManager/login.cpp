#include "login.h"
#include "ui_login.h"
#include "mainwindow.h"
#include "chgpwd.h"

#include <QString>
#include <QMessageBox>
#include <QSqlQuery>
#include <QtGui>

login::login(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::login)
{
    ui->setupUi(this);
}

login::~login()
{
    delete ui;
}

void login::on_pbLogin_clicked()
{
    if(ui->leUid->text().isEmpty()||
       ui->lePwd->text().isEmpty())
    {
        QMessageBox::information(this,tr("提示"),tr("请输入完整信息"),QMessageBox::Ok);
        return;
    }
     QString uid=ui->leUid->text();
     QString pwd=ui->lePwd->text();
     QSqlQuery query;
     query.prepare("select * from staff where sid=:sid and pwd=:pwd");
     query.bindValue(":sid", uid);
     query.bindValue(":pwd", pwd);
     query.exec();
     if(query.next())//有查询结果则表示登录成功
     {
         MainWindow *w=new MainWindow();
         w->show();
         this->hide();
     }
     else
     {
         QMessageBox::warning(this,tr("提示"),tr("请核对用户名与密码！"),QMessageBox::Ok);
         //ui->leUid->clear();
         ui->lePwd->clear();
         ui->lePwd->setFocus();
     }
}

void login::on_pbChgpwd_clicked()
{
    chgpwd *chg = new chgpwd();
    chg->show();
    this->hide();
}

void login::on_pbQuit_clicked()
{
    this->close();
}
