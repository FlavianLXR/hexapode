/*  ---------------------------------------------------------------------------
 *  filename    :   glamvector.cpp
 *  description :   IMPLEMENTATION de la classe GLamVector
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

#include "glamvector.h"

// flèche définie par son vecteur à l'origine
// la direction est maintenue par GLamObject::defaultMatrix()

/*! Constructeur par défaut.
 *  Fabrique une flèche de diamètre 0.05 et de longueur 1, dirigée suivant 0Z+.
 *  La direction du vecteur est maintenue par GLamObject::defaultMatrix().
 */

GLamVector::GLamVector()
	: GLamRevolution()
	, m_vector( 0.0f, 0.0f, 1.0f )
	, m_diameter( 0.05f )
	, m_outLine( 0.03f )
{
	setClassName("GLamVector") ;
	setDirection() ;
	setHalfProfile() ;
}

/*! Constructeur.
 *  Fabrique une flèche de diamètre 0.05 dont la longueur et la direction est
 *  définie par le vecteur @c vector.
 *  La direction du vecteur est maintenue par GLamObject::defaultMatrix().
 */

GLamVector::GLamVector(QVector3D vector )
	: GLamRevolution()
	, m_vector( vector )
	, m_diameter( 0.05f )
	, m_outLine( 0.03f )
{
	setClassName("GLamVector") ;
	setDirection() ;
	setHalfProfile() ;
}

/*! Constructeur de copie. */

GLamVector::GLamVector(const GLamVector& obj )
	: GLamRevolution()
{
	*this = obj ;
}

/*! Destructeur. */

GLamVector::~GLamVector()
{
}

/*! Surcharge de l'opérateur d'affectation. */

GLamVector& GLamVector::operator =(const GLamVector& obj )
{
	*(GLamObject*)this = obj ;
	m_vector = obj.m_vector ;
	m_diameter = obj.m_diameter ;
	m_outLine = obj.m_outLine ;
	setHalfProfile() ;
	return *this ;
}

// diamètre du corps de la flèche

qreal GLamVector::diameter() const { return m_diameter ; }

void GLamVector::setDiameter(qreal diameter )
{
	m_diameter = qAbs( diameter ) ;
	setHalfProfile() ;
}

// débordement p/r à l'origine (peut ètre négatif)

qreal GLamVector::outLine() const { return m_outLine ; }

void GLamVector::setOutLine(qreal outLine )
{
	m_outLine = outLine ;
	setHalfProfile() ;
}

// [private] calcul de la matrice de rotation du vecteur

void GLamVector::setDirection()
{
	QMatrix4x4	m ;
	m.setToIdentity() ;

	// angle de rotation 3D de centre O de A vers B avec /OA = /OB
	// cos(phi) = A.B / ( ||A|| * ||B|| )
	qreal len = m_vector.length() ;
	// l'axe de construction GLamRevolution est OZ -> A = (0,0,len), B = m_vector
	// ==> cos(phi) = len * m_vector.z() / ( len * len ) ;
	qreal phi = qAcos( m_vector.z() / len ) ;
	// axe de rotation = normale au plan OBA
	QVector3D norm = QVector3D::normal(QVector3D(0,0,0), m_vector, QVector3D(0,0,len) ) ;

	m.rotate( qRadiansToDegrees(phi), -norm ) ;

//	setDefaultMatrix( m ) ;
	defaultMatrix() = m ;
}

// [private] définition des points de construction

void GLamVector::setHalfProfile()
{
	QList<QPointF> points ;
	points.clear() ;

	qreal len = m_vector.length() ;
	qreal radius = m_diameter / 2.0f ;

	points << QPointF( 0.0f,   -m_outLine ) ;
	points << QPointF( radius, -m_outLine ) ;
	points << QPointF( radius,     len - 5 * m_diameter ) ;
	points << QPointF( m_diameter, len - 5 * m_diameter ) ;
	points << QPointF( 0.0f, len ) ;

	setPoints(points ) ;
}
