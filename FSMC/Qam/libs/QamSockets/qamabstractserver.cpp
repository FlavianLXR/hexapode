/*  ---------------------------------------------------------------------------
 *  filename    :   qamabstractserver.cpp
 *  description :   IMPLEMENTATION de la classe QamAbstractServer
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

#include "qamabstractserver.h"

/*! Constructeur. */

QamAbstractServer::QamAbstractServer(QObject* parent)
	: QObject(parent)
{
}

/*!
 * Mode Client TCP : méthode invoquée par QamTcpClient pour indiquer
 * l'état de la connexion avec un hôte distant.
 * \param serverAvailable : indicateur de connexion active ou non.
 */
 
void QamAbstractServer::setServerAvailable(bool serverAvailable )
{
	Q_UNUSED( serverAvailable ) ;
}

/*!
 * Mode Serveur TCP : connecteur utilisé par QamTcpServer / QamTcpConnection pour 
 * permettre la fabrication d'une réponse à une requête cliente.
 * Par défaut, la réponse produite est un 'echo' de la requête reçue, avec 
 * émission des signaux received() et sent().
 * \param request : requête reçue.
 */

QByteArray QamAbstractServer::responseToClientRequest(QByteArray& request )
{
	emit received( request ) ;
	emit sent( request ) ;
	return request ;
}

QByteArray QamAbstractServer::responseToRequest(QByteArray& request )	// OBSOLETE
{
	return this->responseToClientRequest( request ) ;
}

/*!
 * Mode Client TCP : connecteur invoqué par QamTcpClient pour permettre le traitement d'une 
 * réponse reçue suite à l'émission d'une requête via le signal request().
 * Par défaut, cette méthode émet simplement le signal received().
 * \param response : réponse reçue.
 */
 
void QamAbstractServer::responseFromServer(QByteArray response )
{
	emit received( response ) ;
}

void QamAbstractServer::response(QByteArray response )	// OBSOLETE
{
	this->responseFromServer( response ) ;
}

/*!
 * Connecteur de remontée des messages d'information liés aux connexions
 * et au trafic.
 * Par défaut, cette méthode émet simplement un signal info().
 * \param message : message d'information.
 */

void QamAbstractServer::networkInfo(const QString& message )
{
	emit info( message ) ;
}
