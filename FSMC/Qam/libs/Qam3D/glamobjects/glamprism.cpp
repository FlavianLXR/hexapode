/*  ---------------------------------------------------------------------------
 *  filename    :   glamprism.cpp
 *  description :   IMPLEMENTATION de la classe GLamPrism
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

#include "glamprism.h"

/*! Constructeur par défaut.
 *  Fabrique une forme pyramidale inscrite dans un carré de coté 1
 *  et de hauteur 0.5. */

GLamPrism::GLamPrism()
	: GLamObject()
	, m_thickness( 0.5 )
	, m_zCentered( false )
{
	setClassName("GLamPrism") ;

    m_baseShape << GLam2D::circularShape(m_thickness, 4 ) ;
//	m_topShape << QPointF() << QPointF() << QPointF() << QPointF() ;
    m_topShape << GLam2D::circularShape(0.001, 4 ) ;
}

/*! Constructeur d'un prisme de section 0XY constante définie par la liste
 *  @a shape de coordonnées 2D et d'épaisseur 0Z @a thickness. */

GLamPrism::GLamPrism(const QList<QPointF>& shape, qreal thickness )
	: GLamObject()
	, m_baseShape( shape )
	, m_topShape( shape )
	, m_thickness( qAbs(thickness) )
	, m_zCentered( false )
{
	setClassName("GLamPrism") ;
}

/*! Constructeur d'un prisme de sections 0XY définies à la base par la liste
 *  de coordonnées 2D @a baseShape et au sommet par la liste
 *  @a topShape. L'épaisseur 0Z est spécifiée par @a thickness.
 *  @n Si le nombre de sommets du polygone de base et celui du polygone
 *  supérieur sont différents, les sommets excédents sont ignorés.
 */

GLamPrism::GLamPrism(const QList<QPointF>& baseShape, const QList<QPointF>& topShape, qreal thickness )
	: GLamObject()
	, m_baseShape( baseShape )
	, m_topShape( topShape )
	, m_thickness( qAbs(thickness) )
	, m_zCentered( false )
{
	setClassName("GLamPrism") ;
}

/*! Constructeur de copie. */
GLamPrism::GLamPrism(const GLamPrism& obj )
	: GLamObject()
{
	*this = obj ;
}

/*! Destructeur. */
GLamPrism::~GLamPrism()
{
}

/*! Surcharge de l'opérateur d'affectation. */
GLamPrism& GLamPrism::operator =(const GLamPrism& obj )
{
	*(GLamObject*)this = obj ;
	m_baseShape = obj.m_baseShape ;
	m_topShape = obj.m_topShape ;
	m_thickness = obj.m_thickness ;
	m_zCentered = obj.m_zCentered ;
	updateNeeded() ;
	return *this ;
}

// propriété 'thickness'

qreal GLamPrism::thickness() const
{
    return m_thickness ;
}

void GLamPrism::setThickness(qreal thickness )
{
    m_thickness = qAbs( thickness ) ;
    updateNeeded() ;
}

/*! Permet de centrer ou non le solide par rapport à l'axe 0Z. */
void GLamPrism::setZCentered(bool centered )
{
	m_zCentered = centered ;
	updateNeeded() ;
}

// liste des polygones du solide

/*! Retourne la liste des polygones du solide avec prise en compte
 *  de la transformation spatiale @a transform. */
QList<GLamPolygon> GLamPrism::polygons(QMatrix4x4 transform ) const
{
	QList<GLamPolygon> list ;
	int n = qMin(m_baseShape.size(), m_topShape.size() ) ;

	qreal zMin = ( m_zCentered ? -m_thickness / 2 : 0.0f ) ;
	qreal zMax = ( m_zCentered ?  m_thickness / 2 : m_thickness ) ;

	// base
	QList<QVector3D> lv ;
	for (int i = n - 1 ; i >= 0 ; --i ) {
		QVector3D v(m_baseShape.at(i).x(), m_baseShape.at(i).y(), zMin ) ;
//		lv << transform * v ;
        lv << transform.map(v) ;
    }
	list << GLamPolygon( lv ) ;

	// top
	lv.clear() ;
	for (int i = 0 ; i < n ; ++i ) {
		QVector3D v(m_topShape.at(i).x(), m_topShape.at(i).y(), zMax ) ;
//		lv << transform * v ;
        lv << transform.map(v) ;
    }
	list << GLamPolygon( lv ) ;

	// contour
	for (int i = 1 ; i <= n ; ++i ) {
		int j = i % n ;
		QVector3D v1(m_topShape.at(i-1).x(), m_topShape.at(i-1).y(), zMax ) ;
		QVector3D v2(m_baseShape.at(i-1).x(), m_baseShape.at(i-1).y(), zMin ) ;
		QVector3D v3(m_baseShape.at(j).x(), m_baseShape.at(j).y(), zMin ) ;
		QVector3D v4(m_topShape.at(j).x(), m_topShape.at(j).y(), zMax ) ;
//		list << GLamPolygon(transform * v1, transform * v2, transform * v3, transform * v4 ) ;
        list << GLamPolygon(transform.map(v1), transform.map(v2), transform.map(v3), transform.map(v4) ) ;
    }
	return list ;
}

/*! Retaille le solide sous forme d'un parallépipède rectangle
 *  (section carrée ou rectangulaire) centré sur l'origine. */
void GLamPrism::resize(qreal x, qreal y, qreal z )
{
	m_baseShape.clear() ;
	m_topShape.clear() ;
	m_thickness = qAbs(z) ;

	m_baseShape << QPointF( x / 2,  y / 2 ) << QPointF(-x / 2,  y / 2 )
				<< QPointF(-x / 2, -y / 2 ) << QPointF( x / 2, -y / 2 ) ;
	m_topShape = m_baseShape ;

	updateNeeded() ;
}

/*! Concrétisation de la classe. */
void GLamPrism::create()
{
	int n = qMin(m_baseShape.size(), m_topShape.size() ) ;

	qreal zMin = ( m_zCentered ? -m_thickness / 2 : 0.0f ) ;
	qreal zMax = ( m_zCentered ?  m_thickness / 2 : m_thickness ) ;

	// base
	for (int i = 2 ; i < n ; ++i ) {
		QVector3D v1(m_baseShape.at(0).x(), m_baseShape.at(0).y(), zMin ) ;
		QVector3D v2(m_baseShape.at(i).x(), m_baseShape.at(i).y(), zMin ) ;
		QVector3D v3(m_baseShape.at(i-1).x(), m_baseShape.at(i-1).y(), zMin ) ;
		triangle(v1, v2, v3 ) ;
	}
	// top
	for (int i = 2 ; i < n ; ++i ) {
		QVector3D v1(m_topShape.at(0).x(), m_topShape.at(0).y(), zMax ) ;
		QVector3D v2(m_topShape.at(i-1).x(), m_topShape.at(i-1).y(), zMax ) ;
		QVector3D v3(m_topShape.at(i).x(), m_topShape.at(i).y(), zMax ) ;
		triangle(v1, v2, v3 ) ;
	}
	// contour
	for (int i = 1 ; i <= n ; ++i ) {
		int j = i % n ;
		QVector3D v1(m_topShape.at(i-1).x(), m_topShape.at(i-1).y(), zMax ) ;
		QVector3D v2(m_baseShape.at(i-1).x(), m_baseShape.at(i-1).y(), zMin ) ;
		QVector3D v3(m_baseShape.at(j).x(), m_baseShape.at(j).y(), zMin ) ;
		QVector3D v4(m_topShape.at(j).x(), m_topShape.at(j).y(), zMax ) ;
        quad(v1, v2, v3, v4 ) ;
    }
}
