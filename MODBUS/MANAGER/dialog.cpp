/*  ---------------------------------------------------------------------------
 *  filename    :   dialog.cpp
 *  description :   IMPLEMENTATION de la classe d'usage UI
 *
 *	project     :	ModipManager
 *  start date  :   janvier 2014
 *  ---------------------------------------------------------------------------
 *  Copyright 2014-2016 by Alain Menu   <alain.menu@ac-creteil.fr>
 *
 *  This file is part of "Qam Modbus over IP Project"
 *
 *  This program is free software ;  you can  redistribute it and/or  modify it
 *  under the terms of the  GNU General Public License as published by the Free
 *  Software Foundation ; either version 3 of the License, or  (at your option)
 *  any later version.
 *
 *  This program is distributed in the hope that it will be useful, but WITHOUT
 *  ANY WARRANTY ; without even the  implied  warranty  of  MERCHANTABILITY  or
 *  FITNESS FOR  A PARTICULAR PURPOSE. See the  GNU General Public License  for
 *  more details.
 *
 *	You should have  received  a copy of the  GNU General Public License  along
 *	with this program. If not, see <http://www.gnu.org/licenses/>.
 *  ---------------------------------------------------------------------------
 */

#include "dialog.h"
#include <QMessageBox>
#include <QFileDialog>

Dialog::Dialog(QWidget *parent)
	: QDialog(parent)
{
	setupUi(this) ;

	pbuAbout->setIcon(QIcon(":/_res/icons/qam47.png") ) ;

#ifdef Q_OS_MAC
	edtDialog->setFont( QFont("Monaco", 12 ) ) ;
#else
	edtDialog->setFont( QFont("Courier", 10 ) ) ;
#endif

	pbuReadAll->setEnabled( false ) ;
	pbuRead->setEnabled( false ) ;

	readSettings() ;	// chargement configuration session précédente
}

// fermeture -> sauvegarde de la configuration courante

void Dialog::closeEvent(QCloseEvent* event )
{
	writeSettings() ;
	event->accept() ;
}

// fenêtre "à propos..." par clic sur logo

void Dialog::on_pbuAbout_clicked()
{
	QMessageBox::information(this,
	   tr("About..."),
	   QString(MODIPMANAGER_ABOUTMESSAGE_HTML) ) ;
}

// référencement d'un nouveau serveur

void Dialog::on_pbuNew_clicked()
{
	QString	fileName = QFileDialog::getOpenFileName(
		this,
		tr("Ouvrir..."),
		m_configDir,
		tr("Configuration Modbus (*.csv)") ) ;

	if ( !fileName.isEmpty() ) {
		// mémorisation du répertoire source utilisé
		m_configDir = QFileInfo( fileName ).absolutePath() ;
#ifdef Q_OS_MAC
		m_configDir = m_configDir + QDir::separator() + "*" ;
#endif
		// configuration du serveur métier

		QamModbusMap* map = new QamModbusMap(QamModbusMap::ClientMode, this ) ;
		QamTcpClient* client = new QamTcpClient(map, this ) ;
		QamModbusMapViewer* viewer = new QamModbusMapViewer( this ) ;

		connect( map, SIGNAL(info(QString,QString)),
				 this, SLOT(info(QString,QString)) ) ;

		map->loadMap( fileName ) ;
		viewer->setModbusMap( map ) ;

		m_modbusMap << map ;
		m_tcpCLient << client ;
		m_modbusMapViewer << viewer ;

		// création nouvel onglet

		int i = tabWidget->addTab(m_modbusMapViewer.last(), map->description() ) ;

//		tabWidget->setTabToolTip( i, map->host() + ":" + QString("%1").arg( map->port() ) ) ;	// new 1.1
		edtHost->setText( map->host() ) ;
		edtPort->setText( QString("%1").arg( map->port() ) ) ;

		tabWidget->setCurrentIndex( i ) ;
	}
}

// connexion on/off au serveur (pour l'onglet actif)

void Dialog::on_pbuConnect_clicked()
{
	int index = tabWidget->currentIndex() ;

	bool connected = m_tcpCLient.at( index )->state() == QAbstractSocket::ConnectedState ;
	if ( connected ) {
		m_tcpCLient.at( index )->sockClose() ;
		pbuConnect->setText("Connect") ;
		pbuReadAll->setEnabled( false ) ;
		pbuRead->setEnabled( false ) ;

		edtHost->setEnabled( true ) ;	// new 1.1
		edtPort->setEnabled( true ) ;
		labHost->setEnabled( true ) ;
		labPort->setEnabled( true ) ;
	}
	else {
//		QString host = m_modbusMap.at( index )->host() ;	// new 1.1
//		quint16 port = m_modbusMap.at( index )->port() ;
		QString host = edtHost->text() ;
		quint16 port = edtPort->text().toUInt() ;

		m_tcpCLient.at( index )->sockConnect(host, port ) ;
		if ( m_tcpCLient.at( index )->waitForConnected(3000) ) {
			pbuConnect->setText("Close") ;
			pbuReadAll->setEnabled( true ) ;
			pbuRead->setEnabled( true ) ;

			edtHost->setEnabled( false ) ;					// new 1.1
			edtPort->setEnabled( false ) ;
			labHost->setEnabled( false ) ;
			labPort->setEnabled( false ) ;
		}
		else {												// new 1.1
			info("tcp/ip", "Connection failed!" ) ;
		}
	}
}

// RAZ de toutes les valeurs (cartographie locale)

void Dialog::on_pbuReset_clicked()
{
	int i = tabWidget->currentIndex() ;
	if ( i < 0 )	return ;

	QamModbusMap::PrimaryTable	table ;

	table = QamModbusMap::Coil ;
	foreach(QString name, m_modbusMap.at(i)->nameList( table ) ) {
		m_modbusMap.at(i)->setLocalValue(table, name, "0" ) ;
	}
	table = QamModbusMap::DiscretInput ;
	foreach(QString name, m_modbusMap.at(i)->nameList( table ) ) {
		m_modbusMap.at(i)->setLocalValue(table, name, "0" ) ;
	}
	table = QamModbusMap::InputRegister ;
	foreach(QString name, m_modbusMap.at(i)->nameList( table ) ) {
		QString display = m_modbusMap.at(i)->display(table, name ) ;
		QString value = "0" ;
		if ( display == "Ascii" )	value = "" ;
		if (( display == "Str8" )||( display == "Str16" ))		value = "" ;
		m_modbusMap.at(i)->setLocalValue(table, name, value ) ;
	}
	table = QamModbusMap::HoldingRegister ;
	foreach(QString name, m_modbusMap.at(i)->nameList( table ) ) {
		QString display = m_modbusMap.at(i)->display(table, name ) ;
		QString value = "0" ;
		if ( display == "Ascii" )	value = "" ;
		if (( display == "Str8" )||( display == "Str16" ))		value = "" ;
		m_modbusMap.at(i)->setLocalValue(table, name, value ) ;
	}
}

// demande d'actualisation de toutes les données
// par interrogations du serveur

void Dialog::on_pbuReadAll_clicked()
{
	int i = tabWidget->currentIndex() ;
	if ( i < 0 )	return ;

	QamModbusMap::PrimaryTable	table ;

	table = QamModbusMap::Coil ;
	foreach(QString name, m_modbusMap.at(i)->nameList( table ) ) {
		m_modbusMap.at(i)->remoteValue(table, name ) ;
	}
	table = QamModbusMap::DiscretInput ;
	foreach(QString name, m_modbusMap.at(i)->nameList( table ) ) {
		m_modbusMap.at(i)->remoteValue(table, name ) ;
	}
	table = QamModbusMap::InputRegister ;
	foreach(QString name, m_modbusMap.at(i)->nameList( table ) ) {
		m_modbusMap.at(i)->remoteValue(table, name ) ;
	}
	table = QamModbusMap::HoldingRegister ;
	foreach(QString name, m_modbusMap.at(i)->nameList( table ) ) {
		m_modbusMap.at(i)->remoteValue(table, name ) ;
	}
}

// demande d'actualisation de la donnée sélectionnée
// par interrogation du serveur

void Dialog::on_pbuRead_clicked()
{
	int i = tabWidget->currentIndex() ;

	int table = m_modbusMapViewer.at(i)->currentTable() ;
	if ( table == -1 )	return ;
	QString name = m_modbusMapViewer.at(i)->currentName() ;

	m_modbusMap.at(i)->remoteValue( (QamModbusMap::PrimaryTable)table, name ) ;
}

// changement d'onglet

void Dialog::on_tabWidget_currentChanged(int index )
{
	bool connected = m_tcpCLient.at( index )->state() == QAbstractSocket::ConnectedState ;
	pbuConnect->setText( connected ? "Close" : "Connect" ) ;
	pbuReadAll->setEnabled( connected ) ;
	pbuRead->setEnabled( connected ) ;

	edtHost->setText( m_modbusMap.at( index )->host() ) ;						// new 1.1
	edtPort->setText( QString("%1").arg( m_modbusMap.at( index )->port() ) ) ;
	edtHost->setEnabled( !connected ) ;
	edtPort->setEnabled( !connected ) ;
	labHost->setEnabled( !connected ) ;
	labPort->setEnabled( !connected ) ;
}

// suppression d'un référencement serveur

void Dialog::on_tabWidget_tabCloseRequested(int index )
{
	bool connected = m_tcpCLient.at( index )->state() == QAbstractSocket::ConnectedState ;	// new 1.1
	if ( connected ) m_tcpCLient.at( index )->sockClose() ;

	m_tcpCLient.removeAt( index ) ;

	m_modbusMapViewer.removeAt( index ) ;
	m_modbusMap.removeAt( index ) ;

	tabWidget->removeTab( index ) ;

	if ( tabWidget->count() == 0 ) {		// new 1.1
		edtHost->setText( "" ) ;
		edtPort->setText( "" ) ;
	}
}

// gestion de la configuration inter-sessions

void Dialog::readSettings()
{
	QSettings settings( QSettings::IniFormat, QSettings::UserScope, "qam.fr", "modipmanager" ) ;

	QPoint	pos = settings.value("MainWindow/pos", QPoint(10,10) ).toPoint() ;
	QSize	size = settings.value("MainWindow/size", QSize(640,480) ).toSize() ;
	resize( size ) ;
	move( pos ) ;

	QString ld = QDir::homePath() ;
#ifdef Q_OS_MAC
	ld = ld + QDir::separator() + "*" ;
#endif
	m_configDir = settings.value("Files/configDir", ld ).toString() ;
}

void Dialog::writeSettings()
{
	QSettings settings( QSettings::IniFormat, QSettings::UserScope, "qam.fr", "modipmanager" ) ;

	settings.setValue("MainWindow/pos", pos() ) ;
	settings.setValue("MainWindow/size", size() ) ;
	settings.setValue("Files/configDir", m_configDir ) ;
}

// remontée des messages d'info.

void Dialog::info(const QString& source, const QString& message )
{
	showPrompt( source + ": ") ;
	showMessage( message + "\n", false, Qt::darkGray  ) ;
}

void Dialog::valueChanged(int table,const QString& name )
{
	showPrompt( "value : ") ;
	showMessage( QamModbusMap::tableAsString( (QamModbusMap::PrimaryTable)table ) + " / " + name + " changed\n", false, Qt::darkRed  ) ;
}

void Dialog::showPrompt(const QString& prompt )
{
	edtDialog->moveCursor(QTextCursor::End ) ;
	edtDialog->setTextColor( Qt::darkGray ) ;
	edtDialog->setFontWeight( QFont::Normal ) ;
	edtDialog->insertPlainText( prompt ) ;
	edtDialog->moveCursor(QTextCursor::End ) ;
}

void Dialog::showMessage(const QString& msg, bool bold, QColor color )
{
	edtDialog->moveCursor(QTextCursor::End ) ;
	edtDialog->setTextColor( color ) ;
	edtDialog->setFontWeight( bold ? QFont::Bold : QFont::Normal ) ;
	edtDialog->insertPlainText( msg ) ;
	edtDialog->moveCursor(QTextCursor::End ) ;
}
