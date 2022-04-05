/*  ---------------------------------------------------------------------------
 *  filename    :   glamwire.h
 *  description :   INTERFACE de la classe GLamWire
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

// spécialisation de GLamCurve pour construction d'enveloppes
// de types Bézier et B-Spline

#ifndef GLAMWIRE_H
#define GLAMWIRE_H

#include <glamcurve.h>

class BezierCurve	// points et vecteurs au départ et à l'arrivée
{					// d'une enveloppe Bézier
  public:
	BezierCurve(QVector3D p0, QVector3D v0, QVector3D p1, QVector3D v1 )
		: startPoint( p0 )
		, startVector( v0 )
		, endPoint( p1 )
		, endVector( v1 )
	{
	}

	QVector3D	startPoint ;
	QVector3D	startVector ;
	QVector3D	endPoint ;
	QVector3D	endVector ;
} ;

class GLamWire : public GLamCurve
{
  public:
	GLamWire() ;
	GLamWire(const BezierCurve& bezierCurve, qreal diameter, int numSlices = 100 ) ;
	GLamWire(QList<QVector3D> splineCurve, qreal diameter, int numSlices = 100 ) ;
	GLamWire(const GLamWire& obj ) ;
	~GLamWire() ;
	GLamWire& operator =(const GLamWire& obj ) ;

	void setColoredStripe(QColor color ) ;

  protected:
	virtual void create() ;
	virtual void draw(GLamObject* parent = 0 ) ;

  private:
	qreal				m_radius ;
	QList<QVector3D>	m_buildPoints ;
	GLamCurve*			m_stripe ;
} ;

#endif	// GLAMWIRE_H
