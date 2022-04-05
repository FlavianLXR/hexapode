/*  ---------------------------------------------------------------------------
 *  filename    :   qamgauge.cpp
 *  description :   IMPLEMENTATION de la classe QamGauge
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

#include "qamgauge.h"
#include <QPainter>

QamGauge::QamGauge( QWidget* parent )
	: QamAbstractAnalog( parent )
	, m_valueVisible( true )
	, m_needleThickness( 3 )
	, m_needleColor( QColor(0, 0, 0, 80 ) )
{
}

QamGauge::~QamGauge()
{
}

// dessin du fond de la jauge (pixmap maintenu par classe de base)

void QamGauge::drawBackground(QPainter& painter ) 
{
	//
	painter.save() ;	// new v5.0
	//
	painter.setRenderHint(QPainter::Antialiasing ) ;

	QColor m_thresholdColor = Qt::white ;

	// dessin du fond (blanc par défaut)
	
	painter.setBrush( Qt::white ) ;
	painter.drawRect( rect() ) ;

	// modèle "Bottom" pour tous les calculs
	
	bool	textOver = ( orientation() == Bottom )||( orientation() == Right ) ;
	bool	vertical = ( orientation() == Left )||( orientation() == Right ) ;
	int	fill ;		// pourcentage de remplissage par la zone colorée
	int	y ;			// coord. Y de la ligne de séparation
	int h ;			// hauteur de la zone colorée

	fill = ( valueVisible() ? 50 : 100 ) ;

	if ( textOver )
			y = height() - (int)( height() * ( fill / 100.0 ) ) ;
	else	y = (int)( height() * ( fill / 100.0 ) ) ;

	h = (int)( height() * ( fill / 100.0 ) ) ;

	// rectangle zone "low"
	
	int		xLow = 0 ;
	int		wLow = (int)( width() * ( lowThreshold() - minimum() ) / ( maximum() - minimum() ) ) ;
	QRect	rLow( xLow, ( textOver ? y : 0 ), wLow, h ) ;

	QLinearGradient	lgLow( rLow.x(), rLow.y(), rLow.x() + rLow.width(), rLow.y() ) ;
	lgLow.setColorAt( 0.7, lowColor() ) ;
   	lgLow.setColorAt( 1.0, isShining() ? m_thresholdColor : lowColor() ) ;

	// rectangle zone "middle"
	
	int		xMid = rLow.x() + rLow.width() ;
	int		wMid = (int)( width() * ( highThreshold() - lowThreshold() ) / ( maximum() - minimum() ) ) ;
	QRect	rMid( xMid, ( textOver ? y : 0 ), wMid, h ) ;

	QLinearGradient	lgMid( rMid.x(), rMid.y(), rMid.x() + rMid.width(), rMid.y() ) ;
	lgMid.setColorAt( 0.0, isShining() ? m_thresholdColor : normalColor() ) ;
	lgMid.setColorAt( 0.3, normalColor() ) ;
	lgMid.setColorAt( 0.7, normalColor() ) ;
	lgMid.setColorAt( 1.0, isShining() ? m_thresholdColor : normalColor() ) ;

	// rectangle zone "high"
	
	int		xHigh = rMid.x() + rMid.width() ;
	int		wHigh = (int)( width() * ( maximum() - highThreshold() ) / ( maximum() - minimum() ) ) ;
	QRect	rHigh( xHigh, ( textOver ? y : 0 ), wHigh, h ) ;

	rHigh.setRight( width() - 1 ) ;

	QLinearGradient	lgHigh( rHigh.x(), rHigh.y(), rHigh.x() + rHigh.width(), rHigh.y() ) ;
	lgHigh.setColorAt( 0.0, isShining() ? m_thresholdColor : highColor() ) ;
	lgHigh.setColorAt( 0.3, highColor() ) ;
	lgHigh.setColorAt( 1.0, highColor() ) ;

	// changement de repère et d'échelle si orientation right | left
	
	if ( vertical ) {
		painter.translate( 0, height() ); 
		painter.rotate( 270.0 ) ;
		double	factor = (double)height() / (double)width() ;
		painter.scale( factor , 1 / factor ) ;
	}

	// dessin des 3 zones sans cadre
	
	painter.setPen( Qt::NoPen ) ;

	if ( lowThreshold() != minimum() ) {
		painter.setBrush( lgLow ) ;
		painter.drawRect( rLow ) ;
	}
	if ( highThreshold() - lowThreshold() ) {
		painter.setBrush( lgMid ) ;
		painter.drawRect( rMid ) ;
	}
	if ( highThreshold() != maximum() ) {
		painter.setBrush( lgHigh ) ;
		painter.drawRect( rHigh ) ;
	}

	// annulation des matrices de transformation et cadre final
	
	painter.resetMatrix() ;
	painter.setPen( Qt::black ) ;
	painter.setBrush( Qt::NoBrush ) ;
	painter.drawRect( rect().adjusted(0,0,-1,-1) ) ;
	//
	painter.restore() ;	// new v5.0
	//
}

// dessin aiguille et valeur

void QamGauge::drawForeground(QPainter& painter )
{
	//
	painter.save() ;
	//
	bool textOver = ( orientation() == Bottom )||( orientation() == Right ) ;
	bool vertical = ( orientation() == Left )||( orientation() == Right ) ;
	int	fill ;		// pourcentage de remplissage par la zone colorée
	int	y ;			// coord. Y de la ligne de séparation
//	int h ;			// hauteur de la zone colorée

	fill = ( valueVisible() ? 50 : 100 ) ;

	if ( textOver )
			y = height() - (int)( height() * ( fill / 100.0 ) ) ;
	else	y = (int)( height() * ( fill / 100.0 ) ) ;

//	h = (int)( height() * ( fill / 100.0 ) ) ;

	// dessin de l'aiguille

	int		xNeedle , yNeedle ;
	QRect	rNeedle ;

	if ( vertical ) {
		xNeedle = (int)( width() * 0.05 ) ;
		yNeedle = height() - (int)( height() * ( value() - minimum() ) / ( maximum() - minimum() ) ) ;
		while ( ( yNeedle + needleThickness() ) >= height() )	yNeedle-- ;
		rNeedle = QRect( xNeedle, yNeedle, width() - 2 * xNeedle, needleThickness() ) ;
	}
	else {
		xNeedle = (int)( width() * ( value() - minimum() ) / ( maximum() - minimum() ) ) ;
		yNeedle = (int)( height() * 0.05 ) ;
		while ( ( xNeedle + needleThickness() ) >= width() )	xNeedle-- ;
		rNeedle = QRect( xNeedle, yNeedle, needleThickness(), height() - 2 * yNeedle ) ;
	}
	painter.setPen( needleColor() ) ;
	painter.setBrush( needleColor() ) ;
	painter.drawRect( rNeedle ) ;

	// sérigraphie " valeur + unité "

	if ( valueVisible() ) {
		QRect	rText ;

		if ( vertical ) {
			rText.setLeft ( (int)( height() * 0.05 ) ) ;
			rText.setRight( height() ) ;
			rText.setTop( textOver ? 0 : (int)( width() * ( fill / 100.0 ) ) ) ;
			rText.setHeight( width() - (int)( width() * ( fill / 100.0 ) ) ) ;

			painter.translate( 0, height() ) ;
			painter.rotate( 270.0 ) ;
		}
		else {
			rText.setLeft( (int)( width() * 0.05 ) ) ;
			rText.setRight( width() ) ;
			rText.setTop( textOver ? 0 : y ) ;
			rText.setBottom( textOver ? y : height() ) ;
		}

		int		align = Qt::AlignLeft | Qt::AlignVCenter ;

		QString	s = QString("%1").arg( value() ) ;
		s += " " + unit() ;

		painter.setPen( palette().color( QPalette::WindowText ) ) ;
		painter.drawText( rText, align, s ) ;
	}
	//
	painter.restore() ;
	//
}

bool	QamGauge::valueVisible() const { return m_valueVisible ; }

void	QamGauge::setValueVisible(bool visible )
{
	m_valueVisible = visible ;
	update() ;
}

int		QamGauge::needleThickness() const { return m_needleThickness ; }

void	QamGauge::setNeedleThickness(int needleThickness )
{
	m_needleThickness = needleThickness ;
	update() ;
}

QColor	QamGauge::needleColor() const { return m_needleColor ; }

void	QamGauge::setNeedleColor(const QColor& color )
{
	m_needleColor = color ;
	update() ;
}

