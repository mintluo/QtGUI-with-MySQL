#include "dlgdetail.h"
#include "ui_dlgdetail.h"
#include <QAbstractItemView>
#include <QDebug>

dlgdetail::dlgdetail(int cid,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dlgdetail)
{
    ui->setupUi(this);
    this->cid=cid;

    model=new QSqlTableModel(this);
    this->model->setTable("orderlist");
    this->model->removeColumn(this->model->fieldIndex("cid"));
    this->model->setHeaderData(0,Qt::Horizontal,tr("编号"));
    this->model->setHeaderData(1,Qt::Horizontal,tr("菜名"));
    this->model->setHeaderData(2,Qt::Horizontal,tr("菜类型"));
    this->model->setHeaderData(3,Qt::Horizontal,tr("菜价格"));
    this->model->setHeaderData(4,Qt::Horizontal,tr("客户名称"));
    ui->tvDetail->setModel(model);
    ui->tvDetail->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //获取数据
    this->datainit();
}

dlgdetail::~dlgdetail()
{
    delete ui;
}

void dlgdetail::on_pbOk_clicked()
{
    this->close();
}

void dlgdetail::datainit()
{

    qDebug()<<"cid :"<<cid;
    this->model->setFilter("cid="+QString::number(this->cid));
    this->model->select();
}
