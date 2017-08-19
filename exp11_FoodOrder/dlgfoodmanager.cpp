#include "dlgfoodmanager.h"
#include "ui_dlgfoodmanager.h"

#include "dlgaddfood.h"
#include "dlgdelete_update.h"
#include <QAbstractItemView>
#include <QDebug>
#include <QSqlRecord>
#include <QSqlField>
#include <QVariant>
#include <QString>
#include <QModelIndex>
#include <QMessageBox>

dlgfoodmanager::dlgfoodmanager(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dlgfoodmanager)
{
    ui->setupUi(this);

    model=new QSqlTableModel(this);
    this->model->setTable("food");
    ui->tvFoodList->setModel(this->model);
    ui->tvFoodList->setEditTriggers(QAbstractItemView::NoEditTriggers);
    this->model->removeColumn(this->model->fieldIndex("flag"));
    this->model->setHeaderData(0,Qt::Horizontal,tr("编号"));
    this->model->setHeaderData(1,Qt::Horizontal,tr("名称"));
    this->model->setHeaderData(2,Qt::Horizontal,tr("类型"));
    this->model->setHeaderData(3,Qt::Horizontal,tr("价格"));
    this->model->setHeaderData(4,Qt::Horizontal,tr("热度"));
    //获取数据
    int index=ui->cbType->currentIndex();
    this->type(index);

    //信号与槽函数链接
    connect(ui->cbType,SIGNAL(activated(int)),this,SLOT(type(int)));
}

dlgfoodmanager::~dlgfoodmanager()
{
    delete ui;
}
/*--------------初始化食物列表处理-----------*/
void dlgfoodmanager::type(int index)
{
    //测试
    qDebug()<<"菜类型标号为:"<<index;
    //获取当前类型的所有菜  并按热度值降序排列 （注意 order 前边有空格）
    this->model->setFilter("flag="+QString::number(index)+" order by fhot_count desc");
    this->model->select();
}

void dlgfoodmanager::on_pbFresh_clicked()
{
    int index=ui->cbType->currentIndex();
    this->type(index);
}

void dlgfoodmanager::on_pbAdd_clicked()
{
    dlgaddfood *dlgadd=new dlgaddfood(this);
    dlgadd->show();
}

void dlgfoodmanager::on_pbUpdate_Delete_clicked()
{
    //获取当前备选中行的数据
    QModelIndex index=ui->tvFoodList->currentIndex();
    QString fid=this->model->record(index.row()).value(0).toString();
    //测试
    qDebug()<<"fid : "<<fid;
    if(fid.isNull()||fid=="0")
    {
        QMessageBox::warning(this,tr("提示"),tr("没有可更改/删除的菜！"),QMessageBox::Ok);
        return;
    }
    dlgdelete_update *dlg2=new dlgdelete_update(fid,this);
    dlg2->show();
}

void dlgfoodmanager::on_pbExit_clicked()
{
    this->close();
}
