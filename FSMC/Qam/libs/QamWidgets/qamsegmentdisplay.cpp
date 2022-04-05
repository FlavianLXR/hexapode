/*  ---------------------------------------------------------------------------
 *  filename    :   qamsegmentdisplay.cpp
 *  description :   IMPLEMENTATION de la classe QamSegmentDisplay
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
#include "qamsegmentdisplay.h"

// points des polygones des segments (grille 90x100)

static QPointF segm_a[4] = {
	QPointF( 12.0,  5.0 ) , QPointF( 66.0,  5.0 ) , QPointF( 60.0, 15.0 ) , QPointF( 18.0, 15.0 )
} ;
static QPointF segm_a_bold[4] = {
	QPointF( 15.0,  5.0 ) , QPointF( 63.0,  5.0 ) , QPointF( 54.0, 20.0 ) , QPointF( 24.0, 20.0 )
} ;

static QPointF segm_b[8] = {
	QPointF( 69.0,  5.0 ) , QPointF( 72.0, 10.0 ) , QPointF( 72.0, 45.0 ) , QPointF( 69.0, 50.0 ) ,
	QPointF( 66.0, 50.0 ) , QPointF( 60.0, 45.0 ) , QPointF( 60.0, 15.0 ) , QPointF( 66.0,  5.0 )
} ;
static QPointF segm_b_bold[8] = {
	QPointF( 66.0,  5.0 ) , QPointF( 72.0, 12.5 ) , QPointF( 72.0, 42.5 ) , QPointF( 66.0, 50.0 ) ,
	QPointF( 63.0, 50.0 ) , QPointF( 54.0, 42.5 ) , QPointF( 54.0, 20.0 ) , QPointF( 63.0,  5.0 )
} ;

static QPointF segm_c[8] = {
	QPointF( 69.0, 50.0 ) , QPointF( 72.0, 55.0 ) , QPointF( 72.0, 90.0 ) , QPointF( 69.0, 95.0 ) ,
	QPointF( 66.0, 95.0 ) , QPointF( 60.0, 85.0 ) , QPointF( 60.0, 55.0 ) , QPointF( 66.0, 50.0 )
} ;
static QPointF segm_c_bold[8] = {
	QPointF( 66.0, 50.0 ) , QPointF( 72.0, 57.5 ) , QPointF( 72.0, 87.5 ) , QPointF( 66.0, 95.0 ) ,
	QPointF( 63.0, 95.0 ) , QPointF( 54.0, 80.0 ) , QPointF( 54.0, 57.5 ) , QPointF( 63.0, 50.0 )
} ;

static QPointF segm_d[4] = {
	QPointF( 18.0, 85.0 ) , QPointF( 60.0, 85.0 ) , QPointF( 66.0, 95.0 ) , QPointF( 12.0, 95.0 )
} ;
static QPointF segm_d_bold[4] = {
	QPointF( 24.0, 80.0 ) , QPointF( 54.0, 80.0 ) , QPointF( 63.0, 95.0 ) , QPointF( 15.0, 95.0 )
} ;

static QPointF segm_e[8] = {
	QPointF( 12.0, 50.0 ) , QPointF( 18.0, 55.0 ) , QPointF( 18.0, 85.0 ) , QPointF( 12.0, 95.0 ) ,
	QPointF(  9.0, 95.0 ) , QPointF(  6.0, 90.0 ) , QPointF(  6.0, 55.0 ) , QPointF(  9.0, 50.0 )
} ;
static QPointF segm_e_bold[8] = {
	QPointF( 15.0, 50.0 ) , QPointF( 24.0, 57.5 ) , QPointF( 24.0, 80.0 ) , QPointF( 15.0, 95.0 ) ,
	QPointF( 12.0, 95.0 ) , QPointF(  6.0, 87.5 ) , QPointF(  6.0, 57.5 ) , QPointF( 12.0, 50.0 )
} ;

static QPointF segm_f[8] = {
	QPointF( 12.0,  5.0 ) , QPointF( 18.0, 15.0 ) , QPointF( 18.0, 45.0 ) , QPointF( 12.0, 50.0 ) ,
	QPointF(  9.0, 50.0 ) , QPointF(  6.0, 45.0 ) , QPointF(  6.0, 10.0 ) , QPointF(  9.0,  5.0 )
} ;
static QPointF segm_f_bold[8] = {
	QPointF( 15.0,  5.0 ) , QPointF( 24.0, 20.0 ) , QPointF( 24.0, 42.5 ) , QPointF( 15.0, 50.0 ) ,
	QPointF( 12.0, 50.0 ) , QPointF(  6.0, 42.5 ) , QPointF(  6.0, 12.5 ) , QPointF( 12.0,  5.0 )
} ;

static QPointF segm_g[6] = {
	QPointF( 18.0, 45.0 ) , QPointF( 60.0, 45.0 ) , QPointF( 66.0, 50.0 ) , QPointF( 60.0, 55.0 ) ,
	QPointF( 18.0, 55.0 ) , QPointF( 12.0, 50.0 )
} ;
static QPointF segm_g_bold[6] = {
	QPointF( 24.0, 42.5 ) , QPointF( 54.0, 42.5 ) , QPointF( 63.0, 50.0 ) , QPointF( 54.0, 57.5 ) ,
	QPointF( 24.0, 57.5 ) , QPointF( 15.0, 50.0 )
} ;

static QPointF segm_p[8] = {
	QPointF( 78.0, 85.0 ) , QPointF( 84.0, 85.0 ) , QPointF( 87.0, 87.5 ) , QPointF( 87.0, 92.5 ) ,
	QPointF( 84.0, 95.0 ) , QPointF( 78.0, 95.0 ) , QPointF( 75.0, 92.5 ) , QPointF( 75.0, 87.5 )
} ;
static QPointF segm_p_bold[8] = {
	QPointF( 78.0, 80.0 ) , QPointF( 87.0, 80.0 ) , QPointF( 90.0, 85.0 ) , QPointF( 90.0, 90.0 ) ,
	QPointF( 87.0, 95.0 ) , QPointF( 78.0, 95.0 ) , QPointF( 75.0, 90.0 ) , QPointF( 75.0, 85.0 )
} ;

static QPointF segm_q[8] = {
	QPointF( 78.0, 45.0 ) , QPointF( 84.0, 45.0 ) , QPointF( 87.0, 47.5 ) , QPointF( 87.0, 52.5 ) ,
	QPointF( 84.0, 55.0 ) , QPointF( 78.0, 55.0 ) , QPointF( 75.0, 52.5 ) , QPointF( 75.0, 47.5 )
} ;
static QPointF segm_q_bold[8] = {
	QPointF( 78.0, 42.5 ) , QPointF( 87.0, 42.5 ) , QPointF( 90.0, 47.5 ) , QPointF( 90.0, 52.5 ) ,
	QPointF( 87.0, 57.5 ) , QPointF( 78.0, 57.5 ) , QPointF( 75.0, 52.5 ) , QPointF( 75.0, 47.5 )
} ;

// table de correspondance polygones / segments

typedef struct {
	QPointF*	points ;
	int			numPoints ;
	char		idSegment ;
} TSegment ;

static TSegment	tSegment[ 9 * 2 ] = {
	{ segm_a, 4, 'a' } ,
	{ segm_b, 8, 'b' } ,
	{ segm_c, 8, 'c' } ,
	{ segm_d, 4, 'd' } ,
	{ segm_e, 8, 'e' } ,
	{ segm_f, 8, 'f' } ,
	{ segm_g, 6, 'g' } ,
	{ segm_p, 8, 'p' } ,
	{ segm_q, 8, 'q' } ,
	{ segm_a_bold, 4, 'a' } ,
	{ segm_b_bold, 8, 'b' } ,
	{ segm_c_bold, 8, 'c' } ,
	{ segm_d_bold, 4, 'd' } ,
	{ segm_e_bold, 8, 'e' } ,
	{ segm_f_bold, 8, 'f' } ,
	{ segm_g_bold, 6, 'g' } ,
	{ segm_p_bold, 8, 'p' } ,
	{ segm_q_bold, 8, 'q' }
} ;

#define TSEGMENT(i)	tSegment[ isBold() ? (i) + 9 : (i) ]

// segments à allumer pour 0..9,A..F

static QString	sval[16] = {
	"abcdef" ,	// 0
	"bc" ,		// 1
	"abdeg" , 	// 2
	"abcdg" ,	// 3
	"bcfg" ,	// 4
	"acdfg" ,	// 5
	"acdefg" ,	// 6
	"abc" ,		// 7
	"abcdefg" ,	// 8
	"abcdfg" ,	// 9
	"abcefg" ,	// A
	"cdefg", 	// B
	"adef" ,	// C
	"bcdeg" ,	// D
	"adefg" ,	// E
	"aefg"	 	// F
//	,""			// -1
} ;

QamSegmentDisplay::QamSegmentDisplay( QWidget* parent )
	: QamAbstractDigital( parent )
	, m_value( 8 )
	, m_dp( true )
	, m_minus( false )
	, m_colon( false )
	, m_bold( false )
	, m_wheel( false )
	, m_custom( false )
	, m_segms( "cdegpq" )
{
	setState( QamWidget::Custom ) ;
}

// valeur 0..15 --> affichage 0..9,A..F

int QamSegmentDisplay::value() const { return m_value ; }

void QamSegmentDisplay::setValue(int value )					// slot
{
/*
	if ( value == -1 ) {	// new v5.1
		m_segms = "" ;
		m_custom = true ;
		update() ;
		return ;
	}
*/
	if ( value < 0 )	value = 0 ;
	m_value = value % 16 ;
	update() ;
}

// caractère '-'

bool QamSegmentDisplay::isMinus() const { return m_minus; }

void QamSegmentDisplay::setMinus(bool minus ) 
{
	m_minus = minus ; 
	update() ;
}

// point décimal complémentaire (révoque le signe ':')

bool QamSegmentDisplay::asDecimalPoint() const { return m_dp ; }

void QamSegmentDisplay::setDecimalPoint(bool dp ) 
{
	m_dp = dp ; 
	if ( m_dp && asColon() )	setColon( false ) ;
	else	update() ;
}

// signe complémentaire ':' (révoque le point décimal)

bool QamSegmentDisplay::asColon() const { return m_colon ; }

void QamSegmentDisplay::setColon(bool colon )
{
	m_colon = colon ;
	if ( m_colon && asDecimalPoint() )	setDecimalPoint( false ) ;
	else update() ;
}

// effet de graissage

bool QamSegmentDisplay::isBold() const { return m_bold ; }

void QamSegmentDisplay::setBold(bool bold )
{
	m_bold = bold ;
	update() ;
}

// valeur modifiable par molette

bool QamSegmentDisplay::asWheelControl() const { return m_wheel ; }

void QamSegmentDisplay::setWheelControl(bool wheelCtrl ) 
{
	m_wheel = wheelCtrl ;
}

// dessin du fond de l'indicateur (pixmap maintenu par classe de base)

void QamSegmentDisplay::drawBackground(QPainter& painter ) 
{
	painter.setRenderHint(QPainter::Antialiasing ) ;
//	painter.fillRect(rect(), QBrush( palette().background() ) ) ;
    painter.fillRect(rect(), QBrush( palette().color(QPalette::Base) ) ) ;

	// nothing to do...
}

// dessin de l'effet éteint/allumé du widget
// les coord. des segments sont calculés sur une grille 90 x 100

void QamSegmentDisplay::drawForeground(QPainter& painter )
{
	//
	painter.save() ;
	//
	painter.eraseRect( rect() ) ;

	// couleur du fond

//	QColor	bcolor = QBrush( palette().background() ).color() ;
    QColor	bcolor = QBrush( palette().color(QPalette::Base) ).color() ;

	// couleur de remplissage

	QBrush	brushBack( offColor() ) ;
	QBrush	brushFront ;

	// dégradé mode 'shining'
	
	QLinearGradient	lgradientFront( 0, 0, 90, 100 ) ;
	lgradientFront.setColorAt(0, Qt::white ) ;
	lgradientFront.setColorAt(0.75, onColor() ) ;

	if ( isShining() ) 	brushFront = lgradientFront ;
	else				brushFront = onColor() ;

	// mise à l'échelle

	QTransform	matrix ;
	matrix.scale( width() / 90.0, height() / 100.0 ) ;
	painter.setTransform( matrix ) ;
	
	// dessin de l'afficheur

	painter.setPen( bcolor ) ;

	if ( !isOn() ) {
		painter.setBrush( brushBack ) ;
		for ( int i = 0 ; i < 9 ; ++i ) {
			painter.drawPolygon( TSEGMENT(i).points, TSEGMENT(i).numPoints )  ;
		}
		painter.restore() ;
		return ;
	}

	if ( m_custom ) {
		for ( int i = 0 ; i < 9 ; ++i ) {
			if ( m_segms.contains( TSEGMENT(i).idSegment ) )
					painter.setBrush( brushFront ) ;
			else	painter.setBrush( brushBack ) ;
			painter.drawPolygon( TSEGMENT(i).points, TSEGMENT(i).numPoints )  ;
		}
		painter.restore() ;
		return ;
	}
	
	if ( m_minus ) {
		painter.setBrush( brushBack ) ;
		for ( int i = 0 ; i < 9 ; ++i ) {
			painter.drawPolygon( TSEGMENT(i).points, TSEGMENT(i).numPoints )  ;
		}
		painter.setBrush( brushFront ) ;
		painter.drawPolygon( TSEGMENT(6).points, TSEGMENT(6).numPoints )  ;
		painter.restore() ;
		return ;
	}

	for ( int i = 0 ; i < 7 ; ++i ) {
		if ( sval[m_value].contains( TSEGMENT(i).idSegment ) )
				painter.setBrush( brushFront ) ;
		else	painter.setBrush( brushBack ) ;
		painter.drawPolygon( TSEGMENT(i).points, TSEGMENT(i).numPoints )  ;
	}

	if (( m_dp )||( m_colon ))  painter.setBrush( brushFront ) ;
	else						painter.setBrush( brushBack ) ;
	painter.drawPolygon( TSEGMENT(7).points, TSEGMENT(7).numPoints )  ;

	if ( m_colon ) 	painter.setBrush( brushFront ) ;
	else			painter.setBrush( brushBack ) ;
	painter.drawPolygon( TSEGMENT(8).points, TSEGMENT(8).numPoints )  ;
	//
	painter.restore() ;
	//
}

// modification de valeur via la molette de la souris

void QamSegmentDisplay::wheelEvent(QWheelEvent* we )
{
	if ( !asWheelControl() )	return ;
	
//	int numSteps = we->delta() / 120 ;
    int numSteps = we->angleDelta().y() / 120 ;     // Qt 6.2.2
    setValue( value() + numSteps ) ;
	we->accept() ;
}

// affichage personnalisé avec segments = fragments de "abcdefgpq"

void QamSegmentDisplay::setCustomDisplay(bool custom, const QString& segments )
{
	m_custom = custom ;
//	if ( !segments.isEmpty() ) m_segms = segments ;
	m_segms = segments ;
}

