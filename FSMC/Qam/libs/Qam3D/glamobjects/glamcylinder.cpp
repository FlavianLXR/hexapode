/*  ---------------------------------------------------------------------------
 *  filename    :   glamcylinder.cpp
 *  description :   IMPLEMENTATION de la classe GLamCylinder
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

#include "glamcylinder.h"

/*! Constructeur par défaut.
 *  Fabrique un cylindre de rayon 0.5 et de longueur 1, en mode 'fil de fer'.
 */

GLamCylinder::GLamCylinder()
	: GLamRevolution()
	, m_radius( 0.5f )
	, m_length( 1.0f )
{
	setClassName("GLamCylinder") ;
	setWireFrame() ;
	setHalfProfile() ;
}

/*! Constructeur d'un cylindre de rayon @a radius et de longueur @a length. */

GLamCylinder::GLamCylinder(qreal radius, qreal length )
	: GLamRevolution()
	, m_radius( qAbs(radius) )
	, m_length( qAbs(length) )
{
	setClassName("GLamCylinder") ;
	setHalfProfile() ;
}

/*! Constructeur de copie. */

GLamCylinder::GLamCylinder(const GLamCylinder& obj )
	: GLamRevolution()
{
	*this = obj ;
}

/*! Destructeur. */

GLamCylinder::~GLamCylinder()
{
}

/*! Surcharge de l'opérateur d'affectation. */

GLamCylinder& GLamCylinder::operator =(const GLamCylinder& obj )
{
	*(GLamRevolution*)this = obj ;
	m_radius = obj.m_radius ;
	m_length = obj.m_length ;
	setHalfProfile() ;
	return *this ;
}

// propriété 'radius'

qreal GLamCylinder::radius() const { return m_radius ; }

void GLamCylinder::setRadius(qreal radius )
{
	m_radius = qAbs(radius) ;
	setHalfProfile() ;
}

// propriété 'length'

qreal GLamCylinder::length() const { return m_length ; }

void GLamCylinder::setLength(qreal length )
{
	m_length = qAbs(length) ;
	setHalfProfile() ;
}

/*! Méthode permettant de modifier simultanément rayon et longueur. */

void GLamCylinder::resize(qreal radius, qreal length )
{
	m_radius = qAbs(radius) ;
	m_length = qAbs(length) ;
	setHalfProfile() ;
}

// [private] détermination du demi-profil longitudinal

void GLamCylinder::setHalfProfile()
{
	QList<QPointF>	points ;
	points.clear() ;
	points << QPointF( 0.0f, 0.0f ) ;
	points << QPointF(m_radius, 0.0f ) ;
	points << QPointF(m_radius, m_length ) ;
	points << QPointF(0.0f, m_length ) ;
	setPoints( points ) ;
}
