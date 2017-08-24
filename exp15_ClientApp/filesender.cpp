#include "filesender.h"
#include "ui_filesender.h"
#include <QTcpSocket>
#include <QFileDialog>
#include <QMessageBox>

FileSender::FileSender(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FileSender)
{
    ui->setupUi(this);
    this->setFixedSize(350,180);
    tcpPort = 6666;
    fileSender = new QTcpServer();
    connect(fileSender,SIGNAL(newConnection()),this,SLOT(sendMessage()));
}

FileSender::~FileSender()
{
    delete ui;
}

void FileSender::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

//当有新连接时触发 新连接在接受方同意时 发送此信号
void FileSender::sendMessage()  //实现文件大小等信息的发送
{
    clientConnection = fileSender->nextPendingConnection();
    connect(clientConnection,SIGNAL(bytesWritten(qint64)),this,SLOT(updateClientProgress(qint64)));

    ui->serverStatusLabel->setText(tr("开始传送文件 %1 ！").arg(theFileName));

    localFile = new QFile(fileName);
    if(!localFile->open((QFile::ReadOnly))){//以只读方式打开
        QMessageBox::warning(this,tr("应用程序"),tr("无法读取文件 %1:\n%2").arg(fileName).arg(localFile->errorString()));
        return;
    }
    TotalBytes = localFile->size();
    QDataStream sendOut(&outBlock,QIODevice::WriteOnly);//使用数据流写入数据，outBlock用于暂存要发送的数据
    sendOut.setVersion(QDataStream::Qt_4_6);//设置数据流的版本，客户端和服务器端使用的版本要相同
    time.start();  //开始计时
    QString currentFile = fileName.right(fileName.size() - fileName.lastIndexOf('/')-1);//待发送文件名
    //为了保证在客户端能接收到完整的文件，在数据流的最开始写入完整文件的大小信息，这样客户端就可以根据大小信息来判断是否接受到了完整的文件
    sendOut<<qint64(0)<<qint64(0)<<currentFile;//依次写入总大小信息空间（初始化为0），文件名大小信息空间，文件名
    TotalBytes += outBlock.size();//这里的总大小是文件名大小等信息和实际文件大小的总和
    sendOut.device()->seek(0);//回到字节流起始位置
    sendOut<<TotalBytes<<qint64((outBlock.size()-sizeof(qint64)*2));//用实际的大小信息代替两个qint64(0)空间
    bytesToWrite = TotalBytes - clientConnection->write(outBlock);//发送完头数据后剩余数据的大小
    //这里只是先发文件头数据，updateClientProgress(qint64 numBytes)才进行文件数据的传输和进度条的更新
    qDebug()<<currentFile<<TotalBytes;
    outBlock.resize(0);
}

//根据发送情况实时更新进度条，实现文件的传送
void FileSender::updateClientProgress(qint64 numBytes)//更新进度条
{
    bytesWritten += (int)numBytes;//已经发送数据的大小
    if(bytesToWrite > 0){//如果已经发送了数据
        //每次发送loadSize大小的数据，这里设置为4KB，如果剩余的数据不足4KB，
        //就发送剩余数据的大小
        outBlock = localFile->read(qMin(bytesToWrite,loadSize));
        bytesToWrite -= (int)clientConnection->write(outBlock);//发送完一次数据后还剩余数据的大小
        outBlock.resize(0);//清空发送缓冲区
    }
    else{
        localFile->close();//如果没有发送任何数据，则关闭文件
    }
    //更新进度条
    ui->progressBar->setMaximum(TotalBytes);
    ui->progressBar->setValue(bytesWritten);

   float useTime = time.elapsed();
   double speed = bytesWritten / useTime;
   ui->serverStatusLabel->setText(tr("已发送 %1MB (%2MB/s) \n共%3MB 已用时:%4秒\n估计剩余时间：%5秒")
                                  .arg(bytesWritten / (1024*1024))//已发送
                                  .arg(speed*1000/(1024*1024),0,'f',2)//速度
                                  .arg(TotalBytes / (1024 * 1024))//总大小
                                  .arg(useTime/1000,0,'f',0)//用时
                                  .arg(TotalBytes/speed/1000 - useTime/1000,0,'f',0));//剩余时间
    if(bytesWritten == TotalBytes)//发送完毕
        ui->serverStatusLabel->setText(tr("传送文件 %1 成功").arg(theFileName));
}

//被拒绝时触发    主窗体调用
void FileSender::refused()   //被对方拒绝
{
    fileSender->close();
    ui->serverStatusLabel->setText(tr("对方拒绝接收！！！"));
}

//当需要发送文件时  主窗体调用初始化
void FileSender::initSender()//初始化
{
    loadSize = 4*1024;//每次发送4k
    TotalBytes = 0;
    bytesWritten = 0;
    bytesToWrite = 0;
    ui->progressBar->reset();

    fileSender->close();
}

//选择完文件 初始化完成后 自动调用 等待  触发主窗口通知接收方
void FileSender::SenderStart(QString file, QString addr)
{
    this->fileName = file;
    this->theFileName = fileName.right(fileName.size() - fileName.lastIndexOf('/')-1);

    if(!fileSender->listen(QHostAddress::Any,tcpPort))//开始监听
    {
        qDebug() << fileSender->errorString();
        close();
        return;
    }
    ui->serverStatusLabel->setText(tr("等待对方接收... ..."));
    emit sendFileName(theFileName,addr);
   // this->close();
}

//发送完成后点击退出
void FileSender::on_btnQuit_clicked()
{
    if(fileSender->isListening())
        fileSender->close();
    this->close();
}
