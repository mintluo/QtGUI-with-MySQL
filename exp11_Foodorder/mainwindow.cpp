#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pbMain_Manager_clicked()
{

}

void MainWindow::on_pbMain_Order_clicked()
{

}

void MainWindow::on_pbMain_Search_clicked()
{

}

void MainWindow::on_pbMain_Count_clicked()
{

}
