#include "modipmaster.h"
#include <QSocketNotifier>
#include <QRegularExpression>

#include <iostream>
using namespace std ;

ModipMaster::ModipMaster(const QString& configFile, QObject* parent )
	: QObject(parent)
{
	// cartographie Modbus

	m_map    = new QamModbusMap( QamModbusMap::ClientMode, this ) ;

	m_map->setVerbose( false ) ;

	connect( m_map, SIGNAL(info(QString,QString)), this, SLOT(info(QString,QString)) ) ;
//	connect( m_map, SIGNAL(valueChanged(int,QString)), this, SLOT(valueChanged(int,QString)) ) ;

	m_map->loadMap( configFile ) ;

	m_table = QamModbusMap::HoldingRegister ;

	// client TCP

	m_client = new QamTcpClient( m_map, this ) ;

	m_client->sockConnect( m_map->host(), m_map->port() ) ;

	// intercepteur de commandes clavier

	m_notifier = new QSocketNotifier(fileno(stdin), QSocketNotifier::Read, this ) ;

	connect(m_notifier, SIGNAL(activated(int)), this, SLOT(readConsole()) ) ;
}

void ModipMaster::readConsole()
{
	QTextStream	in(stdin) ;
	QString line = in.readLine() ;
//	QStringList parse = line.split( QRegExp("\\s+") ) ;
	QStringList parse = line.split( QRegularExpression("\\s+") ) ;

	if ( parse.at(0) == "table" ) {
		if ( parse.size() > 1 ) {
			int t = parse.at(1).toInt() ;
			if (( t == 0 )||( t == 1 )||( t == 3)||( t == 4 ))	m_table = (QamModbusMap::PrimaryTable)t ;
		}
		cout << qPrintable( QamModbusMap::tableAsString( m_table ) ) << endl ;
	}
	if ( parse.at(0) == "list" ) {
		QStringList	nl = m_map->nameList(m_table ) ;
		for ( int i = 0 ; i < nl.size() ; ++i )	cout << qPrintable( nl.at(i) ) << endl ;
	}
	if (( parse.at(0) == "items" )&&( parse.size() > 1 )) {
		QStringList	nl = m_map->itemList(m_table, parse.at(1) ) ;
		for ( int i = 0 ; i < nl.size() ; ++i )	cout << qPrintable( nl.at(i) ) << endl ;
	}
	if (( parse.at(0) == "read" )&&( parse.size() > 1 )) {
		QString res = m_map->remoteValue(m_table, parse.at(1) ) ;
		cout << qPrintable( res ) << endl ;
	}
	if (( parse.at(0) == "write" )&&( parse.size() > 2 )) {
		m_map->setRemoteValue(m_table, parse.at(1), parse.at(2) ) ;

		QString res = m_map->localValue(m_table, parse.at(1) ) ;
		cout << qPrintable( res ) << endl ;
	}
	if ( parse.at(0) == "quit" ) {
		emit quit() ;
	}
}

void ModipMaster::info(const QString& src, const QString& msg )
{
	cout << qPrintable( src ) << ": " << qPrintable( msg ) << endl ;
}
/*
void ModipMaster::valueChanged(int table, const QString& name )
{
	// sous-composant de donnée composée
	if (( name != m_name )&&( name.startsWith(m_name) ))	return ;

	// donnée primaire
	cout << "value : " << qPrintable( name ) ;
	cout << " : " << qPrintable( m_map->localValue( (QamModbusMap::PrimaryTable)table, name ) ) ;
	cout << endl ;

	if ( name == m_name )	return ;

	// donnée secondaire
	cout << "value : " << qPrintable( m_name ) ;
	cout << " : " << qPrintable( m_map->localValue( (QamModbusMap::PrimaryTable)table, m_name ) ) ;
	cout << endl ;
}
*/
