/*  ---------------------------------------------------------------------------
 *  filename    :   glamrevolution.cpp
 *  description :   IMPLEMENTATION de la classe GLamRevolution
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

#include "glamrevolution.h"

#include <QDebug>

/*! Constructeur par défaut.
 *  Fabrique un solide en double cône de volume 1x1x1.
 *  @n La révolution est complète par défaut (amplitude de 360°), le nombre
 *  de secteurs est fixé à 30 et le nombre de tranches à 15.
 */

GLamRevolution::GLamRevolution()
	: GLamObject()
    , m_numSectors( 30 )
    , m_numSlices( 15 )
    , m_eastAngle( 180.0f )
    , m_westAngle( 180.0f )
	, m_closed( true )
    , m_zCentered( false )
{
	setClassName("GLamRevolution") ;
	m_pts.clear() ;
	m_pts << QPointF(0.0, 0.0 ) ;
	m_pts << QPointF(0.5, 0.5 ) ;
	m_pts << QPointF(0.0, 1.0 ) ;
	calculateRadius() ;
	calculateZMin() ;
}

/*! Construction dans le plan XZ par enveloppe segmentée (quand le nombre
 *  de tranches @a numSlices vaut 0)
 *  ou par B-Spline d'ordre 2 (si @a numSlices != 0).
 *  @n La révolution est complète par défaut (amplitude de 360°) et le nombre
 *  de secteurs est fixé à 90.
 */

GLamRevolution::GLamRevolution(const QList<QPointF>& points, uint numSlices )
	: GLamObject()
	, m_numSectors( 90 )
	, m_numSlices( numSlices )
    , m_eastAngle( 180.0f )
    , m_westAngle( 180.0f )
	, m_closed( true )
	, m_zCentered( false )
{
	setClassName("GLamRevolution") ;
	m_pts = GLam2D::bSplineShape(points, m_numSlices ) ;
	calculateRadius() ;
	calculateZMin() ;
}

/*! Construction dans le plan XZ par 2 vecteurs 4D définissant l'enveloppe
 *  d'une courbe de Bézier (si le nombre de tranches @a numSlices est non nul) :
 *  @n ( v0.x(), v0.y() ) = point de départ
 *  @n ( v0.z(), v0.w() ) = vecteur au point de départ
 *  @n ( v1.x(), v1.y() ) = point de fin
 *  @n ( v1.z(), v1.w() ) = vecteur au point de fin
 *  @n
 *  @n Si @a numSlices est nul, l'enveloppe est restreinte à 4 points :
 *  @n ( v0.x(), v0.y() )                   = point 1 de départ
 *  @n ( v0.x() + v0.z(), v0.y() + v0.w() ) = point 2
 *  @n ( v1.x() + v1.z(), v1.y() + v1.w() ) = point 3
 *  @n ( v1.x(), v1.y() )                   = point 4 de fin
 *  @n
 *  @n La révolution est complète par défaut (amplitude de 360°) et le nombre
 *  de secteurs est fixé à 90.
 */

GLamRevolution::GLamRevolution(QVector4D v0, QVector4D v1, uint numSlices )
	: GLamObject()
	, m_numSectors( 90 )
	, m_numSlices( numSlices )
    , m_eastAngle( 180.0f )
    , m_westAngle( 180.0f )
	, m_closed( true )
	, m_zCentered( false )
{
	setClassName("GLamRevolution") ;
	m_pts = GLam2D::bezierShape(v0, v1, m_numSlices ) ;
	calculateRadius() ;
	calculateZMin() ;
}

/*! Constructeur de copie. */

GLamRevolution::GLamRevolution(const GLamRevolution& obj )
	: GLamObject()
{
	*this = obj ;
}

/*! Destructeur. */

GLamRevolution::~GLamRevolution()
{
}

/*! Surcharge de l'opérateur d'affectation. */

GLamRevolution& GLamRevolution::operator =(const GLamRevolution& obj )
{
	*(GLamObject*)this = obj ;

	m_pts = obj.m_pts ;
	m_numSectors = obj.m_numSectors ;
	m_numSlices = obj.m_numSlices ;
    m_eastAngle = obj.m_eastAngle ;
    m_westAngle = obj.m_westAngle ;
	m_closed = obj.m_closed ;
	m_zCentered	= obj.m_zCentered ;
	calculateRadius() ;
	calculateZMin() ;
	updateNeeded() ;

	return *this ;
}

/*! Retourne l'amplitude de révolution en degrés
 * à l'ouest de l'axe 0X (quadrant Y+).
 */

qreal GLamRevolution::westAngle() const		{ return m_westAngle ; }

/*! Retourne l'amplitude de révolution en degrés
 * à l'est de l'axe 0X (quadrant Y-).
 */

qreal GLamRevolution::eastAngle() const		{ return m_eastAngle ; }

/*! Fixe l'amplitude de révolution en degrés
 * à l'ouest de l'axe 0X (quadrant Y+). La valeur est restreinte à l'intervalle
 * 0..180°.
 */

void GLamRevolution::setWestAngle(qreal westAngle )
{
	m_westAngle = qMin( qMax(westAngle, 0.0 ), 180.0 ) ;
	updateNeeded() ;
}

/*! Fixe l'amplitude de révolution en degrés
 * à l'est de l'axe 0X (quadrant Y-). La valeur est restreinte à l'intervalle
 * 0..180°.
 */

void GLamRevolution::setEastAngle(qreal eastAngle )
{
	m_eastAngle = qMin( qMax(eastAngle, 0.0 ), 180.0 ) ;
	updateNeeded() ;
}

/*! Permet de spécifier simultanément les longitudes Est et Ouest définissant
 * l'amplitude de révolution du solide.
 * @sa setWestAngle(), setEastAngle()
 */

void GLamRevolution::setLongitudes(qreal eastAngle, qreal westAngle )
{
    m_eastAngle = qMin( qMax(eastAngle, 0.0 ), 180.0 ) ;
    m_westAngle = qMin( qMax(westAngle, 0.0 ), 180.0 ) ;
	updateNeeded() ;
}

/*! Retourne l'amplitude angulaire courante du solide (en degrés).
 */

qreal GLamRevolution::angle() const	{ return m_eastAngle + m_westAngle ; }

/*! Fixe l'amplitude angulaire du solide (en degrés).
 * L'origine est l'axe 0X, le paramètre @a angle peut être signé,
 * sa valeur absolue est restreinte à l'intervalle 0..360°.
 */

void GLamRevolution::setAngle(qreal angle )
{
    if ( angle < 0 ) {
        m_eastAngle = qMin( qMax(-angle, 0.0 ), 360.0 ) ;
        m_westAngle = 0.0f ;
    }
    else {
        m_eastAngle = 0.0f ;
        m_westAngle = qMin( qMax(angle, 0.0 ), 360.0 ) ;
    }
	updateNeeded() ;
}

/*! Méthode permettant le centrage vertical du solide. */

void GLamRevolution::setZCentered(bool centered )
{
    m_zCentered = centered ;
    calculateZMin() ;
    updateNeeded() ;
}

// propriété 'numSectors'

uint GLamRevolution::numSectors() const	{ return m_numSectors ; }

void GLamRevolution::setNumSectors(uint numSectors )
{
	m_numSectors = qMax(numSectors, (uint)3 ) ;
	updateNeeded() ;
}

// propriété 'numSlices'

uint GLamRevolution::numSlices() const { return m_numSlices ; }

/*! Fixe le nombre de tranches à @a numSlices. Ce modificateur est sans effet
 *  sur les objets GLamRevolution (le nombre de tranches est défini lors de la
 *  construction par courbe de Bézier ou B-Spline).
 */

void GLamRevolution::setNumSlices(uint numSlices )
{
	m_numSlices = qMax(numSlices, (uint)2 ) ;
}

/*! Fixe le nombre de tranches à @a numFaces et le nombre de secteurs à
 *  @a numFaces * 2.
 */

void GLamRevolution::setNumFaces(uint numFaces )
{
	setNumSlices( numFaces ) ;
	setNumSectors( numFaces * 2 ) ;
}

// propriété 'closed'

bool GLamRevolution::isClosed() const { return m_closed ; }

void GLamRevolution::setClosed(bool closed )
{
	m_closed = closed ;
	updateNeeded() ;
}

/*! Retourne le diamètre intérieur du solide. */

qreal GLamRevolution::innerDiameter() const { return m_innerRadius * 2 ; }

/*! Retourne le diamètre extérieur du solide. */

qreal GLamRevolution::outerDiameter() const { return m_outerRadius * 2 ; }

// [private] recherche des rayons interne et externe

void GLamRevolution::calculateRadius()
{
	m_outerRadius = 0.0f ;
	for ( int i = 0 ; i < m_pts.size() ; i++ ) {
		if ( m_pts.at(i).x() > m_outerRadius )	m_outerRadius = m_pts.at(i).x() ;
	}
	m_innerRadius = m_outerRadius ;
	for ( int i = 0 ; i < m_pts.size() ; i++ ) {
		if ( m_pts.at(i).x() < m_innerRadius )	m_innerRadius = m_pts.at(i).x() ;
	}
}

// [private] recherche de l'ordonnée minimale

void GLamRevolution::calculateZMin()
{
	m_zMin = 0.0f ;
	if ( m_zCentered ) {
		qreal zMax = 0.0f ;
		for ( int i = 0 ; i < m_pts.size() ; i++ ) {
			if ( m_pts.at(i).y() < m_zMin )	m_zMin = m_pts.at(i).y() ;
			if ( m_pts.at(i).y() > zMax )	zMax = m_pts.at(i).y() ;
		}
		m_zMin = ( zMax - m_zMin ) / 2.0f ;
	}
}

/*! Méthode d'accès direct aux points de construction de la section radiale
 * du solide. */

QList<QPointF>	GLamRevolution::points() const { return m_pts ; }

void GLamRevolution::setPoints(const QList<QPointF>& points )
{
	m_pts = points ;
	calculateRadius() ;
	updateNeeded() ;
}

/*! Méthode d'accès direct aux polygones de l'enveloppe extérieure 360°
 * du solide. */

QList<GLamPolygon> GLamRevolution::polygons(QMatrix4x4 transform ) const
{
	QList<GLamPolygon> list ;

	qreal step = 2 * M_PI / numSectors() ;
	qreal a1 = 0.0f ;
	qreal a2 ;

	do {
		a2 = a1 + step ;
		if ( a2 > 2 * M_PI )	a2 = 2 * M_PI ;
		for ( int i = 0 ; i < m_pts.size() - 1 ; i++ ) {
			qreal x1 = m_pts.at(i).x() ;
			qreal z1 = m_pts.at(i).y() - m_zMin ;
			qreal x2 = m_pts.at(i+1).x() ;
			qreal z2 = m_pts.at(i+1).y() - m_zMin ;
			list << GLamPolygon(
//				transform * QVector3D( x1 * COS(a1), x1 * SIN(a1), z1 ),
//				transform * QVector3D( x1 * COS(a2), x1 * SIN(a2), z1 ),
//				transform * QVector3D( x2 * COS(a2), x2 * SIN(a2), z2 ),
//				transform * QVector3D( x2 * COS(a1), x2 * SIN(a1), z2 )
                transform.map( QVector3D( x1 * COS(a1), x1 * SIN(a1), z1 ) ),
                transform.map( QVector3D( x1 * COS(a2), x1 * SIN(a2), z1 ) ),
                transform.map( QVector3D( x2 * COS(a2), x2 * SIN(a2), z2 ) ),
                transform.map( QVector3D( x2 * COS(a1), x2 * SIN(a1), z2 ) )
            ) ;
		}
		a1 = a2 ;
	} while ( a2 < 2 * M_PI ) ;

	return list ;
}

/*! Création des vertices et texels de l'objet. */

void GLamRevolution::create()
{
    qreal beginAngle = -qDegreesToRadians( m_eastAngle ) ;
    qreal endAngle   =  qDegreesToRadians( m_westAngle ) ;

	qreal step = 2 * M_PI / numSectors() ;
	qreal a1   = beginAngle ;
	qreal a2 ;

	do {
		a2 = a1 + step ;
		if ( a2 > endAngle )	a2 = endAngle ;

		// secteur longitudinal

		for ( int i = 0 ; i < m_pts.size() - 1 ; i++ ) {
			qreal x1 = m_pts.at(i).x() ;
			qreal z1 = m_pts.at(i).y() - m_zMin ;
			qreal x2 = m_pts.at(i+1).x() ;
			qreal z2 = m_pts.at(i+1).y() - m_zMin ;
			quad(
				QVector3D( x1 * COS(a1), x1 * SIN(a1), z1 ),
				QVector3D( x1 * COS(a2), x1 * SIN(a2), z1 ),
				QVector3D( x2 * COS(a2), x2 * SIN(a2), z2 ),
				QVector3D( x2 * COS(a1), x2 * SIN(a1), z2 )
			) ;
		}

		// fermeture du secteur

		if (( m_closed )&&( m_pts.first() != m_pts.last() )) {
			qreal x1 = m_pts.first().x() ;
			qreal z1 = m_pts.first().y() - m_zMin ;
			qreal x2 = m_pts.last().x() ;
			qreal z2 = m_pts.last().y() - m_zMin ;
			if ( x1 + x2 ) quad(
				QVector3D( x1 * COS(a1), x1 * SIN(a1), z1 ),
				QVector3D( x1 * COS(a2), x1 * SIN(a2), z1 ),
				QVector3D( x2 * COS(a2), x2 * SIN(a2), z2 ),
				QVector3D( x2 * COS(a1), x2 * SIN(a1), z2 )
			) ;
		}

		a1 = a2 ;
	} while ( a2 < endAngle ) ;

	// fermeture des extrémités en cas de révolution incomplète

#ifndef USE_GLULIBTESS
	// Poly2Tri libtess

	if (( angle() < 360.0f )&& isClosed() ) {

		glamTessPolygon( m_pts ) ;
		QList<QPointF> p = glamTessTriangulate() ;

		float co = COS( beginAngle ) ;
		float si = SIN( beginAngle ) ;
		for ( int i = 2 ; i < p.count() ; i += 3 ) {
			QVector3D v1( p[i-2].x() * co,  p[i-2].x() * si, p[i-2].y() - m_zMin ) ;
			QVector3D v2( p[i-1].x() * co,  p[i-1].x() * si, p[i-1].y() - m_zMin ) ;
			QVector3D v3( p[i].x() * co,  p[i].x() * si, p[i].y() - m_zMin ) ;
			triangle(v1, v2, v3 ) ;
		}
		co = COS( endAngle ) ;
		si = SIN( endAngle ) ;
		for ( int i = 2 ; i < p.count() ; i += 3 ) {
			QVector3D v1( p[i-2].x() * co,  p[i-2].x() * si, p[i-2].y() - m_zMin ) ;
			QVector3D v2( p[i-1].x() * co,  p[i-1].x() * si, p[i-1].y() - m_zMin ) ;
			QVector3D v3( p[i].x() * co,  p[i].x() * si, p[i].y() - m_zMin ) ;
			triangle(v1, v2, v3 ) ;
		}
	}
#else
	// GLU like libtess

	if (( angle() < 360.0f )&& isClosed() ) {

		double pt[ m_pts.size() ][3] ;

		glamTessNew(this) ;
		glamTessBeginContour() ;
//			double ptBegin[ m_pts.size() ][3] ;
			for ( int i = 0 ; i < m_pts.size() ; ++i ) {
//				ptBegin[i][0] = m_pts.at(i).x() * COS( beginAngle ) ;
//				ptBegin[i][1] = m_pts.at(i).x() * SIN( beginAngle ) ;
//				ptBegin[i][2] = m_pts.at(i).y() - m_zMin ;
//				glamTessVertex(ptBegin[i] ) ;
				pt[i][0] = m_pts.at(i).x() * COS( beginAngle ) ;
				pt[i][1] = m_pts.at(i).x() * SIN( beginAngle ) ;
				pt[i][2] = m_pts.at(i).y() - m_zMin ;
				glamTessVertex(pt[i] ) ;
			}
		glamTessEndContour() ;
		glamTessDelete() ;

		glamTessNew(this) ;
		glamTessBeginContour() ;
//			double ptEnd[ m_pts.size() ][3] ;
			for ( int i = 0 ; i < m_pts.size() ; ++i ) {
//				ptEnd[i][0] = m_pts.at(i).x() * COS( endAngle ) ;
//				ptEnd[i][1] = m_pts.at(i).x() * SIN( endAngle ) ;
//				ptEnd[i][2] = m_pts.at(i).y() - m_zMin ;
//				glamTessVertex(ptEnd[i] ) ;
				pt[i][0] = m_pts.at(i).x() * COS( endAngle ) ;
				pt[i][1] = m_pts.at(i).x() * SIN( endAngle ) ;
//				pt[i][2] = m_pts.at(i).y() - m_zMin ;
				glamTessVertex(pt[i] ) ;
			}
		glamTessEndContour() ;
		glamTessDelete() ;
	}
#endif
}

/*! Ressource de placage d'une texture sur la pièce de révolution. */

// calcul des texels tq : surface 2D = numSectors() x numSlice() quads

void GLamRevolution::setTexture(const QString& filename )
{
	this->material().setTexture( filename ) ;

	float xStep = 1.0 / numSectors() ;
	float yStep = 1.0 / numSlices() ;

	for ( uint i = 1 ; i < numSectors() + 1 ; ++i ) {
		for ( uint j = 1 ; j < numSlices() + 1 ; ++j ) {
			QVector2D t1( (i-1) * xStep, (j-1) * yStep ) ;
			QVector2D t2( i * xStep, (j-1) * yStep ) ;
			QVector2D t3( i * xStep, j * yStep ) ;
			QVector2D t4( (i-1) * xStep, j * yStep ) ;
			quadTex(t1, t2, t3, t4 ) ;
		}
	}

#ifdef TEXTUREDQUADTO4TRIANGLES
	// note: sans texture, les facettes d'extrémité sont des triangles
	// lorsque le solide est fermé
	// ( optimisation faite par GLamObject::quad() )
	// maintenant que Material().hasTexture() est vrai,
	// il faut recalculer l'objet afin de forcer les facettes d'ext.
	// à 4 sommets... (cf. GLamObject::quad() )

	updateNeeded() ;
#endif
}
