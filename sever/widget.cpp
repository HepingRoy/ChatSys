#include "widget.h"
#include "ui_widget.h"
#include <QDebug>
#include<QHostAddress>
#include<QFileDialog>
#include<QDataStream>



Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    connect(&server,SIGNAL(newConnection()),
            this,SLOT(onNewConnection()));

    bool a=server.listen(QHostAddress::AnyIPv4,8888);
      qDebug()<<"listen："<<a;

}

Widget::~Widget()
{
    delete ui;
}

void Widget::onNewConnection()
{
    //使用这个socket和客户端通信
    QTcpSocket *socket = server.nextPendingConnection();
    clients.append(socket);

    //连接信号和槽
    connect(socket,SIGNAL(readyRead()),
            this,SLOT(onReadyRead()));

    connect(socket,SIGNAL(connected()),
            this,SLOT(onConnected()));

    connect(socket,SIGNAL(disconnected()),
             this,SLOT(onDisconnected()));

    connect(socket,SIGNAL(error(QAbstractSocket::SocketError )),
            this,SLOT(onError(QAbstractSocket::SocketError )));

    imageIndex=0;
    sizePackLast=0;


}


void Widget::onReadyRead()
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
       if(sizeNow<sizeof(quint32)){
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

      //显示发送者的IP
      ui->textMsg->append(socket->localAddress().toString()+":");

         //转发发送者的IP

        QByteArray dataSendIp;//封装成的数据包
        QDataStream streamI(&dataSendIp,QIODevice::WriteOnly);
        QString clientIp="TXT:"+socket->localAddress().toString()+":";
        std::string str=clientIp.toStdString();
        const char* ch=str.c_str();
        streamI<<(quint32)0<<ch;
        streamI.device()->seek(0);//找到第一个0的位置
        streamI<<dataSendIp.size();//把0改成datasend的大小
        for(QList<QTcpSocket*>::iterator itr=clients.begin();itr!=clients.end();itr++)
        {
           QTcpSocket *client = *itr;
           client->write(dataSendIp);
        }




     //判断消息类型
         QString prefix=dataFull.mid(0,4);
         if(prefix=="TXT:"){                                        //文字
             QString textContent=dataFull.mid(4);
            ui->textMsg->insertHtml("<p>"+textContent+"</p>");

            //转发给客户端
            QByteArray dataSendTXT;//封装成的数据包
            QDataStream streamT(&dataSendTXT,QIODevice::WriteOnly);
            QString data="TXT:"+socket->localAddress().toString()+":";
            str=data.toStdString();
            ch=str.c_str();
            streamT<<(quint32)0<<ch+dataFull.mid(4);
            streamT.device()->seek(0);//找到第一个0的位置
            streamT<<dataSendTXT.size();//把0改成datasend的大小
            for(QList<QTcpSocket*>::iterator itr=clients.begin();itr!=clients.end();itr++){
                QTcpSocket *client = *itr;
                client->write(dataSendTXT);
                //client->write(MsgInput.toLocal8Bit());
            }

        }
         else if(prefix=="IMG:"){                                   //图片

            QString htmlTag=QString("<img src=\"%1\"></img>");
            QString index=QString::number(imageIndex++);
            htmlTag=htmlTag.arg(index+".png");

            QFile file(index+".png");
            file.open(QIODevice::WriteOnly);
            file.write(dataFull.mid(4));
            file.close();

            ui->textMsg->insertHtml(htmlTag);//通过html打开图片

            //转发给客户端

            //封装包头
            QByteArray dataSendImg;//封装成的数据包
            QDataStream streamIm(&dataSendImg,QIODevice::WriteOnly);
            streamIm<<(quint32)0<<dataFull;
            streamIm.device()->seek(0);//找到第一个0的位置
            streamIm<<dataSendImg.size();//把0改成datasend的大小
            //发送图片
            for(QList<QTcpSocket*>::iterator itr=clients.begin();itr!=clients.end();itr++){
                 QTcpSocket *client = *itr;
                 client->write(dataSendImg);
            }

            }


}


void Widget::onConnected()//连接成功
{
    qDebug()<<"连接成功";
}

void Widget::onDisconnected()//断开
{
   QObject *obj= this->sender();
   QTcpSocket *socket=qobject_cast<QTcpSocket*>(obj);//转换为socket类型
   clients.removeAll(socket);//从容器中删除
   socket->deleteLater();//删除socket

   qDebug()<<"断开连接";
}

void Widget::onError(QAbstractSocket::SocketError socketError)//出错
{
    qDebug()<<"error:"<<socketError;
}




void Widget::on_textSend_clicked()
{
    QString msgInput="TXT:server:"+ui->textInput->toPlainText();
    std::string str =msgInput.toStdString();//将Qstring转换为char(先转为string再转为char)
    const char* ch = str.c_str();

    for(QList<QTcpSocket*>::iterator itr=clients.begin();itr!=clients.end();itr++){
        QTcpSocket *client = *itr;
        //封装包头
        QByteArray dataSend;//封装成的数据包
        QDataStream stream(&dataSend,QIODevice::WriteOnly);
        stream<<(quint32)0<<ch;
        stream.device()->seek(0);//找到第一个0的位置
        stream<<dataSend.size();//把0改成datasend的大小

        client->write(dataSend);
    }
    ui->textMsg->append("server:"+ui->textInput->toPlainText());
    ui->textInput->clear();
}


void Widget::on_Image_clicked()
{
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
     for(QList<QTcpSocket*>::iterator itr=clients.begin();itr!=clients.end();itr++){
          QTcpSocket *client = *itr;


          client->write(dataSend);
     }


}
