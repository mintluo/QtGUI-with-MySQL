#include "dlgcount.h"
#include "ui_dlgcount.h"

dlgcount::dlgcount(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dlgcount)
{
    ui->setupUi(this);
}

dlgcount::~dlgcount()
{
    delete ui;
}

void dlgcount::on_pbExit_clicked()
{

}

void dlgcount::on_pbCount_clicked()
{

}

void dlgcount::on_pbSearchInfo_clicked()
{

}
