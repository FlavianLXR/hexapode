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
    QByteArray RPOS = "RPOS010"; //datagrams pour position , envoye a 10Hz

    socket->writeDatagram(RPOS.data(),RPOS.size(),QHostAddress("192.168.0.103"),49000);//envoie demande de datagramme RPOS a Xplane

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
    socket->readDatagram(datagram.data(),datagram.size(),&sender,&port);//lecture datagramme recu

    if(isdebug == true )qDebug() << datagram;

    float* p = (float*)(&datagram[33]) ;//exctraction des donnée sur le datagrams de reponse
    QString ps = QString::number(*p);

    float* h = (float*)(&datagram[37]) ;
    QString hs = QString::number(*h);

    float* r = (float*)(&datagram[41]) ;
    QString rs = QString::number(*r);

    if((ps.contains("3140.2") && hs.contains("820340") && rs.contains("2.14218e+08") )== false ){//suppression de donnée phantome

        //qDebug() << ps.toFloat();



        ui->labpitch->setText(ps);//affichage du tangage sur l'application
        slave->SetValue( QString::number(round(ps.toFloat()*100)*-1) ,QString("Ry"));

        ui->labheading->setText(hs);//affichage du lacet sur l'application
        slave->SetValue( QString::number(round(hs.toFloat()*100)) ,QString("Rz"));

        ui->labroll->setText(rs);//affichage du roulis sur l'application
        slave->SetValue( QString::number(round(rs.toFloat()*100)) ,QString("Rx"));

        qDebug() << "=========================================";
        QamMatrix6x1 LengthVerin = slave->MGI(rs.toFloat(),ps.toFloat(),hs.toFloat());//recuperation longeur verins via le MGI


        //qDebug() << round(LengthVerin(0));
        /*slave->SetValue(QString::number(round(LengthVerin(0))),QString("L1"));
        slave->SetValue(QString::number(round(LengthVerin(1))),QString("L2"));
        slave->SetValue(QString::number(round(LengthVerin(2))),QString("L3"));
        slave->SetValue(QString::number(round(LengthVerin(3))),QString("L4"));
        slave->SetValue(QString::number(round(LengthVerin(4))),QString("L5"));
        slave->SetValue(QString::number(round(LengthVerin(5))),QString("L6"));*/


        ui->LL1->setText(QString::number(LengthVerin(0)));//affichage de la longeur des verins
        ui->LL2->setText(QString::number(LengthVerin(1)));
        ui->LL3->setText(QString::number(LengthVerin(2)));
        ui->LL4->setText(QString::number(LengthVerin(3)));
        ui->LL5->setText(QString::number(LengthVerin(4)));
        ui->LL6->setText(QString::number(LengthVerin(5)));

    }


}





