#ifndef FILERECEIVER_H
#define FILERECEIVER_H

#include <QDialog>
#include <QTcpSocket>
#include <QHostAddress>
#include <QFile>
#include <QTime>

namespace Ui {
class FileReceiver;
}

class FileReceiver : public QDialog
{
    Q_OBJECT

public:
    explicit FileReceiver(QWidget *parent = 0);
    ~FileReceiver();
    void setHostAddress(QHostAddress address);
    void setFileName(QString fileName){localFile = new QFile(fileName);}
    void newConnect();

private slots:
    void on_btnCancel_clicked();

    void on_btnClose_clicked();
    void readMessage();//读取收到的文件
    void displayError(QAbstractSocket::SocketError);

protected:
    void changeEvent(QEvent *e);

private:
    Ui::FileReceiver *ui;
    QTcpSocket *fileReceiver;
    quint16 blockSize;//缓存大小
    QHostAddress hostAddress;//服务器地址
    qint16 tcpPort;

    qint64 TotalBytes;//总共需接收的字节数
    qint64 bytesReceived;//已接收字节数
    qint64 bytesToReceive;//待接收字节数
    qint64 fileNameSize;//文件大小
    QString fileName;
    QFile *localFile;
    QByteArray inBlock;

    QTime time;
};

#endif // FILERECEIVER_H
