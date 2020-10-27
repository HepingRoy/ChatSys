#include "widget.h"
#include "ui_widget.h"
#include<QFileDialog>
#include<QFile>
#include<QHostAddress>





Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    connect(&tcpSocket,SIGNAL(readyRead()),
            this,SLOT(onReadyRead()));

    connect(&tcpSocket,SIGNAL(connected()),
            this,SLOT(onConnected()));

    connect(&tcpSocket,SIGNAL(disconnected()),
            this,SLOT(onDisconnected()));

    connect(&tcpSocket,SIGNAL(error(QAbstractSocket::SocketError)),
            this,SLOT(onError(QAbstractSocket::SocketError)));

    imageIndex=0;
    sizePackLast=0;

   // tcpSocket.connectToHost("81.68.107.40",8888);
      tcpSocket.connectToHost(Host,8888);


}



Widget::~Widget()
{
    delete ui;
}




void Widget::onReadyRead()     //接收信息
{
    QObject *obj= this->sender();
    QTcpSocket *socket=qobject_cast<QTcpSocket*>(obj);//转换为socket类型

    //当前缓冲区内的数据的大小
    qint64 sizeNow=0;

    QByteArray dataFull;

    do
    {

     sizeNow=socket->bytesAvailable();
     QDataStream stream(socket);

     if(sizePackLast==0)
     {
       if(sizeNow < sizeof(quint32)){
             return;
         }
         stream>>sizePackLast;//已经有值了
     }

     //包不完整，返回
     if(sizeNow<sizePackLast-4)
     {
         return;
     }


     stream>>dataFull;
     qDebug()<<"read:"<<dataFull;

     //判断剩下的字节数是否会有粘包的情况
     sizeNow=socket->bytesAvailable();
     sizePackLast=0;
 }while(sizeNow>0);


            // ui->textMsg->append(socket->localAddress().toString());
    //判断消息类型
        QString prefix=dataFull.mid(0,4);
        if(prefix=="TXT:"){
            QString textContent=dataFull.mid(4);
            ui->textMsg->append(textContent);
            qDebug()<<"OK";
        }
         else if(prefix=="IMG:"){
            qDebug()<<"IMG is received";

            QString htmlTag=QString("<img src=\"%1\"></img>");
            QString index=QString::number(imageIndex++);
            htmlTag=htmlTag.arg(index+".png");

            QFile file(index+".png");
            file.open(QIODevice::WriteOnly);
            file.write(dataFull.mid(4));
            file.close();

            ui->textMsg->insertHtml(htmlTag);//通过html打开图片

        }

}



void Widget::onConnected()
{
 qDebug()<<"连接成功";
}

void Widget::onDisconnected()
{
    QObject *obj= this->sender();//接受
    QTcpSocket *socket=qobject_cast<QTcpSocket*>(obj);//转换为socket类型
    socket->deleteLater();//删除socket

    qDebug()<<"断开连接";
}

void Widget::onError(QAbstractSocket::SocketError socketError)
{
qDebug()<<"error:"<<socketError;
}



void Widget::on_textSend_clicked()
{
    QString msgInput="TXT:"+ui->textInput->toPlainText();


    std::string str =msgInput.toStdString();//将Qstring转换为char(先转为string再转为char)
    const char* ch = str.c_str();

    //封装包头
    QByteArray dataSend;//封装成的数据包
    QDataStream stream(&dataSend,QIODevice::WriteOnly);
    //stream<<(quint32)0<<msgInput.toLocal8Bit();
    stream<<(quint32)0<<ch;
    stream.device()->seek(0);//找到第一个0的位置
    stream<<dataSend.size();//把0改成datasend的大小

    tcpSocket.write(dataSend);
    ui->textInput->clear();
;   // tcpSocket.write(msgInput.toLocal8Bit());
}

void Widget::on_image_clicked()
{
    //QByteArray hostname=QHostInfo::localHostName().toLatin1();
    //tcpSocket.write(hostname);

    //读取图片
     QString image=QFileDialog::getOpenFileName(this,"title",".","Image Files(*.png *.jpg *.bmp)");
     if(image.isEmpty())
         return;
     QFile file(image);
     file.open(QIODevice::ReadOnly);
     QByteArray data="IMG:"+file.readAll();
     file.close();

     //封装包头
     QByteArray dataSend;//封装成的数据包
     QDataStream stream(&dataSend,QIODevice::WriteOnly);
     stream<<(quint32)0<<data;
     stream.device()->seek(0);//找到第一个0的位置
     stream<<dataSend.size();//把0改成datasend的大小

     //发送图片
      tcpSocket.write(dataSend);



}
