#include "dlgfoodmanager.h"
#include "ui_dlgfoodmanager.h"

dlgfoodmanager::dlgfoodmanager(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dlgfoodmanager)
{
    ui->setupUi(this);
}

dlgfoodmanager::~dlgfoodmanager()
{
    delete ui;
}

void dlgfoodmanager::on_pbFresh_clicked()
{

}

void dlgfoodmanager::on_pbAdd_clicked()
{

}

void dlgfoodmanager::on_pbUpdate_Delete_clicked()
{

}

void dlgfoodmanager::on_pbExit_clicked()
{

}

void dlgfoodmanager::type(int index)
{

}
