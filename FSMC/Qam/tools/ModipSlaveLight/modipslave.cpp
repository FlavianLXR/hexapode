#include "modipslave.h"
#include <QCoreApplication>

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

	m_server->start( m_map->port() ) ;
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
