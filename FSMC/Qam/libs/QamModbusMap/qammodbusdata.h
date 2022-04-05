/*  ---------------------------------------------------------------------------
 *  filename    :   qammodbusdata.h
 *  description :   INTERFACE des classes QamModbusData et QamModbusDataItem
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

#ifndef QAMMODBUSDATA_H
#define QAMMODBUSDATA_H

/*!
  @file
  @brief Modélisation d'une donnée de table primaire, 1 ou 16 bits
 */

/*!
 @class QamModbusData
 @brief Modélisation d'une donnée de table primaire, 1 ou 16 bits.

La classe QamModbusData représente une donnée de table primaire Modbus. La taille
de la donnée, 1 bit ou mot de 16 bits, est définie lors de la construction.
La donnée est identifiée par son adresse (offset dans la table Modbus), cette
adresse est encodée sur 16 bits et doit normalement se situer dans l'intervalle
0..9999 (soit 270E en hexadécimal) ; les valeurs supérieures, jusqu'à 65535 (FFFF
en héxadécimal) sont cependant acceptées.
@n En pratique, une donnée de type mot 16 bits peut être découpée fonctionnellement
en sous-parties (données secondaires) identifiées chacune par un masque (valeur
16 bits indiquant par un ou plusieurs bits à 1 adjacents la sous-partie de la
donnée primaire concernée).
@n La classe QamModbusData gére une liste d'éléments QamModbusDataItem dont le
premier (indice 0) modélise la donnée primaire. Chaque élément dispose d'un nom et
d'un champ commentaire (texte libre), d'un masque (toujours égal à $FFFF pour la
donnée primaire) et d'un format d'affichage.
@n Les formats d'affichage reconnus sont : Hex (hexadécimal), Bool (booléen),
Uint (valeur entière 16 bits non signée), Int (valeur entière 16 bits signée),
Ascii (chaine de 2 caractères) et Bcd (valeur entière 0..9999). Une donnée de
taille 1 bit est forcée au format Bool.
@n Losque la donnée 16 bits représente le premier mot d'un ensemble de mots
d'adresses contiguës, d'autres formats sont possibles : Float (valeur à virgule
flottante codée sur 32 bits, format IEEE 754 / IEC 559), Long (valeur entière
signée sur 32 bits), Str8 (chaine de 8 caractères, codée sur 4 mots) et Str16
(chaine de 16 caractères, codée sur 8 mots).
@n Plusieurs objets QamModbusData permettent de modéliser une donnée composée
de N mots ; par une donnée primaire de masque $FFFF qui maintient l'adresse de base
et le format d'affichage, et par N-1 autres données primaires avec les adresses
suivantes, de masque nul et de format d"affichage forcé à "Hex".

<hr><h2>Propriétés</h2>
<p>@anchor property_isworddata @c isWordData : vrai si donnée de taille 16 bits.
- bool @b isWordData () const ;

<p>@anchor property_wordscount @c wordsCount : vaut toujours 0 pour une donnée
de taille 1 bit ou 1 pour une donnée de taille 16 bits. Lorsque la donnée 16 bits
est le premier élément d'une donnée composée, retourne le nombre de mots de la
composition (2,4 ou 8 suivant le format d'affichage choisi).
- int @b wordsCount () const ;

<p>@anchor property_address @c address : adresse codée sur 16 bits.
- quint16 @b address () const ;
- void @b setAddress (quint16 @a address ) ;

<p>@anchor property_name @c name : nom de la donnée primaire ou d'une de ses
données secondaires.
- QString @b name (int @a itemId = 0 ) const ;
- void @b setName (const QString& @a name, int @a itemId = 0 ) ;

<p>@anchor property_comment @c comment : commentaire associé à la donnée
primaire ou à une de ses données secondaires.
- QString @b comment (int @a itemId = 0 ) const ;
- QString @b comment (const QString& @a name ) const ;
- void @b setComment (const QString& @a comment, int @a itemId = 0 ) ;
- void @b setComment (const QString& @a comment, const QString& @a name ) ;

<p>@anchor property_mask @c mask : masque 16 bits, toujours égal à $FFFF pour
une donnée primaire ; vaut 0 pour les éléments suivants d'une donnée composée.
- quint16 @b mask (int @a  itemId = 0 ) const ;
- quint16 @b mask (const QString& @a name ) const ;
- bool @b setMask (quint16 @a mask, int @a itemId = 0 ) ;
- bool @b setMask (quint16 @a mask, const QString& @a name ) ;

<p>@anchor property_display @c display : format d'affichage de la donnée
primaire ou d'une de ses données secondaires ; dans le cas d'une donnée
composée, le format d'affichage n'est attribué qu'au premier élément de
la suite, les autres ont toujours le format "Hex".
- QString @b display (int @a itemId = 0 ) const ;
- QString @b display (const QString& @a name ) const ;
- bool @b setDisplay (const QString& @a display, int @a itemId = 0 ) ;
- bool @b setDisplay (const QString& @a display, const QString& @a name ) ;

<p>@anchor property_value @c value : valeur brute d'une donnée primaire
ou d'une de ses données secondaires.
- quint16 @b value (int @a itemId = 0 ) const ;
- quint16 @b value (const QString& @a name ) const ;
- bool @b setValue (quint16 @a value, int @a itemId = 0 ) ;
- bool @b setValue (quint16 @a value, const QString& @a name ) ;

 */

#include <QObject>

class QamModbusDataItem ;

class QamModbusData : public QObject
{
	Q_OBJECT

  public:
	explicit QamModbusData(const QString& name, quint16 address, bool data16bits = true, quint16 value = 0, QObject* parent = 0 ) ;
	QamModbusData(const QamModbusData& data ) ;
	QamModbusData& operator =(const QamModbusData& data ) ;

	/*! Propriété @ref property_isworddata "isWordData". */
	inline bool isWordData() const { return m_isWord ; }
	/*! Propriété @ref property_wordscount "wordsCount". */
	inline int wordsCount() const { return m_words ; }

	/*! Propriété @ref property_address "address". */
	inline quint16 address() const { return m_address ; }
	/*! Propriété @ref property_address "address". */
	inline void setAddress(quint16 address ) { m_address = address ; }

	/*! Propriété @ref property_name "name". */
	QString name(int itemId = 0 ) const ;
	/*! Propriété @ref property_name "name". */
	void setName(const QString& name, int itemId = 0 ) ;

	/*! Propriété @ref property_comment "comment". */
	QString comment(int itemId = 0 ) const ;
	/*! Propriété @ref property_comment "comment". */
	QString comment(const QString& name ) const ;
	/*! Propriété @ref property_comment "comment". */
	void setComment(const QString& comment, int itemId = 0 ) ;
	/*! Propriété @ref property_comment "comment". */
	void setComment(const QString& comment, const QString& name ) ;

	/*! Propriété @ref property_mask "mask". */
	quint16 mask(int itemId = 0 ) const ;
	/*! Propriété @ref property_mask "mask". */
	quint16	mask(const QString& name ) const ;
	/*! Propriété @ref property_mask "mask". */
	bool setMask(quint16 mask, int itemId = 0 ) ;
	/*! Propriété @ref property_mask "mask". */
	bool setMask(quint16 mask, const QString& name ) ;

	/*! Propriété @ref property_display "display". */
	QString display(int itemId = 0 ) const ;
	/*! Propriété @ref property_display "display". */
	QString display(const QString& name ) const ;
	/*! Propriété @ref property_display "display". */
	bool setDisplay(const QString& display, int itemId = 0 ) ;
	/*! Propriété @ref property_display "display". */
	bool setDisplay(const QString& display, const QString& name ) ;

	/*! Propriété @ref property_value "value". */
	quint16 value(int itemId = 0 ) const ;
	/*! Propriété @ref property_value "value". */
	quint16 value(const QString& name ) const ;
	/*! Propriété @ref property_value "value". */
	bool setValue(quint16 value, int itemId = 0 ) ;
	/*! Propriété @ref property_value "value". */
	bool setValue(quint16 value, const QString& name ) ;

	QString valueAsString(int itemId = 0 ) const ;
	QString valueAsString(const QString& name ) const ;

	int itemsNumber() const ;
	int itemId(const QString& name ) const ;
	int addItem(const QString& name, quint16 mask, quint16 value = 0 ) ;
	bool areMasksConsistent() const  ;

  signals:
	/*! Signal émis par setValue() lors d'un changement effectif de valeur de la
		donnée primaire ou d'une de ses données secondaires.*/
	void valueChanged(const QString& name ) ;

  private:
	bool						m_isWord ;			// Read-Only (constructor)
	quint16						m_address ;			// Read-Write
	int							m_words ;			// complete size of value
	QList<QamModbusDataItem>	m_items ;			// first one is main data...
} ;

// --------------------------------------------------------------------------------

/*!
 @class QamModbusDataItem
 @brief Modélisation d'une donnée primaire ou d'un champ partiel de donnée 16 bits.

La classe QamModbusDataItem est à usage privé de QamModbusData. Elle encapsule les
caractéristiques d'une donnée : nom, commentaire, masque, format et valeur.
@n Une donnée primaire ou le premier mot d'une donnée composée à toujours un masque
égal à $FFFF. Un champ partiel d'une donnée primaire (dit donnée secondaire) à
un masque non nul différent de $FFFF. Un mot suivant d'une donnée composée à un
masque égal à $0000.
 */

class QamModbusDataItem
{
  public:
	QamModbusDataItem(const QString& name, quint16 mask = 0xFFFF, quint16 value = 0 ) ;

	inline QString name() const { return m_name ; }
	inline void setName(const QString& name ) { m_name = name ; }

	inline QString comment() const { return m_comment ; }
	inline void setComment(const QString& comment ) { m_comment = comment ; }

	inline quint16 mask() const { return m_mask ; }
	bool setMask(quint16 mask ) ;
	int size() const ;
	int offset() const ;

	inline quint16 value() const { return m_value ; }
	inline void setValue(quint16 value ) { m_value = value ; }

	inline QString display() const { return m_display ; }
	bool setDisplay(const QString& display ) ;

	QString	valueAsString() const ;

  private:
	QString		m_name ;		// Read-Write
	QString		m_comment ;		// Read-Write
	quint16		m_mask ;		// Read-Write
	quint16		m_value ;		// Read-Write
	QString		m_display ;		// Read-Write
} ;

#endif // QAMMODBUSDATA_H
