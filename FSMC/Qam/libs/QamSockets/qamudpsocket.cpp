/*  ---------------------------------------------------------------------------
 *  filename    :   qamudpsocket.cpp
 *  description :   IMPLEMENTATION de la classe QamUdpSocket
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
 
#include "qamudpsocket.h"
#include <QCoreApplication>

/*! Constructeur. */

QamUdpSocket::QamUdpSocket(QObject* parent )
	: QUdpSocket(parent)
	, m_distAddr( QHostAddress::Null )
	, m_distPort( 0 )
	, m_hostResolved( false )
{
	connect( this, SIGNAL( readyRead() ), this, SLOT( sockRead() ) ) ;
}

/*!
 * Attachement de la socket.
 * \param port : port d'attache, quelconque si nul.
 */

void QamUdpSocket::sockBind(quint16 port ) 
{
	bool res = bind( port ) ;
	QString s("socket bind" ) ;
	if ( port )	s += QString(" to port %1").arg(port) ;
	else		s += QString(" to random port" ) ; 
	if ( !res ) s += QString(" failed!" ) ;
	emit sockInfo( s ) ;
}

/*! Fermeture de la socket. */

void QamUdpSocket::sockClose()
{
	close() ;
	emit sockInfo("socket closed." ) ;
}

/*!
 * Retourne l'adresse de l'interlocuteur courant.
 * \sa setAddress(), setRemoteHost()
 */
 
QHostAddress QamUdpSocket::address() const { return m_distAddr ; }

/*!
 * Retourne le port de l'interlocuteur courant.
 * \sa setPort(), setRemoteHost()
 */
 
quint16 QamUdpSocket::port() const { return m_distPort ; }

/*!
 * Spécification du destinataire.
 * \param address : adresse de l'hôte.
 * \sa setRemoteHost()
 */
 
void QamUdpSocket::setAddress(const QHostAddress& address )
{
	m_distAddr = address ;
}

/*!
 * Spécification du port de destination.
 * \param port : numéro de port.
 */
 
void QamUdpSocket::setPort(quint16 port )
{
	m_distPort = port ;
}

/*!
 * Spécification de l'hôte distant.
 * Le cas échéant, cette méthode prend en charge la résolution de noms.
 * Elle est bloquante jusqu'à ce que la résolution d'adresse soit effective.
 * \param hostName : adresse IP ou nom réseau,
 * \param port : numéro de port.
 \sa setAddress(), setPort()
 */
 
void QamUdpSocket::setRemoteHost(const QString& hostName, quint16 port )
{
	if ( port )	m_distPort = port ;

	emit sockInfo("trying to resolve host address..." ) ;	

	QHostInfo::lookupHost(hostName, this, SLOT( lookupHost(QHostInfo) ) ) ;

	// attente résolution d'adresse effective
	while ( !m_hostResolved )	QCoreApplication::processEvents() ;
	emit sockInfo("host address resolved." ) ;
}

/*!
 * UDP host address resolution.
 * \internal
 */
 
void QamUdpSocket::lookupHost(const QHostInfo& hostInfo )
{
	if ( hostInfo.error() != QHostInfo::NoError ) {
		emit sockInfo( QString("host not found!") ) ;
		return ;
	}

	QList<QHostAddress> addresses = hostInfo.addresses() ;

	if ( addresses.first().toString() == "::1" )				// loopback IPv6
			setAddress(addresses.at(1) ) ;						// 127.0.0.1
	else	setAddress(addresses.first() ) ;

	m_hostResolved = true ;
}

/*!
 * \internal
 */
 
void QamUdpSocket::sockRead()
{
	while ( hasPendingDatagrams() ) {
		QByteArray  datagram ;
		datagram.resize( pendingDatagramSize() ) ;
		readDatagram(datagram.data(), datagram.size(), &m_distAddr, &m_distPort ) ;
		emit sockInfo(QString("RECV from %1:%2").arg( m_distAddr.toString() ).arg( m_distPort) ) ;
		emit sockReceived( datagram ) ;
	}
}

/*!
 * Demande d'émission à destination du dernier hôte connu (spécifié au préalable
 * par setRemoteHost() ou détecté comme émetteur du dernier datagramme reçu).
 * \param data : datagramme (suite linéaire d'octets) à émettre.
 * \port : port de destination (par défaut, port du dernier interlocuteur)
 */

void QamUdpSocket::sockWrite(const QByteArray& data, quint16 port )
{
	if ( m_distAddr.isNull() ) {
		emit sockInfo("unknown host..." ) ;
		return ;
	}
	if ( port == 0 ) port = m_distPort ;
	writeDatagram( data.data(), data.size(), m_distAddr, port ) ;

	QString s = QString("SEND local port %1 ").arg( localPort() ) ;
	s += QString("to %1:%2").arg( m_distAddr.toString() ).arg( port ) ;
	emit sockInfo( s ) ;
}
