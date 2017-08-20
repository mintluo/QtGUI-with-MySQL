#include "filesender.h"
#include "ui_filesender.h"

FileSender::FileSender(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FileSender)
{
    ui->setupUi(this);
}

FileSender::~FileSender()
{
    delete ui;
}

void FileSender::on_btnQuit_clicked()
{

}
