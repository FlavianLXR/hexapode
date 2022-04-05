/*  ---------------------------------------------------------------------------
 *  filename    :   qamtcpconnection.cpp
 *  description :   IMPLEMENTATION de la classe QamTcpConnection
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

#include "qamtcpconnection.h"

/*!
 * Constructeur.
 * \param id : descripteur socket de la connexion.
 * \param server : serveur "métier" à utiliser comme interlocuteur.
 * \param parent : parent Qt.
 */

QamTcpConnection::QamTcpConnection(qintptr id, QamAbstractServer* server, QObject* parent )
	: QThread(parent)
	, m_socketDescriptor( id )
	, m_dataServer( server )
{
}

/*!
 * Corps d'exécution du thread. Création de la socket cliente et gestion de la connexion.
 */

void QamTcpConnection::run()
{
//	m_dataServer->networkInfo("Connection thread started...") ;

	m_socket = new QTcpSocket() ;

	if ( !m_socket->setSocketDescriptor(m_socketDescriptor) ) {
		m_dataServer->networkInfo( m_socket->errorString() ) ;
		return ;
	}

	connect( m_socket,	SIGNAL(readyRead()),
			 this,		SLOT(readyRead()), Qt::DirectConnection ) ;
	connect( m_socket,	SIGNAL(disconnected()),
			 this,		SLOT(disconnected()) ) ;

	m_dataServer->networkInfo( QString("Client %1 connected").arg(m_socketDescriptor) ) ;

	exec() ;
}

/*!
 * Connecteur de réception d'une requête client. La demande est transmise au serveur
 * "métier" qui à charge de fournir la réponse à retourner au client.
 */

void QamTcpConnection::readyRead()
{
	QByteArray frame = m_socket->readAll() ;
	m_dataServer->networkInfo( QString("Request from client %1").arg(m_socketDescriptor) ) ;
	QByteArray response = m_dataServer->responseToClientRequest( frame ) ;
	if ( !response.isEmpty() )	m_socket->write( response ) ;
}

/*!
 * Connecteur de détection de la déconnexion par le client. Cette action provoque
 * la fin de vie du thread.
 */

void QamTcpConnection::disconnected()
{
	m_dataServer->networkInfo( QString("Client %1 disconnected").arg(m_socketDescriptor) ) ;

	m_socket->deleteLater() ;
	exit( 0 ) ;
}
