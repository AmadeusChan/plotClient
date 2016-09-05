#include "mynetwork.h"

#include <QChar>
#include <QByteArray>
#include <QDebug>
#include <QThread>

myNetwork::myNetwork(QObject *parent):QObject(parent){
    qDebug()<<"myNetwork::myNetwork()";
    connected_flag=false;
    server=NULL;
    socket=NULL;
    qRegisterMetaType<QHostAddress>("QHostAddress");
    strings="";
}

QHostAddress myNetwork::getIpAddress(){
    qDebug()<<"myNetwork::getIpAddress()";
#ifdef _WIN32
    //return QHostAddress(QHostAddress::LocalHost);
#endif
    QList<QHostAddress> list_=QNetworkInterface::allAddresses();
    qDebug()<<list_.size();
    for (int i=0;i<list_.size();++i){
        if (!list_.at(i).isLoopback()){
            qDebug()<<"?"<<list_.at(i);
            if (list_.at(i).protocol()==QAbstractSocket::IPv4Protocol){
                qDebug()<<"#$@@#$@#$";
                return list_.at(i);
            }
        }
    }
}

void myNetwork::setUpServer(){
    qDebug()<<"void myNetwork::setUpServer()";
    qDebug()<<"thread of network:"<<QThread::currentThreadId();
    server=new QTcpServer(this);
    qDebug()<<"getIpAddress():"<<getIpAddress();
    server->listen();
    connect(server,SIGNAL(newConnection()),
            this,SLOT(actionOfConnectedServer()));
    qDebug()<<server->serverAddress()<<" "<<server->serverPort();
    emit serverSetUp(getIpAddress(),server->serverPort());
    //server->waitForNewConnection(-1);
}

void myNetwork::actionOfConnectedServer(){
    qDebug()<<"myNetwork::actionOfConnectedServer()";
        socket=server->nextPendingConnection();
        connected_flag=true;
        connect(socket,SIGNAL(readyRead()),
                this,SLOT(readData()));
        connect(socket,SIGNAL(disconnected()),
                this,SIGNAL(disconnected()));
        emit connected();
}

void myNetwork::setUpClient(QHostAddress address_,int port_){
    qDebug()<<"myNetwork::setUpClient()";
    qDebug()<<"thread of network:"<<QThread::currentThreadId();
    qDebug()<<address_<<port_;
    socket=new QTcpSocket(this);
    socket->connectToHost(address_,port_);
    connect(socket,SIGNAL(connected()),
            this,SLOT(actionOfConnectedClient()));
    //socket->waitForConnected(10000);
    emit clientSetUp(socket->localAddress(),socket->localPort());
}

void myNetwork::actionOfConnectedClient(){
    qDebug()<<"myNetwork::action0OfConnectedClient";
    qDebug()<<socket->localAddress()<<" "<<socket->localPort();
    connected_flag=true;
    connect(socket,SIGNAL(readyRead()),
            this,SLOT(readData()));
    connect(socket,SIGNAL(disconnected()),
            this,SIGNAL(disconnected()));
    emit connected();
}

void myNetwork::readData(){
    qDebug()<<"myNetwork::readData()";
    QByteArray ba=socket->readAll();
    if (ba.isEmpty()) return ;
    char *data_=ba.data();
    QString string_="";
    while (*data_){
        string_+=QChar(*data_);
        ++data_;
    }
    strings+=string_;
    while (1){
        string_="";
        for (int i=0;i<strings.size();++i){
            if (strings.at(i)==QChar('&')){
                strings.remove(0,i+1);
                if (string_.size()) emit dataRead(string_);
                break;
            } else string_+=strings.at(i);
        }
        bool flag=true;
        for (int i=0;i<strings.size();++i){
            if (strings.at(i)==QChar('&')){
                flag=false;
                break;
            }
        }
        if (flag) break;
    }
    /*
    if (string_.size()>=3){
        qDebug()<<"size:"<<sizeof(string_);
        emit dataRead(string_);
    }*/
}

void myNetwork::writeData(QString data_){
    qDebug()<<"myNetwork::writeData()";
    qDebug()<<"SS:"<<&data_;
    qDebug()<<"to Write:"<<data_;
    data_+="&";
    ba.clear();
    ba.append(data_);
    socket->write(ba);
    qDebug()<<"HAHA";
    //emit dataWritten();
}

void myNetwork::disconnectNetwork(){
    qDebug()<<"disconnectNetwork()";
    if (server) delete server;
    server=NULL;
    if (socket) delete socket;
    socket=NULL;
    emit disconnected();
}
