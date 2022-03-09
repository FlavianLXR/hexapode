/*  ---------------------------------------------------------------------------
 *  filename    :   qammodbusmap.cpp
 *  description :   IMPLEMENTATION de la classe QamModbusMap
 *					Modélisation des tables primaires Modbus
 *
 *	project     :	Qam Modbus over TCP/IP
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

#include "qammodbusmap.h"
#define QAMMODBUSMAP_VERSION	"2.1"	// update Qt 6.2.2 janvier 2022

/*! Constructeur. L'argument @a mode permet de spécifier le mode de fonctionnement
 de l'objet.
 @n En mode serveur, QamModbusMap gère un jeu de tables primaires locales
 chargé avec loadMap() et répond aux requêtes des clients.
 @n En mode client, la cartographie chargée avec loadMap() spécifie l'adresse
 du serveur et une image de ses tables primaires. L'objet QamModbusMap tient
 à jour le contenu de la cartographie locale en fonction des résultats de
 requêtes envoyées au serveur.*/

QamModbusMap::QamModbusMap(Mode mode, QObject* parent )
    : QamAbstractServer(parent)
    , m_verbose( true )
    , m_mode( mode )
    , m_host( "localhost" )
    , m_port( 502 )
    , m_desc( "" )
    , m_isServerAvailable( false )
    , m_mbapTi( 0 )
    , m_mbapPi( 0 )
    , m_mbapUi( 0xFF )
{
    m_nullData = new QamModbusData("NULL", 0, true, 0, this ) ;

    m_coils = new QList<QamModbusData>() ;
    m_discreteInputs = new QList<QamModbusData>() ;
    m_inputRegisters = new QList<QamModbusData>() ; ;
    m_holdingRegisters = new QList<QamModbusData>() ;
}

/*! Retourne la version courante de la classe. */

QString QamModbusMap::version()
{

//	return QString("QamModbusMap version %1.%2").arg(MODBUSMAP_VERSION_MAJOR).arg(MODBUSMAP_VERSION_MINOR) ;
    return QString("QamModbusMap version %1").arg(QAMMODBUSMAP_VERSION) ;
}

/*!
 * Nom d'une table primaire sous forme de chaine de caractères.
 * \param table : table concernée.
 * \return Nom de la table ou "Unknown" en cas d'échec.
 */

QString QamModbusMap::tableAsString(PrimaryTable table )
{
    switch( table ) {
    case Coil :				return QString("Coils") ;
    case DiscretInput :		return QString("Discret Inputs") ;
    case InputRegister :	return QString("Input Registers") ;
    case HoldingRegister :	return QString("Holding Registers") ;
    default :				return QString("Unknown") ;
    }
}

// ---------------------------------------------------------------------------
// dialogue réseau mode serveur
// ---------------------------------------------------------------------------

/*!
 * Méthode de réponse à une requête client Modbus TCP. Toute erreur détectée
 * dans la construction de la requête donne lieu à l'émission d'une
 * remontée d'information par le signal info(). Toute requête
 * d'écriture provoque en cas de succès l'émission du signal valueChanged()
 * pour les données primaires affectées.
 * \param request : trame MBAP + PDU de requête.
 * \return Trame MBAP + PDU de réponse, ou trame vide en cas de MBAP entrant invalide.
 */

//QByteArray QamModbusMap::responseToRequest(QByteArray& request )
QByteArray QamModbusMap::responseToClientRequest(QByteArray& request )
{
    QByteArray		resp = request ;
    PrimaryTable	table = Coil ;

/* commenté 07/2021
    m_exception = 0 ;
*/
    // analyse du header MBAP

    if ( request.count() < 7 ) {
        modbusInfo("missing or incomplete MBAP") ;
        return QByteArray() ;
    }

    // new 07/2021
    // une trame tcp peut contenir plusieurs trames MBAP-PDU ...
    // actuellement cela provoque une sortie avec le message précédent ("invalid MBAP Length") !
    // et la première trame n'est pas traitée !
    //
    quint16		mbapLen = ( (quint8)( request.at(4) ) << 8 ) + (quint8)( request.at(5) ) ;
    if ( request.count() > ( 6 + mbapLen ) ) {
        QByteArray ba = request.mid(7) ;
        responseToClientRequest( ba ) ;
    }
    m_exception = 0 ;
    //

    m_mbapTi  = ( (quint8)( request.at(0) ) << 8 ) + (quint8)( request.at(1) ) ;
    m_mbapPi  = ( (quint8)( request.at(2) ) << 8 ) + (quint8)( request.at(3) ) ;
    m_mbapLen = ( (quint8)( request.at(4) ) << 8 ) + (quint8)( request.at(5) ) ;
    m_mbapUi  =   (quint8)( request.at(6) ) ;

    if ( m_mbapPi != 0 ) {
        modbusInfo("invalid MBAP Protocol Identifier") ;
        return QByteArray() ;
    }

    if ( request.count() < ( 6 + m_mbapLen ) ) {
        modbusInfo("invalid MBAP Length") ;
        return QByteArray() ;
    }

    // analyse du PDU

/* commenté 07/2021
    if ( request.count() < 12 ) {
        modbusInfo("missing or incomplete PDU") ;
        return QByteArray() ;
    }
*/
    if ( m_verbose ) {
        QString s ;
        for ( int i = 7 ; i < request.count() ; ++i ) {
            s += QString("%1").arg( quint8( request.at(i) ), 2, 16, QLatin1Char('0') ).toUpper() ;
            s += " " ;
        }
        modbusInfo( "recv " + s ) ;
    }

    m_funct  =   (quint8)( request.at(7) ) ;
    m_addr   = ( (quint8)( request.at(8) ) << 8 ) + (quint8)( request.at(9) ) ;
    m_number = 0 ;
    quint16	nextWord = ( (quint8)( request.at(10) ) << 8 ) + (quint8)( request.at(11) ) ;
    quint8  n ;

    switch ( m_funct ) {
    case 1 :
    case 2 :	table = ( m_funct == 1 ? Coil : DiscretInput ) ;
                m_number = nextWord ;
                if (( m_number == 0)||( m_number > 2000 )) {
                    m_exception = 0x02 ;	// illegal data address
                    return exceptionResponse(resp, m_exception, "invalid PDU number" ) ;
                }
                break ;
    case 3 :
    case 4 :	table = ( m_funct == 3 ? HoldingRegister : InputRegister ) ;
                m_number = nextWord ;
                if (( m_number == 0)||( m_number > 125 )) {
                    m_exception = 0x02 ;	// illegal data address
                    return exceptionResponse(resp, m_exception, "invalid PDU number" ) ;
                }
                break ;
    case 5 :
    case 6 :	table = ( m_funct == 5 ? Coil : HoldingRegister ) ;
                m_number = 1 ;
                m_value  = nextWord ;
                break ;
    case 15 :
    case 16 :	table = ( m_funct == 15 ? Coil : HoldingRegister ) ;
                m_number = nextWord ;
                if (( m_number == 0)||( m_number > ( m_funct == 15 ? 2000 : 120 ) )) {
                    m_exception = 0x02 ;	// illegal data address
                    return exceptionResponse(resp, m_exception, "invalid PDU number" ) ;
                }
                if ( request.count() < 13 ) {
                    m_exception = 0x03 ;	// illegal data value
                    return exceptionResponse(resp, m_exception, "PDU data expected" ) ;
                }
                n = (quint8)( request.at(12) ) ;
                if (( m_funct == 15 )&&( n != ( m_number / 8 + ( m_number % 8 ? 1 : 0 ) ) )) {
                    m_exception = 0x03 ;	// illegal data value
                    return exceptionResponse(resp, m_exception, "invalid PDU data length" ) ;
                }
                if (( m_funct == 16 )&&( n != ( m_number * 2 ) )) {
                    m_exception = 0x03 ;	// illegal data value
                    return exceptionResponse(resp, m_exception, "invalid PDU data length" ) ;
                }
                if ( request.count() != ( 13 + n ) ) {
                    m_exception = 0x03 ;	// illegal data value
                    return exceptionResponse(resp, m_exception, "invalid PDU data count" ) ;
                }
                m_data = request.mid(13) ;
                break ;

    default :	m_exception = 0x01 ;	// illegal function
    }

    if (( !m_exception )&&( !isRangeAvailable(table, m_addr, m_addr + ( m_number - 1 ) ) )) {
        m_exception = 0x02 ;			// illegal data address
        return exceptionResponse(resp, m_exception, "invalid PDU address range" ) ;
    }

    if ( m_exception ) {
        return exceptionResponse(resp, m_exception, QString("exception code %1").arg(m_exception ) ) ;
    }

    // trame de réponse

    modbusInfo( QString("FC %1, Address %2, Number %3").arg(m_funct).arg(m_addr,4,16,QLatin1Char('0')).arg(m_number) ) ;

    if (( m_funct == 1 )||( m_funct == 2 )) {
        resp = resp.left(8) ;	// trame reçue jusqu'au Function Code
        n = m_number / 8 + ( m_number % 8 ? 1 : 0 ) ;
        resp.append( n ) ;
        quint8 byte = 0 ;
        quint8 bit = 0 ;
        for ( int i = 0 ; i < m_number ; ++i ) {
            quint8 v = ( data(table, m_addr + i ).value() ? 1 : 0 ) ;
            byte |= v << bit++ ;
            if (( bit == 8 )||( i == m_number - 1 )) {
                resp.append( byte ) ;
                byte = 0 ;
                bit = 0 ;
            }
        }
    }
    else if (( m_funct == 3 )||( m_funct == 4 )) {
        resp = resp.left(8) ;	// trame reçue jusqu'au Function Code
        n = m_number * 2 ;
        resp.append( n ) ;
        for ( int i = 0 ; i < m_number ; ++i ) {
            quint16 v = data(table, m_addr + i ).value() ;
            resp.append( ( v >> 8 ) & 0xFF ) ;
            resp.append( v & 0xFF ) ;
        }
    }
    else if ( m_funct == 5 ) {
        QamModbusData& d = data(table, m_addr ) ;
        d.setValue( m_value ? 1 : 0 ) ;
        emit valueChanged((int)table, d.name() ) ;
    }
    else if ( m_funct == 6 ) {
        QamModbusData& d = data(table, m_addr ) ;
        d.setValue( m_value ) ;
        emit valueChanged((int)table, d.name() ) ;
    }
    else if ( m_funct == 15 ) {
        resp = resp.left(12) ;	// trame reçue jusqu'au champ Number
        quint8 byte = 0 ;
        quint8 bit = 0 ;
        for ( int i = 0 ; i < m_number ; ++i ) {
            QamModbusData& d = data(table, m_addr + i ) ;

            m_mutex.lock() ;
            d.setValue( ( m_data[ byte ] >> bit ) & 0x01 ) ;
            m_mutex.unlock() ;

            emit valueChanged((int)table, d.name() ) ;
            if ( ++bit == 8 ) {
                bit = 0 ;
                byte++ ;
            }
        }
    }
    else if ( m_funct == 16 ) {
        resp = resp.left(12) ;	// trame reçue jusqu'au champ Number
        for ( int i = 0 ; i < m_number ; ++i ) {
            QamModbusData& d = data(table, m_addr + i ) ;
            quint16 v = ( (quint8)m_data[ i * 2 ] << 8 ) + (quint8)m_data[ i * 2 + 1 ] ;

            m_mutex.lock() ;
            d.setValue( v ) ;
            m_mutex.unlock() ;

            emit valueChanged((int)table, d.name() ) ;
        }
    }

    m_mbapLen = resp.count() - 6 ;
    resp[4] = ( m_mbapLen >> 8 ) & 0xFF ;
    resp[5] =   m_mbapLen & 0xFF ;

    if ( m_verbose ) {
        QString s ;
        for ( int i = 7 ; i < resp.count() ; ++i ) {
            s += QString("%1").arg( quint8( resp.at(i) ), 2, 16, QLatin1Char('0') ).toUpper() ;
            s += " " ;
        }
        modbusInfo( "send " + s ) ;
    }

    return resp ;
}

// test l'existence d'une plage d'adresses dans une table primaire [private]

bool QamModbusMap::isRangeAvailable(PrimaryTable table, quint16 addrBegin, quint16 addrEnd )
{
    for (quint32 r = addrBegin ; r <= addrEnd ; ++r ) {
        if ( !exists(table, (quint16)r ) )	return false ;
    }
    return true ;
}

// fabrique une trame Modbus d'exception [private]

QByteArray QamModbusMap::exceptionResponse(QByteArray& request, quint8 exceptionCode, const QString& message )
{
    QByteArray	resp = request.left(9) ;
    resp[4] = 0 ;
    resp[5] = 3 ;
    resp[7] = resp[7] | 0x80 ;
    resp[8] = exceptionCode ;

    modbusInfo( message ) ;

    return resp ;
}

// ---------------------------------------------------------------------------
// dialogue réseau mode client
// ---------------------------------------------------------------------------

/*!
 * Connecteur de traitement d'une réponse serveur Modbus TCP/IP qui doit
 * correspondre à une requête générée par l'émission du signal request() hérité
 * de QamAbstractServer. L'émission et la réception sont synchronisées.
 * Le signal request() est utilisé par les méthodes remoteValue() et
 * setRemoteValue(), il est déconseillé de l'employer directement en dehors
 * de ces méthodes.
 * La cartographie locale est mise à jour et le signal valueChanged() informe
 * des données modifiées. Les éventuelles erreurs sont remontées par le signal
 * info().
 * \param response : trame MBAP + PDU reçue.
 */

//void QamModbusMap::response(QByteArray response )
void QamModbusMap::responseFromServer(QByteArray response )
{
    if ( m_verbose ) {
        QString s ;
        for ( int i = 7 ; i < response.count() ; ++i ) {
            s += QString("%1").arg( quint8( response.at(i) ), 2, 16, QLatin1Char('0') ).toUpper() ;
            s += " " ;
        }
        modbusInfo( "recv " + s ) ;
    }

    quint16 mbapTi  = ( (quint8)( response.at(0) ) << 8 ) + (quint8)( response.at(1) ) ;
//	quint16 mbapPi  = ( (quint8)( response.at(2) ) << 8 ) + (quint8)( response.at(3) ) ;
//	quint16 mbapLen = ( (quint8)( response.at(4) ) << 8 ) + (quint8)( response.at(5) ) ;
//	quint8  mbapUi  =   (quint8)( response.at(6) ) ;
    quint8  funct   =   (quint8)( response.at(7) ) ;

    if ( mbapTi != m_mbapTi ) {
        modbusInfo("Unexpected Transaction Identifier" ) ;
        emit responseDone() ;
        return ;
    }
    if ( funct & 0x80 ) {
        modbusInfo( QString("Exception %1").arg( (quint8)( response.at(8) ) ) ) ;
        emit responseDone() ;
        return ;
    }
    if ( funct != m_funct ) {
        modbusInfo("Unexpected Function" ) ;
        emit responseDone() ;
        return ;
    }

    PrimaryTable	table = HoldingRegister ;

    if (( funct == 1 )||( funct == 5 )||( funct == 15 ))	table = Coil ;
    if ( funct == 2 )										table = DiscretInput ;
    if ( funct == 4 )										table = InputRegister ;

    // mise à jour de la cartographie locale

    // FC1 et FC2 : lecture de données 1 bit
    if (( funct == 1 )||( funct == 2 )) {
        quint8 n = (quint8)( response.at(8) ) ;
        quint8 num = m_number / 8 + ( m_number % 8 ? 1 : 0 ) ;
        if ( n == num ) {
            m_data = response.mid(9, n ) ;
            quint8 byte = 0 ;
            quint8 bit = 0 ;
            for ( int i = 0 ; i < m_number ; ++i ) {
                QamModbusData& d = data(table, m_addr + i ) ;
                m_mutex.lock() ;
                d.setValue( ( m_data[ byte ] >> bit ) & 0x01 ) ;
                m_mutex.unlock() ;
                emit valueChanged((int)table, d.name() ) ;
                if ( ++bit == 8 ) {
                    bit = 0 ;
                    byte++ ;
                }
            }
        }
        else	modbusInfo("Improper response" ) ;
    }
    // FC3 et FC4 : lecture de données 16 bits
    if (( funct == 3 )||( funct == 4 )) {
        quint8 n      = (quint8)( response.at(8) ) ;
        quint8 number = m_number ;
        quint16 addr  = m_addr ;
        if ( n == number * 2 ) {
            m_data = response.mid(9, n ) ;
            // premier élément en dernier pour màj valeur formatée...
            for ( int i = /*m_number*/number - 1 ; i >= 0 ; --i ) {
                QamModbusData& d = data(table, /*m_addr*/addr + i ) ;
                quint16 v = ( (quint8)m_data[ i * 2 ] << 8 ) + (quint8)m_data[ i * 2 + 1 ] ;
                // if ( i == 0 ) m_value = v ;
                m_mutex.lock() ;
                d.setValue( v ) ;
                m_mutex.unlock() ;
                emit valueChanged((int)table, d.name() ) ;
            }
        }
        else	modbusInfo("Improper response" ) ;
    }
    // FC5 et FC6 : response = echo to the request
    if (( funct == 5 )||( funct == 6 )) {
        quint16 addr   = ( (quint8)( response.at(8) ) << 8 ) + (quint8)( response.at(9) ) ;
        quint16 value  = ( (quint8)( response.at(10) ) << 8 ) + (quint8)( response.at(11) ) ;

        if (( value == m_value )&&( addr == m_addr )) {
            QamModbusData& d = data(table, addr ) ;
            m_mutex.lock() ;
            if ( funct == 5 )	d.setValue( value ? 1 : 0 ) ;
            else				d.setValue( value ) ;
            m_mutex.unlock() ;
            emit valueChanged((int)table, d.name() ) ;
        }
        else	modbusInfo("Improper response" ) ;
    }
    // FC15 : acquittement écriture données 1 bit
    if ( funct == 15 ) {
        quint16 addr   = ( (quint8)( response.at(8) ) << 8 ) + (quint8)( response.at(9) ) ;
        quint16 number = ( (quint8)( response.at(10) ) << 8 ) + (quint8)( response.at(11) ) ;

        if (( number == m_number )&&( addr == m_addr )) {
            quint8 byte = 0 ;
            quint8 bit = 0 ;
            for ( int i = 0 ; i < /*m_number*/number ; ++i ) {
                QamModbusData& d = data(table, /*m_addr*/addr + i ) ;
                m_mutex.lock() ;
                d.setValue( ( m_data[ byte ] >> bit ) & 0x01 ) ;
                m_mutex.unlock() ;
                emit valueChanged((int)table, d.name() ) ;
                if ( ++bit == 8 ) {
                    bit = 0 ;
                    byte++ ;
                }
            }
        }
        else	modbusInfo("Improper response" ) ;
    }
    // FC16 : acquittement écriture données 16 bits
    if ( funct == 16 ) {
        quint16 addr   = ( (quint8)( response.at(8) ) << 8 ) + (quint8)( response.at(9) ) ;
        quint16 number = ( (quint8)( response.at(10) ) << 8 ) + (quint8)( response.at(11) ) ;

        if (( number == m_number )&&( addr == m_addr )) {
            // premier élément en dernier pour màj valeur formatée...
            for ( int i = m_number - 1 ; i >= 0 ; --i ) {
                QamModbusData& d = data(table, /*m_addr*/addr + i ) ;
                quint16 v = ( (quint8)m_data[ i * 2 ] << 8 ) + (quint8)m_data[ i * 2 + 1 ] ;
                m_mutex.lock() ;
                d.setValue( v ) ;
                m_mutex.unlock() ;
                emit valueChanged((int)table, d.name() ) ;
            }
        }
        else	modbusInfo("Improper response" ) ;
    }

    emit responseDone() ;
}

// [private]

void QamModbusMap::setServerAvailable(bool serverAvailable )
{
    m_isServerAvailable = serverAvailable ;
}

// ---------------------------------------------------------------------------
// accesseurs tables/données/formats
// ---------------------------------------------------------------------------

/*!
 * Liste des données primaires d'une table.
 * \param table table concernée
 * \param withComposite pour ajouter les sous-éléments de données composées
 * \return Liste de noms, ou liste vide en cas d'échec.
 */

QStringList QamModbusMap::nameList(PrimaryTable table , bool withComposite )
{
    QStringList list ;

    if ( table == Coil )	{
        for ( int i = 0 ; i < m_coils->size() ; ++i ) {
            list << m_coils->at(i).name() ;
        }
    }
    if ( table == DiscretInput )	{
        for ( int i = 0 ; i < m_discreteInputs->size() ; ++i ) {
            list << m_discreteInputs->at(i).name() ;
        }
    }
    if ( table == InputRegister )	{
        for ( int i = 0 ; i < m_inputRegisters->size() ; ++i ) {

            QamModbusData& data = m_inputRegisters->operator [](i) ;
            if (( data.mask() == 0 )&&( !withComposite ))	continue ;
            list << m_inputRegisters->at(i).name() ;
        }
    }
    if ( table == HoldingRegister )	{
        for ( int i = 0 ; i < m_holdingRegisters->size() ; ++i ) {

            QamModbusData& data = m_holdingRegisters->operator [](i) ;
            if (( data.mask() == 0 )&&( !withComposite ))	continue ;
            list << m_holdingRegisters->at(i).name() ;
        }
    }
    return list ;
}

/*!
 * Liste des noms des données secondaires d'une donnée primaire. La donnée
 * primaire doit appartenir à une table de mots 16 bits.
 * \param name : nom de la donnée primaire.
 * \return Liste de noms, ou liste vide en cas d'échec.
 */

QStringList QamModbusMap::itemList(const QString& name )
{
    QStringList list ;

    for ( int i = 0 ; i < m_holdingRegisters->size() ; ++i ) {
        QamModbusData data = m_holdingRegisters->at(i) ;
        if ( data.name() == name ) {
            for ( int j = 1 ; j < data.itemsNumber() ; ++j )	list << data.name(j) ;
            return list ;
        }
    }
    for ( int i = 0 ; i < m_inputRegisters->size() ; ++i ) {
        QamModbusData data = m_inputRegisters->at(i) ;
        if ( data.name() == name ) {
            for ( int j = 1 ; j < data.itemsNumber() ; ++j )	list << data.name(j) ;
            return list ;
        }
    }
    return list ;
}

/*!
 * Liste des noms des données secondaires d'une donnée primaire. La donnée
 * primaire doit appartenir à une table de mots 16 bits.
 * \param table : table de mots concernée.
 * \param name : nom de la donnée primaire.
 * \return Liste de noms, ou liste vide en cas d'échec.
 */

QStringList QamModbusMap::itemList(PrimaryTable table, const QString& name )
{
    QStringList list ;
    if (( table != InputRegister )&&( table != HoldingRegister ))	return list ;

    QList<QamModbusData>*	tbl = ( table == InputRegister ? m_inputRegisters : m_holdingRegisters ) ;

    for ( int i = 0 ; i < tbl->size() ; ++i ) {
        QamModbusData data = tbl->at(i) ;
        if ( data.name() == name ) {
            for ( int j = 1 ; j < data.itemsNumber() ; ++j )	list << data.name(j) ;
        }
    }
    return list ;
}

/*!
 * Retourne le format d'affichage d'une donnée.
 * \param table : table concernée.
 * \param name : nom de la donnée primaire dans la table.
 * \return Chaine de caractères, ou chaine vide en cas d'échec.
 */
QString QamModbusMap::display(PrimaryTable table, const QString& name )
{
    if ( !exists(table, name ) )	return QString("") ;
    return data(table, name ).display() ;
}

/*!
 * Test d'existence d'une donnée.
 * \param table : table concernée
 * \param name : nom de la donnée dans la table.
 * \return true si donnée trouvée dans la table, false sinon.
 */

bool QamModbusMap::exists(PrimaryTable table, const QString& name )
{
    QList<QamModbusData>*	tbl = m_holdingRegisters ;

    if ( table == Coil )				tbl = m_coils ;
    else if ( table == DiscretInput )	tbl = m_discreteInputs ;
    else if ( table == InputRegister )	tbl = m_inputRegisters ;

    for ( int i = 0 ; i < tbl->size() ; ++i ) {
        if ( tbl->at(i).name() == name )	return true ;
    }

    return false ;
}

/*!
 * Test d'existence d'une donnée.
 * \param table : table concernée
 * \param address : addresse 0..9999 de la donnée dans la table.
 * \return true si donnée trouvée dans la table, false sinon.
 */

bool QamModbusMap::exists(PrimaryTable table, quint16 address )
{
    QList<QamModbusData>*	tbl = m_holdingRegisters ;

    if ( table == Coil )				tbl = m_coils ;
    else if ( table == DiscretInput )	tbl = m_discreteInputs ;
    else if ( table == InputRegister )	tbl = m_inputRegisters ;

    for ( int i = 0 ; i < tbl->size() ; ++i ) {
        if ( tbl->at(i).address() == address )	return true ;
    }

    return false ;
}

/*!
 * Accès à une donnée primaire.
 * \param table : table concernée
 * \param name : nom de la donnée dans la table.
 * \return référence sur l'objet QamModbusData cncerné, ou sur un objet de nom
 * "NULL" en cas d'échec.
 */

QamModbusData& QamModbusMap::data(PrimaryTable table, const QString& name )
{
    QList<QamModbusData>*	tbl = m_holdingRegisters ;

    if ( table == Coil )				tbl = m_coils ;
    else if ( table == DiscretInput )	tbl = m_discreteInputs ;
    else if ( table == InputRegister )	tbl = m_inputRegisters ;

    for ( int i = 0 ; i < tbl->size() ; ++i ) {
        if ( tbl->at(i).name() == name )	return tbl->operator [](i) ;
    }

    return *m_nullData ;
}

/*!
 * Accès à une donnée primaire.
 * \param table : table concernée
 * \param address : addresse 0..9999 de la donnée dans la table.
 * \return référence sur l'objet QamModbusData cncerné, ou sur un objet de nom
 * "NULL" en cas d'échec.
 */

QamModbusData& QamModbusMap::data(PrimaryTable table, quint16 address )
{
    QList<QamModbusData>*	tbl = m_holdingRegisters ;

    if ( table == Coil )				tbl = m_coils ;
    else if ( table == DiscretInput )	tbl = m_discreteInputs ;
    else if ( table == InputRegister )	tbl = m_inputRegisters ;

    for ( int i = 0 ; i < tbl->size() ; ++i ) {
        if ( tbl->at(i).address() == address )	return tbl->operator [](i) ;
    }

    return *m_nullData ;
}

// [private] vérification association 'table' / 'name' [private]
// avec 'name' un nom de donnée primaire, composée, ou secondaire
// retourne false si entrée non trouvée dans la table spécifiée
// si association existe, retourne true après affectation de :
// m_table : table primaire
// m_name : nom de la donnée primaire
// m_item : numéro d'élément ( non nul si 'name' est une donnée secondaire)

bool QamModbusMap::checkPrimaryName(PrimaryTable table, const QString& name )
{
    QString primaryName = name ;
    int		itemNumber = -1 ;

    if ( exists(table, name ) ) {				// donnée primaire / composée ?
        itemNumber = 0 ;						// alors indice toujours 0
    }
    else {										// sinon recherche dans liste des
        QStringList	nl = nameList( table ) ;	// données secondaires
        for ( int i = 0 ; i < nl.size() ; ++i ) {
            QStringList il = itemList( nl.at(i) ) ;
            for ( int j = 0 ; j < il.size() ; ++j ) {
                if ( il.at(j) == name ) {
                    primaryName = nl.at(i) ;	// donnée primaire
                    itemNumber = j + 1 ;		// item donnée secondaire
                }
            }
        }
    }

    if ( itemNumber == -1 )	return false ;		// non trouvé...

    m_table = table ;
    m_name = primaryName ;
    m_item = itemNumber ;
    return true ;
}

// [private] vérifie que 'value' est bien l'image d'une valeur compatible
// avec le 'display de 'data'
// retourne false en cas d'erreur
// sinon, retourne true après affectation de :
// m_addr :
// m_number :
// m_value :
// m_data :

bool QamModbusMap::checkFormattedValue(const QamModbusData& data, const QString& value )
{
    QString disp = data.display( m_item ) ;

    quint16 compositeValue[8] ;
    quint16	words = 1 ;
    bool ok = true ;

    if ( disp == "Hex" ) {
        compositeValue[0] = value.toUShort(&ok, 16 ) ;
    }
    else if ( disp == "Bool" ) {
        compositeValue[0] = ( value.toUInt(&ok, 10 ) ? 1 : 0 ) ;
    }
    else if ( disp == "Int" ) {
        compositeValue[0] = value.toInt(&ok, 10 ) ;
    }
    else if ( disp == "Uint" ) {
        compositeValue[0] = value.toUInt(&ok, 10 ) ;
    }
    else if ( disp == "Ascii" ) {
        QString s = value ;
        while ( s.size() < 2 )	s = " " + s ;
        quint8 b1 = s.at(0).toLatin1() ;
        quint8 b0 = s.at(1).toLatin1() ;
        compositeValue[0] = ( (quint16)b1 << 8 ) + (quint16)b0 ;
    }
    else if ( disp == "Bcd" ) {
        QString s = value ;
        while ( s.size() < 4 )	s = "0" + s ;
        quint8 q3 = s.at(0).toLatin1() - '0' ;
        quint8 q2 = s.at(1).toLatin1() - '0' ;
        quint8 q1 = s.at(2).toLatin1() - '0' ;
        quint8 q0 = s.at(3).toLatin1() - '0' ;
        if (( q3 > 9 )||( q2 > 9 )||( q1 > 9 )||( q0 > 9 ))	{
            ok = false ;
        }
        else {
            compositeValue[0] = ( (quint16)q3 << 12 ) + ( (quint16)q2 << 8 ) + ( (quint16)q1 << 4 ) + (quint16)q0 ;
        }
    }
    else if ( disp == "Float" ) {
        words = 2 ;
        float v = value.toFloat(&ok) ;
        quint16* p = (quint16*)(&v) ;
        compositeValue[0] = *p ;
        compositeValue[1] = *(p + 1 ) ;
    }
    else if ( disp == "Long" ) {
        words = 2 ;
        qint32 v = value.toLong(&ok, 10 ) ;
        quint16* p = (quint16*)(&v) ;
        compositeValue[0] = *p ;
        compositeValue[1] = *(p + 1 ) ;
    }
    else if ( disp == "Str8" ) {
        words = 4 ;
        QString s = value ;
        while ( s.size() < 8 )	s = s + " " ;
        for ( int i = 0 ; i < words ; ++i ) {
            quint8 b1 = s.at( i * 2 ).toLatin1() ;
            quint8 b0 = s.at( i * 2 + 1 ).toLatin1() ;
            compositeValue[i] = ( (quint16)b1 << 8 ) + (quint16)b0 ;
        }
    }
    else if ( disp == "Str16" ) {
        words = 8 ;
        QString s = value ;
        while ( s.size() < 16 )	s = s + " " ;
        for ( int i = 0 ; i < words ; ++i ) {
            quint8 b1 = s.at( i * 2 ).toLatin1() ;
            quint8 b0 = s.at( i * 2 + 1 ).toLatin1() ;
            compositeValue[i] = ( (quint16)b1 << 8 ) + (quint16)b0 ;
        }
    }
    else return false ;

    if ( !ok ) return false ;

    m_addr = data.address() ;
    m_number = words ;
    m_value = compositeValue[0] ;
    m_data.clear() ;
    for ( int i = 0 ; i < m_number ; ++i ) {
        m_data.append( ( compositeValue[i] >> 8 ) & 0xFF ) ;
        m_data.append( compositeValue[i] & 0xFF ) ;
    }
    return true ;
}

// ---------------------------------------------------------------------------
// modificateurs de données Modbus
// ---------------------------------------------------------------------------

/*!
 * Modificateur générique d'une donnée de la cartographie. La valeur doit être
 * exprimée en conformité avec le format d'affichage de la donnée.
 * Cette méthode est équivalente à setLocalValue() en mode serveur ou à
 * setRemoteValue() en mode client.
 * \param table : table concernée.
 * \param name : nom de la donnée dans la table.
 * \param value : valeur exprimée au format d'affichage de la donnée.
 */

void QamModbusMap::setValue(PrimaryTable table, const QString& name, const QString& value )
{
    if ( m_mode == ServerMode )	return setLocalValue(table, name, value ) ;
    if ( m_mode == ClientMode )	return setRemoteValue(table, name, value ) ;
}

/*!
 * Demande de modification de valeur d'une donnée primaire/secondaire/composée dans
 * la cartographie locale ; méthode utilisable dans les deux modes serveur et client.
 * La modification est effective seulement si @a name existe dans @a table et si @a value
 * est exprimée en conformité avec le format d'affichage de la donnée.
 * La méthode émet un signal valueChanged() pour la donnée concernée et celles qui en sont
 * dépendent (cas des données secondaires et composées). Les signaux sont générés
 * même si @a value est invalide pour permettre au demandeur de récupérer la valeur
 * effective mise en forme, qu'elle soit modifiée ou non...
 * \param table : table concernée.
 * \param name : nom de la donnée dans la table.
 * \param value : valeur exprimée au format d'affichage de la donnée.
 */

void QamModbusMap::setLocalValue(PrimaryTable table, const QString& name, const QString& value )
{
    // init. m_table, m_name, m_item
    if ( !checkPrimaryName(table, name ) ) {
        modbusInfo( "can't find entry in table" ) ;
        return ;
    }

    QamModbusData&	dta = data(m_table, m_name ) ;

    // init. m_addr, m_number, m_value, m_data
    if ( !checkFormattedValue(dta, value ) ) {
        modbusInfo( "value incompatible with display format" ) ;
        emit valueChanged((int)m_table, m_name ) ;
        return ;
    }

    switch ( table ) {
        case Coil :
        case DiscretInput :
            m_mutex.lock() ;
            dta.setValue( m_value ) ;
            m_mutex.unlock() ;
            emit valueChanged((int)m_table, m_name ) ;
            break ;
        case InputRegister :
        default :
            // compléments d'une donnée composite
            for ( int i = 1 ; i < m_number ; ++i ) {
                quint16 val = ( (quint8)m_data[ i * 2 ] << 8 ) + (quint8)m_data[ i * 2 + 1 ] ;
                m_mutex.lock() ;
                data(m_table, m_addr + i ).setValue( val ) ;
                m_mutex.unlock() ;
                emit valueChanged((int)m_table, data(m_table, m_addr + i ).name() ) ;
            }
            // donnée primaire (m_item = 0) ou secondaire
            quint16 val = ( (quint8)m_data[ 0 ] << 8 ) + (quint8)m_data[ 1 ] ;
            m_mutex.lock() ;
            data(m_table, m_addr ).setValue( val, m_item ) ;
            m_mutex.unlock() ;
            emit valueChanged((int)m_table, data(m_table, m_addr ).name( m_item ) ) ;
            // si donnée secondaire, la primaire doit aussi être actualisée
            if ( m_item )	emit valueChanged((int)m_table, data(m_table, m_addr ).name() ) ;
            break ;
    }
}

/*!
 * Demande de modification de valeur d'une donnée par requête adressée au serveur (sans effet
 * en mode serveur ou si aucune connexion n'est active). La cartographie locale est mise à jour
 * en fonction de la réponse du serveur ; la valeur courante peut donc ensuite être
 * récupérée avec localValue().
 * @n La modification est effective seulement si @a name existe dans @a table et si @a value
 * est exprimée en conformité avec le format d'affichage de la donnée.
 * La méthode émet un signal valueChanged() pour la donnée concernée et celles qui en sont
 * dépendent (cas des données secondaires et composées). Les signaux sont générés
 * même si @a value est invalide pour permettre au demandeur de récupérer la valeur
 * effective mise en forme, qu'elle soit modifiée ou non...
 * \param table : table concernée.
 * \param name : nom de la donnée dans la table.
 * \param value : valeur exprimée au format d'affichage de la donnée.
 */

void QamModbusMap::setRemoteValue(PrimaryTable table, const QString& name, const QString& value )
{
    if ( m_mode != ClientMode )	return ;

    // init. m_table, m_name, m_item
    if ( !checkPrimaryName(table, name ) ) {
        modbusInfo( "can't find entry in table" ) ;
        return ;
    }

    if ( !m_isServerAvailable )	{
        modbusInfo("no available connection !" ) ;
        emit valueChanged((int)m_table, m_name ) ;
        return ;
    }

    if (( m_table != Coil )&&( m_table != HoldingRegister )) {
        modbusInfo( "unable to write in a read-only table" ) ;
        return ;
    }

    QamModbusData&	dta = data(m_table, m_name ) ;

    // init. m_addr, m_number, m_value, m_data
    if ( !checkFormattedValue(dta, value ) ) {
        modbusInfo( "value incompatible with display format" ) ;
        emit valueChanged((int)m_table, m_name ) ;
        return ;
    }

    // donnée secondaire ? il faut d'abord récupérer sa donnée primaire...
    if ( m_item ) {
        quint16 val = m_value ;	// mémo. info. liées à l'entrée secondaire...
        int item = m_item ;		// car écrasées par update suite à requête de lecture

        m_funct = 3 ;
        buildAndSendReadFrame() ;

        // applique la val. de l'item avec son masque...
        m_mutex.lock() ;
        data(m_table, m_addr ).setValue(val, item ) ;
        m_mutex.unlock() ;
        // ... et récupère la nouvelle vaL de la donnée primaire
        m_value = data(m_table, m_addr ).value() ;
    }

    if ( table == Coil ) {
        m_funct = 5 ;
        m_value = ( m_value ? 0xFF00 : 0x0000 ) ;
    }
    else {
        m_funct = ( m_number == 1 ? 6 : 16 ) ;
    }
    buildAndSendWriteFrame() ;

    //////
//	if ( m_name != name )	emit valueChanged(m_table, name ) ;
}

// FC5, FC6, FC16 [private]

void QamModbusMap::buildAndSendWriteFrame()
{
    m_mbapTi++ ;
    m_mbapLen = ( m_funct == 16 ? 7 + m_number * 2 : 6 ) ;
    QByteArray	frame ;
    frame.append( ( m_mbapTi >> 8 ) & 0xFF ) ;
    frame.append( m_mbapTi & 0xFF ) ;
    frame.append( ( m_mbapPi >> 8 ) & 0xFF ) ;
    frame.append( m_mbapPi & 0xFF ) ;
    frame.append( ( m_mbapLen >> 8 ) & 0xFF ) ;
    frame.append( m_mbapLen & 0xFF ) ;
    frame.append( m_mbapUi ) ;
    frame.append( m_funct ) ;
    frame.append( ( m_addr >> 8 ) & 0xFF ) ;
    frame.append( m_addr & 0xFF ) ;
    if ( m_funct == 16 ) {
        frame.append( ( m_number >> 8 ) & 0xFF ) ;
        frame.append( m_number & 0xFF ) ;
        frame.append( (quint8)( m_number * 2 ) ) ;
        frame.append( m_data ) ;
    }
    else {
        frame.append( ( m_value >> 8 ) & 0xFF ) ;
        frame.append( m_value & 0xFF ) ;
    }

    if ( m_verbose ) {
        QString s ;
        for ( int i = 7 ; i < frame.count() ; ++i ) {
            s += QString("%1").arg( quint8( frame.at(i) ), 2, 16, QLatin1Char('0') ).toUpper() ;
            s += " " ;
        }
        modbusInfo( "send " + s + " ( W: " + m_name + " )" ) ;
    }

    QEventLoop waitLoop ;
    connect(this, SIGNAL( responseDone() ), &waitLoop, SLOT( quit() ) ) ;

    emit request( frame ) ;

    waitLoop.exec() ;
}

// ---------------------------------------------------------------------------
// sélecteurs de données Modbus
// ---------------------------------------------------------------------------

/*!
 * Valeur mise en forme suivant le format d'affichage d'une donnée. Cette méthode
 * est équivalente à localValue() en mode serveur ou à remoteValue() en mode client.
 * \param table : table concernée.
 * \param name : nom de la donnée primaire dans la table.
 * \return Chaine de caractères, ou chaine vide en cas d'échec.
 */

QString QamModbusMap::value(PrimaryTable table, const QString& name )
{
    if ( m_mode == ServerMode )	return localValue(table, name ) ;
    if ( m_mode == ClientMode )	return remoteValue(table, name ) ;
    return QString("") ;
}

/*!
 * Sélecteur de donnée dans la cartographie locale. Cette méthode est utilisable
 * dans les deux modes (serveur ou client). La valeur retournée est mise en forme
 * suivant le format d'affichage de la donnée.
 * \param table : table concernée.
 * \param name : nom de la donnée primaire / secondaire / composée dans la table.
 * \return Chaine de caractères, ou chaine vide en cas d'échec.
 */

QString QamModbusMap::localValue(PrimaryTable table, const QString& name )
{
    // init. m_table, m_name, m_item
    if ( !checkPrimaryName(table, name ) ) {
        modbusInfo( "can't find entry in table" ) ;
        return QString("") ;
    }

    // accès à la donnée primaire de base
    QamModbusData&	dta = data(m_table, m_name ) ;

    // donnée secondaire demandée (16 bits) ?
    if ( m_item )	return dta.valueAsString( m_item ) ;

    // sinon, est-ce une donnée composée (N x 16 bits) ?
    m_addr = dta.address() ;
    quint16	compositeValue[8] ;

    if ( dta.display() == "Float" ) {
        compositeValue[0] = data(m_table, m_addr ).value() ;
        compositeValue[1] = data(m_table, m_addr + 1 ).value() ;
        float v = *( (float*)compositeValue ) ;
        return QString("%1").arg( v ) ;
    }
    else if ( dta.display() == "Long" ) {
        compositeValue[0] = data(m_table, m_addr ).value() ;
        compositeValue[1] = data(m_table, m_addr + 1 ).value() ;
        qint32 v = *( (qint32*)compositeValue ) ;
        return QString("%1").arg( v ) ;
    }
    else if ( dta.display() == "Str8" ) {
        QString res ;
        for (int i = 0 ; i < 4 ; ++i ) {
            compositeValue[i] = data(m_table, m_addr + i ).value() ;
            res += (char)( ( compositeValue[i] >> 8 ) & 0xFF ) ;
            res += (char)( compositeValue[i] & 0xFF ) ;
        }
        return res ;
    }
    else if ( dta.display() == "Str16" ) {
        QString res ;
        for (int i = 0 ; i < 8 ; ++i ) {
            compositeValue[i] = data(m_table, m_addr + i ).value() ;
            res += (char)( ( compositeValue[i] >> 8 ) & 0xFF ) ;
            res += (char)( compositeValue[i] & 0xFF ) ;
        }
        return res ;
    }
    // sinon c'est une donnée primaire (1 ou 16 bits), m_item = 0
    return dta.valueAsString() ;
}

/*!
 * Sélecteur de donnée par interrogation du serveur (sans effet en mode serveur
 * ou si aucune connexion n'est active). La cartographie locale est mise à jour
 * et la valeur retournée est mise en forme suivant le format d'affichage de la
 * donnée.
 * \param table : table concernée.
 * \param name : nom de la donnée primaire dans la table.
 * \return Chaine de caractères, ou chaine vide en cas d'échec.
 */

QString QamModbusMap::remoteValue(PrimaryTable table, const QString& name )
{
    if ( m_mode != ClientMode )		return QString("") ;

    // init. m_table, m_name, m_item
    if ( !checkPrimaryName(table, name ) ) {
        modbusInfo( "can't find entry in table" ) ;
        return QString("") ;
    }

    if ( !m_isServerAvailable )	{
        modbusInfo("no available connection !" ) ;
        emit valueChanged((int)m_table, m_name ) ;
        return QString("") ;
    }

    QamModbusData&	dta = data(m_table, m_name ) ;

    if ( m_table == Coil )					m_funct = 1 ;
    else if ( m_table == DiscretInput )		m_funct = 2 ;
    else if ( m_table == InputRegister )	m_funct = 4 ;
    else m_funct = 3 ;

    m_addr = dta.address() ;

    if (( dta.display() == "Float" )||( dta.display() == "Long" ))	m_number = 2 ;
    else if ( dta.display() == "Str8" )		m_number = 4 ;
    else if ( dta.display() == "Str16" )	m_number = 8 ;
    else m_number = 1 ;

    buildAndSendReadFrame() ;

    return localValue(m_table, name ) ;
}

// FC1, FC2, FC3, FC4 [private]

void QamModbusMap::buildAndSendReadFrame()
{
    m_mbapTi++ ;
    m_mbapLen = 6 ;
    QByteArray	frame ;
    frame.append( ( m_mbapTi >> 8 ) & 0xFF ) ;
    frame.append( m_mbapTi & 0xFF ) ;
    frame.append( ( m_mbapPi >> 8 ) & 0xFF ) ;
    frame.append( m_mbapPi & 0xFF ) ;
    frame.append( ( m_mbapLen >> 8 ) & 0xFF ) ;
    frame.append( m_mbapLen & 0xFF ) ;
    frame.append( m_mbapUi ) ;
    frame.append( m_funct ) ;
    frame.append( ( m_addr >> 8 ) & 0xFF ) ;
    frame.append( m_addr & 0xFF ) ;
    frame.append( ( m_number >> 8 ) & 0xFF ) ;
    frame.append( m_number & 0xFF ) ;

    if ( m_verbose ) {
        QString s ;
        for ( int i = 7 ; i < frame.count() ; ++i ) {
            s += QString("%1").arg( quint8( frame.at(i) ), 2, 16, QLatin1Char('0') ).toUpper() ;
            s += " " ;
        }
        modbusInfo( "send " + s + " ( R: " + m_name + " )" ) ;
    }

    QEventLoop waitLoop ;
    connect(this, SIGNAL( responseDone() ), &waitLoop, SLOT( quit() ) ) ;

    emit request( frame ) ;

    waitLoop.exec() ;
}

// ---------------------------------------------------------------------------
// création de la cartographie locale (remplissage de la 'modbus map')
// ---------------------------------------------------------------------------

/*!
 * Chargement d'une configuration par fichier CSV (encodage UTF-8). Les lignes du fichier
 * liées aux entrées de table sont analysées par la méthode addData().
 * \param filename : chemin d'accès au fichier.
 * \return false si le fichier ne peut être ouvert, true sinon.
 */

bool QamModbusMap::loadMap(const QString& filename )
{
    QFile	file( filename ) ;

//	if ( file.open( QIODevice::ReadOnly | QIODevice::Text ) ) {
//		QTextStream in( &file ) ;

    if ( file.open( QIODevice::ReadOnly) ) {		// new 1.4
        QByteArray fileBytes = file.readAll() ;
        fileBytes.replace("\r\n", "\n" ) ;
        fileBytes.replace("\r", "\n" ) ;
        QTextStream in( fileBytes ) ;

//		in.setCodec( "UTF-8" ) ;					// new 1.4
        in.setEncoding(QStringConverter::Utf8) ;    // new v2.1

        QString line = in.readLine() ;
        int nline = 1 ;

        addDataInfo("loading " + filename + "..." ) ;

        while ( !line.isNull() ) {

            if (( line.isEmpty() )||( line.startsWith("#") )) {
                line = in.readLine() ;
                nline++ ;
                continue;
            }

            QStringList	parse = line.split(";") ;

            if ( parse.size() == 2 ) {
                if ( parse[0] == "HOST" ) {
                    if ( m_mode == ClientMode )	m_host = parse[1].trimmed() ;
                }
                else if ( parse[0] == "PORT" ) {
                    m_port = parse[1].trimmed().toUShort() ;
                }
                else if ( parse[0] == "INFO" ) {
                    m_desc = parse[1].trimmed() ;
                }
                else {
                    addDataInfo("unrecognized line", nline ) ;
                }
            }
            else if ( parse.size() == MODBUSMAP_ENTRY_SIZE ) {
                for ( int i = 0 ; i < MODBUSMAP_ENTRY_SIZE ; ++i ) {
                    parse[i] = parse[i].trimmed() ;
                }
                addData( parse, nline ) ;
            }
            else {
                addDataInfo("invalid number of fields", nline ) ;
            }

            line = in.readLine() ;
            nline++ ;
        }
        file.close() ;
        return true ;
    }
    addDataInfo(filename + " not found!") ;
    return false ;
}

/*!
 * Cette méthode est utilisée par loadMap() pour l'ajout d'une donnée par liste
 * de caractéristiques, elle ne doit théoriquement jamais être employée
 * directement car elle contourne la notion de fichier de configuration commun
 * entre client et serveur. Sa visibilité publique permet néanmoins de mettre
 * en oeuvre rapidement un client devant communiquer avec un serveur à
 * configuration figée (Attention, un fichier de configuration est quand même
 * requis dans cette situation pour spécifier l'adresse et le port de service).
 * @n Toute erreur détectée lors de l'analyse d'une caractéristique donne lieu à
 * l'émission du signal modbusInfo() et à un retour négatif.
 * \param entry : liste des 7 champs (voir description du format des fichiers de
 * configuration).
 * \param line : argument utilisé par loadMap(), à laisser à 0 sinon.
 * \return true en cas de succès, false sinon.
 */

bool QamModbusMap::addData(const QStringList& entry, int line )
{
    if ( entry.size() != MODBUSMAP_ENTRY_SIZE )	return false ;

    bool ok ;
    quint32	number = entry.at(0).toULong(&ok, 10 ) ;
    if ( !ok ) {
        addDataInfo("error while number field conversion", line ) ;
        return false ;
    }

    // number, valeurs interdites : 0, 10000, 20000..30000, 40000, 105537..

    if (( number < 40001L )&&( number % 10000L == 0 )) {
        addDataInfo("number out of range", line ) ;
        return false ;
    }
    if (( number / 10000L == 2 )||(number > 105536L ) ){
        addDataInfo("number out of range", line ) ;
        return false ;
    }

    quint16 words = entry.at(1).toUShort(&ok, 10 ) ;
    if ( !ok ) {
        addDataInfo("error while words field conversion", line ) ;
        return false ;
    }

    quint16 mask = entry.at(2).toUShort(&ok, 16 ) ;
    if ( !ok ) {
        addDataInfo("error while mask field conversion", line ) ;
        return false ;
    }

    QString name = entry.at(3) ;
    QString comment = entry.at(4) ;

//	QString display = entry.at(5) ;				// new 1.4
    QString display = entry.at(5).toLower() ;
    display.replace(0, 1, display[0].toUpper() ) ;

    quint16 value = 0 ;
    quint16 compositeValue[8] ;

    if ( display == "Hex" ) {					// 0000..FFFF (hex)
        value = entry.at(6).toUShort(&ok, 16 ) ;
        if ( !ok ) {
            addDataInfo("error while value conversion", line ) ;
            return false ;
        }
    }
    else if ( display == "Bool" ) {				// 0 | 1
        value = ( entry.at(6).toUInt(&ok, 10 ) ? 1 : 0 ) ;
        if ( !ok ) {
            addDataInfo("error while value conversion", line ) ;
            return false ;
        }
    }
    else if ( display == "Int" ) {				// -32768..+32767
        value = entry.at(6).toInt(&ok, 10 ) ;
        if ( !ok ) {
            addDataInfo("error while value conversion", line ) ;
            return false ;
        }
    }
    else if ( display == "Uint" ) {				// 0..65535
        value = entry.at(6).toUInt(&ok, 10 ) ;
        if ( !ok ) {
            addDataInfo("error while value conversion", line ) ;
            return false ;
        }
    }
    else if ( display == "Ascii" ) {			// "XX"
        QString s = entry[6] ;
        while ( s.size() < 2 )	s = " " + s ;
        quint8 b1 = s.at(0).toLatin1() ;
        quint8 b0 = s.at(1).toLatin1() ;
        value = ( (quint16)b1 << 8 ) + (quint16)b0 ;
    }
    else if ( display == "Bcd" ) {				// 0000..9999 (hex)
        QString s = entry[6] ;
        while ( s.size() < 4 )	s = "0" + s ;
        quint8 q3 = s.at(0).toLatin1() - '0' ;
        quint8 q2 = s.at(1).toLatin1() - '0' ;
        quint8 q1 = s.at(2).toLatin1() - '0' ;
        quint8 q0 = s.at(3).toLatin1() - '0' ;
        if (( q3 > 9 )||( q2 > 9 )||( q1 > 9 )||( q0 > 9 ))	{
            addDataInfo("invalid BCD value", line ) ;
            return false ;
        }
        else {
            value = ( (quint16)q3 << 12 )
                  + ( (quint16)q2 << 8 )
                  + ( (quint16)q1 << 4 )
                  +   (quint16)q0 ;
        }
    }
    else if ( display == "Float" ) {			// IEEE 754 / IEC 559
        if ( words != 2 ) {
            addDataInfo("incompatible words/display fields", line ) ;
            return false ;
        }
        float v = entry.at(6).toFloat(&ok) ;
        if ( !ok ) {
            addDataInfo("error while value conversion", line ) ;
            return false ;
        }
        quint16* p = (quint16*)(&v) ;
        compositeValue[0] = *p ;
        compositeValue[1] = *(p + 1 ) ;
    }
    else if ( display == "Long" ) {				// -2147483648..+2147483647
        if ( words != 2 ) {
            addDataInfo("incompatible words/display fields", line ) ;
            return false ;
        }
        qint32 v = entry.at(6).toLong(&ok, 10 ) ;
        if ( !ok ) {
            addDataInfo("error while value conversion", line ) ;
            return false ;
        }
        quint16* p = (quint16*)(&v) ;
        compositeValue[0] = *p ;
        compositeValue[1] = *(p + 1 ) ;
    }
    else if ( display == "Str8" ) {				// "XXXXXXXX"
        if ( words != 4 ) {
            addDataInfo("incompatible words/display fields", line ) ;
            return false ;
        }
        QString s = entry[6] ;
        while ( s.size() < 8 )	s = s + " " ;
        for ( int i = 0 ; i < words ; ++i ) {
            quint8 b1 = s.at( i * 2 ).toLatin1() ;
            quint8 b0 = s.at( i * 2 + 1 ).toLatin1() ;
            compositeValue[i] = ( (quint16)b1 << 8 ) + (quint16)b0 ;
        }
    }
    else if ( display == "Str16" ) {			// "XXXXXXXXXXXXXXXX"
        if ( words != 8 ) {}
        QString s = entry[6] ;
        while ( s.size() < 16 )	s = s + " " ;
        for ( int i = 0 ; i < words ; ++i ) {
            quint8 b1 = s.at( i * 2 ).toLatin1() ;
            quint8 b0 = s.at( i * 2 + 1 ).toLatin1() ;
            compositeValue[i] = ( (quint16)b1 << 8 ) + (quint16)b0 ;
        }
    }
    else {
        addDataInfo("invalid display field", line ) ;
        return false ;
    }

    // numéro de table et offset

    int table = number / 10000L ;
    if ( table > 4 ) table = 4 ;

    quint16 address = (quint16)( number - table * 10000L - 1L ) ;

    ok = true ;

    for ( int i = 0 ; i < words ; ++i ) {
        if (( exists( (PrimaryTable)table, address + i ) )&&( mask == 0xFFFF )) {
            QString s( tableAsString( (PrimaryTable)table ) ) ;
            s += ", address " + QString("%1").arg(address + i, 4, 16, QLatin1Char('0')).toUpper() ;
            s += " already in use" ;
            addDataInfo(s, line ) ;
            ok = false ;
        }
    }

    if ( !ok ) return false ;

    if (( table == 0 )||( table == 1 )) {
        ok = addBitData( (PrimaryTable)table, address, name, comment, value ) ;
    }
    else {
        if ( words == 1 ) {		// donnée 16 bits, ou item 16 bits
            ok = addWordData( (PrimaryTable)table, address, mask, name, comment, display, value ) ;
        }
        else {					// donnée composite ( N x 16 bits )
            ok = addWordData( (PrimaryTable)table, address, 0xFFFF, name, comment, display, compositeValue[0] ) ;
            for (int i = 1 ; i < words ; ++i ) {
                // attention : 'name' ne doit pas être vide !!!!
                QString n( name ) ;
                n += QString(" [%1]").arg(i) ;
                ok &= addWordData( (PrimaryTable)table, address + i, 0x0000, n, "", "Hex", compositeValue[i] ) ;
            }
        }
    }

    if ( !ok ) {
        QString s( tableAsString( (PrimaryTable)table ) ) ;
        s += ", error while adding data" ;
        addDataInfo(s, line ) ;
        return false ;
    }

    return true ;
}

// ajout d'une donnée de table 1 bit [private]

bool QamModbusMap::addBitData(PrimaryTable table, quint16 address, const QString& name, const QString& comment, quint16 value )
{
    if (( table != DiscretInput )&&( table != Coil ))	return false ;

    QamModbusData*	data = new QamModbusData(name, address, false, value, this ) ;
    data->setComment( comment ) ;
    data->setDisplay("Bool" ) ;

    if ( table == Coil )	m_coils->append( *data ) ;
    else					m_discreteInputs->append( *data ) ;

    return true ;
}

// ajout d'une donnée primaire ou item de donnée de table 16 bits [private]

bool QamModbusMap::addWordData(PrimaryTable table, quint16 address, quint16 mask, const QString& name, const QString& comment, const QString& display, quint16 value )
{
    if (( table != InputRegister )&&( table != HoldingRegister ))	return false ;

    QList<QamModbusData>*	list = ( table == InputRegister ? m_inputRegisters : m_holdingRegisters ) ;

    // nouvelle entrée dans la table ?

    if (( mask == 0xFFFF )||( mask == 0x0000 )) {
        QamModbusData* data = new QamModbusData(name, address, true, value, this ) ;
        data->setMask( mask ) ;
        data->setComment( comment ) ;
        data->setDisplay( display ) ;
        list->append( *data ) ;
        return true ;
    }

    // sinon, nouvel item pour entrée existante

    else {
        for ( int i = 0 ; i < list->size() ; ++i ) {
            QamModbusData& md = list->operator [](i) ;
            if ( md.address() == address ) {
                int itemId = md.addItem(name, mask, value ) ;
                md.setComment(comment, itemId ) ;
                md.setDisplay(display, itemId ) ;
                return true ;
            }
        }
    }
    return false ;
}

// ---------------------------------------------------------------------------
// Remontée d'informations
// ---------------------------------------------------------------------------

// message en provenance des couches TCP/IP [private]

void QamModbusMap::networkInfo(const QString& message )
{
    emit info("tcp/ip", message ) ;
}

// message lié à la configuration [private]

void QamModbusMap::configInfo(const QString& message )
{
    emit info("config", message ) ;
}

// message lié à l'analyse d'une trame Modbus entrante [private]

void QamModbusMap::modbusInfo(const QString& message )
{
    emit info("modbus", message ) ;
}

// relais message produit par addData() [private]

void QamModbusMap::addDataInfo(const QString& message, int line )
{
    QString s( message ) ;
    if ( line )	s += QString(" (line %1)").arg(line) ;
    configInfo( s ) ;
}

