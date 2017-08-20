#include "widget.h"
#include "ui_widget.h"
#include <QFileDialog>

//初始化函数  文件传输只有这里一个变量 所以直接连接信号与槽
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    this->resize(300,550);
    this->move(1000,50);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);

    Sender = new FileSender(this);
    udpSocket = new QUdpSocket(this);
    port = 45454;
    udpSocket->bind(port,QUdpSocket::ShareAddress
                    | QUdpSocket::ReuseAddressHint);
    connect(udpSocket,SIGNAL(readyRead()),this,SLOT(processPendingDatagrams()));
    connect(Sender,SIGNAL(sendFileName(QString,QString)),this,SLOT(sentFileName(QString,QString)));
}

Widget::~Widget()
{
    delete ui;
}

//公有函数  在登录成功后调用  设置昵称  并广播加入网络
void Widget::SetName(QString name)
{
    Name = name;
    sendMessage(NewParticipant);
}

void Widget::on_btnAll_clicked()
{
    if(list.contains("NULL"))
    {//窗口存在
        list["NULL"]->raise();
        list["NULL"]->activateWindow();
        list["NULL"]->show();
        //QMessageBox::information(this,tr("a"),tr("cunzai"));
    }else {
        ChatWidget *chatView = new ChatWidget();
        list.insert("NULL",chatView);
        connect(chatView,SIGNAL(closeChat(QString)),this,SLOT(closeFromChat(QString)));
        connect(this,SIGNAL(sendMessagesZ(QString,QString,QString)),chatView,SLOT(setMessage(QString,QString,QString)));
        connect(chatView,SIGNAL(sendMessagesFromChat(QString,QString)),this,SLOT(sendMessagesToMain(QString,QString)));
        //发送消息
        chatView->setSecAddr("NULL",Name);//设置群聊地址NULL
        chatView->show();
    }
}

void Widget::on_btnCancel_clicked()
{
    this->close();
}

//接收数据  并根据type 分类
void Widget::processPendingDatagrams()   //接收数据UDP
{
    while(udpSocket->hasPendingDatagrams())
    {
        QByteArray datagram;
        datagram.resize(udpSocket->pendingDatagramSize());//获取缓存大小
        udpSocket->readDatagram(datagram.data(),datagram.size());//读取数据
        QDataStream in(&datagram,QIODevice::ReadOnly);
        int messageType;
        in >> messageType;
        QString localHostName,ipAddress,message,secretAddress;
        QString time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
        switch(messageType)
        {
        case RefuseChat:
//            拒绝聊天
            {
                in  >>localHostName >>ipAddress >> secretAddress;//接收主机名、IP、对方私有IP，与发送对应
                if(secretAddress == getIP())
                    QMessageBox::information(this,tr("拒绝"),tr("来自（%1）的%2  拒绝你的聊天").arg(ipAddress).arg(localHostName));
                break;
            }
        case StartChat:
//            开始聊天
            {
                in  >>localHostName >>ipAddress >> secretAddress;
                if(secretAddress == getIP())
                {
                    ChatWidget *chatView = new ChatWidget();//新建聊天窗口
                    list.insert(ipAddress,chatView);
                    connect(chatView,SIGNAL(closeChat(QString)),this,SLOT(closeFromChat(QString)));
                    connect(this,SIGNAL(sendMessagesZ(QString,QString,QString)),chatView,SLOT(setMessage(QString,QString,QString)));
                    connect(chatView,SIGNAL(sendMessagesFromChat(QString,QString)),this,SLOT(sendMessagesToMain(QString,QString)));
                    connect(chatView,SIGNAL(sendFile(QString,QString)),this,SLOT(hasFileToSend(QString,QString)));
                    //设置地址和用户名，发送消息
                    chatView->setSecAddr(ipAddress,Name);
                    chatView->show();
                }
                break;
            }
        case AskStartChat:
//            申请聊天
            in  >>localHostName >>ipAddress >> secretAddress;
            if(secretAddress == getIP())
            {//是发给自己的
                int btn = QMessageBox::information(this,tr("同意聊天"),
                                                   tr("来自%1(%2)的聊天,是否接收？")
                                                   .arg(ipAddress).arg(localHostName),
                                                   QMessageBox::Yes,QMessageBox::No);
                if(btn == QMessageBox::Yes)
                {//同意聊天  新建窗口
                    ChatWidget *chatView = new ChatWidget();
                    list.insert(ipAddress,chatView);
                    connect(chatView,SIGNAL(closeChat(QString)),this,SLOT(closeFromChat(QString)));
                    connect(this,SIGNAL(sendMessagesZ(QString,QString,QString)),chatView,SLOT(setMessage(QString,QString,QString)));
                    connect(chatView,SIGNAL(sendMessagesFromChat(QString,QString)),this,SLOT(sendMessagesToMain(QString,QString)));
                    connect(chatView,SIGNAL(sendFile(QString,QString)),this,SLOT(hasFileToSend(QString,QString)));
                    //发送消息
                    chatView->setSecAddr(ipAddress,Name);
                    chatView->show();
                    sendMessage(StartChat,ipAddress);
                }else if(btn == QMessageBox::No)
                {
                    sendMessage(RefuseChat,ipAddress);
                }
            }
            break;
        case Message://开始传送消息，查看窗口是否打开，并传送消息，之前是否请求？
//            未打开就打开  打开聊天窗口会连接一些 信号与槽
//            分别是 发送消息 接收消息 发送文件 关闭窗口
            {
                in  >>localHostName >>ipAddress >> secretAddress >>message;
//                QMessageBox::information(this,tr("0"),tr("%1sec\n%2").arg(secretAddress).arg(ipAddress));
                if(secretAddress == getIP() )//发给自己的私有
                {
//                    QMessageBox::information(this,tr("0"),tr("%1sec").arg(secretAddress));
                    if(list.contains(ipAddress)){
                        list[ipAddress]->show();
                        emit sendMessagesZ(ipAddress,tr("[ %1 ] %2").arg(localHostName).arg(time),message);
                    }else{
                        ChatWidget *chatView = new ChatWidget();
                        list.insert(ipAddress,chatView);
                        connect(chatView,SIGNAL(closeChat(QString)),this,SLOT(closeFromChat(QString)));
                        connect(this,SIGNAL(sendMessagesZ(QString,QString,QString)),chatView,SLOT(setMessage(QString,QString,QString)));
                        connect(chatView,SIGNAL(sendMessagesFromChat(QString,QString)),this,SLOT(sendMessagesToMain(QString,QString)));
                        connect(chatView,SIGNAL(sendFile(QString,QString)),this,SLOT(hasFileToSend(QString,QString)));
                        //发送消息
                        chatView->setSecAddr(ipAddress,Name);
                        chatView->show();
                        emit sendMessagesZ(ipAddress,tr("[ %1 ] %2").arg(localHostName).arg(time),message);
                    }
                }else if(secretAddress == "NULL")//群消息
                {
                    if(list.contains("NULL")){
                        list["NULL"]->show();
                        emit sendMessagesZ(secretAddress,tr("[ %1 ] %2").arg(localHostName).arg(time),message);
                    }else{
                        ChatWidget *chatView = new ChatWidget();
                        list.insert(secretAddress,chatView);
                        connect(chatView,SIGNAL(closeChat(QString)),this,SLOT(closeFromChat(QString)));
                        connect(this,SIGNAL(sendMessagesZ(QString,QString,QString)),chatView,SLOT(setMessage(QString,QString,QString)));
                        connect(chatView,SIGNAL(sendMessagesFromChat(QString,QString)),this,SLOT(sendMessagesToMain(QString,QString)));
                        connect(chatView,SIGNAL(sendFile(QString,QString)),this,SLOT(hasFileToSend(QString,QString)));
                        //发送消息
                        chatView->setSecAddr(secretAddress,Name);
                        chatView->show();
                        emit sendMessagesZ(secretAddress,tr("[ %1 ] %2").arg(localHostName).arg(time),message);
                    }
                }
                break;
            }
        case NewParticipant://新用户加入 这里不需要修改
            {
                in  >>localHostName >>ipAddress;
                newParticipant(localHostName,ipAddress);
                break;
            }
        case ParticipantLeft://用户离开  不需要修改
            {
                in  >>localHostName;
                participantLeft(localHostName);
                break;
            }
        case FileName://发送文件
            {
                in  >>localHostName >> ipAddress;
                QString clientAddress,fileName;
                in >> clientAddress >> fileName;
                QMessageBox::information(this,tr(""),tr("%1ipaddr\n%2sev\n%3file").arg(ipAddress).arg(clientAddress).arg(message));
                hasPendingFile(localHostName,ipAddress,clientAddress,fileName);
                break;
            }
        case Refuse://拒绝文件
            {
                in  >> localHostName;
                QString serverAddress;
                in >> ipAddress >> serverAddress;

                if(getIP() == serverAddress)
                {
                    this->Sender->refused();
                }
                break;
            }
        }
    }
}

//接收方有文件需要接收，选择同意后拒绝  并打开接收窗口 连接发送方
void Widget::hasPendingFile(QString localHostName, QString serverAddress,  //接收文件
                            QString clientAddress,QString fileName)
{
    QString ipAddress = getIP();
    if(ipAddress == clientAddress)
    {
        int btn = QMessageBox::information(this,tr("接受文件"),
                                           tr("来自%1(%2)的文件：%3,是否接收？")
                                           .arg(localHostName).arg(serverAddress).arg(fileName),
                                           QMessageBox::Yes,QMessageBox::No);
        if(btn == QMessageBox::Yes)
        {
            QString name = QFileDialog::getSaveFileName(0,tr("保存文件"),fileName);
            if(!name.isEmpty())
            {
                FileReceiver *receiver = new FileReceiver(this);
                receiver->setFileName(name);
                receiver->setHostAddress(QHostAddress(serverAddress));
                receiver->move(500,200);
                receiver->show();
            }

        }
        else{
            sendMessage(Refuse,serverAddress);
        }
    }
}


//双击用户 弹出对应聊天框 所有聊天框由qmap 来管理 并在关闭槽函数中 统一销毁
void Widget::on_tableWidget_doubleClicked(QModelIndex index)
{
    QString secretAddress = ui->tableWidget->item(index.row(),1)->text();//对方ip
        if(secretAddress == getIP())//和自己聊天
        {
            QMessageBox::information(this,tr("警告"),tr("你在尝试和自己聊天"));
            return;
        }
    if(list.contains(secretAddress))//聊天窗口已经存在
    {
        list[secretAddress]->raise();
        list[secretAddress]->activateWindow();//激活窗口
        list[secretAddress]->show();//将点中的聊天框设为焦点
        return;
    }
    sendMessage(AskStartChat,secretAddress);//聊天窗口不存在，请求聊天
}

//发送消息 和接收消息相对  根据type 区分
void Widget::sendMessage(MessageType type, QString serverAddress,QString message)  //发送信息
{
    if(Name == "")
        return;

    QByteArray data;
    QHostAddress addr;
    QDataStream out(&data,QIODevice::WriteOnly);
    QString localHostName = Name;
    QString address = getIP();
    out << type<< localHostName;
    switch(type)
    {
    case ParticipantLeft:
        {
            break;
        }
    case NewParticipant:
        {
            out << address;
            break;
        }
    case RefuseChat:
        {
            out<<serverAddress;
            break;
        }
    case StartChat:
        {
            out<<address<<serverAddress;
            break;
        }
    case AskStartChat:
        {
            out<<address<<serverAddress;
            break;
        }
    case Message ://这个他不用发送消息
        {
            out<<address<<serverAddress<<message;
            break;
        }
    case FileName://这个也不用
        {
            out << address << serverAddress << message;
            break;
        }
    case Refuse://拒绝。。
        {
            out << address <<serverAddress;
            break;
        }
    }
    udpSocket->writeDatagram(data,data.length(),QHostAddress::Broadcast, port);
}

//聊天窗口发送文件信号  这里处理 槽
void Widget::sentFileName(QString fileName,QString Addr)
{
    sendMessage(FileName,Addr,fileName);
}


//主窗口关闭槽函数 引发所有子窗口关闭
void Widget::closeEvent(QCloseEvent *)//离开时触发的  应该检查窗口是否都关闭了！！
{
    QMap<QString,ChatWidget *>::Iterator it;
    while(list.size() > 0)
    {
        it=list.begin();
        list[it.key()]->close();
    }
    sendMessage(ParticipantLeft);
}
//窗体改变事件
void Widget::changeEvent(QEvent *e)
{
    QWidget::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
//处理新用户加入 广播
void Widget::newParticipant(QString localHostName,QString ipAddress)
{
    bool bb = ui->tableWidget->findItems(localHostName,Qt::MatchExactly).isEmpty();
    if(bb)
    {
        QTableWidgetItem *host = new QTableWidgetItem(localHostName);
        QTableWidgetItem *ip = new QTableWidgetItem(ipAddress);
        ui->tableWidget->insertRow(0);
        ui->tableWidget->setItem(0,0,host);
        ui->tableWidget->setItem(0,1,ip);
        ui->lbOnline->setText(tr("在线人数：%1").arg(ui->tableWidget->rowCount()));
        sendMessage(NewParticipant);//自己先加入  然后广播  其他人收到后 也广播自己
    }
}
//处理用户离开 广播
void Widget::participantLeft(QString localHostName)
{
    int rowNum = ui->tableWidget->findItems(localHostName,Qt::MatchExactly).first()->row();
    ui->tableWidget->removeRow(rowNum);
    ui->lbOnline->setText(tr("在线人数：%1").arg(ui->tableWidget->rowCount()));
}
//对方确认接收文件后 sender 触发的发送文件槽函数 并显示进度
void Widget::hasFileToSend(QString fileName, QString SecretAddress)
{
    this->Sender->initSender();
    this->Sender->SenderStart(fileName,SecretAddress);
    this->Sender->move(500,200);
    this->Sender->show();
}
//槽函数 聊天窗口发来的信号 这里处理
void Widget::sendMessagesToMain(QString message,QString secretAddr)
{
    sendMessage(Message,secretAddr,message);
}

//聊天窗口关闭信号触发的槽
void Widget::closeFromChat(QString addr)
{
    list.remove(addr);
}

QString Widget::getIP()  //获取ip地址  貌似在新用户的时候用到
{
    QList<QHostAddress> list = QNetworkInterface::allAddresses();
    foreach (QHostAddress address, list)
    {
       if(address.protocol() == QAbstractSocket::IPv4Protocol) //我们使用IPv4地址
        {
           if(address.toString()!="127.0.0.1")
                return address.toString();
        }
    }
    return 0;
}
