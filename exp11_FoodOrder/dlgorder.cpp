#include "dlgorder.h"
#include "ui_dlgorder.h"
#include <QSqlQuery>
#include <QMessageBox>
#include <QAbstractItemView>
#include <QDebug>
#include <QModelIndex>
#include <QSqlRecord>
#include <QString>
#include <QTableWidgetItem>
#include <QSqlError>
#include <QFile>
#include <QIODevice>
#include <QTextStream>
#include <QDateTime>

dlgorder::dlgorder(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dlgorder)
{
    ui->setupUi(this);

    //初始化食物列表 TableView
    model=new QSqlTableModel(this);
    this->model->setTable("food");
    ui->tvList->setModel(this->model);
    ui->tvList->setEditTriggers(QAbstractItemView::NoEditTriggers);     //使其不可编辑
    this->model->removeColumn(this->model->fieldIndex("ftype"));
    this->model->removeColumn(this->model->fieldIndex("flag"));
    this->model->setHeaderData(0,Qt::Horizontal,tr("编号"));
    this->model->setHeaderData(1,Qt::Horizontal,tr("名称"));
    this->model->setHeaderData(2,Qt::Horizontal,tr("单价(元)"));
    this->model->setHeaderData(3,Qt::Horizontal,tr("热度"));
    //获取数据
    int index=ui->cbType->currentIndex();
    this->type(index);

    //初始化订单列表 TableWidget
    ui->twOrder->setEditTriggers(QAbstractItemView::NoEditTriggers);    //使其不可编辑

    //信号与槽函数链接
    connect(ui->cbType,SIGNAL(activated(int)),this,SLOT(type(int)));
}

dlgorder::~dlgorder()
{
    delete ui;
}

void dlgorder::on_pbFresh_clicked()
{
    dlgorder *dlg=new dlgorder(this);
    dlg->show();
    this->hide();
}

void dlgorder::on_pbAdd_clicked()
{
    //判断当前是否有选中的菜
    if(ui->tvList->currentIndex().row()<0)
    {
        QMessageBox::warning(this,tr("提示"),tr("请选择要添加的菜！"),QMessageBox::Ok);
        return;
    }
    //取得当前所选中的食物编号
    QModelIndex index=ui->tvList->currentIndex();
    int fid=this->model->record(index.row()).value("fid").toInt();

    qDebug()<<"fid :"<<fid;
    this->order.append(fid);//将编号保存到数组
    QString name=this->model->record(index.row()).value("fname").toString();    //获取菜名
    int price=this->model->record(index.row()).value("fprice").toInt();         //获取菜价格

    //测试
    qDebug()<<"fname :"<<name<<"    fprice"<<price;

    int mask=0;         //设置重复标记
    int numInt=1;       //数量初始值
    //检测 所点的菜中是否有 与 当前菜相同的菜
    for(int tt=0;tt<ui->twOrder->rowCount();tt++)
    {
        QString cellname=ui->twOrder->item(tt,0)->text();
        qDebug()<<"cellname :"<<cellname;
        //判断有无与当前菜相同的菜
        if(cellname==name)
        {
            QTableWidgetItem *item=ui->twOrder->item(tt,1);
            numInt=item->text().toInt();
            numInt++;
            item->setText(QString::number(numInt));
            ui->twOrder->setItem(tt,1,item);        //修改该菜的数量 +1
            mask=1;
            break;
        }
    }
    if(mask==0)//判断是否已有重复项,如果 没有 则执行 if 里的语句
    {
        QTableWidgetItem *item0=new QTableWidgetItem(name);
        QTableWidgetItem *item1=new QTableWidgetItem(QString::number(numInt));
        QTableWidgetItem *item2=new QTableWidgetItem(QString::number(price));

        int row=ui->twOrder->rowCount();
        qDebug()<<"rows count:"<<row;
        ui->twOrder->setRowCount(row+1);
        ui->twOrder->setItem(row,0,item0);//显示名称
        ui->twOrder->setItem(row,1,item1);// 显示价格
        ui->twOrder->setItem(row,2,item2);//显示数量为1
    }
    this->type(ui->cbType->currentIndex());
}

void dlgorder::on_pbSave_clicked()
{
    //判断输入信息是否完整
    if(ui->leClientID->text().isEmpty()||
            ui->leName->text().isEmpty())
    {
        QMessageBox::warning(this,tr("提示"),tr("请将信息输入完整！"),QMessageBox::Ok);
        return;
    }
    if(ui->twOrder->rowCount()==0)
    {
        QMessageBox::warning(this,tr("提示"),tr("请您先选菜！"),QMessageBox::Ok);
        return;
    }

    QSqlQuery query;        //执行查询sql语句
    QSqlQuery query_update; //执行更新sql语句
    QSqlQuery query_insert; //执行插入sql语句

    int cid=ui->leClientID->text().toInt();//获取顾客标号
    QString cname=ui->leName->text();       //获取顾客姓名

    QString fname;  //菜名
    QString ftype;  //菜类型
    int fprice;     //菜价
    int fhot_count; //菜热度
    bool bAddflag=false;
    bool bRefreshflag=false;
    for(int i=0;i<this->order.size();i++)
    {
        int fid=this->order[i];
        qDebug()<<"fid in vector :"<<fid;
        //执行查询语句 查找当前 编号的菜的详细信息
        query.prepare("select fname,ftype,fprice,fhot_count from food where fid=:fid");
        query.bindValue(":fid",fid);
        if(query.exec())
        {
            if(query.next())
            {
                QSqlRecord rec=query.record();
                fname=query.value(rec.indexOf("fname")).toString();     //获取菜名
                ftype=query.value(rec.indexOf("ftype")).toString();     //获取菜类型
                fprice=query.value(rec.indexOf("fprice")).toInt();      //获取菜价
                fhot_count=query.value(rec.indexOf("fhot_count")).toInt();//获取该菜热度值
            }
            else
            {
                qDebug()<<"没有查询到信息！";
                continue;
            }

            //更新数据库表
            query_insert.prepare("insert into foodorder.orderlist (fname,ftype,fprice,cid,cname) values(:fname,:ftype,:fprice,:cid,:cname)");
            query_insert.bindValue(":fname",fname);
            query_insert.bindValue(":ftype",ftype);
            query_insert.bindValue(":fprice",fprice);
            query_insert.bindValue(":cid",cid);
            query_insert.bindValue(":cname",cname);
            qDebug()<<fname<<ftype<<fprice<<fhot_count<<cid<<cname;
            if(query_insert.exec())
            {
                bAddflag=true;
                //QMessageBox::warning(this,tr("提示"),tr("添加订单成功！"),QMessageBox::Ok);
            }
            else
            {
                QMessageBox::warning(this,tr("提示"),tr("添加订单失败！"),QMessageBox::Ok);
                return;
            }
            query_update.prepare("update food set fhot_count=:fhot_count where fid=:fid");
            query_update.bindValue(":fhot_count",++fhot_count);
            query_update.bindValue(":fid",fid);
            if(query_update.exec())
            {
                bRefreshflag=true;
                //QMessageBox::warning(this,tr("提示"),tr("更新菜单热度成功！"),QMessageBox::Ok);
            }
            else
            {
                QMessageBox::warning(this,tr("提示"),tr("更新菜单热度失败！"),QMessageBox::Ok);
                return;
            }

        }
        else
        {
            QMessageBox::warning(this,tr("提示"),tr("数据库查询出错:%1").arg(query.lastError().text()));
            return;
        }
    }    
    if(bAddflag&&bRefreshflag)
    {
        QMessageBox::warning(this,tr("提示"),tr("添加订单成功！！"),QMessageBox::Ok);
    }
    //---------------------------------------打印订单处理--------------------------------------------///
    QFile list("orderlist.txt");
    if(!list.open(QIODevice::WriteOnly | QIODevice::Text))  //以写的方式打开，并清除原来的数据
    {
        QMessageBox::warning(this,tr("提示"),tr("打印账单出错！"),QMessageBox::Ok);
        return;
    }

    QTextStream out(&list);

    QDateTime datetime=QDateTime::currentDateTime();
    QString lname=QStringLiteral("名称");
    QString lcount=QStringLiteral("单价");
    QString khbh=QStringLiteral("客户编号:");
    QString gkxm=QStringLiteral("顾客姓名:");
    QString yuan=QStringLiteral("元");
    qDebug()<<QStringLiteral("开始打印 订单 ");

    //格式化处理输出信息
    out<<"\t"<<datetime.toString("yyyy年 MM 月 dd日")<<"\n";
    out<<"--------------------------------------"<<"\n";
    out<<khbh<<cid<<"\t"<<gkxm<<cname<<"\n";
    out<<"--------------------------------------"<<"\n";
    out<<lname<<"\t\t"<<lcount<<"\n";
    out<<"--------------------------------------"<<"\n";

    QString tmpname;
    QString tmpprice;
    for(int i=0;i<ui->twOrder->rowCount();i++)
    {
        tmpname=ui->twOrder->item(i,0)->text();
        tmpprice=ui->twOrder->item(i,2)->text();
        out<<tmpname<<"--------------"<<tmpprice<<yuan<<"\n";
    }
    out<<"--------------end---------------"<<"\n";
    list.close();
    qDebug()<<"结束打印 订单";
    QMessageBox::information(this,tr("提示"),tr("打印账单完成！"),QMessageBox::Ok);
}

void dlgorder::on_pbCancel_Order_clicked()
{
    this->close();
}

void dlgorder::type(int index)
{
    //测试
    qDebug()<<"菜类型标号为:"<<index;
    //获取当前类型的所有菜  并按热度值降序排列 （注意 order 前边有空格）
    this->model->setFilter("flag="+QString::number(index)+" order by fhot_count desc");
    this->model->select();
}
