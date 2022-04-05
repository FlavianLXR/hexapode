/*  ---------------------------------------------------------------------------
 *  filename    :   qamabstractanalog.cpp
 *  description :   IMPLEMENTATION de la classe QamAbstractAnalog
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

#include "qamabstractanalog.h"
#include <math.h>

QamAbstractAnalog::QamAbstractAnalog(QWidget* parent )
	: QamWidget(parent )
	, m_orientation( QamWidget::Bottom )
	, m_minimum( 0 )
	, m_maximum( 100 )
	, m_lowThreshold( 25 )
	, m_highThreshold( 75 )
	, m_value( 50 )
	, m_unit( "%" )
	, m_lowColor( Qt::yellow )
	, m_normalColor( Qt::green )
	, m_highColor( Qt::red )
	, m_shining( false )
{
	m_background = new QPixmap( size() ) ;
	m_redraw = false ;
}

QamAbstractAnalog::~QamAbstractAnalog()
{
	delete m_background ;
}

void QamAbstractAnalog::showBackground()
{
	if (( m_background->size() != size() )||( m_redraw )) {
		delete m_background ;
		m_background = new QPixmap( size() ) ;
		QPainter	painter( m_background ) ;
		drawBackground(painter) ;
		m_redraw = false ;
	}
    QPainter painter(this);
    painter.drawPixmap(0, 0, *m_background ) ;
}

void QamAbstractAnalog::updateWithBackground()
{
	m_redraw = true ;
	update() ;
}

// new v5.0
void QamAbstractAnalog::draw(QPainter* painter )
{
	drawBackground( *painter ) ;
	drawForeground( *painter ) ;
}

void QamAbstractAnalog::paintEvent(QPaintEvent* pe )
{
	Q_UNUSED( pe )

	showBackground() ;

	QPainter	painter(this) ;
//	painter.save() ;
	drawForeground( painter ) ;
//	painter.restore() ;
}
//

QamWidget::Orientation QamAbstractAnalog::orientation() const
{
	return m_orientation ;
}

void QamAbstractAnalog::setOrientation(QamWidget::Orientation orientation )
{
	m_orientation = orientation ;
	updateWithBackground() ;
}

double	QamAbstractAnalog::minimum() const { return m_minimum ; }

void	QamAbstractAnalog::setMinimum(double min )
{
	m_minimum = min ;
	updateWithBackground() ;
}

double	QamAbstractAnalog::maximum() const { return m_maximum ; }

void	QamAbstractAnalog::setMaximum(double max )
{
	m_maximum = max ;
	updateWithBackground() ;
}

void	QamAbstractAnalog::setRange(double min, double max )
{
	m_minimum = min ;
	m_maximum = max ;
	updateWithBackground() ;
}

double	QamAbstractAnalog::lowThreshold() const { return m_lowThreshold ; }

void	QamAbstractAnalog::setLowThreshold(double low )
{
	m_lowThreshold = low ;
	updateWithBackground() ;
}

double	QamAbstractAnalog::highThreshold() const { return m_highThreshold ; }

void	QamAbstractAnalog::setHighThreshold(double high )
{
	m_highThreshold = high ;
	updateWithBackground() ;
}

void	QamAbstractAnalog::setThresholds(double low, double high )
{
	m_lowThreshold = low ;
	m_highThreshold = high ;
	updateWithBackground() ;
}

bool	QamAbstractAnalog::isLowLevel() const
{
	return value() < lowThreshold() ;
}

bool	QamAbstractAnalog::isHighLevel() const
{
	return value() > highThreshold() ;
}

bool	QamAbstractAnalog::isNormalLevel() const
{
	return !( isLowLevel() || isHighLevel() ) ;
}

double	QamAbstractAnalog::value() const { return m_value ; }

void	QamAbstractAnalog::setValue(int value )						// slot
{
	setValue( (double)value ) ;
}

void	QamAbstractAnalog::setValue(double value )					// slot
{
	bool l = isLowLevel() ;
	bool h = isHighLevel() ;
	bool n = isNormalLevel() ;

	if ( value < minimum() )	value = minimum() ;
	if ( value > maximum() )	value = maximum() ;

	if ( fabs(value) < 0.000001 )	value = 0.0 ;

	m_value = value ; 
	
	// alarmes de franchissements de seuils

	if ( ( !l )&&( isLowLevel() ) )		emit lowLevel() ;
	if ( ( !h )&&( isHighLevel() ) )	emit highLevel() ;
	if ( ( !n )&&( isNormalLevel() ) )	emit normalLevel() ;

	emit valueChanged( m_value ) ;

	update() ;
}

QString	QamAbstractAnalog::unit() const { return m_unit ; }

void	QamAbstractAnalog::setUnit(const QString& unit )
{
	m_unit = unit ;
	updateWithBackground() ;
}

QColor	QamAbstractAnalog::lowColor() const { return m_lowColor ; }

void	QamAbstractAnalog::setLowColor(const QColor& color )
{
	m_lowColor = color ;
	updateWithBackground() ;
}

QColor	QamAbstractAnalog::normalColor() const { return m_normalColor ; }

void	QamAbstractAnalog::setNormalColor(const QColor& color )
{
	m_normalColor = color ;
	updateWithBackground() ;
}

QColor	QamAbstractAnalog::highColor() const { return m_highColor ; }

void	QamAbstractAnalog::setHighColor(const QColor& color )
{
	m_highColor = color ;
	updateWithBackground() ;
}

bool 	QamAbstractAnalog::isShining() const { return m_shining ; }

void 	QamAbstractAnalog::setShining(bool shining )
{
	m_shining = shining ;
	updateWithBackground() ;
}

