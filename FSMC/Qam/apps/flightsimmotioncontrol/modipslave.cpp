#include "modipslave.h"
#include <QApplication>

#include <iostream>
using namespace std ;

ModipSlave::ModipSlave(const QString& configFile, QObject* parent )
    : QObject(parent)
{
    // cartographie Modbus

    m_map    = new QamModbusMap( QamModbusMap::ServerMode, this ) ;

    m_map->setVerbose( false ) ;

    connect( m_map, SIGNAL(info(QString,QString)), this, SLOT(info(QString,QString)) ) ;
    connect( m_map, SIGNAL(valueChanged(int,QString)), this, SLOT(valueChanged(int,QString)) ) ;

    m_map->loadMap( configFile ) ;

    // serveur TCP

    m_server = new QamTcpServer( m_map, this ) ;
    m_hexapodmgi = new HexapodMGI();
    m_server->start( m_map->port() ) ;
   // m_map->setValue( QamModbusMap::HoldingRegister, "Ry","3.14") ;
    //qDebug() << MGI(20,20,20);
    //QamMatrix6x1 test;// = m_hexapodmgi->actuatorLen() ;
    //test.set(10,10,10,10,10,10);
    //qDebug() << test;
}

void ModipSlave::info(const QString& src, const QString& msg )
{
    cout << qPrintable( src ) << ": " << qPrintable( msg ) << endl ;
}

void ModipSlave::valueChanged(int table, const QString& name )
{
    cout << "value : " << qPrintable( QamModbusMap::tableAsString( (QamModbusMap::PrimaryTable)table ) ) ;
    cout << " : " << qPrintable( name ) ;
    cout << " : " << qPrintable( m_map->value( (QamModbusMap::PrimaryTable)table, name ) ) ;
    cout << endl ;
}
void ModipSlave::SetValue(QString val,QString name)
{

     QamModbusMap::PrimaryTable table = QamModbusMap::HoldingRegister ;
    //qDebug() << val;


     //m_map->setValue(table, name,val) ;

     m_map->setValue(table, name,val) ;

}
QamMatrix6x1 ModipSlave::MGI(float Rx, float Ry, float Rz)
{
    // param = 6-DOF [ Tx,Ty,Tz, Rx,Ry,Rz ] unités mm et degrés

    QamMatrix6x1    kin ;
//    QamModbusMap::PrimaryTable  table = QamModbusMap::HoldingRegister ;

//    // lecture des registres
//    kin(0) =  m_map->value(table, "Tx" ).toFloat() ;
//    kin(1) =  m_map->value(table, "Ty" ).toFloat() ;
//    kin(2) =  m_map->value(table, "Tz" ).toFloat() ;
//    kin(3) =  m_map->value(table, "Rx" ).toFloat() ;
//    kin(4) =  m_map->value(table, "Ry" ).toFloat() ;
//    kin(5) =  m_map->value(table, "Rz" ).toFloat() ;

    kin(0) = 0;
    kin(1) = 0;
    kin(2) = 0;
    kin(3) = Rx;
    kin(4) = Ry;
    kin(5) = Rz;

    m_hexapodmgi->setMGI(kin);
    //qDebug() << kin;
    return m_hexapodmgi->actuatorLen() ;
}
