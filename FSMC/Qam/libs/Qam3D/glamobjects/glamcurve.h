/*  ---------------------------------------------------------------------------
 *  filename    :   glamcurve.h
 *  description :   INTERFACE de la classe GLamCurve
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

// élément de type courbe gauche 3D à section constante
//	- âme du solide :		liste de points (x,y,z)
//	- section du solide :	liste de points (x,y)

// l'âme peut être fermé (premier et dernier points identiques)
// le mode 'sketch' ne montre que les contours des sections...

#ifndef GLAMCURVE_H
#define GLAMCURVE_H

#include "glamobject.h"

class GLamCurve : public GLamObject
{
  public:
	GLamCurve() ;
	GLamCurve(QList<QVector3D> curve, QList<QPointF> shape ) ;
	GLamCurve(const GLamCurve& obj ) ;
	~GLamCurve() ;
	GLamCurve& operator =(const GLamCurve& obj ) ;

    bool isSketched() const { return m_sketch ; }
    void setSketched(bool sketched = true ) ;

  protected:
	virtual void create() ;

    QList<QVector3D> curve() const { return m_curve ; }
    void setCurve(QList<QVector3D> curve ) ;

    QList<QPointF> shape() const { return m_shape ; }
	void setShape(QList<QPointF> shape ) ;

  private:
	QList<QVector3D>	m_curve ;	// points de contruction
	QList<QPointF>		m_shape ;	// section du solide
	bool				m_closed ;	// indicateur de bouclage
	bool				m_sketch ;	// indicateur de mode 'esquisse'
} ;

#endif	// GLAMCURVE_H

/*!
  @class GLamCurve
  @brief Solides de type courbe gauche 3D.

La classe GLamCurve3D permet de construire des éléments gauches de section
constante. L'âme du solide est définie par une liste de points 3D
(propriété @c curve) et la section est spécifiée sous forme d'une liste de
points 2D (propriété shape).

L'âme peut être fermée, le premier et le dernier point sont alors identiques
(propriété @c closed).

Le mode 'esquisse' permet de ne montrer que les contours des sections
(propriété @c sketched).

<hr><h2>Propriétés</h2>

<p>@anchor curve_shape @c shape : section du fil (plan 0XY).
- QList<QPointF> @b shape () const ;
- void @b setShape (QList<QPointF> @a shape ) ;

<hr><h2>Exemple</h2>
<table border="0" width="100%" cellspacing=20px ><tr>
<td>@image html "glamcurve_example.png" width=400px </td>
<td>
@code
// attributs

// construction

// projection

@endcode
</td></tr>
</table>

@n
*/
