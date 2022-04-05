/*  ---------------------------------------------------------------------------
 *  filename    :   qambargraph.cpp
 *  description :   IMPLEMENTATION de la classe QamBargraph
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

#include "qambargraph.h"
#include <math.h>

QamBargraph::QamBargraph(QWidget* parent )
	: QamAbstractDigital( parent )
	, m_orientation( QamWidget::Top )
	, m_size( 8 )
	, m_value( 4 )
	, m_layout( NULL )
	, m_leds( NULL )
{
	resize(40, 120 ) ;
	setSize( m_size ) ;
}

QamBargraph::~QamBargraph()
{
	delete [] m_leds ;
	m_leds = NULL ;
}

// dessin du fond de l'indicateur (pixmap maintenu par classe de base)

void QamBargraph::drawBackground(QPainter& painter ) 
{
	painter.setRenderHint(QPainter::Antialiasing ) ;
	painter.fillRect(rect(), QBrush( palette().background() ) ) ;
	// nothing to do...
}

void QamBargraph::drawForeground(QPainter& painter )
{
	painter.setRenderHint(QPainter::Antialiasing ) ;
	// nothing to do...
//	m_leds[0].draw( &painter ) ;
}

int QamBargraph::size() const { return m_size ; }

void QamBargraph::setSize(int numLeds )
{
	if ( numLeds < 4 )		numLeds = 4 ;
	if ( numLeds > 100 )	numLeds = 100 ;
	
	m_size = numLeds ;
	
	if ( m_leds != NULL )	delete [] m_leds ;
	m_leds = new QamLed[ size() ] ;
	
	if ( m_layout != NULL ) {
		QLayout*	hbl = layout() ;
		delete hbl ;
	}
	
	QBoxLayout::Direction dir ;
	switch ( orientation() ) {
		case QamWidget::Top :	 dir = QBoxLayout::BottomToTop ; break ;
		case QamWidget::Bottom : dir = QBoxLayout::TopToBottom ; break ;
		case QamWidget::Right :	 dir = QBoxLayout::LeftToRight ; break ;
		default : 	 			 dir = QBoxLayout::RightToLeft ; break ;
	}
	
	m_layout = new QBoxLayout( dir ) ;
	m_layout->setSpacing(1) ;
    m_layout->setContentsMargins(1, 1, 1, 1 ) ;

	int w = width() / size() ;
	
	for (int i = 0 ; i < size() ; ++i ) {
		m_leds[i].resize(w, height() ) ;
		m_leds[i].move( i * w, 0 ) ;
		m_layout->addWidget( m_leds + i ) ;
	}
	this->setLayout( m_layout ) ;

	for (int i = 0 ; i < size() ; ++i ) {
		m_leds[i].setShape( QamLed::Plain ) ;
		m_leds[i].setState( m_led.state() ) ;
		m_leds[i].setBlink( m_led.blink() ) ;
		m_leds[i].setShining( m_led.isShining() ) ;
		m_leds[i].setCustomColor( m_led.customColor() ) ;
		m_leds[i].setAlphaOff( m_led.alphaOff() ) ;
	}
	setValue( value() ) ;
}

int	QamBargraph::value() const { return m_value ; }

void QamBargraph::setValue(int value )								// slot
{
	if ( value < 0 )		value = 0 ;
	if ( value > size() )	value = size() ;
	m_value = value ;
	
	for (int i = 0 ; i < size() ; ++i ) {
		if (( m_led.isOn() )&&( m_value > i )) {
			m_leds[i].on() ;
		}
		else {
			m_leds[i].off() ;
		}
	}
}

QamWidget::Orientation QamBargraph::orientation() const { return m_orientation ; }

void QamBargraph::setOrientation(QamWidget::Orientation orientation ) 
{
	m_orientation = orientation ; 
	setSize( size() ) ; 
}

// surcharges des méthodes liées aux propriétés de QamAbstractDigital
// pour diffusion à tous les composants QamLed

QamWidget::State QamBargraph::state() const { return m_led.state() ; }

void QamBargraph::setState(QamWidget::State state )
{
	m_led.setState( state ) ;
	for (int i = 0 ; i < m_size ; ++i )	m_leds[i].setState( state ) ;
	setValue( value() ) ;
}

bool QamBargraph::isOn() const { return m_led.isOn() ; }

void QamBargraph::off()												// slot
{
	m_led.off() ;
	setValue( value() ) ;
}

void QamBargraph::on()												// slot
{
	m_led.on() ;
	setValue( value() ) ;
}

void QamBargraph::toggle()											// slot
{
}

QamWidget::Blink QamBargraph::blink() const { return QamWidget::Fixed ; }

void QamBargraph::setBlink(QamWidget::Blink /*blink*/ )
{
}

void QamBargraph::setFlash(QamWidget::State /*state*/, int /*ms*/ )
{
}

bool QamBargraph::isShining() const { return m_led.isShining() ; }

void QamBargraph::setShining(bool shining )
{
	m_led.setShining( shining ) ;
	for (int i = 0 ; i < m_size ; ++i ) m_leds[i].setShining( shining ) ;
}

QColor QamBargraph::customColor() const { return m_led.customColor() ; }

void QamBargraph::setCustomColor(const QColor& color )
{
	m_led.setCustomColor( color ) ;
	for (int i = 0 ; i < m_size ; ++i )	m_leds[i].setCustomColor( color ) ;
}

int	QamBargraph::alphaOff() const { return m_led.alphaOff() ; }

void QamBargraph::setAlphaOff(int alphaOff )
{
	m_led.setAlphaOff( alphaOff ) ;
	for (int i = 0 ; i < m_size ; ++i )	m_leds[i].setAlphaOff( alphaOff ) ;
}

