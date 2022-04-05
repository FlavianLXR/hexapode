/*  ---------------------------------------------------------------------------
 *  filename    :   qamanalogmeter.cpp
 *  description :   IMPLEMENTATION de la classe QamAnalogMeter
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

#include "qamanalogmeter.h"
#include <QPainter>
#include <math.h>

QamAnalogMeter::QamAnalogMeter( QWidget* parent )
	: QamAbstractAnalog( parent )
	, m_shape( QamWidget::Square )
	, m_valueVisible( true )
	, m_needleColor( Qt::black )
	, m_angle( 260 )									// angle min-max
	, m_ringThickness( 10 )								// % du rayon
	, m_zoneThickness( 8 )								// % du rayon
	, m_ringColor( QColor(52, 30, 30 ) )
	, m_stepNumber( 20 )
	, m_stepMark( 4 )
{
	QFont	widgetFont( font() ) ;
	widgetFont.setPointSize( 20 ) ;
	setFont( widgetFont ) ;
	widgetFont.setPointSize( 12 ) ;
	setStepFont( widgetFont ) ;
	
	setLowColor(QColor(0, 0, 0, 0 ) ) ;					// transparent
	setNormalColor(QColor(0, 200, 0 ) ) ;				// vert foncé
	setHighColor(QColor(200, 0, 0 ) ) ;					// rouge foncé
	
	m_grid = 400 ; 				// résolution de la grille de dessin
	m_radius = m_grid / 2 ;		// rayon (quand axe central)
}

QamAnalogMeter::~QamAnalogMeter()
{
}

// mise à l'échelle
// cas particulier qd 'Shape Flat' : wFlat = 50%, hFlat = 25% des
// dimensions w x h des autres modèles

void QamAnalogMeter::setDrawingArea(QPainter& painter )
{
	int side = qMin(width(), height() ) ;
	int w , h ;
	
	painter.translate(width() / 2, height() / 2 ) ;		// origine au centre
	
	if ( shape() == QamWidget::Flat ) {
		w = width() ; 
		h = height() ;
		m_angle = 26 ;
		m_radius = m_grid * 2 ;
	}
	else {
		w = h = side ;
		m_angle = 260 ;
		m_radius = m_grid / 2 ;
	}
	
	painter.scale( w / (double)(m_grid + 2), h / (double)(m_grid + 2) ) ;
}

// dessin du cadre de l'appareil

QRect	QamAnalogMeter::drawFrame(QPainter& painter, QRect rdraw, QBrush fillColor )
{
	QPen	pen(m_ringColor ) ;

	// cadre extérieur

	int rThickness = (int)(m_grid / 2 * m_ringThickness / 100.0 ) ;
	if ( !isShining() )	rThickness /= 2 ;

	QRadialGradient	ring1(rdraw.center(), rdraw.width() / 2 * sqrt(2), QPointF(-rdraw.width(), -rdraw.height() ) ) ;
	ring1.setColorAt(0.0, Qt::white ) ;
	ring1.setColorAt(1.0, m_ringColor ) ;

	painter.setPen(pen ) ;
	if ( isShining() )	painter.setBrush( QBrush(ring1 ) ) ;
	else				painter.setBrush( QBrush(m_ringColor ) ) ;

	switch ( m_shape ) {
		case QamWidget::Flat :
			painter.drawRoundedRect( rdraw, 5, 5 ) ;
			break; 
		case QamWidget::Square :
		case QamWidget::Plain :
			painter.drawRoundedRect( rdraw, 20, 20 ) ;
			break ;
		default :
			painter.drawEllipse( rdraw ) ;
	}

	// cadre intérieur (effet de relief)
	
	QRadialGradient	ring2(rdraw.center(), rdraw.width() / 2 * sqrt(2), QPointF(rdraw.width(), rdraw.height() ) ) ;
	ring2.setColorAt(0.0, m_ringColor ) ;
	ring2.setColorAt(1.0, Qt::white ) ;

	painter.setPen(Qt::NoPen ) ;
	if ( isShining() )	painter.setBrush( QBrush(ring2 ) ) ;
	else				painter.setBrush( QBrush(m_ringColor ) ) ;

	rdraw.adjust(rThickness / 2, rThickness / 2, -rThickness / 2, -rThickness / 2 ) ;

	switch ( m_shape ) {
		case QamWidget::Flat :
			painter.drawRoundedRect( rdraw, 4, 4 ) ;
			break; 
		case QamWidget::Square :
			painter.drawRoundedRect( rdraw, 18, 18 ) ;
			break ;
		case QamWidget::Plain :
		default :
			painter.drawEllipse( rdraw ) ;
	}

	// remplissage

	painter.setPen(pen ) ;
	painter.setBrush( fillColor ) ;
	
	rdraw.adjust(rThickness / 2, rThickness / 2, -rThickness / 2, -rThickness / 2 ) ;

	QRect qr(rdraw) ; 
	qr.adjust(-3, -3, 3, 3 ) ;			// marge cadre/anneau graduations

	switch ( m_shape ) {
		case QamWidget::Flat :
			painter.drawRoundedRect( qr, 3, 3 ) ;
			break; 
		case QamWidget::Square :
			painter.drawRoundedRect( qr, 16, 16 ) ;
			break ;
		case QamWidget::Plain :
		default :
			painter.drawEllipse( rdraw ) ;
	}
	
	return rdraw ;
}

// dessin du fond de l'appareil (pixmap maintenu par classe de base)

void QamAnalogMeter::drawBackground(QPainter& painter ) 
{
	//
	painter.save() ;	// new v5.0
	//
	painter.setRenderHint(QPainter::Antialiasing ) ;
	painter.fillRect(rect(), QBrush( palette().background() ) ) ;

	setDrawingArea(painter ) ;

	QRect	rdraw(-m_grid / 2, -m_grid / 2, m_grid, m_grid ) ;

	QPen	pen(m_ringColor ) ;

	int rThickness = (int)(m_grid / 2 * m_ringThickness / 100.0 ) ;
	if ( !isShining() )	rThickness /= 2 ;

    QRadialGradient ring3(QPointF(0.0, 0.0 ), m_grid / 2, QPointF(-35.0, -35.0 ) ) ;

	if ( shape() == QamWidget::Flat ) {
		ring3.setRadius( m_grid / 2 * sqrt(2) ) ;
	}
	ring3.setColorAt(0.0, Qt::white ) ;
	ring3.setColorAt(0.5, QColor(240,240,240 ) ) ;
	ring3.setColorAt(1.0, QColor(215,215,215 ) ) ;

	QBrush	fillBrush( isShining() ? QBrush(ring3 ) : QBrush(QColor(215,215,215 ) ) ) ;

	rdraw = drawFrame(painter, rdraw, fillBrush ) ;
		
	if ( shape() == QamWidget::Flat ) {
		painter.translate(0, m_grid + m_grid / 2 ) ;
		rdraw = QRect(-m_radius, -m_radius, m_radius * 2, m_radius * 2 ) ;
		rdraw.adjust(rThickness * 2, rThickness * 2, -rThickness * 2, -rThickness * 2 ) ;
	}

	// zones colorées

	int angleMinToMax = 16 * m_angle ;	// étendue en 1/16e de degrés
	int angle ;							// 5760 --> 360°
	
    QRadialGradient ring4(ring3 ) ;
	ring4.setColorAt(0.0, Qt::white ) ;

	if ( shape() == QamWidget::Flat ) {
//		ring4.setFocalPoint(-m_radius, -m_radius / 2.5 ) ;

		ring4.setCenter(0, -m_radius * 0.15 ) ;					// new 27/11/2012
		ring4.setRadius(m_radius * 1.15) ;
		ring4.setFocalPoint(-m_grid / 2, -m_grid / 2 ) ;
	}

	// zone basse
	
	painter.setPen(Qt::NoPen ) ;
	ring4.setColorAt(1.0, lowColor() ) ;

	if ( lowThreshold() - minimum() > 0 ) {
		if ( isShining() )	painter.setBrush(ring4 ) ;
		else				painter.setBrush(QBrush( lowColor() ) ) ;
		angle = 0 ;
		// angle de départ en degrés = ( 360 - m_angle ) / 2 - 90
		painter.drawPie(rdraw, ( 5760 - angleMinToMax ) / 2 - 1440, angleMinToMax - angle % 5760 ) ;
	}
	
	// zone nominale
	
	ring4.setColorAt(1.0, normalColor() ) ;

	if ( highThreshold() - lowThreshold() > 0 ) {
		if ( isShining() )	painter.setBrush(ring4 ) ;
		else				painter.setBrush(QBrush( normalColor() ) ) ;
		angle = angleMinToMax * ( ( lowThreshold() - minimum() ) / ( maximum() - minimum() ) ) ;
		painter.drawPie(rdraw, ( 5760 - angleMinToMax ) / 2 - 1440, angleMinToMax - angle % 5760 ) ;
	}

	// zone critique

	ring4.setColorAt(1.0, highColor() ) ;

	if ( maximum() - highThreshold() > 0 ) {
		if ( isShining() )	painter.setBrush(ring4 ) ;
		else				painter.setBrush(QBrush( highColor() ) ) ;
		angle = angleMinToMax * ( ( highThreshold() - minimum() ) / ( maximum() - minimum() ) ) ;
		painter.drawPie(rdraw, ( 5760 - angleMinToMax ) / 2 - 1440, angleMinToMax - angle % 5760 ) ;
	}
	
	// intérieur

	if ( shape() == QamWidget::Flat ) {
		ring3.setCenter(0, 0 ) ;
		ring3.setRadius(m_radius ) ;
		ring3.setFocalPoint(-m_grid / 2, -m_grid / 2 ) ;
	}

	int zThickness = (int)(m_radius * m_zoneThickness / 100.0 ) ;
	rdraw.adjust(zThickness, zThickness, -zThickness, -zThickness ) ;
	if ( isShining() )	painter.setBrush(QBrush(ring3 ) ) ;
	else				painter.setBrush(QBrush(QColor(215,215,215 ) ) ) ;

	if ( shape() == QamWidget::Flat ) {
		angle = 0 ;
		painter.drawPie(rdraw, ( 5760 - angleMinToMax ) / 2 - 1440, angleMinToMax - angle % 5760 ) ;
	}
	else {
		painter.drawEllipse( rdraw ) ;
	}

	// graduations
	
	int m_stepWidth = 3 ;						// largeur des traits

	const int mark[6] = { 						// marques triangulaires
		-6, m_radius - rThickness, 
		 6, m_radius - rThickness, 
		 0, m_radius - rThickness - zThickness } ;

	painter.rotate( ( 360 - m_angle ) / 2.0 ) ;
	painter.save() ;
	painter.setBrush(QBrush(Qt::black ) ) ;
	
	if ( shape() == QamWidget::Flat )	pen.setWidth( m_stepWidth - 1 ) ;
	else								pen.setWidth( m_stepWidth ) ;
	
	int len ;
	for ( int i = 0 ; i < m_stepNumber + 1 ; ++i ) {
		len = m_radius - rThickness - zThickness + ( m_stepWidth - 1 ) ;

		if ( i % ( m_stepMark / 2 ) ) len += zThickness / 3 ;

		painter.setPen(pen ) ;
		
		if ( shape() == QamWidget::Flat ) {
			len -= 2 * zThickness / 3 ;
			painter.drawLine(0, m_radius - rThickness * 2 - ( m_stepWidth - 1 ) , 0, len ) ;
		}
		else {
			if ( i % m_stepMark ) 
					painter.drawLine(0, m_radius - rThickness - ( m_stepWidth - 1 ) , 0, len ) ;
			else {
				painter.setPen(Qt::NoPen ) ;
				painter.drawConvexPolygon( QPolygon(3, mark ) ) ;
			}
		}

		painter.rotate( m_angle / (double)m_stepNumber ) ;
	}
	painter.restore() ;
	
	// valeurs des graduations

	if (!m_stepMark )	return ;
		
	int	m_stepRadius = m_radius - rThickness - 2 * zThickness ;		// rayon d'inscription des valeurs

	m_stepRadius -= painter.fontMetrics().size(Qt::TextSingleLine, QString("8888") ).width() / 2 ;
	
	painter.setPen( palette().color( QPalette::WindowText ) ) ;
	painter.rotate( -( 360 - m_angle ) / 2.0 ) ;
	painter.setFont( m_stepFont ) ;
	
	for ( int i = 0 ; i < ( m_stepNumber / m_stepMark + 1 ) ; ++i ) {
		double v = minimum() + i * ( maximum() - minimum() ) / ( m_stepNumber / m_stepMark ) ;
		if ( fabs(v) < 0.000001 ) v = 0.0 ;
		QString val = QString("%1").arg(v) ;
		QSize size = painter.fontMetrics().size(Qt::TextSingleLine, val ) ;
		painter.save() ;
		float angle = m_angle / (float)( m_stepNumber / m_stepMark ) ;
		float offs = ( 360 - m_angle ) / 2.0 + 90 ;

		float x = m_stepRadius * cos( ( offs + angle * i ) * M_PI / 180.0 ) ;
		float y = m_stepRadius * sin( ( offs + angle * i ) * M_PI / 180.0 ) ;
		painter.translate(QPointF(x,y) ) ;
		painter.drawText(QPointF( size.width() / -2.0,  size.height() / 4.0 ), val ) ;
		painter.restore() ;
	}

	//
	painter.restore() ;	// new v5.0
	//
}

// dessin aiguille et valeur

void QamAnalogMeter::drawForeground(QPainter& painter )
{
	//
	painter.save() ;
	//
	painter.setRenderHint(QPainter::Antialiasing ) ;

	setDrawingArea(painter ) ;

	if ( shape() == QamWidget::Flat ) {
		painter.translate(0, m_grid + m_grid / 2 ) ;
	}
	
	painter.save() ;

	// aiguille

	int rThickness = m_ringThickness ;
	if ( !isShining() )	rThickness /= 2 ;
		
	int len = (int)( m_grid / 2  * ( rThickness + m_zoneThickness + 1 ) / 100.0 ) ;
	len = m_radius - len ;

	const int needle[12] = {-4, 0, -1, len, 1, len, 4, 0, 8, -50, -8, -50 } ;

	QPainterPath needlePath ;
	needlePath.moveTo( QPointF(needle[0], needle[1] ) ) ;
	for (int i = 2 ; i < 10 ; i += 2 )	needlePath.lineTo(needle[i], needle[i+1] ) ;
	needlePath.cubicTo( 8.1, -51.0,  5.0, -48.0,  0.0, -48.0 ) ;
	needlePath.cubicTo(-5.0, -48.0, -8.1, -51.0, -8.0, -50.0 ) ;

	painter.setPen(Qt::NoPen ) ;
	painter.setBrush(QBrush( needleColor() ) ) ;
	painter.rotate( ( 360 - m_angle ) / 2.0 ) ;
   	painter.rotate(  ( m_angle * ( value() - minimum() ) ) / (double)( maximum() - minimum() ) ) ;

	painter.drawPath(needlePath ) ;

	painter.restore() ;

	// axe
	
	if ( shape() != QamWidget::Flat ) {	
		int axeRadius = ( isShining() ? 15 : 10 ) ;

		QConicalGradient	cg(QPointF(0.0, 0.0 ), 360 ) ;
		cg.setColorAt(0.0, Qt::white ) ;
		cg.setColorAt(0.5, m_ringColor ) ;
		cg.setColorAt(1.0, Qt::white ) ;
	
		painter.save() ;
		painter.rotate(-135 ) ;
		painter.setPen(Qt::black ) ;
		if ( isShining() )	painter.setBrush(QBrush( cg ) ) ;
		else				painter.setBrush(QBrush( needleColor() ) ) ;
		painter.drawEllipse(-axeRadius, -axeRadius, 2 * axeRadius, 2 * axeRadius ) ;
		if ( isShining() ) {
			axeRadius /= 2 ;
			painter.rotate(180 ) ;
			painter.setPen(Qt::NoPen ) ;
			painter.drawEllipse(-axeRadius, -axeRadius, 2 * axeRadius, 2 * axeRadius ) ;
		}
		painter.restore() ;
	}

	// valeur et unité
	
	int		align = Qt::AlignLeft | Qt::AlignVCenter ;
	QString	s = QString(" %1 %2 ").arg( value() ).arg( unit() )  ;
	
	QRect rText = painter.fontMetrics().boundingRect( s ) ;

	if ( shape() == QamWidget::Flat ) {
		QRect	textFrame ;
		textFrame.setTopLeft( QPoint(-m_grid / 4, -rText.height() - rThickness ) ) ;
		textFrame.setWidth( m_grid / 2 ) ;
		textFrame.setHeight( 2 * (rText.height() + rThickness ) ) ;
		textFrame.moveCenter(QPoint(0, -m_grid - rText.height() - rThickness ) ) ;
		
		QRadialGradient rg(textFrame.center(), textFrame.width() / 2 * sqrt(2), QPointF(-35.0, -35.0 ) ) ;
		rg.setColorAt(0.0, Qt::white ) ;
		rg.setColorAt(0.5, QColor(240,240,240 ) ) ;
		rg.setColorAt(1.0, QColor(215,215,215 ) ) ;

		QBrush	fillBrush( isShining() ? QBrush(rg ) : QBrush(QColor(215,215,215 ) ) ) ;

		drawFrame(painter, textFrame, fillBrush ) ;
		
		rText.moveCenter(QPoint(0, -m_grid - rText.height() - rThickness ) ) ;
	}
	else {
		rText.moveCenter(QPoint(0, (int)(m_radius * 0.66) ) ) ;
	}

	if ( valueVisible() ) {
		painter.setPen( palette().color( QPalette::WindowText ) ) ;
		painter.drawText( rText, align, s ) ;
	}
	//
	painter.restore() ;
	//
}

QamWidget::Shape QamAnalogMeter::shape() const { return m_shape ; }

void QamAnalogMeter::setShape(QamWidget::Shape shape ) 
{ 
	m_shape = shape ; 
	updateWithBackground() ; 
}

bool	QamAnalogMeter::valueVisible() const { return m_valueVisible ; }

void	QamAnalogMeter::setValueVisible(bool visible )
{
	m_valueVisible = visible ;
	update() ;
}

QColor	QamAnalogMeter::needleColor() const { return m_needleColor ; }

void	QamAnalogMeter::setNeedleColor(const QColor& color )
{
	m_needleColor = color ;
	update() ;
}

int	QamAnalogMeter::stepNumber() const { return m_stepNumber ; }

void	QamAnalogMeter::setStepNumber(int stepNumber )
{
	if ( stepNumber < 2 )	stepNumber = 2 ;
	m_stepNumber = stepNumber ;
	updateWithBackground() ;
}

int		QamAnalogMeter::stepMark() const { return m_stepMark ; }

void	QamAnalogMeter::setStepMark(int stepMark )
{
	if ( stepMark < 2 )	stepMark = 2 ;
	m_stepMark = stepMark ;
	updateWithBackground() ;
}

QFont	QamAnalogMeter::stepFont() const { return m_stepFont ; }

void	QamAnalogMeter::setStepFont(const QFont& stepFont )
{
	m_stepFont = stepFont ;
	updateWithBackground() ;
}

