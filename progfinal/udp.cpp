#include <QDebug>
#include <qiodevice.h>
#include <QTimer>
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
    timer = new QTimer();
    timer->start(100);

    QTimer::connect(timer , SIGNAL(timeout()),this,SLOT(disconnected()));

    connect(socket, SIGNAL(readyRead()), this, SLOT(traitementDatagram()),Qt::QueuedConnection);

}

UDP::~UDP()
{
    delete ui;
}
void UDP::disconnected()
{
    ui->statconnection->setText("<font color='red'>DISCONNECTED</font>");
}
void UDP::envoieDatagram()
{
    QByteArray RPOS = "RPOS025"; //datagrams pour position , envoye a 25Hz

    socket->writeDatagram(RPOS.data(),RPOS.size(),QHostAddress("192.168.0.103"),49000);

}
void UDP::traitementDatagram()
{

    timer->start(100);
    ui->statconnection->setText("<font color='lime'>CONNECTED</font>");
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

    if((ps.contains("3140.2") && hs.contains("820340") && rs.contains("2.14218e+08") )== false ){//suppression de donnée phantome

        ui->labpitch->setText(ps);//affichage du tangage sur l'application
        slave->SetValue(ps.toFloat(),QString("Ry"));

        ui->labheading->setText(hs);//affichage du lacet sur l'application
        slave->SetValue(hs.toFloat(),QString("Rz"));

        ui->labroll->setText(rs);//affichage du roulis sur l'application
        slave->SetValue(rs.toFloat(),QString("Rx"));

        qDebug() << "=========================================";
        QamMatrix6x1 LengthVerin = slave->MGI(rs.toFloat(),ps.toFloat(),hs.toFloat());

        ui->LL1->setText(QString::number(LengthVerin(0)));
        ui->LL2->setText(QString::number(LengthVerin(1)));
        ui->LL3->setText(QString::number(LengthVerin(2)));
        ui->LL4->setText(QString::number(LengthVerin(3)));
        ui->LL5->setText(QString::number(LengthVerin(4)));
        ui->LL6->setText(QString::number(LengthVerin(5)));

    }

}






