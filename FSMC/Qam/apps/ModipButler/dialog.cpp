/*  ---------------------------------------------------------------------------
 *  filename    :   dialog.cpp
 *  description :   IMPLEMENTATION de la classe d'usage UI
 *
 *	project     :	ModipButler
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
#include <QSettings>
#include <QDir>
#include <QFileDialog>
#include <QMessageBox>

Dialog::Dialog(QWidget* parent)
	: QDialog(parent)
	, m_isRunning( false )
{
	setupUi(this) ;

	pbuAbout->setIcon(QIcon(":/_res/icons/qam47.png") ) ;

	pbuStart->setEnabled( false ) ;

#ifdef Q_OS_MAC
	edtDialog->setFont( QFont("Monaco", 12 ) ) ;
#else
	edtDialog->setFont( QFont("Courier", 10 ) ) ;
#endif

	// chargement configuration session précédente
	readSettings() ;

	// création du serveur Modbus
	m_modbusMap = new QamModbusMap(QamModbusMap::ServerMode, this ) ;

	connect( m_modbusMap, SIGNAL(info(QString,QString)),
			 this, SLOT(info(QString,QString)) ) ;

	connect( m_modbusMap, SIGNAL(valueChanged(int,QString)),
			 this, SLOT(valueChanged(int,QString)) ) ;
}

// fermeture -> sauvegarde de la configuration courante

void Dialog::closeEvent(QCloseEvent* event )
{
	writeSettings() ;
	event->accept() ;
}

// arrêt/marche de la mise en écoute demandes de connexion TCP

void Dialog::on_pbuStart_clicked()
{
	m_isRunning = !m_isRunning ;
	pbuStart->setText( m_isRunning ? "Stop" : "Start" ) ;

	if ( m_isRunning ) {
		// demandes de connexion acceptée
		m_tcpServer->start( edtPort->text().toUShort() ) ;
		edtPort->setEnabled( false ) ;
	}
	else {
		// nouvelles demandes ignorées
		m_tcpServer->close() ;
		edtPort->setEnabled( true ) ;
	}
}

// fenêtre "à propos..." par clic sur logo

void Dialog::on_pbuAbout_clicked()
{
	QMessageBox::information(this,
	   tr("About..."),
		QString(MODIPBUTLER_ABOUTMESSAGE_HTML) ) ;
}

// gestion de la configuration inter-sessions

void Dialog::readSettings()
{
	QSettings settings( QSettings::IniFormat, QSettings::UserScope, "qam.fr", "modipbutler" ) ;

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
	QSettings settings( QSettings::IniFormat, QSettings::UserScope, "qam.fr", "modipbutler" ) ;

	settings.setValue("MainWindow/pos", pos() ) ;
	settings.setValue("MainWindow/size", size() ) ;
	settings.setValue("Files/configDir", m_configDir ) ;
}

// chargement de la configuration du serveur Modbus

void Dialog::on_pbuLoad_clicked()
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
		m_modbusMap->loadMap( fileName ) ;
		edtPort->setText( QString("%1").arg( m_modbusMap->port() ) ) ;
		// initialisation du visualiseur
		mapView->setModbusMap( m_modbusMap ) ;
		// création du serveur TCP
		m_tcpServer = new QamTcpServer(m_modbusMap, this ) ;
		// actualisation UI
		pbuStart->setEnabled( true ) ;
		pbuLoad->setEnabled( false ) ;
	}
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
	showMessage( m_modbusMap->tableAsString( (QamModbusMap::PrimaryTable)table ) + " / " + name + " changed\n", false, Qt::darkRed  ) ;
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
