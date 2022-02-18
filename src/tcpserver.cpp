#include "tcpserver.h"


TCPServer::TCPServer(QObject *parent):QTcpServer(parent)
{

}

void TCPServer::start(const QHostAddress & adress,quint16 port)
{
    if(this->listen(adress,port))
    {
        qDebug()<<"ServerStarted port:"<<this->serverPort();
        emit msg(Message("Сервер запущен успешно! Порт: "+QString::number(this->serverPort()),Message::ORDINARY));
    }
    else
    {
        qDebug()<<"Error server: "+ errorString();
        emit msg(Message("Ошибка! :"+errorString(),Message::NEGATIVE));
    }
}

void TCPServer::stop()
{
  for(int i=0;i<Sockets.size();i++)
  {
      Sockets.at(i)->close();
      Sockets.at(i)->deleteLater();

  }
  close();
  Sockets.clear();
  emit msg(Message("Сервер остановлен!",Message::ORDINARY));

}

void TCPServer::SendToClient(QTcpSocket* socket, const QString & message)
{
    m_Data.clear();
    QDataStream out(&m_Data,QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_5_15);
    out<<quint16(0)<<message;
    out.device()->seek(0);
    out<<quint16(m_Data.size() - sizeof(quint16));
     socket->write(m_Data);

}


void TCPServer::incomingConnection(qintptr socket_Descriptor)
{
    socket = new QTcpSocket;
    socket->setSocketDescriptor(socket_Descriptor);
    connect(socket,&QTcpSocket::readyRead,this,&TCPServer::slotReadyRead);
    connect(socket,&QTcpSocket::disconnected,[=](){
        emit msg(Message("Клиент отключился!"));

    });

    Sockets.push_back(socket);

    SendToClient(socket,"Server response: Connected!\n Your ip:"+socket->peerAddress().toString());

    emit msg(Message("Клиент: "+socket->peerAddress().toString()+" подключился!"));


}

void TCPServer::slotReadyRead()
{
    socket=(QTcpSocket*)sender();
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_5_15);
    if(in.status()==QDataStream::Ok)
    {
        for(;;)
        {

            if(m_DataSize==0)
            {
                qDebug()<<"nextBlocKSize="<<m_DataSize;
                if(socket->bytesAvailable()<sizeof(quint16))
                {
                    qDebug()<<"Data size < 2 ,break";
                    break;
                }

                in>>m_DataSize;
                qDebug()<<"nextBlockSize:"<<m_DataSize;
            }

            if(socket->bytesAvailable()<m_DataSize)
            {
                qDebug()<<"Data not full!";
                break;

            }

            qDebug()<<"reading...";
            QString out;
            in>>out;
            qDebug()<<out;
            m_DataSize=0;
            SendToClient(socket,out);
            break;

        }


    }
    else
    {
        qDebug()<<"DataStream error!";
    }

}



