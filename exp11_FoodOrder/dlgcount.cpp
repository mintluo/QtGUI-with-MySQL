#include "dlgcount.h"
#include "ui_dlgcount.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QTableWidgetItem>
#include <QFile>
#include <QIODevice>
#include <QTextStream>
#include <QDateTime>
#include <QDebug>
#include <QFont>
#include <QColor>

dlgcount::dlgcount(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dlgcount)
{
    ui->setupUi(this);
    ui->leMoney->setEnabled(false);
    QStringList header;
    header.append(QObject::tr("菜名"));
    header.append(QObject::tr("单价"));
    ui->twList->setHorizontalHeaderLabels(header);
}

dlgcount::~dlgcount()
{
    delete ui;
}

void dlgcount::on_pbExit_clicked()
{
    this->close();
}

void dlgcount::on_pbCount_clicked()
{
    if(ui->leClientID->text().isEmpty())
    {
        QMessageBox::warning(this,tr("提示"),tr("请输入客户编号！"),QMessageBox::Ok);
        return;
    }
    int cid=ui->leClientID->text().toInt();
    float totalcount;//最终结算价钱
    QSqlQuery query;
    query.prepare("select sum(fprice) from foodorder.orderlist where cid=:cid");
    query.bindValue(":cid",cid);
    if(query.exec())
    {
        if(query.next())
        {
            totalcount=query.value(0).toFloat();
        }
        //VIP处理 并 将总消费额显示在用户界面上
        if(ui->cbVip->isChecked())
        {
            totalcount=totalcount*0.8;

        }
        //输出 最终消费总额
        qDebug()<<"totalcount :"<<totalcount;
        //更新用户交互界面
        ui->leMoney->setText(QString::number((double)totalcount));
    }
    else
    {
        QMessageBox::warning(this,tr("提示"),tr("数据表查询出错:%1！").arg(query.lastError().text()),QMessageBox::Ok);
        return;
    }

    ///--------------------------打印账单处理------------------------------------////

    QFile bill("./bill.txt");
    if(!bill.open(QIODevice::WriteOnly))//以写的方式打开，并清除原来的数据
    {
        QMessageBox::warning(this,tr("提示"),tr("打印账单出错！"),QMessageBox::Ok);
        return;
    }
    QTextStream out(&bill);

    QDateTime datetime=QDateTime::currentDateTime();
    QString lname=QStringLiteral("名称");
    QString lcount=QStringLiteral("单价");
    QString VIP=QStringLiteral("VIP客户");
    QString discount=QStringLiteral("享受八折优惠");
    QString jew=QStringLiteral("总金额为");
    QString yuan=QStringLiteral("元");
    out<<datetime.toString("yyyy年 MM 月 dd日")<<"\n";
    out<<lname<<"\t\t"<<lcount<<"\n";
    out<<"--------------------------"<<"\n";
    QString tmpname;
    QString tmpprice;
    for(int i=0;i<ui->twList->rowCount();i++)
    {
        tmpname=ui->twList->item(i,0)->text();
        tmpprice=ui->twList->item(i,1)->text();
        out<<tmpname<<"--------------"<<tmpprice<<yuan<<"\n";
    }
    out<<"---------end----------"<<"\n";
    if(ui->cbVip->isChecked())
    {
        out<<"-----------"<<VIP<<"-----------"<<"\n";
        out<<"-------"<<discount<<"--------"<<"\n";
    }
    out<<"\t\t"<<jew<<totalcount<<yuan<<"\n";
    bill.close();
    QMessageBox::information(this,tr("提示"),tr("打印账单完成！"),QMessageBox::Ok);

    QSqlQuery delquery;
    delquery.prepare("delete from foodorder.orderlist where cid=:cid");
    delquery.bindValue(":cid",cid);
    if(delquery.exec())
    {
        QMessageBox::warning(this,tr("提示"),tr("数据库清理完成！"),QMessageBox::Ok);
    }
    else
    {
        QMessageBox::warning(this,tr("提示"),tr("清理数据出错:%1！").arg(delquery.lastError().text()),QMessageBox::Ok);
    }
}

void dlgcount::on_pbSearchInfo_clicked()
{
    if(ui->leClientID->text().isEmpty())
    {
        QMessageBox::warning(this,tr("提示"),tr("请输入客户编号！"),QMessageBox::Ok);
        return;
    }
    qDebug()<<"获取该消费者的消费信息";
    int cid=ui->leClientID->text().toInt();
    QSqlQuery query;
    query.prepare("select fname,fprice from foodorder.orderlist where cid=:cid");
    query.bindValue(":cid",cid);
    if(query.exec())
    {
        QSqlRecord rec=query.record();
        int in1=rec.indexOf("fname");
        int in2=rec.indexOf("fprice");
        //清除该表中已有的信息 并将行数设置为 0
        ui->twList->setRowCount(0);
        while(query.next())//显示用户信息表格的内容
        {
            QString fname=query.value(in1).toString();
            int fprice=query.value(in2).toInt();

            qDebug()<<"fname:"<<fname<<"\tfprice"<<fprice;
            QTableWidgetItem *item0=new QTableWidgetItem(fname);
            QTableWidgetItem *item1=new QTableWidgetItem(QString::number(fprice));
            int rows=ui->twList->rowCount();
            ui->twList->setRowCount(rows+1);
            ui->twList->setItem(rows,0,item0);//显示名称
            ui->twList->setItem(rows,1,item1);//显示单价
        }
        qDebug()<<"更新结束:！";
    }
    else
    {
        QMessageBox::warning(this,tr("提示"),tr("数据库查询出错:%1").arg(query.lastError().text()),QMessageBox::Ok);
        return;
    }
}
