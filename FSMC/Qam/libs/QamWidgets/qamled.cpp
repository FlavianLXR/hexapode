/*  ---------------------------------------------------------------------------
 *  filename    :   qamled.cpp
 *  description :   IMPLMENTATION de la classe QamLed
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

#include <QtGui>
#include "qamled.h"

QamLed::QamLed( QWidget* parent, const QString& label ) 
	: QamAbstractDigital( parent )
	, m_orientation( QamWidget::Top )
	, m_shape( QamWidget::Circular )
	, m_size( 16 )
	, m_line( 1 )
	, m_text( label )
	, m_autoToggle( false )
	, m_click( false )
{
	setMinimumSize(m_size + 1, m_size + 1 ) ;
	setState( QamWidget::Green ) ;
}

QamLed::~QamLed() {}

// propriétés

QamWidget::Orientation QamLed::orientation() const { return m_orientation ; }

void QamLed::setOrientation(QamWidget::Orientation orientation ) 
{
	m_orientation = orientation ; 
	updateWithBackground() ; 
}

QString QamLed::text() const { return m_text ; }

void QamLed::setText(const QString& text )
{
	m_text = text ;
	updateWithBackground() ; 
}

QamWidget::Shape QamLed::shape() const { return m_shape ; }

void QamLed::setShape(QamWidget::Shape shape ) 
{ 
	m_shape = shape ; 
	updateWithBackground() ; 
}

int QamLed::size() const { return m_size ; }

void QamLed::setSize(int size ) 
{
	if ( size < 4 ) size = 4 ; 
	m_size = size ; 
	setMinimumSize(m_size + 1, m_size + 1 ) ;
	updateWithBackground() ; 
}

int QamLed::line() const { return m_line ; }

void QamLed::setLine(int thickness ) 
{ 
	if ( thickness < 0 )	thickness = 0 ; 
	m_line = thickness ; 
	updateWithBackground() ; 
}

bool QamLed::isAutoToggle() const { return m_autoToggle ; }

void QamLed::setAutoToggle(bool autoToggle )
{
	m_autoToggle = autoToggle ;
	if ( m_autoToggle ) {
		connect(this, SIGNAL( clicked() ), this, SLOT( toggle() ) ) ;
	}
	else {
		disconnect(this, SIGNAL( clicked() ), this, SLOT( toggle() ) ) ;
	}
}

// dessin de la sérigraphie de l'indicateur (pixmap maintenu par classe de base)

void QamLed::drawBackground(QPainter& painter ) 
{
	//
	painter.save() ;	// new v5.0
	//
	painter.setRenderHint(QPainter::Antialiasing ) ;
//    painter.fillRect(rect(), QBrush( palette().background() ) ) ;
    painter.fillRect(rect(), QBrush( palette().window() ) ) ;   // Qt 6.x

	// calcul du rectangle d'inscription de la led

	int		align = 0 ;
	int		ec = line() + 4 ;
	uint 	wled = size() ;
	uint	hled = ( shape() == QamWidget::Flat ? size() / 2 : size() ) ;
	QRect	rled ;
	QPoint	p1, p2 ;

	if (  shape() != QamWidget::Plain ) {
		switch( orientation() ) {
			case QamWidget::Left :	
				align = Qt::AlignRight | Qt::AlignVCenter ;
				rled.setLeft( 0 ) ;
				rled.setTop( ( height() - hled ) / 2 ) ;
				p1.setX( size() + ec ) ; 
				p1.setY( height() / 2 ) ;
//				p2.setX( width() - fontMetrics().width( text() ) - ec ) ;
                p2.setX( width() - fontMetrics().boundingRect( text() ).width() - ec ) ;
                p2.setY( p1.y() ) ;
				break ;
			case QamWidget::Top :	
				align = Qt::AlignHCenter | Qt::AlignBottom ;
				rled.setLeft( ( width() - size() ) / 2 ) ;
				rled.setTop( 0 ) ;
				p1.setX( width() / 2 ) ;
				p1.setY( hled + ec ) ;
				p2.setX( p1.x() ) ;
				p2.setY( height() - fontMetrics().height() - ec ) ;
				break ;
			case QamWidget::Right :
				align = Qt::AlignLeft | Qt::AlignVCenter ;
				rled.setLeft( width() - size() - 1 ) ;
				rled.setTop( ( height() - hled ) / 2 ) ;
//				p1.setX( fontMetrics().width( text() ) + ec ) ;
                p1.setX( fontMetrics().boundingRect( text() ).width() + ec ) ;
                p1.setY( height() / 2 ) ;
				p2.setX( width() - size() - ec - 1 ) ;
				p2.setY( p1.y() ) ;
				break ;
			case QamWidget::Bottom :
				align = Qt::AlignHCenter | Qt::AlignTop ;
				rled.setLeft( ( width() - size() ) / 2 ) ;
				rled.setTop( height() - hled - 1 ) ;
				p1.setX( width() / 2 ) ; 
				p1.setY( fontMetrics().height() + ec ) ;
				p2.setX( p1.x() ); 
				p2.setY( height() - size() - ec - 1 ) ;
				break ;
		} ;
	}
	else {
		wled = width() - 1 ;
		hled = height() - 1 ;
	}

	rled.setWidth( wled ) ;
	rled.setHeight( hled ) ;
	m_rled = rled ;

	// trait de sérigraphie et label

	if ( ( ! text().isEmpty() )&&( shape() != QamWidget::Plain ) ) {
		QPen pen(Qt::black, line() ) ;
		painter.setPen( pen ) ;
		if ( line() ) painter.drawLine( p1, p2 ) ;
		painter.drawText( rect(), align, text() ) ;
	}
	//
	painter.restore() ;	// new v5.0
	//
}

// dessin de l'effet éteint/allumé du widget

void QamLed::drawForeground(QPainter& painter )
{
	//
	painter.save() ;
	//
	uint 	wled = m_rled.width() ;
	uint	hled = m_rled.height() ;
	QColor	fcolor ;
	
	if ( isOn() )	fcolor = onColor() ;
	else			fcolor = offColor() ;

	// fabrication du gradient de couleurs et dessin de la led

	if ( shape() == QamWidget::Circular ) {
		QRadialGradient rgradient(	m_rled.x() + wled / 2, 		// centre x
									m_rled.y() + hled / 2, 		// centre y
									wled / 2, 					// rayon
									m_rled.x() + wled / 3, 		// point focal x
									m_rled.y() + hled / 3 ) ;	// point focal y

	    if ( state() != QamWidget::Off )
			rgradient.setColorAt(0.2, isShining() ? Qt::white : fcolor ) ;
	    rgradient.setColorAt(0.9, fcolor ) ;
	    rgradient.setColorAt(1,   Qt::black ) ;
	
		painter.setBrush(rgradient ) ;
		painter.drawEllipse(m_rled ) ;
	}
	else {
		QLinearGradient	lgradient(	m_rled.x(), m_rled.y(),
									m_rled.x() + wled, m_rled.y() + hled ) ;

	    if ( state() != QamWidget::Off )
			lgradient.setColorAt(0.1, isShining() ? Qt::white : fcolor ) ;
		lgradient.setColorAt(1,   fcolor ) ;

		painter.setBrush(lgradient ) ;
		painter.drawRect(m_rled ) ;
	}
	//
	painter.restore() ;
	//
}

// détection clic pour bascule on/off automatique

void QamLed::mousePressEvent( QMouseEvent* pe )
{
//	if ( m_rled.contains( pe->x(), pe->y() ) )	m_click = true ;
    if ( m_rled.contains( pe->position().x(), pe->position().y() ) )	m_click = true ;
}

void QamLed::mouseReleaseEvent( QMouseEvent* pe )
{
	if ( ! m_click )	return ;
//	if ( m_rled.contains( pe->x(), pe->y() ) ) {
    if ( m_rled.contains( pe->position().x(), pe->position().y() ) ) {
        m_click = false ;
		emit clicked() ;
	}
}

