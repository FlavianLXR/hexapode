/*  ---------------------------------------------------------------------------
 *  filename    :   qamtcpclient.cpp
 *  description :   IMPLEMENTATION de la classe QamTcpClient
 *
 *	project     :	QamSockets Library
 *  start date  :   fév 2006
 *  ---------------------------------------------------------------------------
 *  Copyright 2006-2016 by Alain Menu   <alain.menu@ac-creteil.fr>
 *
 *  This file is part of "QamSockets Library"
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

#include "qamtcpclient.h"
#include <QHostAddress>

/*!
 * Constructeur d'un client TCP avec l'objet serveur métier à utiliser comme 
 * émetteur de requêtes et destinataire des réponses.
 * \param server : serveur "métier" à utiliser.
 * \param parent : parent Qt.
 */

QamTcpClient::QamTcpClient(QamAbstractServer* server, QObject* parent )
	: QTcpSocket(parent)
	, m_server( server )
{
	connect( this, SIGNAL( connected() ),
			 this, SLOT(   sockConnected() ) ) ;
	connect( this, SIGNAL( disconnected() ),
			 this, SLOT(   sockDisconnected() ) ) ;
	connect( this, SIGNAL( readyRead() ),
			 this, SLOT(   sockRead() ) ) ;
//	connect( this, SIGNAL( error(QAbstractSocket::SocketError ) ),
//			 this, SLOT(   sockError(QAbstractSocket::SocketError ) ) ) ;
	connect( this, SIGNAL( errorOccurred(QAbstractSocket::SocketError ) ),
			 this, SLOT(   sockError(QAbstractSocket::SocketError ) ) ) ;

	if ( m_server ) {

		connect( this,		SIGNAL( sockReceived(QByteArray) ),
				 m_server,	SLOT(   responseFromServer(QByteArray) ) ) ;

		connect( m_server,	SIGNAL( request(QByteArray) ),
				 this,		SLOT(   sockWrite(QByteArray) ) ) ;

		connect( this,		SIGNAL( sockInfo(QString) ),
				 m_server,	SLOT(   networkInfo(QString) ) ) ;
	}
}

/*!
 * Demande de connexion au serveur indiqué par son adresse IP ou son nom réseau
 * et par son port de service.
 * \param hostName : identifiant du serveur ou adresse IP.
 * \param port : port de service.
 */

void QamTcpClient::sockConnect(const QString& hostName, quint16 port )
{
	connectToHost(hostName, port ) ;
}

/*!
 * Demande de fermeture de la connexion.
 */

void QamTcpClient::sockClose()
{
	QString hostAddr = peerAddress().toString() ;
	quint16 hostPort = peerPort() ;

	close() ;
	if ( m_server ) m_server->setServerAvailable( false ) ;
	emit sockInfo( QString("connection %1:%2 closed").arg(hostAddr).arg(hostPort) ) ;
}

/*!
 * Demande d'émission à destination du serveur ; sans effet si la connexion
 * n'est pas active.
 * \param data : trame (suite linéaire d'octets) à émettre.
 */

void QamTcpClient::sockWrite(const QByteArray& data )
{
	if ( this->state() != QAbstractSocket::ConnectedState ) return ;
	write( data ) ;
}

// -------------------------------------------------------------------------

void QamTcpClient::sockConnected()
{
	QString hostAddr = peerAddress().toString() ;
	quint16 hostPort = peerPort() ;

	if ( m_server ) m_server->setServerAvailable( true ) ;
	emit sockInfo( QString("connected to %1:%2").arg(hostAddr).arg(hostPort) ) ;
}

void QamTcpClient::sockDisconnected()
{
	if ( m_server ) m_server->setServerAvailable( false ) ;
}

void QamTcpClient::sockRead()
{
	QByteArray  data = this->readAll() ;
	emit sockReceived( data ) ;
}

void QamTcpClient::sockError(QAbstractSocket::SocketError error )
{
	Q_UNUSED( error ) ;
	emit sockInfo( this->errorString() ) ;
}
