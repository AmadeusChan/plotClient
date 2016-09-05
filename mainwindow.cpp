#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QInputDialog>
#include <QMessageBox>
#include <QPainter>
#include <QPoint>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    plots.clear();
    bool okay_;
    QString address_=QInputDialog::getText(this,tr("Server Ip:"),tr("Server Address:"),QLineEdit::Normal,QString("127.0.0.1"),&okay_);
    if (address_.size() && okay_){
        int port_=QInputDialog::getInt(this,tr("Host port"),tr("Host port"),0,-0x7fffffff,0x7fffffff,1,&okay_);
        if (okay_){
            network=new myNetwork(this);
            connect(network,&myNetwork::connected,[=](){
                QMessageBox::information(this,tr("connected!"),tr("connected!"));
                setUpConnection();
            });
            network->setUpClient(QHostAddress(address_),port_);
        }
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setUpConnection(){
    connect(network,&myNetwork::dataRead,[=](QString data_){
        QVector<QString> vector_;
        vector_.clear();
        QString string_="";
        for (int i=0;i<data_.size();++i){
            if (data_.at(i)==QChar('-')){
                vector_.append(string_);
                string_="";
            } else {
                string_+=data_.at(i);
            }
        }
        plots.append(vector_);
        update();
    });
}

void MainWindow::paintEvent(QPaintEvent *event){
    QPainter painter_(this);
    painter_.setBrush(QBrush(Qt::transparent));
    painter_.setPen(QPen(Qt::gray));
    for (int i=0;i<width();i+=50){
        painter_.drawLine(i,0,i,height());
    }
    for (int i=0;i<height();i+=50){
        painter_.drawLine(0,i,width(),i);
    }
    painter_.setPen(QPen(Qt::black));
    for (int i=0;i<plots.size();++i){
        if (plots[i][0]=="C"){
            QPoint center_(plots.at(i).at(1).toInt(),plots.at(i).at(2).toInt());
            int r_=plots.at(i).at(3).toInt();
            painter_.drawEllipse(center_,r_,r_);
        } else if (plots[i][0]=="R"){
            int x_=plots.at(i).at(1).toInt();
            int y_=plots.at(i).at(2).toInt();
            int w_=plots.at(i).at(3).toInt();
            int h_=plots.at(i).at(4).toInt();
            painter_.drawRect(QRect(x_,y_,w_,h_));
        } else if (plots[i][0]=="T"){
            QPoint a(plots.at(i).at(1).toInt(),plots.at(i).at(2).toInt());
            QPoint b(plots.at(i).at(3).toInt(),plots.at(i).at(4).toInt());
            QPoint c(plots.at(i).at(5).toInt(),plots.at(i).at(6).toInt());
            painter_.drawLine(a,b);
            painter_.drawLine(b,c);
            painter_.drawLine(c,a);
        }
    }
}
