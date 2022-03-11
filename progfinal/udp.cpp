#include <QThread>
#include <QDebug>
#include <QQueue>
#include <iostream>
#include <cmath>
#include <qiodevice.h>
#include "udp.h"
#include "ui_udp.h"
#include "modipslave.h"
UDP::UDP(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::UDP)
{

    ui->setupUi(this);
    socket = new QUdpSocket(this);
    socket->bind(QHostAddress("192.168.0.103"), 49000);


    envoieDatagram();

    traitementDatagram();

    connect(socket, SIGNAL(readyRead()), this, SLOT(traitementDatagram()),Qt::QueuedConnection);

}

UDP::~UDP()
{
    delete ui;
}
void UDP::envoieDatagram()
{
    QByteArray RREF = "RPOS00000 "; //datagrams pour position , envoye a 25Hz

    //qDebug() << RREF ;

    socket->writeDatagram(RREF.data(),RREF.size(),QHostAddress("192.168.0.103"),49000);

}

void UDP::traitementDatagram()
{
    bool isdebug = false ; // variable activation debug

    QHostAddress sender;
    u_int16_t port;

    QByteArray datagram ;

    datagram.resize(socket->pendingDatagramSize());

    socket->readDatagram(datagram.data(),datagram.size(),&sender,&port);

        if(isdebug == true )qDebug() << datagram;

        float* p = (float*)(&datagram[33]) ;//exctraction des donnée sur le datagrams de reponse
        QString ps = QString::number(*p);

        float* h = (float*)(&datagram[37]) ;
        QString hs = QString::number(*h);

        float* r = (float*)(&datagram[41]) ;
        QString rs = QString::number(*r);



        if(isdebug == true )qDebug() << "ps ::" << ps;
        if(isdebug == true )qDebug() << "hs ::" << hs ;
        if(isdebug == true )qDebug() << "rs ::" << rs ;

        if((ps.contains("3140.2") && hs.contains("820340") && rs.contains("2.14218e+08") )== false ){//suppression de donnée phantome


            ui->labpitch->setText(ps);//affichage du tangage sur l'application
            slave->SetValue(ps.toFloat(),QString("Ry"));

            ui->labheading->setText(hs);//affichage du lacet sur l'application
            slave->SetValue(hs.toFloat(),QString("Rz"));

            ui->labroll->setText(rs);//affichage du roulis sur l'application
            slave->SetValue(rs.toFloat(),QString("Rx"));

            qDebug() << "=========================================";

            QamMatrix6x1 LengthVerin = slave->MGI(rs.toFloat(),ps.toFloat(),hs.toFloat());

             QString VL1 = QString::number(LengthVerin(0));
             QString VL2 = QString::number(LengthVerin(1));
             QString VL3 = QString::number(LengthVerin(2));
             QString VL4 = QString::number(LengthVerin(3));
             QString VL5 = QString::number(LengthVerin(4));
             QString VL6 = QString::number(LengthVerin(5));

            ui->LL1->setText(VL1);
            ui->LL2->setText(VL2);
            ui->LL3->setText(VL3);
            ui->LL4->setText(VL4);
            ui->LL5->setText(VL5);
            ui->LL6->setText(VL6);

        }
}





