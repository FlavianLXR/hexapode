/*  ---------------------------------------------------------------------------
 *  filename    :   glam2d.h
 *  description :   INTERFACE de la classe GLam2D
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
 
#ifndef GLAM2D_H
#define GLAM2D_H

#include <QtMath>
#include <QList>
#include <QPointF>
#include <QSizeF>
#include <QVector2D>
#include <QVector4D>

// note: qCos(M_PI/2) != 0

#define	EPSILON	1e-6
#define	COS(A)	( qAbs( qCos(A) ) < EPSILON ? 0.0 : qCos(A) )
#define SIN(A)	( qAbs( qSin(A) ) < EPSILON ? 0.0 : qSin(A) )
#define SIGN(X) ( (X) >= 0.0f ? 1.0f : -1.0f )

// classe de manipulations d'une liste de sommets 2D

class GLam2D
{
  public:
	GLam2D(const QList<QPointF>& points ) ;
	~GLam2D() {}

	QList<QPointF> shape() const ;
	void translate(qreal xOffset, qreal yOffset ) ;
	void rotate(qreal angle ) ;
	void scale(qreal scale ) ;
	void expand(qreal value ) ;
	void reduce(qreal value ) ;

	static QList<QPointF> translate(const QList<QPointF>& points, qreal xOffset, qreal yOffset ) ;
	static QList<QPointF> rotate(const QList<QPointF>& points, qreal angle ) ;
	static QList<QPointF> scale(const QList<QPointF>& points, qreal scale ) ;
	static QList<QPointF> expand(const QList<QPointF>& points, qreal value ) ;
	static QList<QPointF> reduce(const QList<QPointF>& points, qreal value ) ;

	static QList<QPointF> invert(const QList<QPointF>& points ) ;
	static QList<QPointF> xMirrored(const QList<QPointF>& points ) ;
	static QList<QPointF> yMirrored(const QList<QPointF>& points ) ;

	static bool isClockWise(const QList<QPointF>& points ) ;
	static QList<QPointF> clockWise(const QList<QPointF>& points ) ;
	static QList<QPointF> counterClockWise(const QList<QPointF>& points ) ;

	static QList<QPointF> noDuplicate(QList<QPointF> points ) ;

	static QList<QPointF> bSplineShape(const QList<QPointF>& points, uint numSlices ) ;
	static QList<QPointF> bezierShape(QVector4D v0, QVector4D v1, uint numSlices ) ;
	static QList<QPointF> circularShape(qreal radius, uint numSlices ) ;
	static QList<QPointF> arcShape(qreal radius, qreal startAngle, qreal endAngle, uint numSlices ) ;
	static QList<QPointF> oblongShape(QSizeF size, uint numSlices ) ;
	static QList<QPointF> ellipticShape(QSizeF size, uint numSlices ) ;
	static QList<QPointF> rectangularShape(QSizeF size ) ;


	static QList<QPointF> cornerShape(QPointF a, QPointF b, QPointF c, uint numSlices, qreal radius = 0.0f ) ;
	static QList<QPointF> roundedShape(const QList<QPointF>& points, uint numSlices, qreal radius = 0.0f ) ;

	static QVector4D median(QPointF a, QPointF b, QPointF c ) ;
	static QVector4D normal(QPointF a, QPointF b ) ;
	static QVector4D normal(QVector4D v ) ;
	static QPointF intersection(QVector4D v0, QVector4D v1, bool* err = 0 ) ;
	static qreal angle(QVector2D v0, QVector2D v1 ) ;
	static qreal angle(QPointF a, QPointF b, QPointF c ) ;

	static QList<QPointF> tabToList(qreal* t, int size ) ;

  private:
	QList<QPointF>	m_shape ;
} ;

#endif // GLAM2D_H

/*!
  @class GLam2D
  @brief Utilitaire de manipulation de listes de sommets 2D.

La classe GLam2D encapsule une liste de sommets 2D accesssible par
la méthode shape() ; cette liste est altérée par les méthodes translate(),
rotate(), scale(), expand() et reduce().

GLam2D propose aussi un ensemble de ressources statiques permettant
de fabriquer des formes classiques comme des rectangles, cercles ou
ellipses mais aussi des constructions plus complexes telles que les formes
oblongues ou les courbes gauches de Bézier.
*/
