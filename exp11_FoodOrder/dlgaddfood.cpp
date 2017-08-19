#include "dlgaddfood.h"
#include "ui_dlgaddfood.h"

dlgaddfood::dlgaddfood(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dlgaddfood)
{
    ui->setupUi(this);
}

dlgaddfood::~dlgaddfood()
{
    delete ui;
}

void dlgaddfood::on_pbAdd_clicked()
{

}

void dlgaddfood::on_pbExit_clicked()
{

}

void dlgaddfood::clear()
{

}
