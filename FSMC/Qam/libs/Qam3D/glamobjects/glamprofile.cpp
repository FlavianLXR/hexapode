/*  ---------------------------------------------------------------------------
 *  filename    :   glamprofile.cpp
 *  description :   IMPLEMENTATION de la classe GLamProfile
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

#include "glamprofile.h"

/*! Constructeur par défaut.
 *  Fabrique un profil de section rectangulaire 0.5 x 1,
 *  extrudé sur une longueur égale à 1 ; en mode 'fil de fer'.
 */

GLamProfile::GLamProfile()
	: GLamObject()
	, m_numSlices( 0 )
	, m_closed( true )
{
	setClassName("GLamProfile") ;
	m_pts.clear() ;
	m_pts << QPointF(0.0f, 0.0f ) ;
	m_pts << QPointF(0.5f, 0.0f ) ;
	m_pts << QPointF(0.5f, 1.0f ) ;
	m_pts << QPointF(0.0f, 1.0f ) ;
	extrudeAlong( 1.0f ) ;
	setWireFrame() ;
}

/*! Constructeur d'un profil droit (non extrudé) d'épaisseur @c thickness.
 */

GLamProfile::GLamProfile(qreal thickness )
	: GLamObject()
	, m_numSlices( 0 )
	, m_closed( true )
{
	setClassName("GLamProfile") ;
	m_pts.clear() ;
	m_pts << QPointF(0.0f, 0.0f ) ;
	m_pts << QPointF(0.0f, qAbs( thickness ) ) ;
	m_path.clear() ;
}

/*! Constructeur d'une section de type enveloppe segmentée
 *  (si @c numSlices est nul) ou de type B-Spline d'ordre 2 définie
 *  par une suite de @c points de contrôle et un nombre @c numSlices de
 *  facettes entre chaque paire de points de contrôle.
 */

GLamProfile::GLamProfile(const QList<QPointF>& points, uint numSlices )
	: GLamObject()
	, m_numSlices( numSlices )
	, m_closed( true )
{
	setClassName("GLamProfile") ;

	QList<QPointF> pts( points ) ;

    if ( numSlices ) {
		pts.push_front( pts.first() ) ;
		pts.push_back( pts.last() ) ;
		m_pts = GLam2D::bSplineShape(pts, m_numSlices ) ;
	}
	else	m_pts = pts ;

	m_path.clear() ;
}

/*! Constructeur d'un profil par une liste de sommets correspondant à une
 *  courbe gauche de Bézier définie par ses points extrèmes, les vecteurs
 *  en ces points et un nombre numSlices de facettes à répartir du point de
 *  départ à celui de fin.
 *  @n Le vecteur @c v0 matérialise le point de départ : position (x,y) et
 *  vecteur directeur (z,w). Le vecteur @c v1 spécifie le point d'arrivée
 *  selon le même principe ; l'enveloppe de construction de la courbe de
 *  Bézier est ainsi définie par 4 points A(v0.x(), v0.y() ),
 *  B(v0.x() + v0.z(), v0.y() + v0.w() ), C(v1.x() + v1.z(), v1.y() + v1.w() )
 *  et D(v1.x(), v1.y() ).
 *  Si @c numSlices vaut 0, la liste fabriquée est restreinte à ces 4 points
 *  de contrôle.
 */

GLamProfile::GLamProfile(QVector4D v0, QVector4D v1, uint numSlices )
	: GLamObject()
	, m_numSlices( numSlices )
	, m_closed( true )
{
	setClassName("GLamProfile") ;
	m_pts = GLam2D::bezierShape(v0, v1, m_numSlices ) ;

	m_path.clear() ;
}

/*! Constructeur de copie. */

GLamProfile::GLamProfile(const GLamProfile& obj )
	: GLamObject()
{
	*this = obj ;
}

/*! Destructeur. */

GLamProfile::~GLamProfile()
{
}

/*! Surcharge de l'opérateur d'affectation. */

GLamProfile& GLamProfile::operator =(const GLamProfile& obj )
{
	*(GLamObject*)this = obj ;
	m_pts = obj.m_pts ;
	m_numSlices = obj.m_numSlices ;
	m_closed = obj.m_closed ;
	m_path = obj.m_path ;
	updateNeeded() ;
	return *this ;
}

/*! Méthode d'extrusion suivant 0Y+ du profil, avec des coupes aux
 *  extrémités d'équerre avec le plan 0XY.
 */

void GLamProfile::extrudeAlong(qreal length )
{
	m_path.clear() ;
	m_path << QVector4D(0.0f, 0.0f, 1.0f, 0.0f ) ;
	m_path << QVector4D(0.0f, qAbs(length), 1.0f, 0.0f ) ;
	updateNeeded() ;
}

/*! Méthode d'extrusion du profil de @c begin(x,y), avec une coupe suivant
 * @c begin(z,w), jusqu'à @c end(x,y), avec une coupe suivant @c end(z,w).
 */

void GLamProfile::extrudeAlong(QVector4D begin, QVector4D end )
{
	m_path.clear() ;
	m_path << begin ;
	m_path << end ;
	updateNeeded() ;
}

/*! Méthode d'extrusion du profil le long d'une suite de segments définie
 * par les sommets @c points, avec des coupes normales aux extrémités.
 */

void GLamProfile::extrudeAlong(const QList<QPointF>& points )
{
	if ( points.size() < 2 )	return ;

	m_path.clear() ;
	m_path << GLam2D::normal(points.at(1), points.at(0) ) ;
	for ( int i = 1 ; i < points.size() - 1 ; ++i ) {
		m_path << GLam2D::median(points.at(i-1), points.at(i), points.at(i+1) ) ;
	}
	m_path << GLam2D::normal(points.at(points.size() - 2), points.last() ) ;
	updateNeeded() ;
}

/*! Méthode d'extrusion du profil le long d'une suite de segments définie
 * par les sommets @c points, avec des coupes aux extrémités définies par
 * les vecteurs @c begin et @c end.
 */

void GLamProfile::extrudeAlong(const QList<QPointF>& points, QVector2D begin, QVector2D end )
{
	if ( points.size() < 2 )	return ;

	m_path.clear() ;
	m_path << QVector4D(points.at(0).x(), points.at(0).y(), begin.x(), begin.y() ) ;
	for ( int i = 1 ; i < points.size() - 1 ; ++i ) {
		m_path << GLam2D::median(points.at(i-1), points.at(i), points.at(i+1) ) ;
	}
	m_path << QVector4D(points.last().x(), points.last().y(), end.x(), end.y() ) ;
	updateNeeded() ;
}

/*! Méthode d'extrusion du profil à l'extérieur d'une liste de segments
 *  définie par les sommets @c points (ordre CCW), avec fermeture du profil.
 */

void GLamProfile::extrudeOutside(const QList<QPointF>& points )
{
	if ( points.size() < 3 )	return ;

	m_path.clear() ;
	m_path << GLam2D::median(points.last(), points.at(0), points.at(1) ) ;
	for ( int i = 1 ; i < points.size() - 1 ; ++i ) {
		m_path << GLam2D::median(points.at(i-1), points.at(i), points.at(i+1) ) ;
	}
	m_path << GLam2D::median(points.at(points.size() - 2), points.last(), points.at(0) ) ;
	m_path << m_path.first() ;
	updateNeeded() ;
}

/*! Méthode d'extrusion du profil à l'intérieur d'une liste de segments
 *  définie par les sommets @c points (ordre CW), avec fermeture du profil.
 */

void GLamProfile::extrudeInside(const QList<QPointF>& points )
{

	if ( points.size() < 3 )	return ;

	m_path.clear() ;
	for ( int i = points.size() - 2 ; i > 0 ; --i ) {
		m_path << GLam2D::median(points.at(i-1), points.at(i), points.at(i+1) ) ;
	}
	m_path << GLam2D::median(points.at(1), points.at(0), points.last() ) ;
	m_path << GLam2D::median(points.at(0), points.last(), points.at(points.size() - 2) ) ;
	m_path << m_path.first() ;
	updateNeeded() ;
}

// fermeture du profil et des extrémités (coupes)

bool GLamProfile::isClosed() const { return m_closed ; }

void GLamProfile::setClosed(bool closed )
{
	m_closed = closed ;
	updateNeeded() ;
}

/*! Détermination des vertices et normales du profil. */

void GLamProfile::create()
{
	if ( m_path.isEmpty() )	return ;

#ifdef USE_GLULIBTESS
	double ptBegin[ m_pts.size() ][3] ;
	double ptEnd[ m_pts.size() ][3] ;
#else
	QPointF		pBegin, pEnd ;
	QVector2D	dirBegin, dirEnd ;
	qreal		phiBegin, phiEnd ;
#endif

	// fermeture du profil ?
	if ( isClosed() )	m_pts << m_pts.first() ;

	for ( int s = 0 ; s < m_path.size() - 1 ; ++s ) {
		// direction à suivre dans le plan 0XY
		QPointF p0(m_path.at(s).x(), m_path.at(s).y() ) ;
		QPointF p1(m_path.at(s + 1).x(), m_path.at(s + 1).y() ) ;
		QVector2D v(p1.x() - p0.x(), p1.y() - p0.y() ) ;
		// angle entre 0Y et la direction à suivre, en radians
		qreal phi = GLam2D::angle( QVector2D(0,1), v ) ;

#ifndef USE_GLULIBTESS
		if ( isClosed() ) {
			if ( s == 0 ) {
				pBegin = p0 ;
				dirBegin = v ;
				phiBegin = phi ;
			}
			if ( s == m_path.size() - 2 ) {
				pEnd = p0 ;
				dirEnd = v ;
				phiEnd = phi ;
			}
		}
#endif

		for ( int i = 0 ; i < m_pts.size() - 1 ; i++ ) {
			// segment de profil dans le plan 0XZ
			qreal x1 = m_pts.at(i).x() ;
			qreal z1 = m_pts.at(i).y() ;
			qreal x2 = m_pts.at(i+1).x() ;
			qreal z2 = m_pts.at(i+1).y() ;
			// alignement avec la direction à suivre, au point p0
			QPointF pp1( p0.x() + x1 * COS(phi), p0.y() + x1 * SIN(phi) ) ;
			QPointF pp2( p0.x() + x2 * COS(phi), p0.y() + x2 * SIN(phi) ) ;
			// vecteurs alignés en pp1 et pp2
			QVector4D vp1(pp1.x(), pp1.y(), v.x(), v.y() ) ;
			QVector4D vp2(pp2.x(), pp2.y(), v.x(), v.y() ) ;
			// projection sur les vecteurs d'extrémité
			bool err = false ;
			QPointF q0 = GLam2D::intersection(vp1, m_path.at(s), &err ) ;
			QPointF q1 = GLam2D::intersection(vp2, m_path.at(s), &err ) ;
			QPointF q2 = GLam2D::intersection(vp2, m_path.at(s+1), &err ) ;
			QPointF q3 = GLam2D::intersection(vp1, m_path.at(s+1), &err ) ;

			if ( !err ) quad(
				QVector3D( q0.x(), q0.y(), z1 ),
				QVector3D( q1.x(), q1.y(), z2 ),
				QVector3D( q2.x(), q2.y(), z2 ),
				QVector3D( q3.x(), q3.y(), z1 )
			) ;
#ifdef USE_GLULIBTESS
			// mémorisation coupes extrèmes
			// (profil initial éventuellement déformé par angles coupes)
			if ( isClosed() ) {
				if ( s == 0 ) {
					ptBegin[i][0] = q0.x() ;
					ptBegin[i][1] = q0.y() ;
					ptBegin[i][2] = z1 ;
				}
				if ( s == m_path.size() - 2 ) {
					ptEnd[i][0] = q3.x() ;
					ptEnd[i][1] = q3.y() ;
					ptEnd[i][2] = z1 ;
				}
			}
#endif
		}
	}

	// fermeture aux extrémités

#ifdef USE_GLULIBTESS
	// GLU like libtess
	if ( isClosed() &&( m_path.first() != m_path.last() )) {
		m_pts.removeLast() ;

		glamTessNew(this) ;
		glamTessBeginContour() ;
		for ( int i = 0 ; i < m_pts.size() ; ++i )	glamTessVertex(ptBegin[i] ) ;
		glamTessEndContour() ;
		glamTessDelete() ;

		glamTessNew(this) ;
		glamTessBeginContour() ;
		for ( int i = 0 ; i < m_pts.size() ; ++i )	glamTessVertex(ptEnd[i] ) ;
		glamTessEndContour() ;
		glamTessDelete() ;
	}
#else
	// Poly2Tri libtess
	if ( isClosed() &&( m_path.first() != m_path.last() )) {
		glamTessPolygon( m_pts ) ;
		QList<QPointF> p = glamTessTriangulate() ;
		QVector<QVector3D>	pv ;
		for ( int i = 0 ; i < p.count() ; ++i ) {
			qreal x1 = p.at(i).x() ;
			qreal z1 = p.at(i).y() ;
			QPointF pp1( pBegin.x() + x1 * COS(phiBegin), pBegin.y() + x1 * SIN(phiBegin) ) ;
			QVector4D vp1(pp1.x(), pp1.y(), dirBegin.x(), dirBegin.y() ) ;
			bool err = false ;
			QPointF q0 = GLam2D::intersection(vp1, m_path.first(), &err ) ;
			pv << QVector3D(q0.x(), q0.y(), z1 ) ;
		}
		for ( int i = 2 ; i < pv.count() ; i += 3 ) {
			triangle(pv.at(i-2), pv.at(i-1), pv.at(i) ) ;
		}

		pv.clear() ;
		for ( int i = 0 ; i < p.count() ; ++i ) {
			qreal x1 = p.at(i).x() ;
			qreal z1 = p.at(i).y() ;
			QPointF pp1( pEnd.x() + x1 * COS(phiEnd), pEnd.y() + x1 * SIN(phiEnd) ) ;
			QVector4D vp1(pp1.x(), pp1.y(), dirEnd.x(), dirEnd.y() ) ;
			bool err = false ;
			QPointF q0 = GLam2D::intersection(vp1, m_path.last(), &err ) ;
			pv << QVector3D(q0.x(), q0.y(), z1 ) ;
		}
		for ( int i = 2 ; i < pv.count() ; i += 3 ) {
			triangle(pv.at(i-2), pv.at(i-1), pv.at(i) ) ;
		}
	}
#endif
}
