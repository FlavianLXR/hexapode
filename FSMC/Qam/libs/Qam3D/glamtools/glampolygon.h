/*  ---------------------------------------------------------------------------
 *  filename    :   glampolygon.h
 *  description :   INTERFACE de la classe GLamPolygon
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
 
#ifndef GLAMPOLYGON_H
#define GLAMPOLYGON_H

#include <QList>
#include <QVector3D>

// polygone spatial (sommets coplanaires)

class GLamPolygon
{
  public:
    GLamPolygon(QVector3D v1, QVector3D v2, QVector3D v3 ) ;
	GLamPolygon(QVector3D v1, QVector3D v2, QVector3D v3, QVector3D v4 ) ;
	GLamPolygon(const QList<QVector3D>& points ) ;

    /*! Retourne les vertices du polygone. */
	inline QList<QVector3D> points() const { return m_points ; }
	QVector3D normal() const ;

  private:
	QList<QVector3D>	m_points ;
} ;

/*!
    @class GLamPolygon
    @brief Polygone spatial.

Classe d'encapsulation d'une facette définie dans l'espace par au moins
3 vertices.

*/

#ifdef GLAMPOLYGON_OLD

#include "glamobject.h"

class GLamHole ;

class GLamPolygon : public GLamObject
{
  public:
	GLamPolygon() ;
	GLamPolygon(qreal thickness, const QList<QPointF>& points, uint numSectors = 0, bool autoCompleted = true ) ;
	GLamPolygon(qreal thickness, QSizeF size, uint numSectors = 0 ) ;
	GLamPolygon(qreal thickness, qreal radius, uint numSectors ) ;
	GLamPolygon(const GLamPolygon& obj ) ;
	~GLamPolygon() ;

	GLamPolygon& operator =(const GLamPolygon& obj ) ;

	qreal thickness() const ;
	void setThickness(qreal thickness ) ;

	void addHole(QPointF position, GLamHole* hole ) ;

	QList<QPointF>	points() const ;

  protected:
	virtual QString className() const { return "GLamPolygon" ; }
	void setPoints(const QList<QPointF>& points ) ;
	virtual void create() ;

  private:
	void createFrontFace() ;
	void createBackFace() ;
	void createEdgeFaces() ;

  private:
	QList<QPointF>		m_pts ;
	qreal				m_thickness ;

	QList<GLamHole*>	m_holes ;
	QList<double*>		m_holesPts ;
} ;

#endif

#endif // GLAMPOLYGON_H
