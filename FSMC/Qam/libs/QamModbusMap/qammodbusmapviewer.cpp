/*  ---------------------------------------------------------------------------
 *  filename    :   qammodbusmapviewer.cpp
 *  description :   IMPLEMENTATION de la classe QamModbusMapViewer
 *					Visualiseur arborescent d'un objet QamModbusMap
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

#include "qammodbusmapviewer.h"

/*! Constructeur. */

QamModbusMapViewer::QamModbusMapViewer(QWidget* parent)
	: QTreeView(parent)
	, m_modbusMap( 0 )
{
#ifdef Q_OS_MAC
	this->setFont( QFont("Monaco", 12 ) ) ;
#else
	this->setFont( QFont("Courier", 12 ) ) ;
#endif

	m_model = new QStandardItemModel(this) ;
	this->setModel( m_model ) ;

	// titres des colonnes [ORDRE IMPERATIF]

	//           0          1            2         3         4            5        6            7
	m_labels <<	"Table" << "Address" << "Mask" << "Name" << "Comment" << "Hex" << "Display" << "Value" ;
	m_iName    = 3 ;	// num. de colonne champ 'Name'
	m_iHex     = 5 ;	// num. de colonne champ 'Hex'
	m_iDisplay = 6 ;	// num. colonne champ 'Display'
	m_iValue   = 7 ;	// num. de colonne champ 'Value'

	m_model->setHorizontalHeaderLabels( m_labels ) ;

	// entr??es tables primaires (colonne 0), non ??ditables [ORDRE IMPERATIF]
	QColor color1(0x00,0x66,0xCC) ;		// llf_bleu
	QColor color2(0x66,0x00,0x66) ;		// llf_pourpre
	m_tables << newItem("Coils", false, color1 ) ;				// row 0
	m_tables << newItem("Discrete Inputs", false, color2 ) ;	// row 1
	m_tables << newItem("Input Registers", false, color1 ) ;	// row 2
	m_tables << newItem("Holding Registers", false, color2 ) ;	// row 3

	for ( int i = 0 ; i < m_tables.size() ; ++i ) {
		m_tables.at(i)->setSelectable( false ) ;
	}

	m_model->invisibleRootItem()->appendRows( m_tables ) ;

	for ( int i = 0 ; i < m_model->rowCount() ; ++i ) {
		for ( int j = 1 ; j < m_model->columnCount() ; ++j ) {
			QStandardItem* item = newItem("", false, (i % 2 ? color2 : color1 ) ) ;
			item->setSelectable( false ) ;
			m_model->setItem(i,j, item ) ;
		}
	}

	m_internalUpdate = false ;
	connect( m_model, SIGNAL( itemChanged(QStandardItem*)),
			 this,    SLOT(   itemChanged(QStandardItem*)) ) ;
}

// interception d'une modification de valeur par ??dition directe [private slot]

void QamModbusMapViewer::itemChanged(QStandardItem* item )
{
	if ( m_internalUpdate )	return ; // si appel provoqu?? par updateModbusData(), abandon

	for ( int i = 0 ; i < m_model->rowCount() ; ++i ) {			// entr??e table

		QStandardItem* item1 = m_model->item(i) ;

		for ( int j = 0 ; j < item1->rowCount() ; ++j ) {		// entr??e donn??e primaire

			QStandardItem* item2 = item1->child(j) ;
			if ( item2 == 0 )	continue ;
			if ( item1->child(j, m_iValue ) == item ) {
				m_modbusMap->setValue(rowToTable(i), item1->child(j, m_iName )->text(), item1->child(j, m_iValue )->text() ) ;
			}

			for ( int k = 0 ; k < item2->rowCount() ; ++k ) {	// entr??e donn??e secondaire
				if ( item2->child(k) == 0 )	continue ;
				if ( item2->child(k, m_iValue ) == item ) {
					m_modbusMap->setValue(rowToTable(i), item2->child(k, m_iName )->text(), item2->child(k, m_iValue )->text() ) ;
				}
			}
		}
	}
}

// demande d'actualisation d'une donn??e primaire [private slot]
// (et de ses ??ventuelles donn??es secondaires)

void QamModbusMapViewer::updateModbusData(int table, const QString& name )
{
	QamModbusMap::PrimaryTable	tbl = (QamModbusMap::PrimaryTable)table ;

	if ( !m_modbusMap->exists(tbl, name) )	return ;

	int row = tableToRow( tbl ) ;							// indice table
	if ( row >= m_model->rowCount() )	return ;

	m_internalUpdate = true ;								// 'inhibe' slot itemChanged()

	QStringList	list = m_modbusMap->itemList(tbl, name ) ;	// donn??es secondaires ?

	QStandardItem* item1 = m_model->item( row ) ;			// item table

	QColor	color(255, 102, 0, 32 ) ;						// colorisation en mode client

	for ( int i = 0 ; i < item1->rowCount() ; ++ i ) {

		QStandardItem* item2 = item1->child(i) ;
		if ( item2 == 0 )	continue ;

		if ( item1->child(i, m_iName )->text() == name ) {
			// m??j donn??e primaire
			quint16 value   = m_modbusMap->data( tbl, name ).value() ;
			item1->child(i, m_iHex )->setText( QString("%1").arg(value, 4, 16, QLatin1Char('0') ).toUpper() ) ;
			if ( !item1->child(i, m_iDisplay )->text().isEmpty() ) {

				item1->child(i, m_iValue )->setText( m_modbusMap->localValue( tbl, name ) ) ;

				if ( m_modbusMap->mode() == QamModbusMap::ClientMode )	item1->child(i, m_iValue )->setBackground( QBrush( color ) ) ;
			}
			// m??j donn??es secondaires le cas ??ch??ant
			if ( !list.isEmpty() )	for ( int j = 0 ; j < item2->rowCount() ; ++j ) {
				if ( item2->child(j) == 0 )	continue ;
				QString itemName = item2->child(j, m_iName )->text() ;
				if ( !list.contains( itemName ) )	continue ;
				quint16 value   = m_modbusMap->data( tbl, name ).value( itemName ) ;
				item2->child(j, m_iHex )->setText( QString("%1").arg(value, 4, 16, QLatin1Char('0') ).toUpper() ) ;
				item2->child(j, m_iValue )->setText( m_modbusMap->data( tbl, name ).valueAsString( itemName ) ) ;

				if ( m_modbusMap->mode() == QamModbusMap::ClientMode )	item2->child(j, m_iValue )->setBackground( QBrush( color ) ) ;
			}
		}

		// new 1.4 : les suites de donn??es compos??es sont maintenant enfants de leur donn??e primaire...
		for ( int j = 0 ; j < item2->rowCount() ; ++j ) {
			if ( item2->child(j) == 0 )	continue ;
			if ( item2->child(j, m_iName )->text() == name ) {
				quint16 value   = m_modbusMap->data( tbl, name ).value() ;
				item2->child(j, m_iHex )->setText( QString("%1").arg(value, 4, 16, QLatin1Char('0') ).toUpper() ) ;
				break ;
			}
		}
		//
	}

	m_internalUpdate = false ;
}

/*!
 * Acc??s ?? la table primaire de l'entr??e en cours de s??lection (surbrillance).
 * \return Num??ro de table compatible avec QamModbusMap::PrimaryTable, ou -1 si aucune s??lection valide.
 */

int QamModbusMapViewer::currentTable()
{
	QModelIndexList index = this->selectedIndexes() ;
	if ( !index.isEmpty() ) {
		// parent du premier item s??lectionn??, cas d'une donn??e primaire
		QStandardItem* item = m_model->itemFromIndex( index.at(0) )->parent() ;
		if ( !item )	return -1 ;
		for ( int i = 0 ; i < m_tables.size() ; ++i ) {
			if ( item->text() == m_tables.at(i)->text() )	return (int)rowToTable(i) ;
		}
		// sinon, donn??e secondaire ?
		item = item->parent() ;
		if ( !item )	return -1 ;
		for ( int i = 0 ; i < m_tables.size() ; ++i ) {
			if ( item->text() == m_tables.at(i)->text() )	return (int)rowToTable(i) ;
		}
	}
	return -1 ;
}

/*!
 * Acc??s au champ nom de l'entr??e en cours de s??lection (surbrillance).
 * \return Nom de l'entr??e ou chaine vide si aucune s??lection en cours.
 */

QString QamModbusMapViewer::currentName()
{
	QModelIndexList index = this->selectedIndexes() ;
	if ( !index.isEmpty() ) {
		QStandardItem* item = m_model->itemFromIndex( index.at(3) ) ;
		if ( item ) return item->text() ;
	}
	return QString() ;
}

// conversions row (int) <--> QamModbusMap::PrimaryTable [private]

QamModbusMap::PrimaryTable QamModbusMapViewer::rowToTable( int row )
{
	if ( row == 0 )	return QamModbusMap::Coil ;
	if ( row == 1 )	return QamModbusMap::DiscretInput ;
	if ( row == 2 )	return QamModbusMap::InputRegister ;
	return QamModbusMap::HoldingRegister ;
}

int QamModbusMapViewer::tableToRow(QamModbusMap::PrimaryTable table )
{
	if ( table == QamModbusMap::Coil )			return 0 ;
	if ( table == QamModbusMap::DiscretInput )	return 1 ;
	if ( table == QamModbusMap::InputRegister )	return 2 ;
	return 3 ;
}

/*!
 * Cr??ation/initialisation de l'arbre de donn??es Modbus.
 * \param map : objet QamModbusMap ?? utiliser.
 */

void QamModbusMapViewer::setModbusMap(QamModbusMap* map )
{
	if ( m_modbusMap )	return ;		// one shot !

	m_modbusMap = map ;

	// remplissage des tables primaires
	setModbusTable(QamModbusMap::Coil,				00001, 0, false ) ;
	setModbusTable(QamModbusMap::DiscretInput,		10001, 1, false ) ;
	setModbusTable(QamModbusMap::InputRegister,		30001, 2, true ) ;
	setModbusTable(QamModbusMap::HoldingRegister,	40001, 3, true ) ;

	// tout d??velopper...
	this->expandAll() ;
	// ajustement horizontal des colonnes
	for ( int i = 0 ; i < m_labels.size() ; ++i )	this->resizeColumnToContents( i ) ;
	// tout replier...
	this->collapseAll() ;
	// ... et montrer seulement la liste des 'numbers' utilis??s
	for ( int i = 0 ; i < m_model->rowCount() ; ++i )	this->expand(m_model->index(i, 0 ) ) ;

	// interception des changements de valeurs
	connect( m_modbusMap, SIGNAL(valueChanged(int,QString)),
			 this,		  SLOT(updateModbusData(int,QString)) ) ;
}

// cr??ation/initialisation cartographie d'une table primaire [private]

void QamModbusMapViewer::setModbusTable(QamModbusMap::PrimaryTable table, quint16 baseNumber, int treeRow, bool isWords )
{
	Q_UNUSED( isWords ) ;

	QStringList	list = m_modbusMap->nameList( table, true ) ;

	if ( list.size() == 0 ) {													// new 1.4
		m_tables[ treeRow ]->setForeground( QBrush( QColor(127,127,127) ) ) ;
		return ;
	}

	QList<QStandardItem *> numbers ;	// colonne 0
	QList<QStandardItem *> addresses ;	// colonne 1
	QList<QStandardItem *> masks ;		// colonne 2
	QList<QStandardItem *> names ;		// colonne 3
	QList<QStandardItem *> comments ;	// colonne 4
	QList<QStandardItem *> hexs ;		// colonne 5
	QList<QStandardItem *> displays ;	// colonne 6
	QList<QStandardItem *> values ;		// colonne 7

	QList<QStandardItem *> row ;

	bool valueEditable = true ;
	// cas des tables read-only en mode client
	if ( m_modbusMap->mode() == QamModbusMap::ClientMode ) {
		if ( table == QamModbusMap::DiscretInput )	valueEditable = false ;
		if ( table == QamModbusMap::InputRegister )	valueEditable = false ;
	}

	for ( int i = 0 ; i < list.size() ; ++i ) {
		quint16 address = m_modbusMap->data( table, list.at(i) ).address() ;
		quint16 value   = m_modbusMap->data( table, list.at(i) ).value() ;
		quint16 mask    = m_modbusMap->data( table, list.at(i) ).mask() ;

		QColor color = ( mask != 0 ? Qt::white : Qt::black ) ;
/*
		numbers << newItem( QString("%1").arg(address + baseNumber ).rightJustified(6, QLatin1Char('0') ), false, color ) ;
		addresses << newItem( QString("%1").arg(address, 4, 16, QLatin1Char('0') ).toUpper(), false, color ) ;
		names << newItem( list.at(i), false, color ) ;
		comments << newItem( m_modbusMap->data( table, list.at(i) ).comment() ) ;
		hexs << newItem( QString("%1").arg(value, 4, 16, QLatin1Char('0') ).toUpper() ) ;

		if ( mask != 0x0000 ) {				// premier/unique mot donn??e primaire ou donn??e secondaire
			masks << newItem("----") ;
			displays << newItem( m_modbusMap->data( table, list.at(i) ).display() ) ;

			values << newItem( m_modbusMap->localValue( table, list.at(i) ), valueEditable ) ;
		}
		else {								// suite de donn??e compos??e
			masks << newItem(" .. ", false, color ) ;
			displays << newItem("") ;
			values << newItem("") ;

			numbers.last()->setSelectable( false ) ;	// non s??lectionnable
			addresses.last()->setSelectable( false ) ;
			names.last()->setSelectable( false ) ;
			comments.last()->setSelectable( false ) ;
			hexs.last()->setSelectable( false ) ;
			masks.last()->setSelectable( false ) ;
			displays.last()->setSelectable( false ) ;
			values.last()->setSelectable( false ) ;
		}
*/
// new 1.4 : donn??es composites d??ployables
		if ( mask != 0x0000 ) {				// premier/unique mot donn??e primaire ou donn??e secondaire

			numbers << newItem( QString("%1").arg(address + baseNumber ).rightJustified(6, QLatin1Char('0') ), false, color ) ;
			addresses << newItem( QString("%1").arg(address, 4, 16, QLatin1Char('0') ).toUpper(), false, color ) ;
			names << newItem( list.at(i), false, color ) ;
			comments << newItem( m_modbusMap->data( table, list.at(i) ).comment() ) ;
			hexs << newItem( QString("%1").arg(value, 4, 16, QLatin1Char('0') ).toUpper() ) ;
			masks << newItem("----") ;
			displays << newItem( m_modbusMap->data( table, list.at(i) ).display() ) ;
			values << newItem( m_modbusMap->localValue( table, list.at(i) ), valueEditable ) ;
		}
		else {								// suite de donn??e compos??e
			row.clear() ;
			row << newItem( QString("%1").arg(address + baseNumber ).rightJustified(6, QLatin1Char('0') ), false, color ) ;
			row << newItem( QString("%1").arg(address, 4, 16, QLatin1Char('0') ).toUpper(), false, color ) ;
			row << newItem("", false ) ; //newItem(" .. ", false, color ) ;
			row << newItem( list.at(i), false ) ;
			row.last()->setForeground( QBrush( Qt::white ) ) ;	// masquage du nom
			row << newItem("", false ) ;
			row << newItem( QString("%1").arg(value, 4, 16, QLatin1Char('0') ).toUpper() ) ;
			row << newItem("", false ) ;
			row << newItem("", false ) ;
			numbers[ numbers.size() - 1 ]->appendRow( row ) ;
		}
//
		QStringList list2 = m_modbusMap->itemList( table, list.at(i) ) ;
		if ( !list2.isEmpty() ) {
			for ( int j = 0 ; j < list2.size() ; ++j ) {

				quint16 mask = m_modbusMap->data( table, list.at(i) ).mask( list2.at(j) ) ;
				quint16 val  = m_modbusMap->data( table, list.at(i) ).value( list2.at(j) ) ;

				QColor color(0xCC,0xCC,0x00) ;	// llf_vert

				row.clear() ;
				row << newItem("") ;
				row << newItem("") ;
				row << newItem( QString("%1").arg(mask, 4, 16, QLatin1Char('0') ).toUpper(), false, color ) ;
				row << newItem( list2.at(j), false, color ) ;
				row << newItem( m_modbusMap->data( table, list.at(i) ).comment( list2.at(j) ), false, color ) ;
				row << newItem( QString("%1").arg(val, 4, 16, QLatin1Char('0') ).toUpper(), false, color ) ;
				row << newItem( m_modbusMap->data( table, list.at(i) ).display( list2.at(j) ), false, color ) ;
				row << newItem( m_modbusMap->data( table, list.at(i) ).valueAsString( list2.at(j) ), valueEditable, color ) ;
				numbers[ numbers.size() - 1 ]->appendRow( row ) ;
			}
		}
	}

	m_tables[ treeRow ]->appendColumn( numbers ) ;
	m_tables[ treeRow ]->appendColumn( addresses ) ;
	m_tables[ treeRow ]->appendColumn( masks ) ;
	m_tables[ treeRow ]->appendColumn( names ) ;
	m_tables[ treeRow ]->appendColumn( comments ) ;
	m_tables[ treeRow ]->appendColumn( hexs ) ;
	m_tables[ treeRow ]->appendColumn( displays ) ;
	m_tables[ treeRow ]->appendColumn( values ) ;
}

// cr??ation d'un nouvel ??l??ment avec choix ??ditable O/N [private]

QStandardItem* QamModbusMapViewer::newItem(const QString& item, bool editable, QColor bkColor )
{
	QStandardItem* ni = new QStandardItem( item ) ;
	ni->setEditable( editable ) ;
	bkColor.setAlpha( 48 ) ;					// 32 in v1.0
	ni->setBackground( QBrush( bkColor ) ) ;
	return ni ;
}
