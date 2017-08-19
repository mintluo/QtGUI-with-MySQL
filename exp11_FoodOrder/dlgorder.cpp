#include "dlgorder.h"
#include "ui_dlgorder.h"

dlgorder::dlgorder(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dlgorder)
{
    ui->setupUi(this);
}

dlgorder::~dlgorder()
{
    delete ui;
}

void dlgorder::on_pbFresh_clicked()
{

}

void dlgorder::on_pbAdd_clicked()
{

}

void dlgorder::on_pbSave_clicked()
{

}

void dlgorder::on_pbCancel_Order_clicked()
{

}

void dlgorder::type(int index)
{

}
