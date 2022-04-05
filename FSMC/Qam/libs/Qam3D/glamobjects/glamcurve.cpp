/*  ---------------------------------------------------------------------------
 *  filename    :   glamcurve.cpp
 *  description :   IMPLEMENTATION de la classe GLamCurve
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

#include "glamcurve.h"

#define	V3D(x)	QVector3D(x, x, x )

// constructeur par défaut

GLamCurve::GLamCurve()
	: GLamObject()
	, m_closed( false )
	, m_sketch( false )
{
	setClassName("GLamCurve") ;
	m_curve << QVector3D(0,0,0) << QVector3D(0,0,1) ;
	m_shape << GLam2D::circularShape(0.1, 16 ) ;
}

// constructeur : enveloppe par liste de points 3D
// et section par liste de points 2D

GLamCurve::GLamCurve(QList<QVector3D> curve, QList<QPointF> shape )
	: GLamObject()
	, m_curve( curve )
	, m_shape( shape )
	, m_sketch( false )
{
	setClassName("GLamCurve") ;
}

// constructeur de copie

GLamCurve::GLamCurve(const GLamCurve& obj )
	: GLamObject()
{
	*this = obj ;
}

// destructeur

GLamCurve::~GLamCurve()
{
}

// surcharge opérateur d'affectation

GLamCurve& GLamCurve::operator =(const GLamCurve& obj )
{
	*(GLamObject*)this = obj ;
	m_curve = obj.m_curve ;
	m_shape = obj.m_shape ;
	m_closed = obj.m_closed ;
	updateNeeded() ;
	return *this ;
}

void GLamCurve::create()
{
	if ( m_curve.size() < 2 )	return ;

#ifndef USE_GLULIBTESS
	QMatrix4x4	mtBegin, mtEnd ;
#endif

	QList<QVector3D>	pNew ;	// section courante
	QList<QVector3D>	pOld ;	// section précédente

	// l'enveloppe est-elle bouclée ?
	m_closed = qFuzzyCompare(m_curve.first() + V3D(1.0), m_curve.last() + V3D(1.0) ) ;
	// si oui, correction dernier point pour correspondance exacte
	if ( m_closed )	m_curve.last() = m_curve.first() ;

	// pour chaque point de construction de l'âme

	for ( int i = 0 ; i < m_curve.size() ; ++i ) {

		// calcul de la normale à la section inter-segment,
		// cas particuliers aux extrémités

		QVector3D	v ;

		if ( i == 0 ) {
			if ( m_closed )
					v = m_curve.at(1) - m_curve.at(m_curve.size() - 2) ;
			else	v = m_curve.at(i+1) - m_curve.at(i) ;
		}
		else if ( i == m_curve.size() - 1 ) {
			if ( m_closed )
					v = m_curve.at(1) - m_curve.at(m_curve.size() - 2) ;
			else	v = m_curve.at(i) - m_curve.at(i-1) ;
		}
		else {
			v = m_curve.at(i+1) - m_curve.at(i-1) ;
		}

		v.normalize() ;

		// matrice de transformation

		QMatrix4x4	mt ;
		qreal a = qRadiansToDegrees( qAcos( v.z() ) ) ;
		mt.translate( m_curve.at(i) ) ;
		mt.rotate(a, QVector3D( -v.y(), v.x(), 0 ) ) ;

#ifndef USE_GLULIBTESS
		if ( i == 0 )					mtBegin = mt ;
		if ( i == m_curve.size() - 1 )	mtEnd = mt ;
#endif

		// calcul des sommets inter-segments (section)

		for ( int j = 0 ; j < m_shape.size() ; ++j ) {
			QVector3D	pt(m_shape.at(j).x(), m_shape.at(j).y(), 0 ) ;
			pNew << mt.map( pt ) ;
		}

		// tracé du segment de solide (facettes entre 2 sections)
		// en mode ébauche, tracé restreint à la section courante

		if ( i ) {
			for ( int j = 1 ; j <= m_shape.size() ; ++j ) {
				int k = j % m_shape.size() ;
				if ( !m_sketch ) {
					quad(pNew.at(k), pNew.at(j-1), pOld.at(j-1), pOld.at(k) ) ;
				}
				else {
					triangle(pNew.at(k), pNew.at(j-1), pNew.at(k) + V3D(1e-6) ) ;
				}
			}
		}

		// mode ébauche non fermé, montrer la section initiale ( i == 0 )
		else if (( m_sketch )&&( !m_closed )) {
			for ( int j = 1 ; j <= m_shape.size() ; ++j ) {
				int k = j % m_shape.size() ;
				triangle(pNew.at(k), pNew.at(j-1), pNew.at(k) + V3D(1e-6) ) ;
			}
		}

#ifdef USE_GLULIBTESS
		// fermeture des extrémités par tesselation (libtess)
		// autorise les sections concaves/convexes...
		if (( !m_closed )&&( !m_sketch )) {
			glamTessNew(this) ;
			glamTessBeginContour() ;
			double pt[ pNew.size() ][3] ;
			if ( i == 0 ) {
				for ( int j = pNew.size() - 1 ; j >= 0 ; --j ) {
					pt[j][0] = pNew.at(j).x() ;
					pt[j][1] = pNew.at(j).y() ;
					pt[j][2] = pNew.at(j).z() ;
					glamTessVertex(pt[j] ) ;
				}
			}
			if ( i == m_curve.size() - 1 ) {
				for ( int j = 0 ; j < pNew.size() ; ++j ) {
					pt[j][0] = pNew.at(j).x() ;
					pt[j][1] = pNew.at(j).y() ;
					pt[j][2] = pNew.at(j).z() ;
					glamTessVertex(pt[j] ) ;
				}
			}
			glamTessEndContour() ;
			glamTessDelete() ;
		}
#endif
		// mémorisation de la section

		pOld = pNew ;
		pNew.clear() ;
	}

#ifndef USE_GLULIBTESS
	// fermeture des extrémités par tesselation (libtess2)
	if (( !m_closed )&&( !m_sketch )) {
		glamTessPolygon( m_shape ) ;
		QList<QPointF> p = glamTessTriangulate() ;
		// au départ...
		QVector<QVector3D>	pv ;
		for ( int i = 0 ; i < p.count() ; ++i ) {
			QVector3D v(p.at(i).x(), p.at(i).y(), 0 ) ;
			pv << mtBegin.map( v ) ;
		}
		for ( int i = 2 ; i < pv.count() ; i += 3 ) {
			triangle(pv.at(i-2), pv.at(i-1), pv.at(i) ) ;
		}
		// ...et à l'arrivée
		pv.clear() ;
		for ( int i = 0 ; i < p.count() ; ++i ) {
			QVector3D v(p.at(i).x(), p.at(i).y(), 0 ) ;
			pv << mtEnd.map( v ) ;
		}
		for ( int i = 2 ; i < pv.count() ; i += 3 ) {
			triangle(pv.at(i-2), pv.at(i-1), pv.at(i) ) ;
		}
	}
#endif
}

void GLamCurve::setCurve(QList<QVector3D> curve )
{
	m_curve = curve ;
	updateNeeded() ;
}

void GLamCurve::setShape(QList<QPointF> shape )
{
	m_shape = shape ;
	updateNeeded() ;
}

void GLamCurve::setSketched(bool sketched )
{
	m_sketch = sketched ;
	setWireFrame( m_sketch ) ;
	updateNeeded() ;
}
