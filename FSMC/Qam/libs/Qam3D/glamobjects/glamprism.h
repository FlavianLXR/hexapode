/*  ---------------------------------------------------------------------------
 *  filename    :   glamprism.h
 *  description :   INTERFACE de la classe GLamPrism
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

#ifndef GLAMPRISM_H
#define GLAMPRISM_H

#include "glamobject.h"
#include "../glamtools/glampolygon.h"

class GLamPrism : public GLamObject
{
  public:
	GLamPrism() ;
	GLamPrism(const QList<QPointF>& shape, qreal thickness ) ;
	GLamPrism(const QList<QPointF>& baseShape, const QList<QPointF>& topShape, qreal thickness ) ;
	GLamPrism(const GLamPrism& obj ) ;
	~GLamPrism() ;

	GLamPrism& operator =(const GLamPrism& obj ) ;

    /*! Propriété @ref prism_thickness "thickness". */
    qreal thickness() const ;
    /*! Propriété @ref prism_thickness "thickness". */
    void setThickness(qreal thickness ) ;

	void setZCentered(bool centered = true ) ;

	QList<GLamPolygon> polygons(QMatrix4x4 transform = QMatrix4x4() ) const ;

  protected:
	virtual void create() ;
	void resize(qreal x, qreal y, qreal z ) ;

  private:
	QList<QPointF>		m_baseShape ;
	QList<QPointF>		m_topShape ;
	qreal				m_thickness ;
	bool				m_zCentered ;
} ;

#endif // GLAMPRISM_H

/*!
  @class GLamPrism
  @brief Solide prismatique.

La classe GLamPrism permet la construction de solides prismatiques dont la
forme dans le plan 0XY est définie par une liste de points dans l'ordre
anti-horaire (CCW).
@n La forme doit être convexe (pour les contours concaves,
voir la classe GLamSolid).
@n L'épaisseur du solide est sa dimension 0Z+. Le plan du sommet peut avoir
une forme différente de celle de la base.

<hr><h2>Propriétés</h2>

<p>@anchor prism_thickness @c thickness : épaisseur du solide.
- qreal @b thickness () const ;
- void @b setThickness (qreal &a thickness ) ;

<hr><h2>Exemple</h2>
<table border="0" width="100%" cellspacing=20px ><tr>
<td>@image html "glamprism_example.png" width=400px </td>
<td>
@code
// attributs
    GLamPrism*  m_prism[3] ;

// construction
    m_prism[0] = new GLamPrism ;
    m_prism[0]->defaultMatrix().translate(-1.5, 1.5 ) ;

    QList<QPointF> shape ;
    shape << QPointF() << QPointF(0.7,0) << QPointF(0.7,0.7) << QPointF(0,1) ;
    shape << GLam2D::yMirrored( shape ) ;
    shape = GLam2D::noDuplicate( shape ) ;

    m_prism[1] = new GLamPrism(shape, 2 ) ;
    m_prism[1]->defaultMatrix().translate(-1, 0 ) ;
    m_prism[1]->defaultMatrix().rotate(90, 1, 0, 0 ) ;
    m_prism[1]->material().set(GLamMaterial::RedPlastic) ;

    m_prism[2] = new GLamPrism(GLam2D::oblongShape(QSizeF(2,1.2),30), GLam2D::circularShape(0.4, 30), 0.5 ) ;
    m_prism[2]->defaultMatrix().translate(1, 1 ) ;
    m_prism[2]->defaultMatrix().rotate(-40, 0,-1, 1 ) ;
    m_prism[2]->material().set(GLamMaterial::Chrome) ;
    m_prism[2]->material().setAmbient(0.05, 0.25, 0.15 ) ;

// projection
    for ( int i = 0 ; i < 3 ; ++i ) m_prism[i]->draw() ;

@endcode
</td></tr>
</table>

@n
*/
