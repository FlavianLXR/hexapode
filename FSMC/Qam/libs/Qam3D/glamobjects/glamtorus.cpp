/*  ---------------------------------------------------------------------------
 *  filename    :   glamtorus.cpp
 *  description :   IMPLEMENTATION de la classe GLamTorus
 *
 *	project     :	Qam3D/GLam: 3D object collection
 *  start date  :   décembre 2009
 *  ---------------------------------------------------------------------------
 *  Copyright 2009-2016 by Alain Menu   <alain.menu@ac-creteil.fr>
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

#include "glamtorus.h"

/*! Constructeur par défaut.
 *  Fabrique un tore de diamètre nominal 1 et de section circulaire de
 *  diamètre 0.1, en mode 'fil de fer'.
 */

GLamTorus::GLamTorus()
	: GLamRevolution()
	, m_diameter( 1.0f )
	, m_thickness( 0.1f )
	, m_height( 0.0f )
{
	setClassName("GLamTorus") ;
	setWireFrame() ;
	setHalfProfile() ;
}

/*! Constructeur d'un tore de diamètre nominal @a diameter et de section
 *  circulaire de diamètre @a thickness.
 */

GLamTorus::GLamTorus(qreal diameter, qreal thickness )
	: GLamRevolution()
	, m_diameter( qAbs(diameter) )
	, m_thickness( qAbs(thickness) )
	, m_height( 0.0f )
{
	setClassName("GLamTorus") ;
	setHalfProfile() ;
}

/*! Constructeur d'un tore de diamètre nominal @a diameter et de section
 *  ellitique de dimensions @a size.
 */

GLamTorus::GLamTorus(qreal diameter, QSizeF size )
	: GLamRevolution()
	, m_diameter( qAbs(diameter) )
	, m_thickness( qAbs(size.width()) )
	, m_height( qAbs(size.height()) )
{
	setClassName("GLamTorus") ;
	setHalfProfile() ;
}

/*! Constructeur de copie. */

GLamTorus::GLamTorus(const GLamTorus& obj )
	: GLamRevolution()
{
	*this = obj ;
}

/*! Destructeur. */

GLamTorus::~GLamTorus()
{
}

/*! Surcharge de l'opérateur d'affectation. */

GLamTorus& GLamTorus::operator =(const GLamTorus& obj )
{
	*(GLamRevolution*)this = obj ;
	m_diameter = obj.m_diameter ;
	m_thickness = obj.m_thickness ;
	m_height = obj.m_height ;
	setHalfProfile() ;
	return *this ;
}

// diamètre nominal

qreal GLamTorus::diameter() const { return m_diameter ; }

void GLamTorus::setDiameter(qreal diameter )
{
	m_diameter = qAbs( diameter ) ;
	setHalfProfile() ;
}

// diamètre de la section

qreal GLamTorus::thickness() const { return m_thickness ; }

void GLamTorus::setThickness(qreal thickness )
{
	m_thickness = qAbs( thickness ) ;
	setHalfProfile() ;
}

// rectangle d'inscription de la section elliptique

QSizeF GLamTorus::size() const { return QSizeF(m_thickness, m_height ) ; }

void GLamTorus::setSize(QSizeF size )
{
	m_thickness = qAbs( size.width() ) ;
	m_height = qAbs( size.height() ) ;
	setHalfProfile() ;
}

// nombre de tranches (facettes) suivant 0Z

void GLamTorus::setNumSlices(uint numSlices )
{
	GLamRevolution::setNumSlices( numSlices ) ;
	setHalfProfile() ;
}

// nombre de tranches (0Z) et secteurs (RZ)

void GLamTorus::setNumFaces(uint numFaces )
{
	GLamRevolution::setNumFaces( numFaces ) ;
	setHalfProfile() ;
}

// [private] détermination du demi-profil longitudinal

void GLamTorus::setHalfProfile()
{
	qreal nr = m_diameter / 2 ;
	qreal sr = m_thickness / 2 ;

	QList<QPointF>	points ;

	if ( m_height > 0.0f )
			points = GLam2D::ellipticShape(size(), numSlices() * 2 ) ;
	else	points = GLam2D::circularShape(sr, numSlices() * 2 ) ;

	points = GLam2D::translate(points, nr, 0 ) ;

	setPoints(points ) ;
}
