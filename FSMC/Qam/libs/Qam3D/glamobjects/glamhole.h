/*  ---------------------------------------------------------------------------
 *  filename    :   glamhole.h
 *  description :   INTERFACE de la classe GLamHole
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

#ifndef GLAMHOLE_H
#define GLAMHOLE_H

#include "glamobject.h"

#include "glamprofile.h"
#include "glamsolid.h"

class GLamHole : public GLamObject
{
  public:
	GLamHole() ;
	GLamHole(const QList<QPointF>& shape, qreal thickness = 1.0f ) ;
	GLamHole(const GLamHole& obj ) ;
	~GLamHole() ;

	GLamHole& operator =(const GLamHole& obj ) ;

    /*! Propriété @ref hole_shape "shape". */
    inline QList<QPointF> shape() const { return m_shape ; }
    /*! Propriété @ref hole_shape "shape". */
    void setShape(QList<QPointF> shape ) ;

    /*! Propriété @ref hole_thickness "thickness". */
    inline qreal thickness() const { return m_thickness ; }
    /*! Propriété @ref hole_thickness "thickness". */
    void setThickness(qreal thickness ) ;

    /*! Propriété @ref hole_profile "profile". */
    inline GLamProfile profile() const { return *m_profile ; }
    /*! Propriété @ref hole_profile "profile". */
    void setProfile(const GLamProfile* profile )  ;

  protected:
	virtual void create() ;

  private:
	friend class GLamSolid ;

	QList<QPointF>	m_shape ;
	GLamProfile*	m_profile ;
	bool			m_hasDefaultProfile ;
	qreal			m_thickness ;
} ;

#endif // GLAMHOLE_H

/*!
  @class GLamHole
  @brief Profilé intérieur.

La classe GLamHole permet de matérialiser des enlèvements de matière sur
des objets de classe GLamSolid.
Un GLamHole est un objet indépendant défini par une forme
dans le plan 0XY (propriété @c shape) et une épaisseur 0Z+ (propriété @c thickness)
qui vaut par défaut 1.0.
Le profil intérieur peut être modifié, il est par défaut 'droit vertical'.

<hr><h2>Propriétés</h2>

<p>@anchor hole_shape @c shape : contour (plan 0XY).
- QList<QPointF> @b shape () const ;
- void @b setShape (QList<QPointF> @a shape ) ;

<p>@anchor hole_thickness @c thickness : épaisseur de matière.
- qreal @b thickness () const ;
- void @b setThickness (qreal @a thickness ) ;

<p>@anchor hole_profile @c profile : Profilé extérieur (habillage de la tranche
du solide).
- GLamProfile @b profile () const ;
- void @b setProfile (const GLamProfile* @a profile ) ;

<hr><h2>Exemple</h2>
<table border="0" width="100%" cellspacing=20px ><tr>
<td>@image html "GLamHole_example.png" width=400px </td>
<td>
@code
// <<< SUITE DE L'EXEMPLE DE LA CLASSE GLamProfile >>>

// attributs
    GLamSolid*  m_hole[3] ;

// construction
    m_hole[0] = new GLamSolid( *m_profile[0] ) ;
    GLamHole hole( GLam2D::oblongShape(QSizeF(1.6, 0.4 ), 24 ) ) ;
    edge = m_profile[0]->profile() ;
    hole.setProfile( &edge ) ;
    m_hole[0]->addHole( &hole, QPointF(0, -0.25 ) ) ;

    m_hole[1] = new GLamSolid( *m_profile[1] ) ;
    hole = GLamHole( GLam2D::circularShape(0.2, 60) ) ;
    m_hole[1]->addHole( &hole ) ;

    pts.clear() ;
    pts << QPointF(0, 0.1 ) << QPointF(0.5, 0.95 ) << QPointF(-0.5, 0.95 ) ;
    pts << QPointF(0, 0.1 ) << QPointF(0.5, 0.95 ) ;
    for ( int i = 0 ; i < 5 ; ++i ) {
        GLamHole hole( GLam2D::bSplineShape(pts, 20) ) ;
        m_hole[1]->addHole( &hole ) ;
        pts = GLam2D::rotate(pts, 360 / 5 ) ;
    }

    m_hole[2] = new GLamSolid( *m_profile[2] ) ;
    hole = GLamHole( GLam2D::scale(m_profile[2]->shape(), 0.8 ) ) ;
    edge = m_profile[2]->profile() ;
    hole.setProfile( &edge ) ;
    m_hole[2]->addHole( &hole ) ;

// projection
    for ( int i = 0 ; i < 3 ; ++i ) m_hole[i]->draw() ;

@endcode
</td></tr>
</table>

@n
*/
