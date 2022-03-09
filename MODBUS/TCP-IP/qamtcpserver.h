/*  ---------------------------------------------------------------------------
 *  filename    :   qamtcpserver.h
 *  description :   INTERFACE de la classe QamTcpServer
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

#ifndef QAMTCPSERVER_H
#define QAMTCPSERVER_H

/*!
  @file
  @brief Serveur TCP multithread
 */

/*!
 @class QamTcpServer
 @brief Serveur TCP multithread.

 La classe QamTcpServer permet la mise en oeuvre d'un serveur TCP/IP
 multi-clients. La logique applicative de réponse aux requêtes est assurée
 par un objet répondant au modèle de développement spécifié par la classe
 QamAbstractServer (voir documentation de QamAbstractServer).
 */

#include <QTcpServer>
#include "qamabstractserver.h"
#include "qamtcpconnection.h"

class QamTcpServer : public QTcpServer
{
    Q_OBJECT

  public:
    explicit QamTcpServer(QamAbstractServer* server, QObject* parent = 0 ) ;
    void start(int listenPort = 4000 ) ;

  public slots :
    void sockClose() ;

  protected:
    virtual void incomingConnection(qintptr socketDescriptor ) ;

  private:
    int					m_port ;
    QamAbstractServer*	m_dataServer ;
} ;

#endif // QAMTCPSERVER_H
