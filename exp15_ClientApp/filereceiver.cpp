#include "filereceiver.h"
#include "ui_filereceiver.h"
#include <QFileDialog>
#include <QMessageBox>

FileReceiver::FileReceiver(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FileReceiver)
{
    ui->setupUi(this);
    this->setFixedSize(350,180);

    TotalBytes = 0;
    bytesReceived = 0;
    fileNameSize = 0;

    fileReceiver = new QTcpSocket(this);
    tcpPort = 6666;
    connect(fileReceiver,SIGNAL(readyRead()),this,SLOT(readMessage()));
    connect(fileReceiver,SIGNAL(error(QAbstractSocket::SocketError)),this,
            SLOT(displayError(QAbstractSocket::SocketError)));
}

FileReceiver::~FileReceiver()
{
    delete ui;
}

void FileReceiver::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}//设置服务器地址并连接服务器
void FileReceiver::setHostAddress(QHostAddress address)
{
    hostAddress = address;
    newConnect();
}
//连接服务器
void FileReceiver::newConnect()
{
    blockSize = 0;
    fileReceiver->abort();
    fileReceiver->connectToHost(hostAddress,tcpPort);
    time.start();
}

//槽 读取收到的文件
void FileReceiver::readMessage()
{
    QDataStream in(fileReceiver);
    in.setVersion(QDataStream::Qt_4_6);

    float useTime = time.elapsed();
    if(bytesReceived <= qint64(sizeof(qint64)*2)){//如果接收到的数据小于16个字节，那么是刚开始接收数据，我们保存接收到的头文件信息
        if((fileReceiver->bytesAvailable() >= qint64(sizeof(qint64)*2)) && (fileNameSize == 0)){
            in>>TotalBytes>>fileNameSize;//接收数据总大小信息和文件名大小信息
            bytesReceived += sizeof(qint64)*2;
        }
        if((fileReceiver->bytesAvailable() >= fileNameSize) && (fileNameSize != 0)){
            in>>fileName;//接收文件名，并建立文件
            bytesReceived +=fileNameSize;

            if(!localFile->open(QFile::WriteOnly)){
                QMessageBox::warning(this,tr("应用程序"),tr("无法读取文件 %1:\n%2.").arg(fileName).arg(localFile->errorString()));
                return;
            }
        }
        else
            return;
    }
    if(bytesReceived < TotalBytes){//如果接收的数据小于总数据，那么写入文件
        bytesReceived += fileReceiver->bytesAvailable();
        inBlock = fileReceiver->readAll();
        localFile->write(inBlock);
        inBlock.resize(0);
    }
    //更新进度条
    ui->progressBar->setMaximum(TotalBytes);
    ui->progressBar->setValue(bytesReceived);
    qDebug()<<bytesReceived<<"received"<<TotalBytes;

    double speed = bytesReceived / useTime;
    ui->FileReceiverStatusLabel->setText(tr("已接收 %1MB (%2MB/s) \n共%3MB 已用时:%4秒\n估计剩余时间：%5秒")
                                         .arg(bytesReceived / (1024*1024))//已接收
                                         .arg(speed*1000/(1024*1024),0,'f',2)//速度
                                         .arg(TotalBytes / (1024 * 1024))//总大小
                                         .arg(useTime/1000,0,'f',0)//用时
                                         .arg(TotalBytes/speed/1000 - useTime/1000,0,'f',0));//剩余时间

    if(bytesReceived == TotalBytes)//接收数据完成时
    {
        fileReceiver->close();
        ui->FileReceiverStatusLabel->setText(tr("接收文件 %1 完毕").arg(fileName));
        localFile->close();   //接收完文件后，一定要关闭，不然可能出问题
    }
}
void FileReceiver::displayError(QAbstractSocket::SocketError socketError) //错误处理
{
    switch(socketError)
    {
        case QAbstractSocket::RemoteHostClosedError : break;
        default : qDebug() << fileReceiver->errorString();
    }
}

void FileReceiver::on_btnCancel_clicked()
{

    fileReceiver->abort();
}

void FileReceiver::on_btnClose_clicked()
{

    fileReceiver->abort();
    this->close();
}
