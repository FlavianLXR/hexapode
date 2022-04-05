/*  ---------------------------------------------------------------------------
 *  filename    :   glamspring.cpp
 *  description :   IMPLEMENTATION de la classe GLamSpring
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

#include "glamspring.h"

// constructeur par défaut

GLamSpring::GLamSpring()
	: GLamCurve()
	, m_iRadius( 0.5 )
	, m_wRadius( 0.05 )
	, m_rLength( 1.0 )
	, m_spins( 5 )
	, m_slices( 30 )
{
	setClassName("GLamSpring" ) ;
	setLength( m_rLength ) ;
}

// constructeur

GLamSpring::GLamSpring(qreal internalDiameter, qreal wireDiameter, qreal restingLength, int spins, int numSlices )
	: GLamCurve()
	, m_iRadius( internalDiameter / 2.0 )
	, m_wRadius( wireDiameter / 2.0 )
	, m_rLength( restingLength )
	, m_spins( spins )
	, m_slices( numSlices )
{
	setClassName("GLamSpring" ) ;
	m_spins = qMax(m_spins, 3 ) ;
	setLength( m_rLength ) ;
}

// constructeur de copie

GLamSpring::GLamSpring(const GLamSpring& obj )
	: GLamCurve()
{
	*this = obj ;
}

//destructeur

GLamSpring::~GLamSpring()
{
}

// surcharge de l'opérateur d'affectation

GLamSpring& GLamSpring::operator =(const GLamSpring& obj )
{
	*(GLamCurve*)this = obj ;

	m_iRadius = obj.m_iRadius ;
	m_wRadius = obj.m_wRadius ;
	m_rLength = obj.m_rLength ;
	m_spins = obj.m_spins ;
	m_slices = obj.m_slices ;
	m_length = obj.m_length ;

	updateNeeded() ;
	return *this ;
}

// longueur courante

qreal GLamSpring::length() const
{
	return m_length ;
}

// longueur au repos (sans compression)

qreal GLamSpring::restingLength() const
{
	return m_rLength ;
}

// longueur minimale (compression maximale)

qreal GLamSpring::minLength() const
{
	return ( m_spins + 1 ) * m_wRadius * 2 ;
}

// modification de la compression du ressort
// sans effet et retourne faux si la longueur demandée est impossible

bool GLamSpring::setLength(qreal length )
{
	if ( length > m_rLength )	return false ;
	if ( length < minLength() )	return false ;

	m_length = length ;
	m_a0 = 0.0 ;
	m_z0 = m_wRadius ;

	QList<QVector3D> curve ;

	// 1er point de construction
	curve << QVector3D(m_iRadius + m_wRadius, 0, m_z0 ) ;
	// tour d'appui bas
	curve << arcShape(270, 0 ) ;
	curve << arcShape( 90, 2 * m_wRadius ) ;
	// spires
	for ( int i = 0 ; i < m_spins - 2 ; ++i ) {
		curve << arcShape(360, ( m_length - 6 * m_wRadius ) / ( m_spins - 2 ) ) ;
	}
	// tour d'appui haut
	curve << arcShape( 90, 2 * m_wRadius ) ;
	curve << arcShape(270, 0 ) ;

	// génération de l'objet
	setCurve( curve ) ;
	setShape(GLam2D::circularShape(m_wRadius, m_slices ) ) ;

	return true ;
}

// points de construction pour une rotation angle / élévation dz
// note: le point d'origine n'est pas généré pour éviter les doublons

QList<QVector3D> GLamSpring::arcShape(qreal angle, qreal dz )
{
	QList<QVector3D> curve ;
	qreal radius = m_iRadius + m_wRadius ;
	int   slices = angle / 360.0 * m_slices ;

	for ( int i = 1 ; i <= slices ; ++i ) {
		m_a0 += angle / slices ;
		m_z0 += dz / slices ;
		qreal a = qDegreesToRadians( m_a0 ) ;
		curve << QVector3D( radius * COS(a), radius * SIN(a), m_z0 ) ;
	}
	return curve ;
}
