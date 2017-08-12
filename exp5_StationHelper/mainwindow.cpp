#include <QMessageBox>
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    model = new QSqlRelationalTableModel(this);
    model->setTable("stations_train_pass");//详细信息要打开的表
    model->setEditStrategy(QSqlRelationalTableModel::OnManualSubmit);//表明提交修改才能生效
    model->setRelation(1,QSqlRelation("stations","sid","sname"));//stations_train_pass表中的sid(第1列)将由stations表中的sname关联显示
    model->select();
    ui->FirstBtn->setEnabled(false);
    ui->PrevBtn->setEnabled(false);
    query.exec("select * from trains");
    if(query.next())
    {
        ui->Ttype_LineEdit->setText(query.value(1).toString());
        ui->Tid_LineEdit->setText(query.value(0).toString());
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_CheckBtn_clicked()
{

    QString tid = ui->Tra_Num_Line->text();
    QSqlQuery query;
    ui->tableWidget->removeRow(0);
    query.exec("select * from trains where tid="+tid);
    if(query.next())
    {
        QString ttype=query.value(1).toString();
        QString startStation=query.value(2).toString();//获取的是INT型的站id号
        QString endStation=query.value(3).toString();
        QString startStationName,endStationName;
        query.exec("select sname from stations where sid="+startStation);
        if(query.next())
        {
            startStationName=query.value(0).toString();//与stations表中的sname关联，通过sid获取sname
            query.exec("select sname from stations where sid="+endStation);
            if(query.next())
            {
                endStationName=query.value(0).toString();
                ui->tableWidget->insertRow(0);
                ui->tableWidget->setItem(0,0,new QTableWidgetItem(tid));
                ui->tableWidget->setItem(0,1,new QTableWidgetItem(ttype));
                ui->tableWidget->setItem(0,2,new QTableWidgetItem(startStationName));
                ui->tableWidget->setItem(0,3,new QTableWidgetItem(endStationName));
            }
        }
    }
    else
    {
        QMessageBox::warning(this,tr("error"),tr("not found!"));
    }
}

void MainWindow::on_FirstBtn_clicked()
{
    query.first();
    QString tid = query.value(0).toString();
    QString type = query.value(1).toString();
    ui->Ttype_LineEdit->setText(type);
    ui->Tid_LineEdit->setText(tid);
    ui->NextBtn->setEnabled(true);
    ui->LastBtn->setEnabled(true);
    ui->FirstBtn->setEnabled(false);
    ui->PrevBtn->setEnabled(false);
}

void MainWindow::on_PrevBtn_clicked()
{
    if(query.previous())
    {
        QString tid = query.value(0).toString();
        QString type = query.value(1).toString();
        ui->Ttype_LineEdit->setText(type);
        ui->Tid_LineEdit->setText(tid);

        if(!query.previous())//尝试去取前一个，若已经是顶部，提前将按钮置无效
        {
            ui->FirstBtn->setEnabled(false);
            ui->PrevBtn->setEnabled(false);
        }
        query.next();
    }
    else
    {
        ui->FirstBtn->setEnabled(false);
        ui->PrevBtn->setEnabled(false);
        query.next();
    }
    ui->NextBtn->setEnabled(true);
    ui->LastBtn->setEnabled(true);
}

void MainWindow::on_NextBtn_clicked()
{
    if(query.next())
    {
        QString tid = query.value(0).toString();
        QString type = query.value(1).toString();
        ui->Ttype_LineEdit->setText(type);
        ui->Tid_LineEdit->setText(tid);

        if(!query.next())//尝试去取下一个，若已经是底部，提前将按钮置无效
        {
            ui->NextBtn->setEnabled(false);
            ui->LastBtn->setEnabled(false);
        }
        query.previous();
    }
    else
    {
        ui->NextBtn->setEnabled(false);
        ui->LastBtn->setEnabled(false);
        query.previous();
    }
    ui->FirstBtn->setEnabled(true);
    ui->PrevBtn->setEnabled(true);
}

void MainWindow::on_LastBtn_clicked()
{
    query.last();
    QString tid = query.value(0).toString();
    QString type = query.value(1).toString();
    ui->Ttype_LineEdit->setText(type);
    ui->Tid_LineEdit->setText(tid);
    ui->NextBtn->setEnabled(false);
    ui->LastBtn->setEnabled(false);
    ui->FirstBtn->setEnabled(true);
    ui->PrevBtn->setEnabled(true);
}

void MainWindow::on_DetailBtn_clicked()
{
    QString name = ui->Tra_Num_Line->text();
    model->setFilter(QObject::tr("tid= '%1'").arg(name));//过滤出tid匹配的信息
    model->select();
    if(model->rowCount()==0)
    {
        QMessageBox::warning(this,tr("error"),tr("not found!"));
    }
    else
    {
        QTableView *view = new QTableView;
        view->setModel(model);//显示绑定的视图
        view->setFixedSize(sizeHint());
        view->show();
    }
}
