/*  ---------------------------------------------------------------------------
 *  filename    :   hexapodconfigurator.cpp
 *  description :   IMPLEMENTATION de la classe HexapodConfigurator
 *
 *	project     :	Scène 3D LLF Maquette éch. 1:1 FlightSim
 *  start date  :   novembre 2017
 *  ---------------------------------------------------------------------------
 *  Copyright 2017 by Alain Menu   <alain.menu@ac-creteil.fr>
 *
 *  This file is part of "FlightSim Mock-up"
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

#include "hexapod.h"
#include "hexapodconfigurator.h"

HexapodConfigurator::HexapodConfigurator(QWidget* parent )
    : QGroupBox(tr("Hexapod Template"), parent)
	, m_hexapod( 0 )
	, m_sendToHexapod( false )
	, m_monoColumn( false )
{
	m_vbox = new QVBoxLayout ;
    m_vbox->setSpacing(8) ;

	m_spinLowRadius  = createSpinBox(0, 1000, "spinLowRadius" ) ;
	m_spinLowGap     = createSpinBox(0, 1000, "spinLowGap" ) ;
	m_spinHighRadius = createSpinBox(0, 1000, "spinHighRadius" ) ;
	m_spinHighGap    = createSpinBox(0, 1000, "spinHighGap" ) ;
	m_spinMinLen     = createSpinBox(0, 1000, "spinMinLen" ) ;
	m_spinMaxLen     = createSpinBox(0, 1000, "spinMaxLen" ) ;
	m_edtStroke      = new QLineEdit ;
	m_edtRefAltitude = new QLineEdit ;
	m_spinAngle      = createSpinBox(0,   45, "spinAngle" ) ;
    m_spinTrans      = createSpinBox(0,  200, "spinTrans" ) ;
    m_spinSpeed      = createSpinBox(0, 1000, "spinSpeed" ) ;

    addSpinBox(m_vbox, m_spinLowRadius, "low radius (mm):" ) ;
    addSpinBox(m_vbox, m_spinLowGap, "low gap (mm):" ) ;
    addSpinBox(m_vbox, m_spinHighRadius, "high radius (mm):" ) ;
    addSpinBox(m_vbox, m_spinHighGap, "high gap (mm):" ) ;
	addLine( m_vbox ) ;
    addSpinBox(m_vbox, m_spinMinLen, "min len (mm):" ) ;
    addSpinBox(m_vbox, m_spinMaxLen, "max len (mm):" ) ;
	addLine( m_vbox ) ;
    addEditBox(m_vbox, m_edtStroke, "stroke (mm):" ) ;
    addEditBox(m_vbox, m_edtRefAltitude, "ref. altitude (mm):" ) ;
	addLine( m_vbox ) ;
    addSpinBox(m_vbox, m_spinAngle, "max Rx/Ry/Rz (deg.):" ) ;
    addSpinBox(m_vbox, m_spinTrans, "max Tx/Ty (mm):" ) ;
    addSpinBox(m_vbox, m_spinSpeed, "max speed (mm/s):" ) ;

    // new v0.7
    QHBoxLayout*	hbox = new QHBoxLayout ;
    hbox->addWidget( new QLabel( "CSV file:" ) ) ;
    m_btnLoad = new QPushButton("Load") ;
    m_btnLoad->setObjectName("btnLoad") ;
    m_btnSave = new QPushButton("Save") ;
    m_btnSave->setObjectName("btnSave") ;
    hbox->addWidget(m_btnLoad ) ;
    hbox->addWidget(m_btnSave ) ;
    addLine( m_vbox ) ;
    m_vbox->addLayout(hbox) ;
    //

	this->setLayout(m_vbox) ;

	QMetaObject::connectSlotsByName ( this ) ;
}

QSpinBox* HexapodConfigurator::createSpinBox(int min, int max, QString name )
{
	QSpinBox* spin = new QSpinBox ;
	spin->setMinimum(min) ;
	spin->setMaximum(max) ;
	spin->setObjectName( name ) ;
	return spin ;
}

void HexapodConfigurator::addLine(QVBoxLayout* vbox )
{
	QFrame* line = new QFrame ;
	line->setFrameShape( QFrame::HLine ) ;
	vbox->addWidget( line ) ;
}

void HexapodConfigurator::addSpinBox(QVBoxLayout* vbox, QSpinBox* spin, QString label )
{
	spin->setFixedWidth( 60 ) ;
	spin->setAlignment( Qt::AlignRight ) ;
    // non éditable : utiliser le clavier pour +-1 | +-10
    auto lineEdit = spin->findChild<QLineEdit*>() ;
    lineEdit->setReadOnly(true) ;
    //
	if ( m_monoColumn ) {
		vbox->addWidget( new QLabel( label ) ) ;
		vbox->addWidget( spin ) ;
	}
	else {
		QHBoxLayout*	hbox = new QHBoxLayout ;
		hbox->addWidget( new QLabel( label ) ) ;
		hbox->addWidget( spin ) ;
		vbox->addLayout(hbox) ;
	}
}

void HexapodConfigurator::addEditBox(QVBoxLayout* vbox, QLineEdit* edt, QString label, bool ro )
{
	edt->setFixedWidth(50) ;
	edt->setAlignment( Qt::AlignRight ) ;
	edt->setReadOnly( ro ) ;
	if ( m_monoColumn ) {
		vbox->addWidget( new QLabel( label ) ) ;
		vbox->addWidget( edt ) ;
	}
	else {
		QHBoxLayout*	hbox = new QHBoxLayout ;
		hbox->addWidget( new QLabel( label ) ) ;
		hbox->addWidget( edt ) ;
		vbox->addLayout(hbox) ;
	}
}

void HexapodConfigurator::bindTo(Hexapod* hexapod )
{
	m_hexapod = hexapod ;
	set( m_hexapod->data() ) ;
}

void HexapodConfigurator::set(const HexapodData& data )
{
	m_sendToHexapod = false ;
	m_spinLowRadius->setValue( data.baseRadius ) ;
	m_spinLowGap->setValue( data.baseGap ) ;
	m_spinHighRadius->setValue( data.topRadius ) ;
	m_spinHighGap->setValue( data.topGap ) ;
	m_spinMinLen->setValue( data.minLen ) ;
	m_spinMaxLen->setValue( data.maxLen ) ;
	m_edtStroke->setText( QString("%1").arg(data.maxLen - data.minLen) ) ;
	updateAltitude() ;
	m_spinAngle->setValue( data.maxAngle ) ;
    m_spinTrans->setValue( data.maxTrans ) ;
    m_spinSpeed->setValue( data.maxSpeed ) ;
	m_sendToHexapod = true ;
}

void HexapodConfigurator::updateAltitude()
{
	int v = m_hexapod->minAltitude() ;
	m_edtRefAltitude->setText( QString("%1").arg( v ) ) ;
}

void HexapodConfigurator::on_spinLowRadius_valueChanged(int i )
{
	if (( !m_hexapod )||( !m_sendToHexapod ))	return ;

	HexapodData d = m_hexapod->data() ;
	d.baseRadius = i ;
	m_hexapod->setData( d ) ;
	updateAltitude() ;
}

void HexapodConfigurator::on_spinLowGap_valueChanged(int i )
{
	if (( !m_hexapod )||( !m_sendToHexapod ))	return ;

	HexapodData d = m_hexapod->data() ;
	d.baseGap = i ;
	m_hexapod->setData( d ) ;
	updateAltitude() ;
}

void HexapodConfigurator::on_spinHighRadius_valueChanged(int i )
{
	if (( !m_hexapod )||( !m_sendToHexapod ))	return ;

	HexapodData d = m_hexapod->data() ;
	d.topRadius = i ;
	m_hexapod->setData( d ) ;
	updateAltitude() ;
}

void HexapodConfigurator::on_spinHighGap_valueChanged(int i )
{
	if (( !m_hexapod )||( !m_sendToHexapod ))	return ;

	HexapodData d = m_hexapod->data() ;
	d.topGap = i ;
	m_hexapod->setData( d ) ;
	updateAltitude() ;
}

void HexapodConfigurator::on_spinMinLen_valueChanged(int i )
{
	if (( !m_hexapod )||( !m_sendToHexapod ))	return ;

	HexapodData d = m_hexapod->data() ;
	d.minLen = i ;
	m_edtStroke->setText( QString("%1").arg(d.maxLen - d.minLen) ) ;
	m_hexapod->setData( d ) ;
	updateAltitude() ;
}

void HexapodConfigurator::on_spinMaxLen_valueChanged(int i )
{
	if (( !m_hexapod )||( !m_sendToHexapod ))	return ;

	HexapodData d = m_hexapod->data() ;
	d.maxLen = i ;
	m_edtStroke->setText( QString("%1").arg(d.maxLen - d.minLen) ) ;
	m_hexapod->setData( d ) ;
}

void HexapodConfigurator::on_spinAngle_valueChanged(int i )
{
	if (( !m_hexapod )||( !m_sendToHexapod ))	return ;

	HexapodData d = m_hexapod->data() ;
	d.maxAngle = i ;
	m_hexapod->setData( d ) ;
}

void HexapodConfigurator::on_spinTrans_valueChanged(int i )
{
    if (( !m_hexapod )||( !m_sendToHexapod ))	return ;

    HexapodData d = m_hexapod->data() ;
    d.maxTrans = i ;
    m_hexapod->setData( d ) ;
}

void HexapodConfigurator::on_spinSpeed_valueChanged(int i )
{
	if (( !m_hexapod )||( !m_sendToHexapod ))	return ;

	HexapodData d = m_hexapod->data() ;
	d.maxSpeed = i ;
	m_hexapod->setData( d ) ;
}
