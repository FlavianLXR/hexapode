/*  ---------------------------------------------------------------------------
 *  filename    :   glamsphere.cpp
 *  description :   IMPLEMENTATION de la classe GLamSphere
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

#include "glamsphere.h"

/*! Constructeur par défaut. Fabrique une sphère pleine de diamètre 1
 *  en mode 'fil de fer'.
 */

GLamSphere::GLamSphere()
	: GLamRevolution()
	, m_diameter( 1.0f )
	, m_height( 1.0f )
	, m_thickness( 0.0f )
	, m_southAngle( 90.0f )
	, m_northAngle( 90.0f )
{
	setClassName("GLamSphere") ;
	setWireFrame() ;
	setHalfProfile() ;
}

/*! Constructeur de sphère pleine ou creuse. */

GLamSphere::GLamSphere(qreal diameter, qreal thickness )
	: GLamRevolution()
	, m_diameter( qAbs(diameter) )
	, m_height( qAbs(diameter) )
	, m_thickness( qAbs(thickness) )
	, m_southAngle( 90.0f )
	, m_northAngle( 90.0f )
{
	setClassName("GLamSphere") ;
	setHalfProfile() ;
}

/*! Constructeur d'ellipsoïde pleine ou creuse. */

GLamSphere::GLamSphere(QSizeF size, qreal thickness )
	: GLamRevolution()
	, m_diameter( qAbs(size.width()) )
	, m_height( qAbs(size.height()) )
	, m_thickness( qAbs(thickness) )
	, m_southAngle( 90.0f )
	, m_northAngle( 90.0f )
{
	setClassName("GLamSphere") ;
	setHalfProfile() ;
}

/*! Constructeur de copie. */

GLamSphere::GLamSphere(const GLamSphere& obj )
	: GLamRevolution()
{
	*this = obj ;
}
/*! Destructeur. */

GLamSphere::~GLamSphere()
{
}

/*! Surcharge de l'opérateur d'affectation. */

GLamSphere& GLamSphere::operator =(const GLamSphere& obj )
{
	*(GLamRevolution*)this = obj ;
	m_diameter = obj.m_diameter ;
	m_height = obj.m_height ;
	m_thickness = obj.m_thickness ;
	m_southAngle = obj.m_southAngle ;
	m_northAngle = obj.m_northAngle ;
	setHalfProfile() ;
	return *this ;
}

// enveloppe externe : diameter = plan 0XY , height = plan 0XZ

qreal GLamSphere::diameter() const	{ return m_diameter ; }
qreal GLamSphere::height() const	{ return m_height ; }
QSizeF GLamSphere::size() const		{ return QSizeF(m_diameter, m_height ) ; }

void GLamSphere::setDiameter(qreal diameter )
{
	m_diameter = qAbs( diameter ) ;
	m_height = qAbs( diameter ) ;
	setHalfProfile() ;
}

void GLamSphere::setHeight(qreal height )
{
	m_height = qAbs( height ) ;
	setHalfProfile() ;
}

void GLamSphere::setSize(QSizeF size )
{
	m_diameter = qAbs( size.width() ) ;
	m_height = qAbs( size.height() ) ;
	setHalfProfile() ;
}

// latitudes 0..90° p/r 0X : south = RY- , north = RY+

qreal GLamSphere::southAngle() const	{ return m_southAngle ; }
qreal GLamSphere::northAngle() const	{ return m_northAngle ; }

void GLamSphere::setSouthAngle(qreal southAngle )
{
	m_southAngle = qMin( qMax(southAngle, 0.0 ), 90.0 ) ;
	setHalfProfile() ;
}

void GLamSphere::setNorthAngle(qreal northAngle )
{
	m_northAngle = qMin( qMax(northAngle, 0.0 ), 90.0 ) ;
	setHalfProfile() ;
}

/*! Méthode permettant d'affecter simultanément les latitudes Sud et Nord.*/

void GLamSphere::setLatitudes(qreal southAngle, qreal northAngle )
{
	m_southAngle = qMin( qMax(southAngle, 0.0 ), 90.0 ) ;
	m_northAngle = qMin( qMax(northAngle, 0.0 ), 90.0 ) ;
	setHalfProfile() ;
}

// épaisseur de matière (interne)

qreal GLamSphere::thickness() const	{ return m_thickness ; }

void GLamSphere::setThickness(qreal thickness )
{
	m_thickness = qAbs( thickness ) ;
	setHalfProfile() ;
}

// nombre de tranches (facettes) suivant 0Z

void GLamSphere::setNumSlices(uint numSlices )
{
	GLamRevolution::setNumSlices( numSlices ) ;
	setHalfProfile() ;
}

// nombre de tranches (0Z) et secteurs (RZ)

void GLamSphere::setNumFaces(uint numFaces )
{
	GLamRevolution::setNumFaces( numFaces ) ;
	setHalfProfile() ;
}

// [private] détermination du demi-profil longitudinal

void GLamSphere::setHalfProfile()
{
	qreal rw = m_diameter / 2 ;
	qreal rh = m_height / 2 ;
	qreal th = m_thickness ;
	qreal beginAngle = -qDegreesToRadians( m_southAngle ) ;
	qreal endAngle   =  qDegreesToRadians( m_northAngle ) ;

	QList<QPointF>	points ;

	points.clear() ;

	// arète extérieure
	qreal step = M_PI / numSlices() ;
	qreal a = beginAngle ;
	do {
		points << QPointF( rw * COS(a), rh * SIN(a) ) ;
		a += step ;
	} while ( a < endAngle ) ;
	a = endAngle ;
	points << QPointF( rw * COS(a), rh * SIN(a) ) ;

    // arète intérieur (sphère creuse)
	if (( th > 0.0f )&&( th < qMin(rw, rh ) )) {
		rw -= th ;
		rh -= th ;
		do {
			points << QPointF( rw * COS(a), rh * SIN(a) ) ;
			a -= step ;
		} while ( a > beginAngle ) ;
		a = beginAngle ;
		points << QPointF( rw * COS(a), rh * SIN(a) ) ;
	}
    else {
        // si sphère pleine avec latitude(s) partielles, convergence au centre
        if ( (m_northAngle + m_southAngle) < 180.0 ) points << QPointF() ;
    }

	setPoints(points ) ;
}
