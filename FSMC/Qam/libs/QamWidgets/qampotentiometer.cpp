/*  ---------------------------------------------------------------------------
 *  filename    :   qampotentiometer.cpp
 *  description :   IMPLEMENTATION de la classe QamPotentioMeter
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

#include "qampotentiometer.h"
#include <QPainter>
#include <QMouseEvent>
#include <math.h>

QamPotentioMeter::QamPotentioMeter( QWidget* parent )
	: QamAbstractAnalog( parent )
	, m_shape( QamWidget::Square )
	, m_valueVisible( false )
	, m_angle( 260 )									// angle min-max
	, m_ringThickness( 10 )								// % du rayon
	, m_zoneThickness( 8 )								// % du rayon
	, m_ringColor( QColor(52, 30, 30 ) )
	, m_stepNumber( 10 )
	, m_stepMark( 1 )
	, m_click( false )
{
	QFont	widgetFont( font() ) ;
	widgetFont.setPointSize( 20 ) ;
	setFont( widgetFont ) ;
	setStepFont( widgetFont ) ;
	
	setMaximum( 10 ) ;
	setValue( 0 ) ;
	setLowThreshold( 0 ) ;
	setHighThreshold( 10 ) ;
	setUnit("") ;

	setLowColor(QColor(0, 0, 0, 0 ) ) ;					// transparent
	setNormalColor(QColor(0, 0, 0, 0 ) ) ;
	setHighColor(QColor(0, 0, 0, 0 ) ) ;
	
	m_grid = 400 ; 				// résolution de la grille de dessin
	m_radius = m_grid / 2 ;		// rayon (quand axe central)
}

QamPotentioMeter::~QamPotentioMeter()
{
}

// mise à l'échelle (pot. rotatif)

void QamPotentioMeter::setDrawingArea(QPainter& painter )
{
	int w , h ;
	
	painter.translate(width() / 2, height() / 2 ) ;		// origine au centre

	if ( shape() == QamWidget::Flat ) {
		w = width() ;
		h = height() ;
	}
	else {
		w = h = qMin(width(), height() ) ;
		painter.scale( w / (double)(m_grid + 2), h / (double)(m_grid + 2) ) ;
	}
}

QRect	QamPotentioMeter::inDrawingArea(const QRect& rect )
{
	QRect	r( rect ) ;
	int		w = ( shape() == QamWidget::Flat ? width() : qMin(width(), height() ) ) ;
	int		h = ( shape() == QamWidget::Flat ? height() : qMin(width(), height() ) ) ;

	if ( shape() != QamWidget::Flat ) {
		r.setWidth( r.width() * w / (double)(m_grid + 2) ) ;
		r.setHeight( r.height() * h / (double)(m_grid + 2) ) ;
	}
	r.moveCenter(QPoint( width() / 2, height() / 2 ) ) ;
	return r ;
}

// dessin du cadre de l'appareil

QRect	QamPotentioMeter::drawFrame(QPainter& painter, QRect rdraw, QBrush fillColor )
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
		case QamWidget::Square :
		case QamWidget::Plain :
			painter.drawRoundedRect( rdraw, 20, 20 ) ;
			break ;
		default :
			break ;
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
		case QamWidget::Square :
			painter.drawRoundedRect( rdraw, 18, 18 ) ;
			break ;
		case QamWidget::Plain :
			painter.drawEllipse( rdraw ) ;
			break ;
		default :
			break ;
	}

	// remplissage

	painter.setPen(pen ) ;
	painter.setBrush( fillColor ) ;
	
	rdraw.adjust(rThickness / 2, rThickness / 2, -rThickness / 2, -rThickness / 2 ) ;

	QRect qr(rdraw) ; 
	qr.adjust(-3, -3, 3, 3 ) ;			// marge cadre/anneau graduations

	switch ( m_shape ) {
		case QamWidget::Square :
			painter.drawRoundedRect( qr, 16, 16 ) ;
			break ;
		case QamWidget::Plain :
			painter.drawEllipse( rdraw ) ;
			break ;
		default :
			break ;
	}
	
	return rdraw ;
}

// dessin du fond de l'appareil (pixmap maintenu par classe de base)

void QamPotentioMeter::drawBackground(QPainter& painter )
{
	//
	painter.save() ;	// new v5.0
	//
	painter.setRenderHint(QPainter::Antialiasing ) ;
	painter.fillRect(rect(), QBrush( palette().background() ) ) ;

	setDrawingArea(painter ) ;

	// cadre

	QRect	rdraw(-m_grid / 2, -m_grid / 2, m_grid, m_grid ) ;

	QPen	pen(m_ringColor ) ;

	int rThickness = (int)(m_grid / 2 * m_ringThickness / 100.0 ) ;
	if ( !isShining() )	rThickness /= 2 ;

    QRadialGradient ring3(QPointF(0.0, 0.0 ), m_grid / 2, QPointF(-35.0, -35.0 ) ) ;

	ring3.setColorAt(0.0, Qt::white ) ;
	ring3.setColorAt(0.5, QColor(240,240,240 ) ) ;
	ring3.setColorAt(1.0, QColor(215,215,215 ) ) ;

	QBrush	fillBrush( isShining() ? QBrush(ring3 ) : QBrush(QColor(215,215,215 ) ) ) ;

	rdraw = drawFrame(painter, rdraw, fillBrush ) ;
		
	// zone intérieure

	int zThickness = (int)(m_radius * m_zoneThickness / 100.0 ) ;

	if ( shape() == QamWidget::Square ) {

		painter.setPen(Qt::NoPen ) ;
		rdraw.adjust(zThickness, zThickness, -zThickness, -zThickness ) ;
		if ( isShining() )	painter.setBrush(QBrush(ring3 ) ) ;
		else				painter.setBrush(QBrush(QColor(215,215,215 ) ) ) ;
		painter.drawEllipse( rdraw ) ;
	}

	// graduations (potentiomètre rotatif)

	int	m_stepRadius = m_radius - 2 * rThickness ;
	int m_wText = painter.fontMetrics().size(Qt::TextSingleLine, QString("8888") ).width() ;

	m_stepRadius -= m_wText / 2 ;								// rayon d'inscription des valeurs

	int m_stepWidth = 3 ;										// largeur des traits

	int y0 = m_radius - 2 * rThickness - m_wText - zThickness ;	// début graduation
	int y1 = y0 - zThickness ;									// fin graduation

	y0 -= m_stepWidth - 1 ;										// correction suivant épaisseur du trait
	y1 += m_stepWidth - 1 ;

	m_buttonRect.setTopLeft(QPoint(-y1, -y1 ) ) ;				// aire du bouton central
	m_buttonRect.setBottomRight(QPoint(y1, y1 ) ) ;
	m_buttonRect.adjust(zThickness / 2 ,zThickness / 2,-zThickness / 2,-zThickness / 2 ) ;

	if (( shape() == QamWidget::Square )||( shape() == QamWidget::Circular )) {

		painter.setBrush(QBrush(Qt::black ) ) ;

		pen.setWidth( m_stepWidth ) ;
		painter.setPen(pen ) ;

		painter.rotate( ( 360 - m_angle ) / 2.0 ) ;					// départ sur minimum() de l'échelle

		painter.save() ;
		for ( int i = 0 ; i < m_stepNumber + 1 ; ++i ) {
			painter.drawLine(0, y0 , 0, y1 ) ;
			painter.rotate( m_angle / (double)m_stepNumber ) ;
		}
		painter.restore() ;

		// valeurs des graduations

		if ( m_stepMark ) {

			painter.setPen( palette().color( QPalette::WindowText ) ) ;
			painter.save() ;
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
			painter.restore() ;
		}
	}

	// curseur fixe (potentiomètre rotatif)

	if ( shape() == QamWidget::Plain ) {
		painter.save() ;
		painter.setPen(Qt::NoPen ) ;
		painter.setBrush(Qt::white ) ;
		painter.rotate( ( 360 + m_angle ) / 2.0 ) ;

		int w  = 10 ;
		int y0 = rdraw.width() / 2 ;
		int y1 = y0 + 3 * zThickness ;

		if ( isShining() ) {
			const int cursor[12] = {- w / 2, y0, -w / 2, y1, 0, y1 + zThickness / 3, w / 2, y1, w / 2, y0 } ;
			QPainterPath cursorPath ;
			cursorPath.moveTo( QPointF(cursor[0], cursor[1] ) ) ;
			for (int i = 2 ; i < 10 ; i += 2 )	cursorPath.lineTo(cursor[i], cursor[i+1] ) ;
			cursorPath.lineTo(cursor[0], cursor[1] ) ;
			painter.drawPath(cursorPath ) ;
		}
		else {
			QRect cursor(0, 0, 2 * w, 2 * w ) ;
			cursor.moveCenter(QPoint(0, y0 + 2 * w ) ) ;
			painter.drawEllipse( cursor ) ;
		}

		painter.restore() ;
	}

	// bouton (potentiomètre rotatif)

	if ( shape() != QamWidget::Flat ) {
		QColor	color = m_ringColor.lighter(200) ;
		QConicalGradient	cg(QPointF(0.0, 0.0 ), 360 ) ;
		cg.setColorAt(0.0, Qt::white ) ;
		cg.setColorAt(0.25, color ) ;
		cg.setColorAt(0.5, Qt::white ) ;
		cg.setColorAt(0.75, color ) ;
		cg.setColorAt(1.0, Qt::white ) ;

		painter.save() ;

		painter.rotate(-135 ) ;
		painter.setPen(m_ringColor ) ;

		if ( isShining() )	painter.setBrush(QBrush( cg ) ) ;
		else				painter.setBrush(QBrush( palette().background() ) ) ;
		painter.drawEllipse( m_buttonRect ) ;

		if ( isShining() ) {
			painter.rotate(15 ) ;
			painter.setPen(Qt::NoPen ) ;
			QRect	r( m_buttonRect ) ;
			r.adjust(rThickness,rThickness,-rThickness,-rThickness );
			painter.drawEllipse( r ) ;
		}

		painter.restore() ;
	}

	// potentiomètre linéaire

	if ( shape() == QamWidget::Flat ) {
		m_buttonRect = rect() ;
		m_flatCursor = QRect(0, 0, width() / 2, width() / 4 ) ;

		QString	s = QString("%1").arg( unit() )  ;
		painter.setFont( font() ) ;
		QRect rText = painter.fontMetrics().boundingRect( s ) ;
		m_yFlatMin = height() / 2 - rText.height() - m_flatCursor.height() / 2  ;
		m_yFlatMax = -height() / 2 + m_flatCursor.height() / 2 + 1 ;

		int xc = 4 ;

		// coulisse verticale

		if ( isShining() ) {
			painter.setPen(Qt::NoPen ) ;
			painter.setBrush(QBrush(m_ringColor ) ) ;
		}
		else {
			painter.setPen(m_ringColor ) ;
			painter.setBrush(QBrush( palette().background() ) ) ;
		}
		painter.drawRoundedRect(-xc, m_yFlatMax - 2 * xc, 2 * xc, m_yFlatMin - m_yFlatMax + 4 * xc, xc, xc ) ;

		// graduations

		QPen	pen(Qt::black ) ;
		pen.setWidthF( 0.5 ) ;
		painter.setPen( pen ) ;

		for ( int i = 0 ; i < m_stepNumber + 1 ; ++i ) {
			int y = m_yFlatMin - ( m_yFlatMin - m_yFlatMax ) / (double)(m_stepNumber) * i ;
			painter.drawLine(-m_flatCursor.width() / 2 + xc, y , -2 * xc, y ) ;
			painter.drawLine( 2 * xc, y , m_flatCursor.width() / 2 - xc, y ) ;
		}

		// valeurs des graduations

		if ( m_stepMark ) {
			painter.setPen( palette().color( QPalette::WindowText ) ) ;
			painter.setFont( m_stepFont ) ;

			for ( int i = 0 ; i < ( m_stepNumber / m_stepMark + 1 ) ; ++i ) {
				double v = minimum() + i * ( maximum() - minimum() ) / ( m_stepNumber / m_stepMark ) ;
				if ( fabs(v) < 0.000001 ) v = 0.0 ;
				QString val = QString("%1").arg(v) ;
				QRect	r = painter.fontMetrics().boundingRect( val ) ;
				
				r.adjust(-5,0,0,0 ) ;						// new 27/11/2012
				
				int y = m_yFlatMin - ( m_yFlatMin - m_yFlatMax ) / (double)( m_stepNumber / m_stepMark ) * i ;
				r.moveTopRight(QPoint(-m_flatCursor.width() / 2 - xc, y - r.height() / 2 ) ) ;
				painter.drawText(r, Qt::AlignRight | Qt::AlignVCenter, val );
			}
		}

		// sérigraphie(s) triangulaire(s)

		QPainterPath cursorPath ;

		cursorPath.moveTo( m_flatCursor.width() / 2 + xc, m_yFlatMin ) ;
		cursorPath.lineTo( m_flatCursor.width() / 2 + xc, m_yFlatMax ) ;
		if ( ( minimum() * maximum() ) < 0.0 ) {
			cursorPath.lineTo( m_flatCursor.width() / 2 + 5 * xc, m_yFlatMax ) ;
			cursorPath.lineTo( m_flatCursor.width() / 2 + xc, m_yFlatMin - ( (m_yFlatMin - m_yFlatMax) * -minimum() ) / (double)( maximum() - minimum() ) ) ;
			cursorPath.lineTo( m_flatCursor.width() / 2 + 5 * xc, m_yFlatMin ) ;
		}
		else {
			if ( minimum() < maximum() )	cursorPath.lineTo( m_flatCursor.width() / 2 + 5 * xc, m_yFlatMax ) ;
			else							cursorPath.lineTo( m_flatCursor.width() / 2 + 5 * xc, m_yFlatMin ) ;
		}
		cursorPath.moveTo( m_flatCursor.width() / 2 + xc, m_yFlatMin ) ;
		painter.setPen(Qt::NoPen ) ;
		painter.setBrush( QBrush(QColor(215,215,215 ) ) ) ;
		painter.drawPath( cursorPath ) ;
	}
	//
	painter.restore() ;	// new v5.0
	//
}

void QamPotentioMeter::drawForeground(QPainter& painter )
{
	//
	painter.save() ;
	//
	painter.setRenderHint(QPainter::Antialiasing ) ;
	setDrawingArea(painter ) ;

	int zThickness = (int)(m_radius * m_zoneThickness / 100.0 ) ;
	int rThickness = (int)(m_grid / 2 * m_ringThickness / 100.0 ) ;
	int	m_stepRadius = m_radius - 2 * rThickness ;

	// sérigraphie mobile pot. rotatif

	if (( m_stepMark )&&( shape() == QamWidget::Plain )) {

		painter.setPen( palette().color( QPalette::WindowText ) ) ;
		painter.setFont( m_stepFont ) ;

		painter.rotate( -90 ) ;
		painter.rotate( ( m_angle * ( value() - minimum() ) ) / (double)( maximum() - minimum() ) ) ;

		float offs = -( 360 - m_angle ) / 2.0 + 90 ;
		float angle = -m_angle / (float)( m_stepNumber / m_stepMark ) ;

		for ( int i = 0 ; i < ( m_stepNumber / m_stepMark + 1 ) ; ++i ) {

			double v = minimum() + i * ( maximum() - minimum() ) / ( m_stepNumber / m_stepMark ) ;
			if ( fabs(v) < 0.000001 ) v = 0.0 ;
			QString val = QString("%1").arg(v) ;
			QSize size = painter.fontMetrics().size(Qt::TextSingleLine, val ) ;

			painter.save() ;
			painter.rotate( offs + angle * i ) ;
			painter.drawText(-size.width() / 2, m_stepRadius, val ) ;
			painter.restore() ;
		}
	}

	// curseur sur bouton pot. rotatif

	if (( shape() == QamWidget::Square )||( shape() == QamWidget::Circular )) {

		painter.setPen(Qt::NoPen ) ;
		painter.setBrush(QBrush(m_ringColor ) ) ;

		painter.save() ;
		painter.rotate( ( 360 - m_angle ) / 2.0 ) ;
		painter.rotate(  ( m_angle * ( value() - minimum() ) ) / (double)( maximum() - minimum() ) ) ;

		int w  = 10 ;
		int y0 = m_buttonRect.height() / 2 ;
		int y1 = y0 - 3 * zThickness ;

		if ( isShining() ) {
			const int cursor[12] = {- w / 2, y1, -w / 2, y0, 0, y0 + zThickness / 3, w / 2, y0, w / 2, y1 } ;
			QPainterPath cursorPath ;
			cursorPath.moveTo( QPointF(cursor[0], cursor[1] ) ) ;
			for (int i = 2 ; i < 10 ; i += 2 )	cursorPath.lineTo(cursor[i], cursor[i+1] ) ;
			cursorPath.lineTo(cursor[0], cursor[1] ) ;
			painter.drawPath(cursorPath ) ;
		}
		else {
			QRect cursor(0, 0, 2 * w, 2 * w ) ;
			cursor.moveCenter(QPoint(0, y0 - 2 * w ) ) ;
			painter.drawEllipse( cursor ) ;
		}
		painter.restore() ;
	}

	// curseur mobile pot. linéaire

	else if ( shape() == QamWidget::Flat ) {
		QRect	cursor(m_flatCursor ) ;

		int y = m_yFlatMin - ( (m_yFlatMin - m_yFlatMax) * ( value() - minimum() ) ) / (double)( maximum() - minimum() ) ;
		cursor.moveCenter(QPoint(0, y ) ) ;

		painter.setPen(m_ringColor ) ;

		if ( isShining() ) {
			QLinearGradient lg(0,cursor.top(), 0, cursor.bottom() ) ;
			QColor	color = m_ringColor.lighter(200) ;
			lg.setColorAt(0, color );
			lg.setColorAt(0.5, Qt::white );
			lg.setColorAt(1, color );
			painter.setBrush( lg ) ;
		}
		else	painter.setBrush(QBrush( palette().background() ) ) ;

		painter.drawRoundedRect(cursor, 5, 5 ) ;

		if ( !isShining() ) {
			cursor.adjust(0, cursor.height() / 2 + 1, 0, -cursor.height() / 2 - 1 ) ;
			painter.drawRect( cursor ) ;
		}

	}

	// unité (ou label...), ici pour prise en compte changement fonte...

	if ( valueVisible() ) {
		painter.save() ;
		painter.setFont( font() ) ;
		painter.setPen( palette().color( QPalette::WindowText ) ) ;
		QString	s = QString("%1").arg( unit() )  ;
		QRect rText = painter.fontMetrics().boundingRect( s ) ;

		switch( shape() ) {
		case QamWidget::Square :
		case QamWidget::Circular :
			rText.moveCenter(QPoint(0, (int)(m_radius * 0.70) ) ) ;
			break ;
		case QamWidget::Flat :
			rText.moveCenter(QPoint(0, height() / 2 - rText.height() / 2 - 1 ) ) ;
			break ;
		default :
			rText.moveCenter(QPoint(0, 0 ) ) ;
			painter.rotate( 90 ) ;
		}

		painter.drawText( rText, Qt::AlignCenter, s ) ;
		painter.restore() ;
	}
	//
	painter.restore() ;
	//
}

// amorce changement de valeur (à gauche ou à droite de l'axe, sauf pour pot. linéaire)

void QamPotentioMeter::mousePressEvent( QMouseEvent* pe )
{
	m_click = true ;
	m_mousePos = pe->pos() ;
	m_mouseSide = ( m_mousePos.x() > width() / 2 ? 1 : -1 ) ;
	if ( shape() == QamWidget::Flat )	m_mouseSide = - 1 ;
}

// fin de changement valeur

void QamPotentioMeter::mouseReleaseEvent( QMouseEvent* /*pe*/ )
{
	m_click = false ;
}

// changement valeur par pas de 0,1 (Press..Move..Release)

void QamPotentioMeter::mouseMoveEvent(QMouseEvent* pe )
{
	if ( ! m_click )	return ;

	m_mousePos = pe->pos() - m_mousePos ;
	setValue( value() + m_mouseSide * m_mousePos.y() / 10.0 ) ;
	m_mousePos = pe->pos() ;
}

// changement valeur par pas de 1 (Wheel +/-)

void QamPotentioMeter::wheelEvent(QWheelEvent* we )
{
	if ( !inDrawingArea( m_buttonRect ).contains( we->x(), we->y() ) ) return ;

	int numSteps = we->delta() / 120 ;
	setValue( value() + numSteps ) ;
	we->accept() ;
}

// propriétés

QamWidget::Shape QamPotentioMeter::shape() const { return m_shape ; }

void QamPotentioMeter::setShape(QamWidget::Shape shape ) 
{ 
	m_shape = shape ; 
	updateWithBackground() ; 
}

bool	QamPotentioMeter::valueVisible() const { return m_valueVisible ; }

void	QamPotentioMeter::setValueVisible(bool visible )
{
	m_valueVisible = visible ;
	update() ;
}

int	QamPotentioMeter::stepNumber() const { return m_stepNumber ; }

void	QamPotentioMeter::setStepNumber(int stepNumber )
{
	if ( stepNumber < 2 )	stepNumber = 2 ;
	m_stepNumber = stepNumber ;
	updateWithBackground() ;
}

int		QamPotentioMeter::stepMark() const { return m_stepMark ; }

void	QamPotentioMeter::setStepMark(int stepMark )
{
	if ( stepMark < 0 )	stepMark = 0 ;
	m_stepMark = stepMark ;
	updateWithBackground() ;
}

QFont	QamPotentioMeter::stepFont() const { return m_stepFont ; }

void	QamPotentioMeter::setStepFont(const QFont& stepFont )
{
	m_stepFont = stepFont ;
	updateWithBackground() ;
}

