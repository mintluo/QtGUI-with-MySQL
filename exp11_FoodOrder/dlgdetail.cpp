#include "dlgdetail.h"
#include "ui_dlgdetail.h"

dlgdetail::dlgdetail(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dlgdetail)
{
    ui->setupUi(this);
}

dlgdetail::~dlgdetail()
{
    delete ui;
}

void dlgdetail::on_pbOk_clicked()
{

}

void dlgdetail::datainit()
{

}
