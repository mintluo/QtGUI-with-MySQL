#include "login.h"
#include "ui_login.h"
#include <QMessageBox>
#include <QtSql>
#include <QSqlQuery>
#include <QSqlError>
#include "database.h"
#include "widget.h"
#include "register.h"

Login::Login(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);
}

Login::~Login()
{
    delete ui;
}

void Login::on_btnLogin_clicked()
{
    //打开数据库
    if(!database::Open()){
        QMessageBox::about(this,tr("错误！"),tr("打开数据库失败\n%1").arg(database::db.lastError().text()));
        return ;
    }
    QSqlQuery query;
    query.exec("select * from user where userName='"+ui->leUsername->text()+"' AND passwd='"+ui->lePasswd->text()+"'");
    //sql语句是否成功执行
    if(!query.isActive()){
        QMessageBox::about(0,tr("结果"),tr("登录失败\n%1").arg(query.lastError().text()));
        database::Close();  //关闭数据库
        return ;
    }
    if(query.size() == 0)
    {
        QMessageBox::about(0,tr("结果"),tr("登录失败\n%1").arg(query.lastError().text()));
        database::Close();
        return;
    }
    database::Close();
    //打开系统主界面
    Widget *w = new Widget;
    w->setWindowTitle(ui->leUsername->text());
    w->show();
    this->close();//关闭登录界面
}
//打开注册界面
void Login::on_btnRegister_clicked()
{
    Register *reg = new Register;
    reg->show();
}
