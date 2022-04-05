/*  ---------------------------------------------------------------------------
 *  filename    :   glamwire.cpp
 *  description :   IMPLEMENTATION de la classe GLamWire
 *
 *	project     :	Qam3D/GLam: 3D object collection
 *  start date  :   décembre 2009
 *  ---------------------------------------------------------------------------
 *  Copyright 2009-2020 by Alain Menu   <alain.menu@ac-creteil.fr>
 *
 *  This file is part of "Qam3D/GLam Qt Library"
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

#include "glamwire.h"

#define	V3D(x)	QVector3D(x, x, x )

// constructeur par défaut

GLamWire::GLamWire()
	: GLamCurve()
	, m_radius( 0.1 )
	, m_stripe( 0 )
{
	setClassName("GLamWire") ;
}

// constructeur 'Bézier'

GLamWire::GLamWire(const BezierCurve& bezierCurve, qreal diameter, int numSlices )
	: GLamCurve()
	, m_radius( diameter / 2.0 )
	, m_stripe( 0 )
{
	setClassName("GLamWire") ;

	QVector3D p0 = bezierCurve.startPoint ;
	QVector3D p1 = bezierCurve.startPoint + bezierCurve.startVector ;
	QVector3D p2 = bezierCurve.endPoint + bezierCurve.endVector ;
	QVector3D p3 = bezierCurve.endPoint ;

	m_buildPoints << p0 << p1 << p2 << p3 ;

	QList<QVector3D> curve ;

	for ( int i = 0 ; i < numSlices ; i++ ) {
		float t = i / (float)( numSlices - 1 ) ;
		float a = ( 1 - t ) * ( 1 - t ) * ( 1 - t ) ;
		float b = 3 * t * ( 1 - t ) * ( 1 - t ) ;
		float c = 3 * t * t * ( 1 - t ) ;
		float d = t * t * t ;
		QVector3D v ;
		v.setX( p0.x() * a + p1.x() * b + p2.x() * c + p3.x() * d ) ;
		v.setY( p0.y() * a + p1.y() * b + p2.y() * c + p3.y() * d ) ;
		v.setZ( p0.z() * a + p1.z() * b + p2.z() * c + p3.z() * d ) ;
		curve << v ;
	}
	setCurve( curve ) ;
	setShape( GLam2D::circularShape(m_radius, 60 ) ) ;
}

// constructeur 'B-Spline'

GLamWire::GLamWire(QList<QVector3D> splineCurve, qreal diameter, int numSlices )
	: GLamCurve()
	, m_radius( diameter / 2.0 )
	, m_stripe( 0 )
{
	setClassName("GLamWire") ;

	m_buildPoints = splineCurve ;

	// nombre de points de contrôle
	int numPtsCtrl = splineCurve.size() ;

	// correction nombre de segments
	while( numSlices % numPtsCtrl )	++numSlices ;

	// duplication des points extrèmes
	splineCurve.prepend( splineCurve.first() ) ;
	splineCurve.append( splineCurve.last() ) ;
	numPtsCtrl += 2 ;

	// enveloppe B-Spline de degré 2
	QList<QVector3D> curve ;
	int i = 0 ;
	int m = 2 ;
	int resol = numSlices / numPtsCtrl ;

	while ( m < numPtsCtrl ) {
		for ( int j = 0 ; j < resol ; j++ ) {
			float t = j / (float)( resol - 1 ) ;
			float a = 1/2.0 - t + ( t * t ) / 2.0 ;
			float b = 1/2.0 + t - t * t ;
			float c = ( t * t ) / 2.0 ;
			QVector3D v ;
			v.setX( a * splineCurve.at( m - 2 ).x() + b * splineCurve.at( m - 1 ).x() + c * splineCurve.at( m ).x() ) ;
			v.setY( a * splineCurve.at( m - 2 ).y() + b * splineCurve.at( m - 1 ).y() + c * splineCurve.at( m ).y() ) ;
			v.setZ( a * splineCurve.at( m - 2 ).z() + b * splineCurve.at( m - 1 ).z() + c * splineCurve.at( m ).z() ) ;
			curve << v ;
			++i ;
			if ( i == numSlices )	{ j = resol ; m = numPtsCtrl + 2 ; break ; }
		}
		++m ;
	}
	setCurve( curve ) ;
	setShape( GLam2D::circularShape(m_radius, 60 ) ) ;
}

// constructeur de copie

GLamWire::GLamWire(const GLamWire& obj )
	: GLamCurve()
{
	*this = obj ;
}

// destructeur

GLamWire::~GLamWire()
{
	if ( m_stripe )	delete m_stripe ;
}

// surcharge opérateur d'affectation

GLamWire& GLamWire::operator =(const GLamWire& obj )
{
	*(GLamCurve*)this = obj ;
	m_radius = obj.m_radius ;
	m_buildPoints = obj.m_buildPoints ;
	m_stripe = obj.m_stripe ;
	updateNeeded() ;
	return *this ;
}

// enveloppe et âme de la courbe en mode esquisse

void GLamWire::create()
{
	if ( isSketched() ) {
		// points de construction maintenus localement
		for ( int i = 1 ; i < m_buildPoints.size() ; ++i ) {
			quad(	m_buildPoints.at(i),
					m_buildPoints.at(i-1),
					m_buildPoints.at(i-1) + V3D(1e-3) ,
					m_buildPoints.at(i) + V3D(1e-3) ) ;
		}
		// âme de la courbe maintenue par classe de base
		for ( int i = 1 ; i < curve().size() ; ++i ) {
			quad(	curve().at(i),
					curve().at(i-1),
					curve().at(i-1) + V3D(1e-3) ,
					curve().at(i) + V3D(1e-3) ) ;
		}
		return ;
	}
	GLamCurve::create() ;
}

// cherry on the cake: option deux bandes colorées

void GLamWire::setColoredStripe(QColor color )
{
	QList<QPointF>	shape ;
	shape << GLam2D::arcShape(m_radius + 0.001, -20,  20, 6 ) ;
	shape << GLam2D::arcShape(m_radius + 0.001, 160, 200, 6 ) ;
	QList<QVector3D> curve = this->curve() ;
	curve.removeFirst() ;
	curve.removeLast() ;
	m_stripe = new GLamCurve(curve, shape ) ;
	m_stripe->material().setColor( color ) ;
}

// surcharge de la primitive de dessin pour ajout des bandes colorées

void GLamWire::draw(GLamObject* parent )
{
	if (( m_stripe )&&( !isSketched() )) {
		m_stripe->defaultMatrix() = this->defaultMatrix() ;
		m_stripe->draw( parent ) ;
	}
	GLamCurve::draw( parent ) ;
}
