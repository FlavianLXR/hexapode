/*  ---------------------------------------------------------------------------
 *  filename    :   qammodbusdata.cpp
 *  description :   IMPLEMENTATION de QamModbusData et QamModbusDataItem
*					Modélisation d'une donnée de table primaire, 1 ou 16 bits
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

#include "qammodbusdata.h"

/*! Constructeur. Initialise l'adresse @a address de la donnée primaire et sa
  taille 1 ou 16 bits (argument @a data16bits) ; assure la création d'un premier
  élément QamModbusDataItem (indice 0) avec le nom @a name et la valeur @a value.
  */

QamModbusData::QamModbusData(const QString& name, quint16 address, bool data16bits, quint16 value, QObject* parent )
	: QObject(parent)
	, m_isWord( data16bits )
	, m_address( address )
{
	m_words = ( data16bits ? 1 : 0 ) ;

	QamModbusDataItem	mainItem(name, 0xFFFF, value ) ;
	m_items << mainItem ;
}

/*!	Constructeur de copie. */

QamModbusData::QamModbusData(const QamModbusData& data )
	: QObject( 0 )
{
	m_isWord = data.m_isWord ;
	m_address = data.m_address ;
	for ( int i = 0 ; i < data.m_items.size() ; ++i )	m_items.append( data.m_items[i] ) ;
}

/*! Surcharge de l'opérateur d'affectation. */

QamModbusData& QamModbusData::operator =(const QamModbusData& data )
{
	m_isWord = data.m_isWord ;
	m_address = data.m_address ;
	for ( int i = 0 ; i < data.m_items.size() ; ++i )	m_items.append( data.m_items[i] ) ;
	return *this ;
}

// accesseurs champ nom (texte libre)
// ---------------------------------------------------------------------------

QString QamModbusData::name(int itemId ) const
{
	if ( itemId < m_items.size() )	return m_items[itemId].name() ;
	return QString("") ;
}

void QamModbusData::setName(const QString &name, int itemId )
{
	if ( itemId >= m_items.size() )	return ;
	m_items[itemId].setName( name ) ;
}

// accesseurs champ commentaire (texte libre)
// ---------------------------------------------------------------------------

QString QamModbusData::comment(int itemId ) const
{
	if ( itemId < m_items.size() )	return m_items[itemId].comment() ;
	return QString("") ;
}

QString QamModbusData::comment(const QString& name ) const
{
	for ( int i = 0 ; i < m_items.size() ; ++i ) {
		if ( m_items[i].name() == name )	return m_items[i].comment() ;
	}
	return QString("") ;
}

void QamModbusData::setComment(const QString& comment, int itemId )
{
	if ( itemId < m_items.size() )	m_items[itemId].setComment( comment ) ;
}

void QamModbusData::setComment(const QString& comment, const QString& name )
{
	for ( int i = 0 ; i < m_items.size() ; ++i ) {
		if ( m_items[i].name() == name )	m_items[i].setComment( comment ) ;
	}
}

// accesseurs valeur brute (1 ou 16 bits)
// ---------------------------------------------------------------------------

quint16 QamModbusData::value(int itemId ) const
{
	if ( itemId < m_items.size() )	return m_items[itemId].value() ;
	return 0 ;
}

quint16 QamModbusData::value(const QString& name ) const
{
	for ( int i = 0 ; i < m_items.size() ; ++i ) {
		if ( m_items[i].name() == name )	return m_items[i].value() ;
	}
	return 0 ;
}

bool QamModbusData::setValue(quint16 value, int itemId )
{
	if ( itemId >= m_items.size() )	return false ;
	return setValue( value, m_items[itemId].name() ) ;
}

bool QamModbusData::setValue(quint16 value, const QString& name )
{
	for ( int i = 0 ; i < m_items.size() ; ++i ) {
		if ( m_items[i].name() == name ) {
			m_items[i].setValue( value ) ;
			quint16 newGlobalValue = m_items[0].value() ;
			// si affectation globale, mise à jour de la valeur des items
			if ( i == 0 ) {
				for ( int j = 1 ; j < m_items.size() ; ++j ) {
					m_items[j].setValue( ( newGlobalValue & m_items[j].mask() ) >> m_items[j].offset() ) ;
				}
			}
			// sinon, mise à jour de la valeur globale
			else {
				newGlobalValue &= ~m_items[i].mask() ;
				newGlobalValue |= ( value << m_items[i].offset() ) & m_items[i].mask() ;
				m_items[0].setValue( newGlobalValue ) ;
			}
			emit valueChanged( m_items[i].name() ) ;
			return true ;
		}
	}
	return false ;
}

// accesseurs masque 16 bits
// ---------------------------------------------------------------------------

quint16 QamModbusData::mask(int itemId ) const
{
	if ( itemId < m_items.size() )	return m_items[itemId].mask() ;
	return 0 ;
}

quint16	QamModbusData::mask(const QString& name ) const
{
	for ( int i = 0 ; i < m_items.size() ; ++i ) {
		if ( m_items[i].name() == name )	return m_items[i].mask() ;
	}
	return 0 ;
}

bool QamModbusData::setMask(quint16 mask, int itemId )
{
	if ( itemId < m_items.size() )	return m_items[itemId].setMask( mask ) ;
	return false ;
}

bool QamModbusData::setMask(quint16 mask, const QString& name )
{
	for ( int i = 0 ; i < m_items.size() ; ++i ) {
		if ( m_items[i].name() == name )	return m_items[i].setMask( mask ) ;
	}
	return false ;
}

// accesseurs format d'affichage
// ---------------------------------------------------------------------------

QString QamModbusData::display(int itemId ) const
{
	if ( itemId < m_items.size() )	return m_items[itemId].display() ;
	return QString("") ;
}

QString QamModbusData::display(const QString& name ) const
{
	for ( int i = 0 ; i < m_items.size() ; ++i ) {
		if ( m_items[i].name() == name )	return m_items[i].display() ;
	}
	return QString("") ;
}

bool QamModbusData::setDisplay(const QString& display, int itemId )
{
	if ( itemId >= m_items.size() )	return false ;
	return setDisplay( display, m_items[itemId].name() ) ;
}

bool QamModbusData::setDisplay(const QString& display, const QString& name )
{
	for ( int i = 0 ; i < m_items.size() ; ++i ) {
		if ( m_items[i].name() == name ) {
			if ( i == 0 ) {
				if (( display == "Float" )||( display == "Long" ))	m_words = 2 ;
				if ( display == "Str8" )	m_words = 4 ;
				if ( display == "Str16" )	m_words = 8 ;
			}
			return m_items[i].setDisplay( m_isWord ? display : "Bool" ) ;
		}
	}
	return false ;
}

// valeur sous forme affichable (fonction de 'display')
// ---------------------------------------------------------------------------

/*!
 * Valeur sous forme affichable de la donnée primaire ou d'une de ses
 * données secondaires, fonction du format défini par la propriété
 * @ref property_display "display".
 * \param itemId : indice de l'élément (0 pour la donnée primaire)
 * \return Valeur formatée sous forme de chaine de caractères, ou chaine vide
 * en cas d'échec.
 */

QString QamModbusData::valueAsString(int itemId ) const
{
	if ( itemId < m_items.size() )	return m_items[itemId].valueAsString() ;
	return QString("") ;
}

/*!
 * Valeur sous forme affichable de la donnée primaire ou d'une de ses
 * données secondaires, fonction du format défini par la propriété
 * @ref property_display "display".
 * \param name : nom de l'élément
 * \return Valeur formatée sous forme de chaine de caractères, ou chaine vide
 * en cas d'échec.
 */

QString QamModbusData::valueAsString(const QString& name ) const
{
	for ( int i = 0 ; i < m_items.size() ; ++i ) {
		if ( m_items[i].name() == name )	return m_items[i].valueAsString() ;
	}
	return QString("") ;
}

// accès aux items d'une donnée 16 bits (item 0 = main item pour 1 ou 16 bits)
// ---------------------------------------------------------------------------

/*!
 * Retourne le nombre d'éléments de la donnée. Le résultat est toujours
 * supérieur ou égal à 1, l'élément d'indice 0 représentant la donnée primaire.
 * \return Nombre d'éléments.
 */

int QamModbusData::itemsNumber() const
{
	return m_items.size() ;
}

/*!
 * Numéro d'indice d'une donnée primaire ou secondaire.
 * \param name : nom de la donnée recherchée
 * \return 0 pour la donnée primaire, indice positif pour une donne secondaire ;
 * ou -1 si le nom n'est pa strouvé.
 */

int QamModbusData::itemId(const QString& name ) const
{
	for ( int i = 0 ; i < m_items.size() ; ++i ) {
		if ( m_items[i].name() == name )	return i ;
	}
	return -1 ;
}

/*!
 * Ajout d'une donnée secondaire.
 * \param name : nom de la donnée
 * \param mask : masque 16 bits
 * \param value : valeur de la donnée
 * \return  Indice de la nouvelle donnée secondaire ou -1 si le masque
 * est invalide ou losque la donnée primaire est de taille 1 bit.
 */

int QamModbusData::addItem(const QString& name, quint16 mask, quint16 value )
{
	if ( !m_isWord )	return -1 ;	// ne concerne que les données 16 bits

	QamModbusDataItem	newItem( name, mask, value ) ;
	if (( newItem.size() == -1 )||( newItem.offset() == -1 )) {
		return -1 ;
	}
	m_items << newItem ;
	setValue(value, name ) ;		// force màj valeur globale
	return m_items.size() - 1 ;
}

/*!
 * Vérifie la non-superposition des masques des données secondaires.
 * \return true si les masques sont cohérents, false sinon.
 */

bool QamModbusData::areMasksConsistent() const
{
	// ne concerne que les données 16 bits
	if ( !m_isWord )	return true ;
	// avec au moins deux items...
	if ( m_items.size() > 2 ) {
		quint16 res = m_items[1].mask() ;
		for ( int i = 2 ; i < m_items.size() ; ++i ) {
			// superposition ?
			if ( res & m_items[i].mask() )	return false ;
			// mémorise bits utilisés
			res |= m_items[i].mask() ;
		}
	}
	return true ;
}

// ---------------------------------------------------------------------------
// classe d'usage local
// ---------------------------------------------------------------------------

QamModbusDataItem::QamModbusDataItem(const QString& name, quint16 mask, quint16 value )
	: m_name( name )
	, m_mask( mask )
	, m_value( value )
	, m_display( "Hex" )
{
}

// modifie le masque, sauf si proposition invalide (masque non nul discontinu)

bool QamModbusDataItem::setMask(quint16 mask )
{
	quint16	oldMask = m_mask ;
	m_mask = mask ;
	if ( m_mask == 0x0000 )	return true ;

	if (( size() == -1 )||( offset() == -1 )) {
		m_mask = oldMask ;
		return false ;
	}
	return true ;
}

bool QamModbusDataItem::setDisplay(const QString& display )
{
	if ( display == "Hex" )		{ m_display = display ; return true ; }
	if ( display == "Bool" )	{ m_display = display ; return true ; }
	if ( display == "Int" )		{ m_display = display ; return true ; }
	if ( display == "Uint" )	{ m_display = display ; return true ; }
	if ( display == "Ascii" )	{ m_display = display ; return true ; }
	if ( display == "Bcd" )		{ m_display = display ; return true ; }

	if ( display == "Float" )	{ m_display = display ; return true ; }
	if ( display == "Long" )	{ m_display = display ; return true ; }
	if ( display == "Str8" )	{ m_display = display ; return true ; }
	if ( display == "Str16" )	{ m_display = display ; return true ; }

	return false ;
}

QString	QamModbusDataItem::valueAsString() const
{
	if ( m_display == "Bool" )	return QString("%1").arg(m_value ? "1" : "0" ) ;
	if ( m_display == "Int" )	return QString("%1").arg( (qint16)(m_value) ) ;
	if ( m_display == "Uint" )	return QString("%1").arg( m_value ) ;
	if ( m_display == "Ascii" ) return QString("%1%2").arg( char( ( m_value >> 8 ) & 0xFF ) ).arg( char( m_value & 0xFF ) ) ;
	if ( m_display == "Bcd" )	return QString("%1%2%3%4").arg( char( ( ( m_value >> 12 ) & 0xF ) + '0' ) ).arg( char( ( ( m_value >> 8 ) & 0xF ) + '0' ) ).arg( char( ( ( m_value >> 4 ) & 0xF ) + '0' ) ).arg( char( ( m_value & 0x0F ) + '0' ) ) ;

	// pour tous les autres 'display' (morceau de donnée composée),
	// valeur 16 bits retournée au format 'Hex'

	return QString("%1").arg(m_value, 4, 16, QLatin1Char('0') ).toUpper() ;
}

// taille du masque (nb. de bits à 1 adjacents), retourne -1 si non adjacents

int QamModbusDataItem::size() const
{
	quint16	m = m_mask ;
	int		n = 0 ;
	bool	b = false ;	// indicateur de discontinuité

	for ( int i = 0 ; i < 16 ; i++ ) {
		if ( m & 1 ) {
			if ( !b )	n++ ;
			else		return -1 ;
		}
		else {
			if ( n ) b = true ;
		}
		m >>= 1 ;
	}
	return n ;
}

// décalage du masque p/r bit 0, retourne -1 si masque nul

int QamModbusDataItem::offset() const
{
	quint16	m = m_mask ;
	int		n = 0 ;

	for ( int i = 0 ; i < 16 ; i++ ) {
		if ( m & 1 ) return n ;
		n++ ;
		m >>= 1 ;
	}
	return -1 ;
}
