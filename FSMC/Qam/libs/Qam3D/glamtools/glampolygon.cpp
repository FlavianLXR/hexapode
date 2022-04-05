/*  ---------------------------------------------------------------------------
 *  filename    :   glampolygon.cpp
 *  description :   IMPLEMENTATION de la classe GLamPolygon
 *
 *	project     :	Qam3D/GLam useful resources
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

#include "glampolygon.h"

/*! Constructeur d'un polygone type "tri" ; 3 vertices. */

GLamPolygon::GLamPolygon(QVector3D v1, QVector3D v2, QVector3D v3 )
{
	m_points << v1 << v2 << v3 ;
//	m_points << v3 << v2 << v1 ;
}

/*! Constructeur d'un polygone type "quad" ; 4 vertices. */

GLamPolygon::GLamPolygon(QVector3D v1, QVector3D v2, QVector3D v3, QVector3D v4 )
{
	if      ( v1 == v2 )	m_points << v1 << v3 << v4 ;
	else if ( v3 == v4 )	m_points << v1 << v2 << v3 ;
	else					m_points << v1 << v2 << v3 << v4 ;

//	if      ( v1 == v2 )	m_points << v4 << v3 << v1 ;
//	else if ( v3 == v4 )	m_points << v3 << v2 << v1 ;
//	else					m_points << v4 << v3 << v2 << v1 ;
}

/*! Constructeur d'un polygone par liste de vertices. */

GLamPolygon::GLamPolygon(const QList<QVector3D>& points )
{
	m_points = points ;
}

/*! Retourne la normale du polygone (vecteur perpendiculaire à sa surface). */

QVector3D GLamPolygon::normal() const
{
	if ( m_points.size() < 3 )	return QVector3D() ;
	return QVector3D::normal(m_points.at(0), m_points.at(1), m_points.at(2) ) ;
//	return QVector3D::normal(m_points.at(2), m_points.at(1), m_points.at(0) ) ;
}



#ifdef GLAMPOLYGON_OLD

#include "glamhole.h"

//#include <QDebug>

GLamPolygon::GLamPolygon()
	: GLamObject()
	, m_thickness( 0.5f )
{
	m_holes.clear() ;
	m_pts = GLam2D::circularShape(0.5f, 4 ) ;
	setWireFrame() ;
}

// polygone quelconque défini par liste de sommets
// B-Spline ordre 2 si numSectors != 0
// avec doublage des 2 premiers points si autoCompleted == true

GLamPolygon::GLamPolygon(qreal thickness, const QList<QPointF>& points, uint numSectors, bool autoCompleted )
	: GLamObject()
	, m_thickness( qAbs(thickness) )
{
	m_holes.clear() ;

	m_pts = points ;
	if ( numSectors && autoCompleted ) {
		m_pts << m_pts.at(0) ;
		m_pts << m_pts.at(1) ;
	}
	m_pts = GLam2D::bSplineShape(m_pts, numSectors ) ;
}

// surface rectangulaire
// oblongue si numSectors != 0

GLamPolygon::GLamPolygon(qreal thickness, QSizeF size, uint numSectors )
	: GLamObject()
	, m_thickness( qAbs(thickness) )
{
	m_holes.clear() ;
	m_pts = GLam2D::oblongShape(size, numSectors ) ;
}

// polygone régulier défini par rayon et nombre de faces

GLamPolygon::GLamPolygon(qreal thickness, qreal radius, uint numSectors )
	: GLamObject()
	, m_thickness( qAbs(thickness) )
{
	m_holes.clear() ;
	m_pts = GLam2D::circularShape(radius, numSectors ) ;
}

GLamPolygon::GLamPolygon(const GLamPolygon& obj )
	: GLamObject()
{
	*this = obj ;
}

GLamPolygon::~GLamPolygon()
{
	// TODO : delete m_holesPts ;
}

GLamPolygon& GLamPolygon::operator =(const GLamPolygon& obj )
{
	*(GLamObject*)this = obj ;

	m_pts.clear() ;
	m_pts = obj.m_pts ;
	m_thickness = obj.m_thickness ;
	m_holes = obj.m_holes ;

	updateNeeded() ;
	return *this ;
}

void GLamPolygon::addHole(QPointF position, GLamHole* hole )
{
/*
	hole->setWireFrame( isWireFrame() ) ;

	hole->setOffset( position ) ;

	m_holes << hole ;
	double*	p = new double[ hole->numPoints() * 3 ] ;
	for ( int n = 0 ; n < hole->numPoints() ; ++n ) {
		*( p + 3 * n )     = hole->point(n).x() ;
		*( p + 3 * n + 1 ) = hole->point(n).y() ;
		*( p + 3 * n + 2 ) = m_thickness ;
	}
	m_holesPts << p ;

	updateNeeded() ;
*/
}

// épaisseur de matière

qreal GLamPolygon::thickness() const { return m_thickness ; }

void GLamPolygon::setThickness(qreal thickness )
{
	m_thickness = qAbs(thickness) ;
	updateNeeded() ;
}

// accès direct aux points du polygone

QList<QPointF>	GLamPolygon::points() const { return m_pts ; }

void GLamPolygon::setPoints(const QList<QPointF>& points )
{
	m_pts = points ;
	updateNeeded() ;
}

// ---------------------------------------------------------------------------

void GLamPolygon::create()
{
	m_pts << m_pts.at(0) ;	// fermeture contour extérieur

	createFrontFace() ;		// surfaces plan XY
	createBackFace() ;

	createEdgeFaces() ;		// contours
}

void GLamPolygon::createFrontFace()
{
	glamTessNew(this) ;

	glamTessBeginContour() ;
		double pt[ m_pts.size() ][3] ;
		for ( int i = 0 ; i < m_pts.size() ; ++i ) {
			pt[i][0] = m_pts.at(i).x() ;
			pt[i][1] = m_pts.at(i).y() ;
			pt[i][2] = m_thickness ;
			glamTessVertex(pt[i] ) ;
		}
	glamTessEndContour() ;

	// holes
/*
	for ( int i = 0 ; i < m_holes.size() ; ++i ) {
		int npts = m_holes.at(i)->numPoints() ;
		glamTessBeginContour() ;
			for ( int n = 0 ; n < npts ; ++n ) {
				*( m_holesPts.at(i) + 3 * n + 2 ) = m_thickness ;
				glamTessVertex( m_holesPts.at(i) + 3 * n ) ;
			}
		glamTessEndContour() ;
	}
*/
	glamTessDelete() ;
}

void GLamPolygon::createBackFace()
{
	glamTessNew(this) ;

	glamTessBeginContour() ;
		double pt[ m_pts.size() ][3] ;
		for ( int i = m_pts.size() - 1 ; i >= 0 ; --i ) {
			pt[i][0] = m_pts.at(i).x() ;
			pt[i][1] = m_pts.at(i).y() ;
			pt[i][2] = 0.0f ;
			glamTessVertex(pt[i] ) ;
		}
	glamTessEndContour() ;

	// holes
/*
	for ( int i = 0 ; i < m_holes.size() ; ++i ) {
		int npts = m_holes.at(i)->numPoints() ;
		glamTessBeginContour() ;
			for ( int n = npts - 1 ; n >= 0 ; --n ) {
				*( m_holesPts.at(i) + 3 * n + 2 ) = 0.0f ;
				glamTessVertex(m_holesPts.at(i) + 3 * n ) ;
			}
		glamTessEndContour() ;
	}
*/
	glamTessDelete() ;
}

void GLamPolygon::createEdgeFaces()
{
	// contour extérieur

	for ( int i = 0 ; i < m_pts.size() - 1 ; ++i ) {
		zExtrude(
			QVector3D( m_pts.at(i).x(), m_pts.at(i).y(), 0.0f ),
			QVector3D( m_pts.at(i+1).x(), m_pts.at(i+1).y(), m_thickness )
		) ;
	}

	// contours intérieurs
/*
	for ( int i = 0 ; i < m_holes.size() ; ++i ) {
		m_vertices << m_holes.at(i)->vertices() ;
		m_normals << m_holes.at(i)->normals() ;
	}
*/
}

#endif
