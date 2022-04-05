/*  ---------------------------------------------------------------------------
 *  filename    :   glam2d.cpp
 *  description :   IMPLEMENTATION de la classe GLam2D
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

#include "glam2d.h"

#include <QDebug>

/*! Constructeur.
 * La liste @a points permet d'initialiser les sommets de la liste
 * interne maintenue par la classe.
 */

GLam2D::GLam2D(const QList<QPointF>& points )
	: m_shape( points )
{
}

/*! Retourne la liste interne de sommets. */

QList<QPointF> GLam2D::shape() const { return m_shape ; }

/*! Décalage planaire de tous les sommets de la liste interne. */

void GLam2D::translate(qreal xOffset, qreal yOffset )
{
	m_shape = GLam2D::translate(m_shape, xOffset, yOffset ) ;
}

/*! Rotation de @a angle degrés de tous les sommets de la liste
 * interne autour du centre du repère 2D de leur définition. */

void GLam2D::rotate(qreal angle )
{
	m_shape = GLam2D::rotate(m_shape, angle ) ;
}

/*! Changement d'échelle pour tous les sommets de la liste interne. */

void GLam2D::scale(qreal scale )
{
	m_shape = GLam2D::scale(m_shape, scale ) ;
}

/*! Expansion des sommets de la liste interne par rapport à l'origine
 * du repère.
 * @sa reduce()
 */

void GLam2D::expand(qreal value )
{
	m_shape = GLam2D::expand(m_shape, value ) ;
}

/*! Réduction des sommets de la liste interne par rapport à l'origine
 * du repère (une réduction est équivalente à une expansion négative).
 * @sa expand()
 */

void GLam2D::reduce(qreal value )
{
	m_shape = GLam2D::expand(m_shape, -value ) ;
}

// ---------------------------------------------------------------------------
//	méthodes statiques
// ---------------------------------------------------------------------------

/*! Retourne une liste de sommets construite à partir de celle en argument après
 * décalage planaire de tous les points suivant @a xOffset et @a yOffset.
 */

QList<QPointF> GLam2D::translate(const QList<QPointF>& points, qreal xOffset, qreal yOffset )
{
	QList<QPointF> pts ;
	for ( int n = 0 ; n < points.size() ; ++n )	{
		pts << QPointF(points.at(n).x() + xOffset, points.at(n).y() + yOffset ) ;
	}
	return pts ;
}

/*! Retourne une liste de sommets construite à partir de celle en argument après
 * rotation de @a angle degrés de tous les points autour du centre du repère.
 */

QList<QPointF> GLam2D::rotate(const QList<QPointF>& points, qreal angle )
{
	QList<QPointF> pts ;
	angle = qDegreesToRadians( angle ) ;
	for ( int n = 0 ; n < points.size() ; ++n )	{
		qreal x = points.at(n).x() ;
		qreal y = points.at(n).y() ;
		pts << QPointF(x * COS(angle) - y * SIN(angle), x * SIN(angle) + y * COS(angle) ) ;
	}
	return pts ;
}

/*! Retourne une liste de sommets construite à partir de celle en argument après
 * mise à l'échelle de tous les points avec le facteur @a scale.
 */

QList<QPointF> GLam2D::scale(const QList<QPointF>& points, qreal scale )
{
	QList<QPointF> pts ;
	for ( int n = 0 ; n < points.size() ; ++n )	{
		pts << QPointF(points.at(n).x() * scale, points.at(n).y() * scale ) ;
	}
	return pts ;
}

/*! Retourne une liste de sommets construite à partir de celle en argument après
 * expansion de @a value de tous les points par rapport à l'origine du repère.
 * @sa reduce()
 */

QList<QPointF> GLam2D::expand(const QList<QPointF>& points, qreal value )
{
	QList<QPointF> pts ;
	if ( points.size() < 3 )	return pts ;

	int n = points.size() ;

	// vecteurs médians en chacun des sommets
	QList<QVector4D> path ;
	path << GLam2D::median(points.last(), points.at(0), points.at(1) ) ;
	for ( int i = 1 ; i < n - 1 ; ++i ) {
		path << GLam2D::median(points.at(i-1), points.at(i), points.at(i+1) ) ;
	}
	path << GLam2D::median(points.at(n-2), points.last(), points.at(0) ) ;

	// si la liste n'est pas un contour fermé
	// les vecteurs aux extrémités sont perpendiculaires...
	if ( points.first() != points.last() ) {
		path.removeFirst() ;
		path.prepend( GLam2D::normal(points.at(1), points.at(0) ) ) ;
		path.removeLast() ;
		path.append( GLam2D::normal(points.at(n-2), points.at(n-1) ) ) ;
	}

	for ( int s = 0 ; s < n ; ++s ) {
		// direction à suivre dans le plan 0XY
		QPointF p0 = points.at(s) ;
		QPointF p1 = points.at( (s + 1) % n ) ;
		QVector2D v(p1.x() - p0.x(), p1.y() - p0.y() ) ;
		// angle entre 0Y et la direction à suivre, en radians
		qreal phi = GLam2D::angle( QVector2D(0,1), v ) ;
		// alignement avec la direction à suivre, au point p0
		QPointF pp1( p0.x() + value * COS(phi), p0.y() + value * SIN(phi) ) ;
		// vecteur aligné en pp1
		QVector4D vp1(pp1.x(), pp1.y(), v.x(), v.y() ) ;
		// projection sur le vecteur d'extrémité amont
		bool err= false ;
		QPointF q0 = GLam2D::intersection(vp1, path.at(s), &err ) ;
		pts << ( err ? p0 : q0 ) ;
		// si le contour est ouvert et que les deux derniers sommets
		// sont en cours de traitement, la projection est aussi
		// réalisée sur le vecteur de fin (aval)
		if (( points.first() != points.last() )&&( s == n - 2 )) {
			QPointF q0 = GLam2D::intersection(vp1, path.at(s+1), &err ) ;
			pts << ( err ? p0 : q0 ) ;
			break ;
		}
	}
	return pts ;
}

/*! Retourne une liste de sommets construite à partir de celle en argument après
 * réduction de @a value de tous les points par rapport à l'origine du repère
 * (une réduction est équivalente à une expansion négative).
 * @sa expand()
 */

QList<QPointF> GLam2D::reduce(const QList<QPointF>& points, qreal value )
{
	return GLam2D::expand(points, -value ) ;
}

/*! Retourne une liste de sommets construite par inversion d'ordre
 * des points de celle reçue en argument (inversion CW <--> CCW).
 * @sa isClockWise(), clockWise(), counterClockWise()
 */

QList<QPointF> GLam2D::invert(const QList<QPointF>& points )
{
	QList<QPointF> pts ;
	pts.clear() ;
	for ( int i = points.size() - 1 ; i >= 0 ; --i ) {
		pts << points.at(i) ;
	}
	return pts ;
}

/*! Retourne une liste de sommets construite par symétrie par rapport
 * à l'axe OX des points de la liste reçue en argument.
 * @sa yMirrored()
 */

QList<QPointF> GLam2D::xMirrored(const QList<QPointF>& points )
{
	QList<QPointF> pts ;
	pts.clear() ;
	for ( int i = points.size() - 1 ; i >= 0 ; --i ) {
		pts << QPointF( points.at(i).x(), -points.at(i).y() ) ;
	}
	return pts ;
}

/*! Retourne une liste de sommets construite par symétrie par rapport
 * à l'axe OY des points de la liste reçue en argument.
 * @sa xMirrored()
 */

QList<QPointF> GLam2D::yMirrored(const QList<QPointF>& points )
{
	QList<QPointF> pts ;
	pts.clear() ;
	for ( int i = points.size() - 1 ; i >= 0 ; --i ) {
		pts << QPointF( -points.at(i).x(), points.at(i).y() ) ;
	}
	return pts ;
}

/*! Indique si la liste de sommets reçue en argument représente
 * un polygone défini suivant l'ordre horaire (CW).
 * @sa clockWise(), counterClockWise()
 */

bool GLam2D::isClockWise(const QList<QPointF>& points )
{
	float sum = 0.0 ;
	for ( int i = 0 ; i < points.size() ; ++i ) {
		QPointF p1 = points.at(i) ;
		QPointF	p2 = points.at( ( i + 1 ) % points.size() ) ;
		sum += ( p2.x() - p1.x() ) * ( p2.y() + p1.y() ) ;
	}
	return ( sum > 0.0 ) ;
}

/*! Retourne une version orientée sens horaire (CW) de la liste
 * de sommets reçue en argument.
 * @sa isClockWise(), invert()
 */

QList<QPointF> GLam2D::clockWise(const QList<QPointF>& points )
{
	return ( isClockWise( points ) ? points : invert( points ) ) ;
}

/*! Retourne une version orientée sens anti-horaire (CCW) de la liste
 * de sommets reçue en argument.
 * @sa isClockWise(), invert()
 */

QList<QPointF> GLam2D::counterClockWise(const QList<QPointF>& points )
{
	return ( !isClockWise( points ) ? points : invert( points ) ) ;
}

/*! Retourne une liste de sommets construite à partir de celle reçue en argument
 * après suppression des éventuels sommets en doublon qu'elle contient.
 */

QList<QPointF> GLam2D::noDuplicate(QList<QPointF> points )
{
	QList<QPointF> pts ;
	pts.clear() ;
	for ( int i = 0 ; i < points.size() ; ++i ) {
		if ( !pts.contains( points.at(i) ) )	pts << points.at(i) ;
	}
	return pts ;
}

/*! Fabrique une liste de sommets correspondant à une B-Spline d'ordre 2
 * définie par une suite de @a points de contrôle et un nombre @a numSlices
 * de facettes entre 2 points de contrôle. Pour mémoire, une B-Spline d'ordre 2
 * est un contour fermé lorsque les 2 premiers points de contrôle sont dupliqués
 * en fin de liste.
 * @n Si @a numSlices vaut 0, la liste fabriquée est restreinte aux seuls points
 * de contrôle.
 */

QList<QPointF> GLam2D::bSplineShape(const QList<QPointF>& points, uint numSlices )
{
	QList<QPointF> pts ;
	if ( !numSlices )	pts = points ;
	else {
		int	m = 2 ;
		while ( m < points.size() ) {
			for ( uint slice = 0 ; slice <= numSlices ; slice++ ) {
				qreal t = slice / (qreal)numSlices ;
				qreal a = 1 / 2.0 - t + qPow( t, 2 ) / 2.0 ;
				qreal b = 1 / 2.0 + t - qPow( t, 2 ) ;
				qreal c = qPow( t, 2 ) / 2.0 ;
				qreal x = a * points[ m - 2 ].x() + b * points[ m - 1 ].x() + c * points[ m ].x() ;
				qreal y = a * points[ m - 2 ].y() + b * points[ m - 1 ].y() + c * points[ m ].y() ;
				pts << QPointF(x, y ) ;
			}
			m++ ;
		}
	}
	return noDuplicate( pts ) ;
}

/*! Fabrique une liste de sommets correspondant à une courbe gauche de Bézier
 * définie par ses points extrèmes, les vecteurs en ces points et un nombre
 * @a numSlices de facettes à répartir du point de départ à celui de fin.
 * @n Le vecteur @a v0 matérialise le point de départ : position (x,y) et
 * vecteur directeur (z,w). Le vecteur @a v1 spécifie le point d'arrivée selon
 * le même principe ; l'enveloppe de  construction de la courbe de Bézier est
 * ainsi définie par 4 points A(v0.x(), v0.y() ), B(v0.x() + v0.z(), v0.y() +
 * v0.w() ), C(v1.x() + v1.z(), v1.y() + v1.w() ) et D(v1.x(), v1.y() ).
 * @n Si @a numSlices vaut 0, la liste fabriquée est restreinte à ces 4 points
 * de contrôle.
 */

QList<QPointF> GLam2D::bezierShape(QVector4D v0, QVector4D v1, uint numSlices )
{
	QList<QPointF> pts ;
	if ( !numSlices ) {
		pts << QPointF(v0.x(), v0.y() ) ;
		pts << QPointF(v0.x() + v0.z(), v0.y() + v0.w() ) ;
		pts << QPointF(v1.x() + v1.z(), v1.y() + v1.w() ) ;
		pts << QPointF(v1.x(), v1.y() ) ;
	}
	else {
		for ( uint slice = 0 ; slice <= numSlices ; slice++ ) {
			qreal t = slice / (qreal)numSlices ;
			qreal a = qPow( 1 - t, 3 ) ;
			qreal b = 3 * t * qPow( 1 - t, 2 ) ;
			qreal c = 3 * qPow(t, 2 ) * ( 1 - t ) ;
			qreal d = qPow(t, 3 ) ;
			qreal x = v0.x() * a + ( v0.x() + v0.z() ) * b + ( v1.x() + v1.z() ) * c + v1.x() * d ;
			qreal y = v0.y() * a + ( v0.y() + v0.w() ) * b + ( v1.y() + v1.w() ) * c + v1.y() * d ;
			pts << QPointF(x, y ) ;
		}
	}
	return pts ;
}

/*! Fabrique une liste de sommets correspondant à un polygone régulier
 * à @a numSlices facettes de rayon @a radius, centré à l'origine.
 * Les facettes (min. 3) sont réparties régulièrement sur la circonférence.
 */

QList<QPointF> GLam2D::circularShape(qreal radius, uint numSlices )
{
	QList<QPointF> pts ;
	numSlices = qMax(numSlices, (uint)3 ) ;
	for ( uint slice = 0 ; slice < numSlices ; slice++ ) {
		float a = 2 * M_PI * slice / numSlices ;
		pts << QPointF( radius * COS(a), radius * SIN(a) ) ;
	}
	return pts ;
}

/*! Fabrique une liste de sommets correspondant à un
 * arc de cercle centré à l'origine défini par un rayon @a radius,
 * des angles de début @a startAngle et de fin @a endAngle en degrés
 * et un nombre @a numSlices de facettes (min. 3) réparties régulièrement.
 */

QList<QPointF> GLam2D::arcShape(qreal radius, qreal startAngle, qreal endAngle, uint numSlices )
{
	QList<QPointF> pts ;
	numSlices = qMax(numSlices, (uint)3 ) ;
	qreal angle = endAngle - startAngle ;
	for ( uint slice = 0 ; slice <= numSlices ; slice++ ) {
		float a = qDegreesToRadians( startAngle + angle * slice / numSlices ) ;
		pts << QPointF( radius * COS(a), radius * SIN(a) ) ;
	}
	return pts ;
}

/*! Fabrique une liste de sommets correspondant à une forme oblongue
 * inscrite dans le rectangle @a size. Le nombre @a numSlices de facettes
 * (min. 4) est partagé entre les 4 quarts de cercle d'extrémités.
 * @n Si @a numSlices vaut 0, la forme obtenue est un simple rectangle.
 */

QList<QPointF> GLam2D::oblongShape(QSizeF size, uint numSlices )
{
	if ( !numSlices ) return GLam2D::rectangularShape(size ) ;

	QList<QPointF> pts ;
	qreal w = size.width() ;
	qreal h = size.height() ;
	uint  n = qMax(numSlices, (uint)4 ) / 4 ;
	qreal radius = qMin( w, h ) / 2 ;
	w = w / 2 - radius ;
	h = h / 2 - radius ;
	for ( uint i = 0 ; i <= n ; ++i ) {
		double a = M_PI / 2 * i / n ;
		pts << QPointF( radius * COS(a) + w, radius * SIN(a) + h ) ;
	}
	for ( uint i = 0 ; i <= n ; ++i ) {
		double a = M_PI / 2 * i / n + M_PI / 2 ;
		pts << QPointF( radius * COS(a) - w, radius * SIN(a) + h ) ;
	}
	for ( uint i = 0 ; i <= n ; ++i ) {
		double a = M_PI / 2 * i / n + M_PI ;
		pts << QPointF( radius * COS(a) - w, radius * SIN(a) - h ) ;
	}
	for ( uint i = 0 ; i <= n ; ++i ) {
		double a = M_PI / 2 * i / n + 3 * M_PI / 2 ;
		pts << QPointF( radius * COS(a) + w, radius * SIN(a) - h ) ;
	}

	return noDuplicate( pts ) ;
}

/*! Fabrique une liste de sommets correspondant à une forme elliptique
 * inscrite dans le rectangle @a size. Le nombre @a numSlices de facettes
 * (min. 4) est réparti sur la circonférence.
 * @n Si @a numSlices vaut 0, la forme obtenue est un simple rectangle.
 */

QList<QPointF> GLam2D::ellipticShape(QSizeF size, uint numSlices )
{
	if ( !numSlices ) return GLam2D::rectangularShape(size ) ;

	QList<QPointF> pts ;
	qreal rw = size.width() / 2 ;
	qreal rh = size.height() / 2 ;

	numSlices = qMax(numSlices, (uint)4 ) ;
	for ( uint slice = 0 ; slice < numSlices ; slice++ ) {
		float a = 2 * M_PI * slice / numSlices ;
		if ( rw >= rh )	pts << QPointF(  rw * COS(a), rh * SIN(a) ) ;
		else			pts << QPointF( -rw * SIN(a), rh * COS(a) ) ;
	}
	return pts ;
}

/*! Fabrique une liste de sommets correspondant à une forme rectangulaire
 * définie par largeur et hauteur de @a size.
 */

QList<QPointF> GLam2D::rectangularShape(QSizeF size )
{
	QList<QPointF> pts ;
	qreal w = size.width() ;
	qreal h = size.height() ;

	pts << QPointF( w / 2, h / 2 ) ;
	pts << QPointF(-w / 2, h / 2 ) ;
	pts << QPointF(-w / 2, -h / 2 ) ;
	pts << QPointF( w / 2, -h / 2 ) ;

	return pts ;
}

/*! Fabrique une liste de sommets correspondant à un
 * coin de forme anguleuse (@a numSlices == 0), biseautée (@a numSlices == 1)
 * ou arrondie (@a numSlices > 1) au point @a b entre les segments /ab et /bc
 * de rayon @a radius ou min(ab,bc) si @a radius == 0.
 */

QList<QPointF> GLam2D::cornerShape(QPointF a, QPointF b, QPointF c, uint numSlices, qreal radius )
{
	QList<QPointF> pts ;

	// forme anguleuse ?
	if ( !numSlices ) { pts << b ; return pts ; }

	// détermination du rayon
	if ( radius == 0.0f ) {
		qreal ab = QVector2D(b.x() - a.x(), b.y() - a.y() ).length() ;
		qreal bc = QVector2D(c.x() - b.x(), c.y() - b.y() ).length() ;
		radius = qMin(ab, bc ) ;
	}

	// vecteur médian entre /ab et /cb
	bool isConcave = ( GLam2D::angle(a,b,c) < 0 ) ;
	QVector4D v = isConcave ? GLam2D::median(a, b, c ) : GLam2D::median(c, b, a ) ;
	QVector2D vm( v.z(), v.w() ) ;
	vm.normalize() ;
	// angle entre /ba et /bc
	QVector2D vba( a.x() - b.x(), a.y() - b.y() ) ;
	QVector2D vbc( c.x() - b.x(), c.y() - b.y() ) ;
	vba.normalize() ;
	vbc.normalize() ;
	qreal beta  = GLam2D::angle(vba, vbc ) ;

	// triangle rectangle
	// centre o, op = radius et angle obp = beta / 2
	qreal bo = radius / SIN( qAbs( beta / 2.0f ) ) ;
	vm *= bo ;
	vba *= bo * COS( qAbs( beta / 2.0f ) ) ;
	// centre de rotation
	QPointF center(b.x() + vm.x(), b.y() + vm.y() ) ;
	// point de départ sur segment /ab
	QPointF p(b.x() + vba.x() - center.x(), b.y() + vba.y() - center.y() ) ;
	// angle à parcourir
	beta = isConcave ? beta - M_PI : M_PI + beta ;

	// secteurs
	for ( uint i = 0 ; i <= numSlices ; ++i ) {
		qreal phi = beta * i / numSlices ;
		qreal x = p.x() * COS(phi) - p.y() * SIN(phi) ;
		qreal y = p.x() * SIN(phi) + p.y() * COS(phi) ;
		pts << QPointF( x + center.x(), y + center.y() ) ;
	}

	return pts ;
}

/*! Fabrique une liste de sommets correspondant à un
 * contour fermé dont les sommets @a points sont arrondis/biseautés par
 * cornerShape() à qui sont transmis les paramètres @a numSlices et @a radius.
 */

QList<QPointF> GLam2D::roundedShape(const QList<QPointF>& points, uint numSlices, qreal radius )
{
	QList<QPointF> pts ;

	int imax = points.size() - 1 ;
	for ( int i = 0 ; i <= imax ; ++i ) {
		if ( radius == 0.0f ) {
			pts << points.at(i) ;
			continue ;
		}
		QPointF a = points.at( (i + imax) % points.size() ) ;
		QPointF b = points.at(i) ;
		QPointF c = points.at( (i + 1) % points.size() ) ;
		pts += GLam2D::cornerShape(a, b, c, numSlices, radius ) ;
	}
	return pts ;
}

/*! Ressource de calcul du
 * vecteur médian au point @a b des segments /ab et /cb avec
 * QVector4D(x,y) = position , QVector4D(z,w) = vecteur directeur.
 * le vecteur est toujours orienté à droite du parcours a -> b -> c.
 */

QVector4D GLam2D::median(QPointF a, QPointF b, QPointF c )
{
	QVector4D v( b ) ;
	QVector2D vab(b.x() - a.x(), b.y() - a.y() ) ;
	QVector2D vcb(b.x() - c.x(), b.y() - c.y() ) ;
	vab.normalize() ;
	vcb.normalize() ;
	// déterminant nul ==> vecteurs colinéaires (parallèles)
	qreal det = vab.x() * vcb.y() - vab.y() * vcb.x() ;
	// vecteur normal à /ab ?
	if ( qAbs( det ) < EPSILON ) {
		v.setZ( vab.y() ) ;
		v.setW(-vab.x() ) ;
	}
	// direction du vecteur
	else {
		v.setZ(vab.x() + vcb.x() ) ;
		v.setW(vab.y() + vcb.y() ) ;
	}
	// sens du vecteur
	if ( angle(a,b,c) < 0.0f ) {
		v.setZ( -v.z() ) ;
		v.setW( -v.w() ) ;
	}
	return v ;
}

/*! Ressource de calcul du
 * vecteur au point @a b normal à droite du segment orienté /ab avec
 * QVector4D(x,y) = position , QVector4D(z,w) = vecteur directeur.
 */

QVector4D GLam2D::normal(QPointF a, QPointF b )
{
	QPointF c ;	// symétrie de a p/r b
	c.setX( 2 * b.x() - a.x() ) ;
	c.setY( 2 * b.y() - a.y() ) ;
	return median(a, b, c ) ;
}

/*! Ressource de calcul du
 * vecteur au point v(x,y) normal à v(z,w).
 * La position QVector4D(x,y) reste inchangée.
 */

QVector4D GLam2D::normal(QVector4D v )
{
	QPointF a(v.x() - v.z(), v.y() - v.w() ) ;
	QPointF b(v.x(), v.y() ) ;
	QPointF c(v.x() + v.z(), v.y() + v.w() ) ;
	return median(a, b, c ) ;
}

/*! Ressource de calcul du
 * point d'intersection entre la droite définie par @a v0
 * et celle définie par @a v1 (avec
 * QVector4D(x,y) = position , QVector4D(z,w) = vecteur directeur).
 * @n @a err = true si les vecteurs sont colinéaires ;
 * sinon, @a err non modifié.
 */

QPointF GLam2D::intersection(QVector4D v0, QVector4D v1, bool* err )
{
	// déterminant nul ==> vecteurs colinéaires (parallèles)
	qreal det = v0.z() * v1.w() - v0.w() * v1.z() ;
	if ( qAbs( det ) < EPSILON ) {
		if ( err )	*err = true ;
		return QPointF() ;
	}

	qreal a0, b0, a1, b1 ;

	// cas avec un des vecteurs vertical
	if ( v0.z() == 0 ) {
		a1 = v1.w() / v1.z() ;
		b1 = v1.y() - a1 * v1.x() ;
		return QPointF(v0.x(), a1 * v0.x() + b1 ) ;
	}
	if ( v1.z() == 0 ) {
		a0 = v0.w() / v0.z() ;
		b0 = v0.y() - a0 * v0.x() ;
		return QPointF(v1.x(), a0 * v1.x() + b0 ) ;
	}

	a0 = v0.w() / v0.z() ;
	b0 = v0.y() - a0 * v0.x() ;
	a1 = v1.w() / v1.z() ;
	b1 = v1.y() - a1 * v1.x() ;

	qreal x = ( b1 - b0 ) / ( a0 - a1 ) ;
	qreal y = a0 * x + b0 ;
	return QPointF(x, y ) ;
}

/*! Ressource de calcul de
 * l'angle signé (en radians) du vecteur @a v0 vers le vecteur @a v1.
 */

qreal GLam2D::angle(QVector2D v0, QVector2D v1 )
{
	v0.normalize() ;	// ||A|| = 1
	v1.normalize() ;	// ||B|| = 1
	// angle nul ?
	if ( v0 == v1 )	return 0.0f ;
	qreal dx = qAbs( v0.x() - v1.x() ) ;
	qreal dy = qAbs( v0.y() - v1.y() ) ;
	if (( dx < EPSILON )&&( dy < EPSILON )) return 0.0f ;
	// produit scalaire (dot product)
	qreal dp = v0.x() * v1.x() + v0.y() * v1.y() ;
	// cos(phi) = ( A.B ) / ( ||A||.||B|| )
	qreal phi = qAcos( dp ) ;
	// déterminant (signe de phi = signe du déterminant)
	qreal det = v0.x() * v1.y() - v0.y() * v1.x() ;
	// vecteurs opposés ?
	if ( qAbs( det ) < EPSILON ) return M_PI ;
	// angle en radians
	return SIGN( det ) * phi ;
}

/*! Ressource de calcul de
 * l'angle signé (en radians) de /ab vers /bc.
 */

qreal GLam2D::angle(QPointF a, QPointF b, QPointF c )
{
	return angle(	QVector2D(b.x() - a.x(), b.y() - a.y() ),
					QVector2D(c.x() - b.x(), c.y() - b.y() ) ) ;
}

/*! Ressource de conversion tableau de réels --> liste de sommets.
 */

QList<QPointF> GLam2D::tabToList(qreal* t, int size )
{
	QList<QPointF>	pts ;
	for ( int i = 1 ; i < size ; i += 2 ) pts << QPointF(t[i-1], t[i] ) ;
	return pts ;
}
