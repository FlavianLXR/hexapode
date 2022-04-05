/*  ---------------------------------------------------------------------------
 *  filename    :   qamdigitalmeter.cpp
 *  description :   IMPLEMENTATION de la classe QamDigitalMeter
 *
 *	project     :	Qam Custom Widgets Collection
 *  start date  :   jan 2004
 *  ---------------------------------------------------------------------------
 *  Copyright 2004-2016 by Alain Menu   <alain.menu@ac-creteil.fr>
 *
 *  This file is part of "QamWidgets Library"
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

#include "qamdigitalmeter.h"
#include <math.h>

QamDigitalMeter::QamDigitalMeter(QWidget* parent )
	: QamAbstractDigital( parent )
	, m_size( 4 )
	, m_precision( 0 )
	, m_value( 0 )
	, m_layout( NULL )
	, m_leadingZeros( true )
    , m_digits( NULL )
{
	setSize(m_size ) ;
}

QamDigitalMeter::~QamDigitalMeter()
{
	delete [] m_digits ;
	m_digits = NULL ;
}

// dessin du fond de l'indicateur (pixmap maintenu par classe de base)

void QamDigitalMeter::drawBackground(QPainter& painter ) 
{
	painter.setRenderHint(QPainter::Antialiasing ) ;
//	painter.fillRect(rect(), QBrush( palette().background() ) ) ;
    painter.fillRect(rect(), QBrush( palette().color(QPalette::Base) ) ) ;
	// nothing to do...
}

void QamDigitalMeter::drawForeground(QPainter& painter )
{
	painter.setRenderHint(QPainter::Antialiasing ) ;
	// nothing to do...
}

// nombre d'afficheurs 7 segments (de 2 à 9)

int QamDigitalMeter::size() const { return m_size ; }

void QamDigitalMeter::setSize(int numDigits )
{
	if ( numDigits < 2 )	numDigits = 2 ;
	if ( numDigits > 9 )	numDigits = 9 ;

	m_size = numDigits ;
	if ( precision() >= size() )	setPrecision( size() - 1 ) ;
	
	if ( m_digits != NULL )	delete [] m_digits ;
	m_digits = new QamSegmentDisplay[ size() ] ;
	
	if ( m_layout != NULL ) {
		QLayout*	hbl = layout() ;
		delete hbl ;
	}
	m_layout = new QHBoxLayout ;
	m_layout->setSpacing(0 ) ;
	m_layout->setContentsMargins(0, 0, 0, 0 ) ;
	
	int w = width() / size() ;
	
	for (int i = 0 ; i < size() ; ++i ) {
		m_digits[i].resize(w, height() ) ;
		m_digits[i].move( i * w, 0 ) ;
		m_layout->addWidget( m_digits + i ) ;
	}
	this->setLayout( m_layout ) ;
	
	for (int i = 0 ; i < size() ; ++i ) {
		m_digits[i].setState( m_sd.state() ) ;
		m_digits[i].setBlink( m_sd.blink() ) ;
		m_digits[i].setShining( m_sd.isShining() ) ;
		m_digits[i].setCustomColor( m_sd.customColor() ) ;
		m_digits[i].setAlphaOff( m_sd.alphaOff() ) ;
		m_digits[i].setBold( m_sd.isBold() ) ;
	}
	setValue( value() ) ;
}

// valeur affichée

double QamDigitalMeter::value() const { return m_value ; }

void QamDigitalMeter::setValue(int value )						// slot
{
	setValue( (double)value ) ;
}

void QamDigitalMeter::setValue(double value )					// slot
{
	bool minus = false ;						// valeur négative
	bool over = false ;							// dépassement de capacité
	int  numd = size() ;						// nombre de chiffres dispo.

	m_value = value ;							// mémo. sans perte
	
	if ( precision() ) value *= pow(10, precision() ) ;

	if ( value < 0 ) {							// si valeur négative...
		minus = true ;
		value *= -1 ;							// valeur absolue
		numd-- ;								// digit de gauche = signe -
		m_digits[0].setDecimalPoint( false ) ;		
		m_digits[0].setMinus() ;
	}

	if ( value >= pow(10, numd ) )	over = true ;
	
	for (int i = ( minus ? 1 : 0 ) ; i < size() ; ++i ) {

		m_digits[i].setDecimalPoint( false ) ;		
		if ( over ) {
			m_digits[i].setMinus() ;
		}
		else {
			m_digits[i].setMinus( false ) ;
			int v = (int)( value / pow(10, size() - i - 1 ) ) % 10 ;
			m_digits[i].setValue( v ) ;
		}
		value = (long)value % (long)pow(10, size() - i - 1 ) ;
	}
	if ( !over ) {
		if ( precision() ) 
			m_digits[ size() - precision() - 1 ].setDecimalPoint( true ) ;
	}

	if ( !m_leadingZeros ) {					// new v5.1

		for ( int i = 0 ; i < size() ; ++i )	m_digits[i].setCustomDisplay( false ) ;

		int lz = size() - precision() - 2 ;
		for ( int i = ( minus ? 1 : 0 ) ; i <= lz ; ++i ) {
			if ( m_digits[i].value() != 0 )	break ;
			m_digits[i].setCustomDisplay( true ) ;
		}
	}
	
	update() ;
}

// nombre de chiffres à droite du point décimal (intervalle 0 .. size()-1)

int QamDigitalMeter::precision() const { return m_precision ; }

void QamDigitalMeter::setPrecision(int precision )
{
	if ( precision < 0 ) precision = 0 ;
	if ( precision >= size() )	precision = size() - 1 ;
	
	m_precision = precision ;
	setValue( value() ) ;		// mise à jour de l'affichage...
}

// effet de graissage

bool QamDigitalMeter::isBold() const
{
	return m_sd.isBold() ;
}

void QamDigitalMeter::setBold(bool bold )
{
	m_sd.setBold( bold ) ;
	for (int i = 0 ; i < m_size ; ++i )	m_digits[i].setBold( bold ) ;
}

// affichage personnalisé

void QamDigitalMeter::setCustomDisplay(int digit, int value )
{
    if (( digit < 0 )||( digit >= size() ))	return ;
    m_digits[ digit ].setValue( value ) ;
}

void QamDigitalMeter::setCustomDisplay(int digit, const QString& segments )
{
	if ( digit == -1 ) {
		for (int i = 0 ; i < size() ; ++i ) {
			m_digits[i].setCustomDisplay( false ) ;
		}
	}
	if (( digit < 0 )||( digit >= size() ))	return ;
	
	m_digits[ digit ].setCustomDisplay(true, segments ) ;
}

// surcharges des méthodes liées aux propriétés de QamAbstractLed
// pour diffusion à tous les composants QamSegmentDisplay

QamWidget::State QamDigitalMeter::state() const
{
	return m_sd.state() ;
}

void QamDigitalMeter::setState(QamWidget::State state )
{
	m_sd.setState( state ) ;
	for (int i = 0 ; i < m_size ; ++i )	m_digits[i].setState( state ) ;
}

bool QamDigitalMeter::isOn() const
{
	return m_sd.isOn() ;
}

void QamDigitalMeter::off()										// slot
{
	m_sd.off() ;
	for (int i = 0 ; i < m_size ; ++i ) m_digits[i].off() ;
}

void QamDigitalMeter::on()										// slot
{
	m_sd.on() ;
	for (int i = 0 ; i < m_size ; ++i ) m_digits[i].on() ;
}

void QamDigitalMeter::toggle()									// slot
{
	m_sd.toggle() ;
	for (int i = 0 ; i < m_size ; ++i )	m_digits[i].toggle() ;
}

QamWidget::Blink QamDigitalMeter::blink() const
{
	return m_sd.blink() ;
}

void QamDigitalMeter::setBlink(QamWidget::Blink blink )
{
	m_sd.setBlink( blink ) ;
	for (int i = 0 ; i < m_size ; ++i )	m_digits[i].setBlink( blink ) ;
}

void QamDigitalMeter::setFlash(QamWidget::State state, int ms )
{
	m_sd.setFlash(state, ms ) ;
	for (int i = 0 ; i < m_size ; ++i )	m_digits[i].setFlash(state, ms ) ;
}

bool QamDigitalMeter::isShining() const
{
	return m_sd.isShining() ;
}

void QamDigitalMeter::setShining(bool shining )
{
	m_sd.setShining( shining ) ;
	for (int i = 0 ; i < m_size ; ++i ) m_digits[i].setShining( shining ) ;
}

QColor QamDigitalMeter::customColor() const
{
	return m_sd.customColor() ;
}

void QamDigitalMeter::setCustomColor(const QColor& color )
{
	m_sd.setCustomColor( color ) ;
	for (int i = 0 ; i < m_size ; ++i )	m_digits[i].setCustomColor( color ) ;
}

int	QamDigitalMeter::alphaOff() const { return m_sd.alphaOff() ; }

void QamDigitalMeter::setAlphaOff(int alphaOff )
{
	m_sd.setAlphaOff( alphaOff ) ;
	for (int i = 0 ; i < m_size ; ++i )	m_digits[i].setAlphaOff( alphaOff ) ;
}

bool QamDigitalMeter::leadingZeros() const { return m_leadingZeros ; }

void QamDigitalMeter::setLeadingZeros(bool leadingZeros )
{
	m_leadingZeros = leadingZeros ;
	setValue( value() ) ;
}
