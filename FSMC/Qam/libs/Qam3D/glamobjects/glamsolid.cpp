/*  ---------------------------------------------------------------------------
 *  filename    :   glamsolid.cpp
 *  description :   IMPLEMENTATION de la classe GLamSolid
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

#include "glamsolid.h"

/*! Constructeur par défaut.
 *  Fabrique un solide de contour héxagonal et d'épaisseur 0.5,
 *  en mode 'fil de fer'.
 */

GLamSolid::GLamSolid()
	: GLamObject()
	, m_thickness( 0.5f )
	, m_profile( 0 )
	, m_hasDefaultProfile( true )
{
	setClassName("GLamSolid") ;
    m_shape = GLam2D::circularShape(0.5f, 5 ) ;
	setWireFrame() ;
}

/*! Constructeur d'un solide de contour &c shape et d'épaisseur
 *  @c thickness (0.0 par défaut).
 */

GLamSolid::GLamSolid(const QList<QPointF>& shape, qreal thickness )
	: GLamObject()
	, m_shape( shape )
	, m_thickness( qAbs(thickness) )
	, m_profile( 0 )
	, m_hasDefaultProfile( true )
{
	setClassName("GLamSolid") ;

	// contour sans doublons...
	if ( m_shape.last() == m_shape.first() )	m_shape.removeLast() ;
}

/*! Constructeur de copie. */

GLamSolid::GLamSolid(const GLamSolid& obj )
	: GLamObject()
	, m_profile( 0 )
{
	*this = obj ;
}

/*! Destructeur. */

GLamSolid::~GLamSolid()
{
	if ( m_profile ) delete m_profile ;
	while ( !m_holes.isEmpty() )	delete m_holes.takeLast() ;
}

/*! Surcharge de l'opérateur d'affectation. */

GLamSolid& GLamSolid::operator =(const GLamSolid& obj )
{
	*(GLamObject*)this = obj ;
	m_shape = obj.m_shape ;
	m_thickness = obj.m_thickness ;

	if ( m_profile ) delete m_profile ;
	if ( obj.m_profile ) {
		m_profile = new GLamProfile( *(obj.m_profile) ) ;
	}
	else m_profile = 0 ;

	while ( !m_holes.isEmpty() )	delete m_holes.takeLast() ;
	m_holes.clear() ;
	m_holesOffset.clear() ;

	for ( int hole = 0 ; hole < obj.m_holes.size() ; ++hole ) {
		addHole(obj.m_holes.at(hole), obj.m_holesOffset.at(hole) ) ;
	}

	m_hasDefaultProfile = obj.m_hasDefaultProfile ;
	updateNeeded() ;
	return *this ;
}

// modificateurs

void GLamSolid::setThickness(qreal thickness )
{
    m_thickness = qAbs(thickness) ;
    updateNeeded() ;
}

void GLamSolid::setShape(QList<QPointF> shape )
{
    m_shape = shape ;
    updateNeeded() ;
}

void GLamSolid::setProfile(const GLamProfile* profile )
{
	if ( m_profile ) delete m_profile ;
	m_profile = new GLamProfile( *profile ) ;
	m_hasDefaultProfile = false ;
	m_profile->setClosed( false ) ;
//	m_profile->extrudeOutside( GLam2D::counterClockWise(m_shape) ) ;
	m_profile->extrudeOutside( m_shape ) ;
}

/*! Méthode de retrait de matière (évidement) par ajout au solide
 *  d'un objet GLamHole aux coordonnées &c position dans le plan 0XY.
 *  Le 'trou' peut avoir un profilé intérieur différent de celui
 *  du solide.
 */

void GLamSolid::addHole(const GLamHole* hole, QPointF position )
{
	// maintien d'une copie de l'objet GLamHole
	m_holes << new GLamHole( *hole ) ;
	// sauvegarde de la position relative au repère local
	m_holesOffset << position ;

	updateNeeded() ;
}

/*! Création des vertices de l'objet. */

void GLamSolid::create()
{
	// mise à jour des éléments connexes
	if ( m_profile ) m_profile->update() ;

	for ( int i = 0 ; i < m_holes.size() ; ++i ) {
		m_holes.at(i)->update() ;
	}

#ifndef USE_GLULIBTESS

	// Poly2Tri libtess

	glamTessPolygon( m_shape ) ;

	for ( int hole = 0 ; hole < m_holes.size() ; ++hole ) {
//        glamTessHole( m_holes[hole]->shape() ) ;            // translate ???
        glamTessHole( GLam2D::translate(m_holes.at(hole)->shape(), m_holesOffset.at(hole).x(), m_holesOffset.at(hole).y() ) ) ;
    }

	QList<QPointF>	p = glamTessTriangulate() ;	// CCW

	for ( int i = 2 ; i < p.count() ; i += 3 ) {
		QVector3D v1( p[i-2].x(), p[i-2].y(), 0 ) ;
		QVector3D v2( p[i-1].x(), p[i-1].y(), 0 ) ;
		QVector3D v3( p[i].x(), p[i].y(), 0 ) ;
		// back face CW
		if ( m_thickness != 0.0f ) triangle(v1, v3, v2 ) ;
		// front face CCW
		v1.setZ( m_thickness ) ;
		v2.setZ( m_thickness ) ;
		v3.setZ( m_thickness ) ;
		triangle(v1, v2, v3 ) ;
	}
#else

	// GLU like libtess

	int numPts = m_shape.size() ;
	double pt[ numPts ][3] ;

	glamTessNew(this) ;

	glamTessBeginContour() ;
	for ( int i = numPts - 1 ; i >= 0 ; i-- ) {
		pt[i][0] = m_shape.at(i).x() ;
		pt[i][1] = m_shape.at(i).y() ;
		pt[i][2] = 0 ;
		glamTessVertex(pt[i] ) ;
	}
	glamTessEndContour() ;
	//
	for ( int hole = 0 ; hole < m_holes.size() ; ++hole ) {
		QList<QPointF> shape = m_holes[hole]->shape() ;
		int numPts = shape.size() ;
		double pt[ numPts ][3] ;
		glamTessBeginContour() ;
		for ( int i = numPts - 1 ; i >= 0 ; i-- ) {
			pt[i][0] = m_shape.at(i).x() ;
			pt[i][1] = m_shape.at(i).y() ;
			pt[i][2] = 0 ;
			glamTessVertex(pt[i] ) ;
		}
		glamTessEndContour() ;
	}
	//
	glamTessDelete() ;

	if ( m_thickness != 0.0f ) {
		glamTessNew(this) ;
		glamTessBeginContour() ;
		for ( int i = 0 ; i < numPts ; i++ ) {
			pt[i][0] = m_shape.at(i).x() ;
			pt[i][1] = m_shape.at(i).y() ;
			pt[i][2] = m_thickness ;
			glamTessVertex(pt[i] ) ;
		}
		glamTessEndContour() ;
		//
		for ( int hole = 0 ; hole < m_holes.size() ; ++hole ) {
			QList<QPointF> shape = m_holes[hole]->shape() ;
			int numPts = shape.size() ;
			double pt[ numPts ][3] ;
			glamTessBeginContour() ;
			for ( int i = 0 ; i < numPts ; i++ ) {
				pt[i][0] = m_shape.at(i).x() ;
				pt[i][1] = m_shape.at(i).y() ;
				pt[i][2] = m_thickness ;
				glamTessVertex(pt[i] ) ;
			}
			glamTessEndContour() ;
		}
		//
		glamTessDelete() ;
	}
#endif

	// edge faces
	if ( m_thickness != 0.0f )	createEdgeFaces() ;
}

// [private] bords extérieurs et intérieurs du solide

void GLamSolid::createEdgeFaces()
{
	// contour extérieur droit
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
	else if ( m_profile ) {
		for ( int i = 2 ; i < m_profile->numVertexIndices() ; i += 3 ) {
			QVector3D v1 = m_profile->vertex(i-2) ;
			QVector3D v2 = m_profile->vertex(i-1) ;
			QVector3D v3 = m_profile->vertex(i) ;
			triangle(v1, v2, v3 ) ;
		}
//		m_vertices << m_profile->m_vertices ;
//		m_flatNormals << m_profile->m_flatNormals ;
	}

	// pour chaque trou...
	for ( int hole = 0 ; hole < m_holes.size() ; ++hole ) {

        QList<QPointF> pts = m_holes.at(hole)->shape() ;
        if ( !m_holesOffset.at(hole).isNull() ) pts = GLam2D::translate(pts, m_holesOffset.at(hole).x(), m_holesOffset.at(hole).y() ) ;

		// contour intérieur droit
		if ( m_holes.at(hole)->m_hasDefaultProfile ) {
			int n = m_holes.at(hole)->shape().count() ;
            for ( int i = 0 ; i < n ; ++i ) {
				zExtrude(
					QVector3D( pts[i].x(), pts[i].y(), 0.0f ),
					QVector3D( pts[(i+1)%n].x(), pts[(i+1)%n].y(), m_thickness )
				) ;
			}
		}
		// ou contour intérieur profilé
		else {
			GLamProfile profile( *(m_holes.at(hole)->m_profile) ) ;
            profile.extrudeInside( pts ) ;
            profile.update() ;
			for ( int i = 2 ; i < profile.numVertexIndices() ; i += 3 ) {
				QVector3D v1 = profile.vertex(i-2) ;
				QVector3D v2 = profile.vertex(i-1) ;
				QVector3D v3 = profile.vertex(i) ;
				triangle(v1, v2, v3 ) ;
			}
		}
	}
}
