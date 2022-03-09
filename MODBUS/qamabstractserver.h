/*  ---------------------------------------------------------------------------
 *  filename    :   qamabstractserver.h
 *  description :   INTERFACE de la classe QamAbstractServer
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

#ifndef QAMABSTRACTSERVER_H
#define QAMABSTRACTSERVER_H

/*!
  @file
  @brief Classe modèle "client/serveur métier TCP"
 */

/*!
 @class QamAbstractServer
 @brief Classe modèle "client/serveur métier TCP".

La classe QamAbstractServer sert de modèle de développement
pour l'interlocuteur des classes QamTcpServer et QamTcpConnection lors
de l'implémentation d'un serveur. Elle doit aussi être utilisée comme
modèle pour QamTcpClient lors de la conception d'une application maître.

En pratique, la classe QamAbstractServer modélise la couche TCP/IP 'Application'
(couches OSI 5 à 7) ; elle doit permettre au développeur de se concentrer sur
l'aspect 'métier' des échanges réseaux.

<b>Note de version</b> :
- la méthode virtuelle <b>responseToRequest()</b> est remplacée par responseToClientRequest()
- la méthode virtuelle <b>response()</b> est remplacée par responseFromServer()

<h3>Exemple minimal de client TCP</h3>

Par défaut, la méthode QamAbstractServer::responseFromServer() provoque l'émission
du signal received() ; la classe QamAbstractServer peut donc être utilisée directement
par QamTcpClient, sans spécialisation.

Soit une classe Qt d'usage disposant :
- d'un attribut QamAbstractServer * tcpJob ;
- d'un attribut QamTcpClient * tcpClient ;
- d'un signal <b>myRequest(const QByteArray& )</b> pour envoyer les requêtes ;
- d'un slot <b>myReceived(const QByteArray& )</b> pour intercepter les réponses ;
- d'un slot <b>myInfo(const QString& )</b> pour recevoir les messages liés au trafic réseau.

1- Démarrage du client
@code
    tcpJob = new QamAbstractServer(this) ;
    connect( tcpJob, SIGNAL( info(const QString&) ),          this,   SLOT( myInfo(const QString&) ) ) ;
    connect( tcpJob, SIGNAL( received(const QByteArray&) ) ), this,   SLOT( myReceived(const QByteArray&) ) ) ;
    connect( this,   SIGNAL( myRequest(const QByteArray&) ),  tcpJob, SIGNAL( request(const QByteArray&) ) ) ;

    tcpClient = new QamTcpClient(tcpJob, this ) ;
    tcpClient->sockConnect(<hostname>, <port> ) ;
@endcode

2- Envoi d'une requête : par émission du signal <b>myRequest()</b>

3- Réception d'une réponse : slot <b>myReceived()</b>

4- Fermeture du client
@code
    tcpClient->sockClose() ;
    delete tcpClient ;
    delete tcpJob ;
@endcode

<h3>Exemple minimal de serveur TCP</h3>

La classe QamAbstractServer, et plus particulièrement sa méthode responseToClientRequest(),
doit être spécialisée pour adapter les réponses à faire aux requêtes des clients.
@code
class MyTcpJob : public QamAbstractServer
{
    Q_OBJECT

  public:
    explicit MyTcpJob(QObject* parent = 0 ) : QamAbstractServer(parent) {}

  public slots:
    virtual QByteArray responseToClientRequest(QByteArray& request )
    {
        emit received( request ) ;

        QByteArray response = ... ; 	// TODO : construire la réponse à 'request'

        emit sent( response ) ;
        return response ;				// OBLIGATOIRE, réponse effective !
    }
} ;
@endcode
Soit une classe Qt d'usage disposant :
- d'un attribut <b>MyTcpJob</b> * tcpJob ;
- d'un attribut QamTcpServer * tcpServer ;
- d'un slot <b>mySent(const QByteArray& )</b> pour la remontée des réponse (facultatif)  ;
- d'un slot <b>myReceived(const QByteArray& )</b> pour la remontée des requêtes (facultatif) ;
- d'un slot <b>myInfo(const QString& )</b> pour recevoir les messages liés au trafic réseau.

1- Démarrage du serveur
@code
    tcpJob = new MyTcpJob(this) ;
    connect( tcpJob, SIGNAL( info(const QString&) ),          this, SLOT( myInfo(const QString&) ) ) ;
    connect( tcpJob, SIGNAL( received(const QByteArray&) ) ), this, SLOT( myReceived(const QByteArray&) ) ) ;
    connect( tcpJob, SIGNAL( sent(const QByteArray&) ),       this, SLOT( mySent(const QByteArray&) ) ) ;

    tcpServer = new QamTcpServer(tcpJob, this ) ;
    tcpServer->start( <port> ) ;
@endcode

2- Réponses aux requêtes : assurées par <b>MyTcpJob::responseToClientRequest()</b>

3- Fermeture du serveur
@code
    tcpServer->sockClose() ;
    delete tcpServer ;
    delete tcpJob ;
@endcode

 */

#include <QObject>
#include <QByteArray>

class QamAbstractServer : public QObject
{
    Q_OBJECT

  public:
    explicit QamAbstractServer(QObject* parent = 0 ) ;

    virtual void setServerAvailable(bool serverAvailable ) ;

  public slots:
    /*! \internal OBSOLETE : méthode remplacée par responseToClientRequest() */
    virtual QByteArray responseToRequest(QByteArray& request ) ;
    virtual QByteArray responseToClientRequest(QByteArray& request ) ;
    /*! \internal OBSOLETE : méthode remplacée par responseFromServer() */
    virtual void response(QByteArray response ) ;
    virtual void responseFromServer(QByteArray response ) ;
    virtual void networkInfo(const QString& message ) ;

  signals:
    /*! Signal de demande d'émission d'une requête (mode client). */
    void request(QByteArray request ) ;
    /*! Indicateur de données reçues. */
    void received(const QByteArray& data ) ;
    /*! Indicateur de données émises. */
    void sent(const QByteArray& data ) ;
    /*! Indicateur de message d'information. */
    void info(const QString& message ) ;
} ;

#endif // QAMABSTRACTSERVER_H
