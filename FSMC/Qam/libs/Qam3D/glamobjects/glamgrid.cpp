/*  ---------------------------------------------------------------------------
 *  filename    :   glamgrid.cpp
 *  description :   IMPLEMENTATION de la classe GLamGrid
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

#include "glamgrid.h"

/*! Constructeur par défaut. */
GLamGrid::GLamGrid()
	: GLamObject()
	, m_xSize( 1 )
	, m_ySize( 1 )
	, m_step( 1.0f )
{
	setClassName("GLamGrid") ;
	setWireFrame() ;
}

/*! Constructeur. La grille construite est carrée si l'argument @a ySize
 * est omis.
 */
GLamGrid::GLamGrid(int xSize, int ySize )
	: GLamObject()
	, m_xSize( xSize )
	, m_ySize( ySize )
	, m_step( 1.0f )
{
	setClassName("GLamGrid") ;
	if ( m_ySize == 0 )	m_ySize = m_xSize ;
	setWireFrame() ;
}
/*! Constructeur de copie. */
GLamGrid::GLamGrid(const GLamGrid& obj )
	: GLamObject()
{
	*this = obj ;
}

GLamGrid::~GLamGrid()
{
}

/*! Opérateur d'affectation. */
GLamGrid& GLamGrid::operator =(const GLamGrid& obj )
{
	*(GLamObject*)this = obj ;

	m_xSize = obj.m_xSize ;
	m_ySize = obj.m_ySize ;
	m_step  = obj.m_step ;

	updateNeeded() ;

	return *this ;
}

// propriété 'size'

QSize GLamGrid::size() const
{
    return QSize( qAbs( m_xSize ), qAbs( m_ySize ) ) ;
}

void GLamGrid::setSize(int xSize, int ySize )
{
     m_xSize = xSize ;
     m_ySize = ( ySize == 0 ? m_xSize : ySize ) ;
     updateNeeded() ;
}

void GLamGrid::setSize(const QSize& size )
{
    setSize( size.width(), size.height() ) ;
}

// propriété 'step'

qreal GLamGrid::step() const { return m_step ; }


void GLamGrid::setStep(qreal step )
{
	m_step = qAbs( step ) ;
	updateNeeded() ;
}

/*! Concrétisation de la classe. */

void GLamGrid::create()
{
    float halfW = qAbs( m_xSize / 2.0f ) ;
    float halfH = qAbs( m_ySize / 2.0f ) ;

	this->setWireFrame( true ) ;	// v0.83 : mode WireFrame forcé...

	for ( float y = -halfH ; y < halfH ; y += m_step ) {
		for ( float x = -halfW ; x < halfW ; x += m_step ) {
			QVector3D v1( x + m_step, y + m_step, 0.0f ) ;
			QVector3D v2( x ,         y + m_step, 0.0f ) ;
			QVector3D v3( x ,         y,          0.0f ) ;
			QVector3D v4( x + m_step, y,          0.0f ) ;
			quad(v1, v2, v3, v4 ) ;
		}
	}
/*
	float e = 0.01f ;
	for ( float x = -halfW ; x <= halfW ; x += m_step ) {
		if ( isWireFrame() ) {
//			m_vertices << QVector3D(x,  halfH, 0.0f ) ;
//			m_vertices << QVector3D(x, -halfH, 0.0f ) ;
//			m_normals  << QVector3D(0.0f, 0.0f, 1.0f ) ;
//			m_normals  << QVector3D(0.0f, 0.0f, 1.0f ) ;
			// test réduction m_normals
//			m_normals_old  << QVector3D(0.0f, 0.0f, 1.0f ) ;
//			int ind = m_normals_old.size() - 1 ;
//			m_vertices_old << QVector4D(x,  halfH, 0.0f, ind ) ;
//			m_vertices_old << QVector4D(x, -halfH, 0.0f, ind ) ;
			int in  = addNormal( QVector3D(0.0f, 0.0f, 1.0f ) ) ;
			int iv1 = addVertex( QVector3D(x,  halfH, 0.0f ) ) ;
			int iv2 = addVertex( QVector3D(x, -halfH, 0.0f ) ) ;
			addLine(iv1, iv2, in ) ;
		}
		else {
			QVector3D v1( x - e / 2, -halfH, e ) ;
			QVector3D v2( x + e / 2,  halfH, e ) ;
			QVector3D v3( x + e / 2, -halfH, 0.0f ) ;
			QVector3D v4( x - e / 2,  halfH, 0.0f ) ;
			xExtrude(v1, v2 ) ; xExtrude(v3, v4 ) ;
			zExtrude(v2, v3 ) ; zExtrude(v1, v4 ) ;
		}
	}
	for ( float y = -halfH ; y <= halfH ; y += m_step ) {
		if ( isWireFrame() ) {
//			m_vertices << QVector3D( halfW, y, 0.0f ) ;
//			m_vertices << QVector3D(-halfW, y, 0.0f ) ;
//			m_normals  << QVector3D(0.0f, 0.0f, 1.0f ) ;
//			m_normals  << QVector3D(0.0f, 0.0f, 1.0f ) ;
			// test réduction m_normals
//			m_normals_old  << QVector3D(0.0f, 0.0f, 1.0f ) ;
//			int ind = m_normals_old.size() - 1 ;
//			m_vertices_old << QVector4D( halfW, y, 0.0f, ind ) ;
//			m_vertices_old << QVector4D(-halfW, y, 0.0f, ind ) ;
			int in  = addNormal( QVector3D(0.0f, 0.0f, 1.0f ) ) ;
			int iv1 = addVertex( QVector3D( halfW, y, 0.0f ) ) ;
			int iv2 = addVertex( QVector3D(-halfW, y, 0.0f ) ) ;
			addLine(iv1, iv2, in ) ;
		}
		else {
			QVector3D v1(  halfW, y - e / 2, e ) ;
			QVector3D v2( -halfW, y + e / 2, e ) ;
			QVector3D v3(  halfW, y + e / 2, 0.0f ) ;
			QVector3D v4( -halfW, y - e / 2, 0.0f ) ;
			xExtrude(v1, v2 ) ; xExtrude(v3, v4 ) ;
			zExtrude(v2, v3 ) ; zExtrude(v1, v4 ) ;
		}
	}
*/
}
