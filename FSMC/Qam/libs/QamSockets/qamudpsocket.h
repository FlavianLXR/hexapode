/*  ---------------------------------------------------------------------------
 *  filename    :   qamudpsocket.h
 *  description :   INTERFACE de la classe QamUdpSocket
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

#ifndef QAMUDPSOCKET_H
#define QAMUDPSOCKET_H

/*!
  @file
  @brief Socket UDP
 */

/*!
 @class QamUdpSocket
 @brief Emetteur/récepteur de datagrammes UDP.

 QamUdpSocket peut être utilisée par une application pour établir
 un dialogue sous forme de datagrammes UDP avec un hôte distant.
 
 En mode serveur, la socket doit être attachée à un port par la méthode sockBind(),
 le serveur répond toujours par défaut au dernier émetteur connu.
 L'émetteur du dernier datagramme reçu est automatiquement
 utilisé pour les émissions suivantes par sockWrite().
 
 En mode client, l'attachement est réalisé par un appel à sockBind() sans argument.
 La spécification du destinataire est assurée par 
 setRemoteHost() qui prend en charge, le cas échéant, la résolution d'adresses ; 
 l'envoi de datagrammes est ensuite réalisé par sockWrite().
 
 Dans tous les cas, l'application doit intercepter les signaux sockReceived() et
 sockInfo() pour la réception de données et la remontée d'informations.
 La socket est fermée par appel à sockClose().
 
 Pour l'émission de datagrammes, l'application peut soit connecter un signal dédié sur le slot
 sockWrite(), soit invoquer directement celui-ci.
 
 Exemple de mise en oeuvre par une classe Qt d'usage disposant :
 - d'un attribut QamUdpSocket * udpSocket ;
 - d'un slot @b info() de traitement des messages d'information ;
 - d'un slot @b received() de traitement des datagrammes entrants ;
 - et d'un signal @b request() de demande d'émission d'un datagramme (facultatif).
 
 1- Création de la socket et mise en place des connexions :
 @code
	udpSocket = new QamUdpSocket( this ) ;
	connect( udpSocket, SIGNAL( sockInfo(const QString& ) ),        this,      SLOT( info(const QString& ) ) ) ;
	connect( udpSocket, SIGNAL( sockReceived(const QByteArray& ) ), this,      SLOT( received(const QByteArray& ) ) ) ;
    connect( this,      SIGNAL( request(const QByteArray& ) ),      udpSocket, SLOT( sockWrite(const QByteArray& ) ) ) ;
 @endcode 
 
 2a- Cas d'une socket en attente d'un datagramme ("serveur") :
 @code
	udpSocket->sockBind( <port> ) ;
 @endcode 
 
 2b- Cas d'une socket à l'initiative de l'envoi d'un datagramme ("client") :
 @code
    udpSocket->sockBind() ;
    udpSocket->setRemoteHost( <hostname>, <port> ) ;
 @endcode
 
 3- Fermeture de la socket
 @code 
    udpSocket->sockClose() ;
    delete udpSocket ;
 @endcode 
 */
 
#include "_ABOUT"
#include <QUdpSocket>
#include <QHostInfo>

class QamUdpSocket : public QUdpSocket
{
	Q_OBJECT

  public:
	explicit QamUdpSocket(QObject* parent = 0 ) ;

	void sockBind(quint16 port = 0 ) ;
	void sockClose() ;
	
	void setAddress(const QHostAddress& address ) ;
	void setPort(quint16 port ) ;
	
	void setRemoteHost(const QString& hostName, quint16 port ) ;

	QHostAddress address() const ;
	quint16 port() const ;
	
  private slots :
	void sockRead() ;
	void lookupHost(const QHostInfo& hostInfo ) ;

  public slots :
	void sockWrite(const QByteArray& data, quint16 port = 0 ) ;

  signals:
	/*! Signal émis lors de la réception d'un datagramme sur la socket UDP. */
	void sockReceived(const QByteArray& ) ;
	/*! Signal de remontée d'informations liées à l'état de la connexion. */
	void sockInfo(const QString& ) ;

  private :
	QHostAddress    m_distAddr ;
	quint16         m_distPort ;
	bool			m_hostResolved ;
} ;

#endif // QAMUDPCLIENT_H
