#ifndef FILESENDER_H
#define FILESENDER_H

#include <QDialog>
#include <QTcpServer>
#include <QFile>
#include <QTime>

namespace Ui {
class FileSender;
}

class FileSender : public QDialog
{
    Q_OBJECT

public:
    explicit FileSender(QWidget *parent = 0);
    ~FileSender();
    void refused();
    void initSender();//初始化
    void SenderStart(QString,QString);

signals:
    void sendFileName(QString fileName,QString addr);

private slots:
    void on_btnQuit_clicked();//放弃传送
    void sendMessage();//开始发送数据
    void updateClientProgress(qint64 numBytes);//根据发送情况实时更新进度条

protected:
    void changeEvent(QEvent *e);

private:
    Ui::FileSender *ui;
    qint16 tcpPort;
    QTcpServer *fileSender;//TCP服务器对象
    QString fileName;
    QString theFileName;
    QFile *localFile;//要发送的文件

    qint64 TotalBytes;//数据总大小
    qint64 bytesWritten;//已发送数据大小
    qint64 bytesToWrite;//剩余数据大小
    qint64 loadSize;//每次发送数据大小
    QByteArray outBlock;//数据缓冲区，存放每次要发送的数据
    QTcpSocket*clientConnection;
    QTime time;//计时器
};
#endif // FILESENDER_H
