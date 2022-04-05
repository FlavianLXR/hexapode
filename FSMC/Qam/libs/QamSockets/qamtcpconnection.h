/*  ---------------------------------------------------------------------------
 *  filename    :   qamtcpconnection.h
 *  description :   INTERFACE de la classe QamTcpConnection
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

#ifndef QAMTCPCONNECTION_H
#define QAMTCPCONNECTION_H

/*!
  @file
  @brief Thread de gestion d'une connexion TCP
 */

/*!
 @class QamTcpConnection
 @brief Thread de gestion d'une connexion TCP.

 La classe QamTcpConnection prend en charge l'établissement et la gestion d'une
 connexion TCP avec un client. Elle utilise comme serveur pour répondre aux
 requêtes l'objet dérivé de QamAbstractServer (classe abstraite) reçu lors de
 la construction. Le serveur est aussi destinataire de différents messages de
 suivi de connexion et/ou d'erreur.
 */

#include <QThread>
#include <QTcpSocket>
#include "qamabstractserver.h"

class QamTcpConnection : public QThread
{
	Q_OBJECT

  public:
	explicit QamTcpConnection(qintptr id, QamAbstractServer* server, QObject* parent = 0 ) ;
	void run() ;

  private slots:
	void readyRead() ;
	void disconnected() ;

  private:
	QTcpSocket*			m_socket ;				// socket cliente
	qintptr				m_socketDescriptor ;
	QamAbstractServer*	m_dataServer ;			// serveur métier
} ;

#endif // QAMTCPCONNECTION_H
