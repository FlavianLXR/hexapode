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
    QByteArray RREF = "RPOS00001 "; //datagrams pour position , envoye a 25Hz

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
        if(ps.contains("3140.2") == false ){//suppression de donnée phantome
            ui->labpitch->setText(ps);//affichage du tangage sur l'application
            slave->SetValue(ps.toFloat(),QString("Ry"));
        }

        if(isdebug == true )qDebug() << "hs ::" << hs ;
        if(hs.contains("820340") == false ){
            ui->labheading->setText(hs);//affichage du lacet sur l'application
            slave->SetValue(hs.toFloat(),QString("Rz"));
        }

        if(isdebug == true )qDebug() << "rs ::" << rs ;
        if(rs.contains("2.14218e+08") == false ){
            ui->labroll->setText(rs);//affichage du roulis sur l'application
            slave->SetValue(rs.toFloat(),QString("Rx"));
            qDebug() << "=========================================";
            QamMatrix6x1 LengthVerin = slave->MGI(rs.toFloat(),ps.toFloat(),hs.toFloat());
            qDebug() << LengthVerin;

        }




}





