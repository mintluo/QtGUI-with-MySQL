#include "dlgdelete_update.h"
#include "ui_dlgdelete_update.h"
#include <QSqlQuery>
#include <QMessageBox>
#include <QSqlRecord>
#include <QString>

dlgdelete_update::dlgdelete_update(QString fid , QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dlgdelete_update)
{
    ui->setupUi(this);
    this->fid=fid;
    this->Datainit();
}

dlgdelete_update::~dlgdelete_update()
{
    delete ui;
}
/*------------------查询出数据，并将其更新到界面中-----------------------*/
void dlgdelete_update::Datainit()
{
    //查询数据 并将数据显示在交互界面
    QSqlQuery query;
    query.prepare("select fname,flag,fprice,fhot_count from food where fid=:fid");
    query.bindValue(":fid", this->fid);
    query.exec();
    if(query.next())
    {
        QSqlRecord rec=query.record();
        int ind_name=rec.indexOf("fname");
        int ind_type=rec.indexOf("flag");
        int ind_price=rec.indexOf("fprice");
        int ind_hot=rec.indexOf("fhot_count");
        QString name=query.value(ind_name).toString();
        ui->leName->setText(name);                      //显示菜名
        int type=query.value(ind_type).toInt();
        ui->cbType->setCurrentIndex(type);              //显示类型
        QString price=query.value(ind_price).toString();
        ui->lePrice->setText(price);                    //显示价格
        QString hot=query.value(ind_hot).toString();
        ui->leHotLevel->setText(hot);                   //显示热度值
    }
}

void dlgdelete_update::on_pbDelete_clicked()
{
    //删除当前数据
    QSqlQuery query;
    query.prepare("delete from food where fid=:fid");
    query.bindValue(":fid", this->fid);
    int ok=QMessageBox::warning(this,tr("提示"),tr("确定要删除当前菜？"),QMessageBox::Yes|QMessageBox::No,QMessageBox::Yes);
    if(ok==QMessageBox::Yes)
    {
        if(query.exec())
        {
            QMessageBox::information(this,tr("提示"),tr("删除成功"),QMessageBox::Ok);
            this->clear();
        }
        else
        {
            QMessageBox::information(this,tr("提示"),tr("删除不成功"),QMessageBox::Ok);
        }
    }
    else
    {
        return;
    }
}

void dlgdelete_update::on_pbUpdate_clicked()
{
    //判断输入信息是否完整
    if(ui->leHotLevel->text().isEmpty()||
            ui->leName->text().isEmpty()||
            ui->lePrice->text().isEmpty()||
            ui->cbType->currentText().isEmpty())
    {
        QMessageBox::warning(this,tr("提示"),tr("请将信息输入完整！"),QMessageBox::Ok);
        return;
    }

    //获取用户修改过的信息
    QString name=ui->leName->text();        //获取菜名
    QString type=ui->cbType->currentText(); //获取菜类型
    int flag=ui->cbType->currentIndex();    //获取菜类型标号
    int price=ui->lePrice->text().toInt();  //获取菜价
    int hot=ui->leHotLevel->text().toInt(); //获取热度值

    QSqlQuery query;
    query.prepare("update food set fname=:fname,ftype=:ftype,fprice=:fprice,flag=:flag,fhot_count=:fhot_count where fid=:fid");
    query.bindValue(":fid", this->fid);
    query.bindValue(":fname", name);
    query.bindValue(":ftype", type);
    query.bindValue(":fprice",price);
    query.bindValue(":flag",flag);
    query.bindValue(":fhot_count",hot);
    int ok=QMessageBox::warning(this,tr("提示"),tr("确定要更改当前菜单？"),QMessageBox::Yes|QMessageBox::No,QMessageBox::Yes);
    if(ok==QMessageBox::Yes)
    {
        if(query.exec())
        {
            QMessageBox::information(this,tr("提示"),tr("更新成功"),QMessageBox::Ok);
            this->clear();
        }
        else
        {
            QMessageBox::information(this,tr("提示"),tr("更新不成功"),QMessageBox::Ok);
        }
    }
    else
    {
        return;
    }
}

void dlgdelete_update::on_pbExit_clicked()
{
    this->close();
}

void dlgdelete_update::clear()
{
    ui->leHotLevel->clear();
    ui->cbType->setCurrentIndex(0);
    ui->leName->clear();
    ui->lePrice->clear();

}
