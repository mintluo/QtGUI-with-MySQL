#include "filereceiver.h"
#include "ui_filereceiver.h"

FileReceiver::FileReceiver(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FileReceiver)
{
    ui->setupUi(this);
}

FileReceiver::~FileReceiver()
{
    delete ui;
}

void FileReceiver::on_btnCancel_clicked()
{

}

void FileReceiver::on_btnClose_clicked()
{

}
