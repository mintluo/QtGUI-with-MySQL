#include "dlgsearch.h"
#include "ui_dlgsearch.h"
#include "dlgdetail.h"
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QMessageBox>
#include <QStringList>
#include <QLabel>
#include <QTableWidgetItem>
#include <QString>
#include <QAbstractItemView>

dlgsearch::dlgsearch(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dlgsearch)
{
    ui->setupUi(this);
    //初始化 表格
   // QStringList header;
   // header<<tr("编号")<<tr("姓名")<<tr("订餐数量");
//    header.append(tr("编号"));
//    header.append(tr("姓名"));
//    header.append(tr("订餐数量"));
   // ui->twList->setHorizontalHeaderLabels(header);
    ui->twList->setEditTriggers(QAbstractItemView::NoEditTriggers); //使其不可编辑
    ui->twList->show();
    //填充数据表
    this->init();
}

dlgsearch::~dlgsearch()
{
    delete ui;
}

void dlgsearch::init()
{
    int cid;        //客户编号
    QString cname;  //客户姓名
    int count;      //客户点菜数量
    QSqlQuery query;
    query.prepare("select cid,cname,count(fname) from foodorder.orderlist group by cid");
    if(query.exec())
    {
        QSqlRecord rec=query.record();
        int index_cid=rec.indexOf("cid");
        int index_cname=rec.indexOf("cname");
        int index_count=rec.indexOf("count(fname)");
        while(query.next())
        {
            cid=query.value(index_cid).toInt();
            cname=query.value(index_cname).toString();
            count=query.value(index_count).toInt();

            int row=ui->twList->rowCount();
            ui->twList->setRowCount(row+1);

            QTableWidgetItem *item0=new QTableWidgetItem(QString::number(cid));
            QTableWidgetItem *item2=new QTableWidgetItem(QString::number(count));
            QTableWidgetItem *item1=new QTableWidgetItem(cname);
            ui->twList->setItem(row,0,item0);
            ui->twList->setItem(row,1,item1);
            ui->twList->setItem(row,2,item2);
        }
    }
    else
    {
        QMessageBox::warning(this,tr("提示"),tr("数据库查询出错：%1").arg(query.lastError().text()),QMessageBox::Ok);
        return;
    }
}

void dlgsearch::on_pbClientDetail_clicked()
{
    if(ui->leClientID->text().isEmpty())
    {
        QMessageBox::warning(this,tr("提示"),tr("请输入客户编号"),QMessageBox::Ok);
        this->init();
        return;
    }

    int result=0;
    QSqlQuery query;
    query.prepare("select distinct cid from foodorder.orderlist");
    if(query.exec())
    {
        while(query.next())//查找用户编号
        {
            //判断用户是否存在
            if(query.value(0).toString()==ui->leClientID->text())
            {
                result=1;
                break;
            }
        }
        if(result==1)
        {
            int cid=ui->leClientID->text().toInt();
            dlgdetail *dlg=new dlgdetail(cid,this);
            dlg->show();
            this->hide();
        }
        else//错误提醒
        {
            QMessageBox::warning(this,tr("提示"),tr("不存在此用户！"),QMessageBox::Ok);
            return;
        }
    }
    else
    {
        QMessageBox::warning(this,tr("提示"),tr("数据库查询出错：%1").arg(query.lastError().text()),QMessageBox::Ok);
        return;
    }
}

void dlgsearch::on_pbCancel_clicked()
{
    this->close();
}
