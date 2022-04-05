/*  ---------------------------------------------------------------------------
 *  filename    :   glamhole.cpp
 *  description :   IMPLEMENTATION de la classe GLamHole
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

#include "glamhole.h"

GLamHole::GLamHole()
	: GLamObject()
	, m_profile( 0 )
	, m_hasDefaultProfile( true )
	, m_thickness( 1.0f )
{
	setClassName("GLamHole") ;
	m_shape = GLam2D::circularShape(0.1f, 4 ) ;
	setWireFrame() ;
}

GLamHole::GLamHole(const QList<QPointF>& shape, qreal thickness )
	: GLamObject()
	, m_shape( shape )
	, m_profile( 0 )
	, m_hasDefaultProfile( true )
	, m_thickness( thickness )
{
	setClassName("GLamHole") ;
}

GLamHole::GLamHole(const GLamHole& obj )
	: GLamObject()
	, m_profile( 0 )
{
	*this = obj ;
}

GLamHole::~GLamHole()
{
	if ( m_profile ) delete m_profile ;
}


GLamHole& GLamHole::operator =(const GLamHole& obj )
{
	*(GLamObject*)this = obj ;
	m_shape = obj.m_shape ;
	if ( m_profile ) delete m_profile ;

	if ( obj.m_profile ) {
		m_profile = new GLamProfile( *(obj.m_profile) ) ;
	}
	else m_profile = 0 ;

	m_hasDefaultProfile = obj.m_hasDefaultProfile ;
	m_thickness = obj.m_thickness ;
	updateNeeded() ;
	return *this ;
}

// profil intérieur

void GLamHole::setProfile(const GLamProfile* profile )
{
	if ( m_profile ) delete m_profile ;
	m_profile = new GLamProfile( *profile ) ;
	m_hasDefaultProfile = false ;
	m_profile->setClosed( false ) ;
	if ( m_shape.last() == m_shape.first() )	m_shape.removeLast() ;

    m_profile->extrudeInside( m_shape ) ;
}

// épaisseur 0Z (supplantée par m_profile le cas échéant)

void GLamHole::setThickness(qreal thickness )
{
	m_thickness = qAbs(thickness) ;
	updateNeeded() ;
}

// détermination des vertices et normales
// (un GLamHole peut être projeté sur la scène directement...)

void GLamHole::create()
{
	// contour droit
	if ( m_hasDefaultProfile ) {
		int n = m_shape.size() ;
		for ( int i = 0 ; i < n ; ++i ) {
			zExtrude(
				QVector3D( m_shape.at(i).x(), m_shape.at(i).y(), 0.0f ),
				QVector3D( m_shape.at((i+1)%n).x(), m_shape.at((i+1)%n).y(), m_thickness )
			) ;
		}
	}
	// ou profilé
	else {
		if ( m_profile ) {
			m_profile->update() ;
			for ( int i = 2 ; i < m_profile->numVertexIndices() ; i += 3 ) {
				QVector3D v1 = m_profile->vertex(i-2) ;
				QVector3D v2 = m_profile->vertex(i-1) ;
				QVector3D v3 = m_profile->vertex(i) ;
				triangle(v1, v2, v3 ) ;
			}
		}
	}
}
