/*  ---------------------------------------------------------------------------
 *  filename    :   glampipe.cpp
 *  description :   IMPLEMENTATION de la classe GLamPipe
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

#include "glampipe.h"

/*! Constructeur par défaut.
 *  Fabrique un tube droit de diamètre 1 et de longueur 1, avec une épaisseur
 *  de matière 0.1, en mode 'fil de fer'.
 */

GLamPipe::GLamPipe()
	: GLamRevolution()
	, m_baseRadius( 0.5f )
	, m_topRadius( 0.5f )
	, m_length( 1.0f )
	, m_thickness( 0.1f )
{
	setClassName("GLamPipe") ;
	setWireFrame() ;
	setHalfProfile() ;
}

/*! Constructeur d'un tube droit de diamètre @a diameter, d'épaisseur
 *  @a thickness et de longueur @a length. */

GLamPipe::GLamPipe(qreal diameter, qreal thickness, qreal length )
	: GLamRevolution()
	, m_baseRadius( qAbs(diameter / 2) )
	, m_topRadius( qAbs(diameter / 2) )
	, m_length( qAbs(length) )
	, m_thickness( qAbs(thickness) )
{
	setClassName("GLamPipe") ;
	setHalfProfile() ;
}

/*! Constructeur d'un tronc de cône de diamètre initial @a baseDiameter, de
 *  diamètre final @a topDiameter, d'épaisseur @a thickness et de longueur
 * @a length. */

GLamPipe::GLamPipe(qreal baseDiameter, qreal topDiameter, qreal thickness, qreal length )
	: GLamRevolution()
	, m_baseRadius( qAbs(baseDiameter / 2) )
	, m_topRadius( qAbs(topDiameter / 2) )
	, m_length( qAbs(length) )
	, m_thickness( qAbs(thickness) )
{
	setClassName("GLamPipe") ;
	setHalfProfile() ;
}

/*! Constructeur de copie. */

GLamPipe::GLamPipe(const GLamPipe& obj )
	: GLamRevolution()
{
	*this = obj ;
}

/*! Destructeur. */

GLamPipe::~GLamPipe()
{
}

/*! Surcharge de l'opérateur d'affectation. */

GLamPipe& GLamPipe::operator =(const GLamPipe& obj )
{
	*(GLamRevolution*)this = obj ;
	m_baseRadius = obj.m_baseRadius ;
	m_topRadius = obj.m_topRadius ;
	m_length = obj.m_length ;
	m_thickness = obj.m_thickness ;
	setHalfProfile() ;
	return *this ;
}

// diamètre à la base (Z=0)

qreal GLamPipe::baseDiameter() const { return m_baseRadius * 2 ; }

void GLamPipe::setBaseDiameter(qreal baseDiameter )
{
	m_baseRadius = qAbs(baseDiameter / 2) ;
	setHalfProfile() ;
}

// diamètre au sommet (Z+)

qreal GLamPipe::topDiameter() const	{ return m_topRadius * 2 ; }

void GLamPipe::setTopDiameter(qreal topDiameter )
{
	m_topRadius = qAbs(topDiameter / 2) ;
	setHalfProfile() ;
}

// longueur (0Z)

qreal GLamPipe::length() const { return m_length ; }

void GLamPipe::setLength(qreal length )
{
	m_length = qAbs(length) ;
	setHalfProfile() ;
}

// épaisseur de matière

qreal GLamPipe::thickness() const { return m_thickness ; }

void GLamPipe::setThickness(qreal thickness )
{
	m_thickness = qAbs(thickness) ;
	setHalfProfile() ;
}

// détermination du demi-profil longitudinal

void GLamPipe::setHalfProfile()
{
	qreal th = m_thickness ;
	qreal yc = 0.0f ;

	// section constante : correction épaisseur
	if ( m_baseRadius == m_topRadius )	th = qMin(m_thickness, m_baseRadius ) ;
	// sinon, projection arète intérieure sur axe 0Y
	else yc = m_length * (th - m_baseRadius) / (m_topRadius - m_baseRadius ) ;

	QList<QPointF>	points ;
	points.clear() ;

	// arète extérieure
	points << QPointF(m_baseRadius, 0.0f ) ;
	points << QPointF(m_topRadius, m_length ) ;
	// sommet bouché ou non
	if ( th > m_topRadius ) {
		points << QPointF(0.0f, m_length ) ;
		if ( yc < m_length )	points << QPointF(0.0f, yc ) ;
	}
	else {
		points << QPointF(m_topRadius - th, m_length ) ;
	}
	// base bouchée ou non
	if ( th > m_baseRadius ) {
		if ( yc > 0.0f )	points << QPointF(0.0f, yc ) ;
		points << QPointF(0.0f, 0.0f ) ;
	}
	else {
		points << QPointF(m_baseRadius - th, 0.0f ) ;
		points << QPointF(m_baseRadius, 0.0f ) ;
	}

	setPoints( points ) ;
}
