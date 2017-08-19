#include "dlgaddfood.h"
#include "ui_dlgaddfood.h"
#include <QMessageBox>
#include <QSqlQuery>

dlgaddfood::dlgaddfood(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dlgaddfood)
{
    ui->setupUi(this);
}

dlgaddfood::~dlgaddfood()
{
    delete ui;
}
/*-----------------------添加菜---------------*/
void dlgaddfood::on_pbAdd_clicked()
{
    //判断信息是否完整
    if(ui->cbType->currentText().isEmpty()||
            ui->leName->text().isEmpty()||
            ui->lePrice->text().isEmpty())
    {
        QMessageBox::warning(this,tr("提示"),tr("请将信息输入完整！"),QMessageBox::Ok);
        return;
    }

    QString name=ui->leName->text();        //菜名
    QString type=ui->cbType->currentText(); //菜类型
    int flag=ui->cbType->currentIndex();    //菜系类型标号
    int price=ui->lePrice->text().toInt();  //菜价格
    int hot=0;                              //初始热度

    //准备将要执行的 sql 语句
    QSqlQuery query;
    query.prepare("insert into food (fname,ftype,fprice,flag,fhot_count) values(:fname,:ftype,:fprice,:flag,:fhot_count)");
    query.bindValue(":fname", name);
    query.bindValue(":ftype", type);
    query.bindValue(":fprice",price);
    query.bindValue(":flag",flag);
    query.bindValue(":fhot_count",hot);
    int ok=QMessageBox::warning(this,tr("提示"),tr("确定要增加当前菜单？"),QMessageBox::Yes|QMessageBox::No,QMessageBox::Yes);
    if(ok==QMessageBox::Yes)
    {
        if(query.exec())
        {
            QMessageBox::information(this,tr("提示"),tr("添加成功"),QMessageBox::Ok);
            this->clear();
        }
        else
        {
            QMessageBox::information(this,tr("提示"),tr("添加不成功"),QMessageBox::Ok);
        }
    }
    else
    {
        return;
    }
}

void dlgaddfood::on_pbExit_clicked()
{
    this->close();
}

void dlgaddfood::clear()
{
    ui->cbType->setCurrentIndex(0);
    ui->leName->clear();
    ui->lePrice->clear();

}
