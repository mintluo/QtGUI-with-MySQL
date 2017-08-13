#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QTableWidget>
#include <QMessageBox>
#include <QDateTime>
#include <QDate>
#include <QString>
#include <QSqlTableModel>
#include <QSqlRecord>
#include <QAbstractItemView>
#include <QDebug>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->tabWidget->setCurrentIndex(0);
    //tab 1
    priceinfo_model=new QSqlTableModel(this);
    priceinfo_model->setTable("roomtype");
    priceinfo_model->setEditStrategy(QSqlTableModel::OnManualSubmit); //修改提交过之后更新到数据库
    priceinfo_model->select();
    //获取当前列 的索引
    priceinfo_model->removeColumn(priceinfo_model->fieldIndex("tid"));
    priceinfo_model->removeColumn(priceinfo_model->fieldIndex("amount"));
    priceinfo_model->removeColumn(priceinfo_model->fieldIndex("sid"));
    priceinfo_model->setHeaderData(0,Qt::Horizontal,tr("房间类型"));
    priceinfo_model->setHeaderData(1,Qt::Horizontal,tr("价格"));
    priceinfo_model->setHeaderData(2,Qt::Horizontal,tr("押金"));
    priceinfo_model->setHeaderData(3,Qt::Horizontal,tr("描述"));
    ui->tvPriceInfo->setEditTriggers(QAbstractItemView::NoEditTriggers);//使其不可编辑

    ui->tvPriceInfo->setModel(this->priceinfo_model);

    //初始化   roominfo_model;

    this->roominfo_model=new QSqlTableModel(this);
    this->roominfo_model->setTable("room");
    this->roominfo_model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    this->roominfo_model->select();
    this->roominfo_model->setHeaderData(0,Qt::Horizontal,tr("房间号"));
    this->roominfo_model->setHeaderData(2,Qt::Horizontal,tr("状态"));
    //注意hideColumn的位置
    ui->tvRoomInfo->setEditTriggers(QAbstractItemView::NoEditTriggers);//使其不可编辑
    ui->tvRoomInfo->setModel(this->roominfo_model);
    ui->tvRoomInfo->hideColumn(1);
    //默认第一个房间
    ui->rb1->setChecked(true);
    this->roominfo_model->setFilter("tid=2");//房型tid从2开始
    this->roominfo_model->select();

    //tab2  登记入住初始化
    ui->leID->setText("");
    ui->leUserName->setText("");
    ui->leRoomID->setText("");
    ui->leNumOfNig->setText("");

    //tab3  初始化
    checkout_model=new QSqlTableModel(this);

    checkout_model->setTable("checkin");
    checkout_model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    checkout_model->setHeaderData(0,Qt::Horizontal,tr("客房号"));
    checkout_model->setHeaderData(1,Qt::Horizontal,tr("房间号"));
    checkout_model->setHeaderData(2,Qt::Horizontal,tr("入住时间"));
    checkout_model->setHeaderData(3,Qt::Horizontal,tr("入住天数"));
    checkout_model->setHeaderData(4,Qt::Horizontal,tr("押金"));
    checkout_model->select();
    //checkout_model->removeColumn(this->checkout_model->fieldIndex("engagedate"));
    //checkout_model->removeColumn(this->checkout_model->fieldIndex("engagecash"));
    checkout_model->removeColumn(this->checkout_model->fieldIndex("sid"));
    ui->tvQuery->setModel(checkout_model);
    ui->tvQuery->setEditTriggers(QAbstractItemView::NoEditTriggers);//使其不可编辑


    //tab4
    model_checkin=new QSqlTableModel(this);
    model_checkin->setTable("checkin");
    model_checkin->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model_checkin->select();
    model_checkin->setHeaderData(0,Qt::Horizontal,tr("客户号"));
    model_checkin->setHeaderData(1,Qt::Horizontal,tr("房间号"));
    model_checkin->setHeaderData(2,Qt::Horizontal,tr("入住日期"));
    model_checkin->setHeaderData(3,Qt::Horizontal,tr("入住天数"));

    //注意hideColumn的位置
    ui->tvCheckin->setModel(this->model_checkin);
    ui->tvCheckin->hideColumn(4);
    ui->tvCheckin->hideColumn(5);
    ui->tvCheckin->hideColumn(6);
    ui->tvCheckin->setEditTriggers(QAbstractItemView::NoEditTriggers);//使其不可编辑


    model_checkout=new QSqlTableModel(this);
    model_checkout->setTable("checkout");
    model_checkout->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model_checkout->select();
    model_checkout->setHeaderData(0,Qt::Horizontal,tr("客户号"));
    model_checkout->setHeaderData(2,Qt::Horizontal,tr("入住日期"));
    model_checkout->setHeaderData(1,Qt::Horizontal,tr("房间号"));
    model_checkout->setHeaderData(3,Qt::Horizontal,tr("退款日期"));
    model_checkout->setHeaderData(4,Qt::Horizontal,tr("付款金额"));
    ui->tvCheckout->setModel(this->model_checkout);
    ui->tvCheckout->hideColumn(5);
    ui->tvCheckout->setEditTriggers(QAbstractItemView::NoEditTriggers);//使其不可编辑
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_rb1_clicked()
{
    this->roominfo_model->setFilter("tid=2");
    this->roominfo_model->select();
}

void MainWindow::on_rb2_clicked()
{
    this->roominfo_model->setFilter("tid=3");
    this->roominfo_model->select();
}

void MainWindow::on_rb3_clicked()
{
    this->roominfo_model->setFilter("tid=4");
    this->roominfo_model->select();
}

void MainWindow::on_rb4_clicked()
{
    this->roominfo_model->setFilter("tid=5");
    this->roominfo_model->select();
}

void MainWindow::on_rb5_clicked()
{
    this->roominfo_model->setFilter("tid=6");
    this->roominfo_model->select();
}

void MainWindow::on_rb6_clicked()
{
    this->roominfo_model->setFilter("tid=7");
    this->roominfo_model->select();
}


void MainWindow::on_pbSubmit_clicked()
{
    QString cnum=ui->leID->text();
    QString cname=ui->leUserName->text();
    QString rid=ui->leRoomID->text();
    QString day=ui->leNumOfNig->text();
    QString state;
    QDateTime time;
    int cash=0;
    QSqlQuery query;
    query.exec("select state from room where rid="+rid);
    if(query.next())
    {
        state=query.value(0).toString();
        if(state == tr("占用"))
        {QMessageBox::warning(this,tr("info"),tr("房间被占用！"));return;}
    }
    query.exec("SELECT * FROM client WHERE cnumber='"+cnum+"'");
    if(!query.next()) {
        query.prepare(tr("INSERT INTO client VALUES (:id, :name)"));
        query.bindValue(":id", cnum);
        query.bindValue(":name", cname);
        query.exec();
        QMessageBox::information(this,tr("info"),tr("欢迎首次光临!"));
    }else QMessageBox::information(this,tr("info"),tr("欢迎光临!"));
    query.exec("SELECT foregift FROM room,roomtype WHERE room.tid=roomtype.tid and room.rid="+rid);
    if(query.next()) {
        cash = query.value(0).toInt();
    }
    QString stime=time.currentDateTime().toString(QLatin1String("yyyy-MM-dd hh:mm:ss"));
    //QString ssid = tr("200713998");
    //写入 checkin 数据库
    query.prepare("INSERT INTO checkin (cnumber,rid, opendate,engagedate,engagecash) VALUES (:cnum, :rid, :opendate,:engagedate,:engagecash)");
    query.bindValue(":cnum", cnum);
    query.bindValue(":rid", rid);
    query.bindValue(":opendate",stime);
    query.bindValue(":engagedate",day);
    query.bindValue(":engagecash",cash);
    //query.bindValue(":sid",ssid);
    //qDebug()<<"cnum:"<<cnum<<"rid"<<rid<<"opendate"<<stime<<"engagedate"<<day<<"engagecash"<<cash<<"sid"<<ssid;


    if(query.exec())
    {
        query.prepare(tr("UPDATE room SET state ='占用' WHERE rid = ? "));
        query.bindValue(0, rid);
        query.exec();
        QMessageBox::information(this,tr("info"),tr("入住成功！"));
    }
    else  QMessageBox::warning(this,tr("info"),tr("入住失败！"));
}

void MainWindow::on_pbCancle_clicked()
{
    //tab  登记入住初始化
    ui->leID->setText("");
    ui->leUserName->setText("");
    ui->leRoomID->setText("");
    ui->leNumOfNig->setText("");
}



void MainWindow::on_pbOut_Search_clicked()
{
    if(ui->leOut_ID->text().isEmpty())
    {
        QMessageBox::information(this,tr("提示"),tr("请输入身份信息！"),QMessageBox::Ok);
        return;
    }

    QString cnumber=ui->leOut_ID->text().trimmed();
    this->checkout_model->setFilter(QObject::tr("cnumber=%1").arg(cnumber));
    this->checkout_model->select();
    ui->tvQuery->setModel(checkout_model);
}

void MainWindow::on_pbCheckout_clicked()
{
    //如果当前行的记录不为空
    if(this->checkout_model->rowCount()==0)
    {
        QMessageBox::information(this,tr("提示"),tr("请核对信息！"),QMessageBox::Ok);
        return;
    }
    QDateTime closedatetime=QDateTime::currentDateTime();
    QDateTime opendatetime;
    QDate   closedate=closedatetime.date();
    QDate   opendate;

    int realdate=1;
    opendatetime=this->checkout_model->record(0).value("opendate").toDateTime();
    opendate=opendatetime.date();
    if(closedate.year()==opendate.year())
    {
        realdate +=closedate.dayOfYear()-opendate.dayOfYear();
    }
    else
    {
        int tempyear=opendate.year();
        while(tempyear!=closedate.year())
        {
            if(QDate::isLeapYear(opendate.year()))
            {
                realdate +=366;
            }
            else
            {
                realdate +=365;
            }
            tempyear++;
        }
        realdate +=closedate.dayOfYear()-opendate.dayOfYear();
    }

    //将实际天数显示在  text 中
    ui->leRealdate->setText(QString::number(realdate));

    int price=0;
    QString rid;
    rid=this->checkout_model->record(0).value("rid").toString();

    //执行 query 语句 查找price
    QSqlQuery query;
    query.prepare("select roomtype.price from roomtype where roomtype.tid=(select room.tid from room where room.rid=:rid)");
    query.bindValue(":rid",rid);
    query.exec();

    if(query.first())
    {
        price=query.value(0).toInt();
        price *=realdate;
    }


    //当前数据 显示到text中
    ui->lePay->setText(QString::number(price));

    //更新checkin及checkout表格。
    QString ID=this->checkout_model->record(0).value("cnumber").toString();
    query.prepare("delete from checkin where cnumber=:cnumber and opendate=:opendate");
    query.bindValue(":cnumber",ID);
    query.bindValue(":opendate",opendatetime);
    query.exec();

    query.prepare("insert into checkout (cnumber,rid,opendate,closedate,cash) values(:cnumber,:rid,:opendate,:closedate,:cash)");
    query.bindValue(":cnumber",ID);
    query.bindValue(":rid",rid);
    query.bindValue(":opendate",opendatetime);
    query.bindValue(":closedate",closedatetime);
    query.bindValue(":cash",price);
    query.exec();

    //更改 room 的状态
    query.prepare("update room set  room.state='空闲' where rid=:rid");
    query.bindValue(":rid", rid);
    query.exec();

}

//  tab4 查询记录信息
void MainWindow::on_tabWidget_currentChanged(int index)
{
    if(index==3)
    {
        model_checkin->select();
        //注意hideColumn的位置
        ui->tvCheckin->setModel(this->model_checkin);
        model_checkout->select();
        ui->tvCheckout->setModel(this->model_checkout);
    }
    else if(index==2)
    {
        this->checkout_model->select();
        ui->tvQuery->setModel(checkout_model);
        ui->leRealdate->setEnabled(false);
        ui->lePay->setEnabled(false);
    }
}
