#include "chatwidget.h"
#include "ui_chatwidget.h"
#include <QFileDialog>
#include <QColorDialog>
#include <QScrollBar>
#include <QKeyEvent>

//此函数是网上的  将html格式的qstring 转换
QString simplifyRichText( QString f_richText )
{
    QRegExp rxBody("<body[^>]*>(.*)</body>");
    if(rxBody.indexIn(f_richText))
    {
        f_richText = rxBody.cap(1); // throw away anything not in the body, and the body tags as well
        f_richText.replace(QRegExp("^\\s+"),""); // throw away any leading whitespace (be it new lines or spaces)
    }
    QRegExp rxBody1("<span[^>]*>(.*)</span>");
    if(f_richText.indexOf(rxBody1) != -1){
        if(rxBody1.indexIn(f_richText))
        {
            f_richText = rxBody1.cap(1); // throw away anything not in the body, and the body tags as well
            f_richText.replace(QRegExp("^\\s+"),""); // throw away any leading whitespace (be it new lines or spaces)
        }
    }
    f_richText.replace(QRegExp("<p style=\"[^\"]*\">"),"<p>"); // throw away paragraph styles


    if(f_richText == "<p></p>") // thats what we get for an empty QTextEdit
        return QString();
    f_richText.replace("<p>","");
    f_richText.replace("</p>","");

    return f_richText;
}

//初始化    连接编辑框 和字体装换的信号与槽  设置位置。。
ChatWidget::ChatWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChatWidget)
{
    ui->setupUi(this);
    ui->tbHistory->hide();//隐藏聊天记录栏
    ui->teEdit->setFocusPolicy(Qt::StrongFocus);//设置焦点在消息框
    ui->teEdit->setFocus();
    ui->teEdit->installEventFilter(this);//安装事件过滤器

    this->resize(500,400);

    connect(ui->teEdit,SIGNAL(currentCharFormatChanged(QTextCharFormat)),this,SLOT(currentFormatChanged(const QTextCharFormat)));
}

ChatWidget::~ChatWidget()
{
    delete ui;
}

//收到主窗体发来的聊天消息  对比是否是此聊天框接收  并将聊天记录储存 显示
void ChatWidget::setMessage(QString address, QString first, QString second)
{
    if(address == secretAddress ){
        ui->tbShow->setTextColor(Qt::blue);
        ui->tbShow->setCurrentFont(QFont("Times New Roman",12));
        ui->tbShow->append(first);
        ui->tbShow->append(second);

//        QMessageBox::information(this,tr("a"),tr("%1\n%2").arg(first).arg(second));

        if(!saveFile.open(QFile::WriteOnly | QFile::Text | QFile::Append))

        {
            QMessageBox::warning(this,tr("保存文件"),
                                 tr("无法保存文件 :\n %1").arg(saveFile.errorString()));
            return;
        }
        QTextStream out(&saveFile);//保存聊天内容
        out << first <<endl << simplifyRichText(second) <<endl;

//        out << first.trimmed() << endl << second.trimmed()<<endl;

        saveFile.close();
    }
}

//获取本机ip
QString ChatWidget::getIP()  //获取ip地址
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

//获取此聊天框对应的聊天人ip 用不到？
QString ChatWidget::getSecAddr()
{
    return this->secretAddress;
}

//公有函数  在创建之前设置聊天人ip 并创建聊天记录文件 存在saveFile  相对路径
void ChatWidget::setSecAddr(QString addr,QString niname)
{
    this->secretAddress = addr;
    if(secretAddress == "NULL"){//是否是群聊
        ui->lbConn->setText(tr("对方ip: All"));
        ui->textSave->hide();
    }
    else
        ui->lbConn->setText(tr("对方ip：%1").arg(secretAddress));
    QDir dir;
    dir.mkpath("history");

    saveFile.setFileName(tr("history\\%1%2.chat").arg(addr).arg(niname));
    this->setWindowTitle(tr("聊天--%1").arg(niname));
}

//发送消息给聊天人 触发主窗口的槽函数  并显示储存消息
void ChatWidget::on_btnSend_clicked()
{
    if(ui->teEdit->toPlainText() == "")
    {
        QMessageBox::warning(0,tr("警告"),tr("发送内容不能为空"),QMessageBox::Ok);
        return;
    }
    QString time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    QString message = ui->teEdit->toHtml();
    if(secretAddress != "NULL"){
        ui->tbShow->setTextColor(Qt::blue);
        ui->tbShow->setCurrentFont(QFont("Times New Roman",12));
        ui->tbShow->append(tr("[ 你说 ]  %1").arg(time));
        ui->tbShow->append(message);

        if(!saveFile.open(QFile::WriteOnly | QFile::Text | QFile::Append))
        {
            QMessageBox::warning(this,tr("保存文件"),
                                 tr("无法保存文件 :\n %1").arg(saveFile.errorString()));
            return;
        }
        QTextStream out(&saveFile);
        out <<tr("[ 你说 ]  %1").arg(time)<<endl << simplifyRichText(message)<<endl;

        //        out <<tr("[ 你说 ]  %1").arg(time).trimmed()<<endl << message.trimmed()<<endl;
        saveFile.close();
    }

    ui->teEdit->clear();
    ui->teEdit->setFocus();

    emit sendMessagesFromChat(message,secretAddress);
}
//加粗
void ChatWidget::on_textbold_clicked(bool checked)
{
    if(checked)
        ui->teEdit->setFontWeight(QFont::Bold);
    else
        ui->teEdit->setFontWeight(QFont::Normal);
    ui->teEdit->setFocus();
}

//倾斜
void ChatWidget::on_textitalic_clicked(bool checked)
{
    ui->teEdit->setFontItalic(checked);
    ui->teEdit->setFocus();
}

//下滑线
void ChatWidget::on_textunderline_clicked(bool checked)
{
    ui->teEdit->setFontUnderline(checked);
    ui->teEdit->setFocus();
}

//颜色
void ChatWidget::on_textcolor_clicked()
{
    QColor color = QColorDialog::getColor(color,this);
    if(color.isValid())
    {
        ui->teEdit->setTextColor(color);
        ui->teEdit->setFocus();
    }
}
//事件处理  在输入时按下ctrl + enter 发送
bool ChatWidget::eventFilter(QObject *target, QEvent *event)
{
    if(target == ui->teEdit)
    {
        if(event->type() == QEvent::KeyPress)
        {
            QKeyEvent *k = static_cast<QKeyEvent *>(event);
            //if (k->key() == Qt::Key_Return)
            if (k->key() == Qt::Key_Return && (k->modifiers() & Qt::ControlModifier))
            {
                on_btnSend_clicked();
                return true;
            }
        }
    }
    return QWidget::eventFilter(target,event);
}

//聊天窗口关闭槽函数  发送信号 让主窗体删除 map 表项
void ChatWidget::closeEvent(QCloseEvent *)//离开时触发的  应该检查窗口是否都关闭了！！
{
    emit closeChat(secretAddress);
    //    this->destroyed(0);
}

//关闭 触发槽
void ChatWidget::on_btnClose_clicked()
{
    this->hide();
}

//清空聊天框内容
void ChatWidget::on_textBin_clicked()
{
    ui->tbShow->clear();
}

//发送文件 触发主窗口的槽  让接收方确认  群发时此按钮hide
void ChatWidget::on_textSave_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this);
    if(fileName.isEmpty())
    {
        QMessageBox::information(this,tr("警告"),tr("请选择文件"));
        return;
    }

    emit sendFile(fileName,secretAddress);
}

//具体用处没发现
void ChatWidget::currentFormatChanged(const QTextCharFormat &format)
{//当编辑器的字体格式改变时，我们让部件状态也随之改变
    ui->fontComboBox->setCurrentFont(format.font());

    if(format.fontPointSize()<9)  //如果字体大小出错，因为我们最小的字体为9
        ui->fontComboBox->setCurrentIndex(3); //即显示12
    else ui->comboBox->setCurrentIndex(
            ui->comboBox->findText(QString::number(format.fontPointSize())));

    ui->textbold->setChecked(format.font().bold());
    ui->textitalic->setChecked(format.font().italic());
    ui->textunderline->setChecked(format.font().underline());
    color = format.foreground().color();
}

//字体大小
void ChatWidget::on_comboBox_currentIndexChanged(QString size)
{
    ui->teEdit->setFontPointSize(size.toDouble());
    ui->teEdit->setFocus();
}

//显示聊天记录的按键
void ChatWidget::on_textHistory_clicked()
{
    if(ui->tbHistory->isHidden())
    {
        if(!saveFile.open(QFile::ReadOnly))
        {
            QMessageBox::warning(this,tr("保存文件"),
                                 tr("无法保存文件 :\n %1").arg(saveFile.errorString()));
            return ;
        }

        QTextStream t(&saveFile);

        ui->tbHistory->setText(t.readAll());//显示聊天记录
        ui->tbHistory->verticalScrollBar()->setValue(ui->tbHistory->verticalScrollBar()->maximum());
        saveFile.close();
        ui->tbHistory->show();
        this->resize(800,400);
    }
    else{
        ui->tbHistory->hide();
        this->resize(500,400);
    }
}
