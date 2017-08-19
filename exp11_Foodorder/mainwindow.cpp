#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "dlgfoodmanager.h"
#include "dlgorder.h"
#include "dlgsearch.h"
#include "dlgcount.h"
#include <QAbstractItemView>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->model=new QSqlTableModel(this);
    this->model->setTable("food");
    ui->tvMainList->setModel(model);
    //格式化表
    this->model->removeColumn(this->model->fieldIndex("fid"));
    this->model->removeColumn(this->model->fieldIndex("flag"));
    this->model->setHeaderData(0,Qt::Horizontal,tr("名称"));
    this->model->setHeaderData(1,Qt::Horizontal,tr("类型"));
    this->model->setHeaderData(2,Qt::Horizontal,tr("价格"));
    this->model->setHeaderData(3,Qt::Horizontal,tr("热度"));

    //按热度值获取表中数据 并按降序排列(ASC 或 DESC 关键字来设置查询结果是按升序或降序排列)
    this->model->setFilter("fhot_count>30 order by fhot_count desc");
    this->model->select();

    //设置 tableview 不可编辑
    ui->tvMainList->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pbMain_Manager_clicked()
{
    dlgfoodmanager *dlg1=new dlgfoodmanager(this);
    dlg1->show();
}

void MainWindow::on_pbMain_Order_clicked()
{
    dlgorder *dlg2=new dlgorder(this);
    dlg2->show();
}

void MainWindow::on_pbMain_Search_clicked()
{
    dlgsearch  *dlg3=new dlgsearch(this);
    dlg3->show();
}

void MainWindow::on_pbMain_Count_clicked()
{
    dlgcount *dlg4=new dlgcount(this);
    dlg4->show();
}

void MainWindow::on_pbMain_Exit_clicked()
{
    this->close();
}
