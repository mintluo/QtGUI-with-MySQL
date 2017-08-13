#include "chgpwd.h"
#include "ui_chgpwd.h"
#include "login.h"

#include <QMessageBox>
#include <QString>
#include <QSqlQuery>

chgpwd::chgpwd(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::chgpwd)
{
    ui->setupUi(this);
}

chgpwd::~chgpwd()
{
    delete ui;
}

void chgpwd::on_pbSubmit_clicked()
{

    QString uid=ui->leUid->text().trimmed();
    QString pwd=ui->lePwd->text().trimmed();
    QString newpwd=ui->leNewPwd->text().trimmed();
    QString newpwdcopy=ui->leNewPwdcopy->text().trimmed();
    if(uid.isEmpty()||
       pwd.isEmpty()||
       newpwd.isEmpty()||
       newpwdcopy.isEmpty())
    {
        QMessageBox::information(this,tr("提示"),tr("请输入完整信息"),QMessageBox::Ok);
        return;
    }
    if(newpwd!=newpwdcopy)
    {
        QMessageBox::information(this,tr("提示"),tr("请核对密码！"),QMessageBox::Ok);
        return;
    }

    QSqlQuery query;
    query.prepare("select * from staff where sid=:sid and pwd=:pwd");
    query.bindValue(":sid", uid);
    query.bindValue(":pwd", pwd);
    query.exec();
      //验证用户是否存在
    if(query.next())
    {
         query.clear();
         query.prepare("update staff set pwd=:pwd where sid=:sid");
         query.bindValue(":sid", uid);
         query.bindValue(":pwd", newpwd);
         if(query.exec())
         {
             if(query.isActive())
             {
                QMessageBox::information(this,tr("提示"),tr("Update password successfully!"),QMessageBox::Ok);
                return;
             }
         }
         else
         {
             QMessageBox::information(this,tr("提示"),tr("更新密码不成功!"),QMessageBox::Ok);
             return;
         }
    }
    else
    {
         //用户信息不正确，给出提示信息
        QMessageBox::information(this,tr("提示"),tr("user id or password is wrong!"),QMessageBox::Ok);
        ui->leNewPwd->clear();
        ui->leNewPwdcopy->clear();
        ui->lePwd->clear();
        ui->leUid->clear();
        ui->leUid->setFocus();
    }
}

void chgpwd::on_pbQuit_clicked()
{
    login *log=new login();
    log->show();
    this->hide();
}
