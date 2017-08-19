#include "dlgsearch.h"
#include "ui_dlgsearch.h"

dlgsearch::dlgsearch(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::dlgsearch)
{
    ui->setupUi(this);
}

dlgsearch::~dlgsearch()
{
    delete ui;
}

void dlgsearch::on_pbClientDetail_clicked()
{

}

void dlgsearch::on_pbCancel_clicked()
{

}

void dlgsearch::init()
{

}
