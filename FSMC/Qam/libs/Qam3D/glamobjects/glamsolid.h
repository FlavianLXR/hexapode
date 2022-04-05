/*  ---------------------------------------------------------------------------
 *  filename    :   glamsolid.h
 *  description :   INTERFACE de la classe GLamSolid
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

#ifndef GLAMSOLID_H
#define GLAMSOLID_H

#include "glamobject.h"
#include "glamprofile.h"
#include "glamhole.h"

class GLamSolid : public GLamObject
{
  public:
	GLamSolid() ;
	GLamSolid(const QList<QPointF>& shape, qreal thickness = 0.0f ) ;
	GLamSolid(const GLamSolid& obj ) ;
	~GLamSolid() ;

	GLamSolid& operator =(const GLamSolid& obj ) ;

    /*! Propriété @ref solid_thickness "thickness". */
	inline qreal thickness() const { return m_thickness ; }
    /*! Propriété @ref solid_shape "shape". */
    inline QList<QPointF> shape() const { return m_shape ; }
    /*! Propriété @ref solid_profile "profile". */
    inline GLamProfile profile() const { return *m_profile ; }

    /*! Propriété @ref solid_thickness "thickness". */
    void setThickness(qreal thickness ) ;
    /*! Propriété @ref solid_shape "shape". */
    void setShape(QList<QPointF> shape ) ;
    /*! Propriété @ref solid_profile "profile". */
    void setProfile(const GLamProfile* profile ) ;

	void addHole(const GLamHole* hole, QPointF position = QPointF() ) ;

  protected:
	virtual void create() ;

  private:
	void createEdgeFaces() ;

  private:
	QList<QPointF>		m_shape ;
	qreal				m_thickness ;
	GLamProfile*		m_profile ;
	bool				m_hasDefaultProfile ;
	QList<GLamHole*>	m_holes ;
	QList<QPointF>		m_holesOffset ;
} ;

#endif // GLAMSOLID_H

/*!
  @class GLamSolid
  @brief Classe de base des solides d'épaisseur 0Z+.

Un solide est matérialisé par son contour @c shape dans le plan 0XY et
par son épaisseur @c thickness suivant 0Z+. Le profil extérieur est par défaut
'droit vertical', il peut être modifié au moyen d'un objet GLamProfile adpaté.
@n Un solide peut subir des enlèvements de matière sous la forme d'objets GLamHole
de formes et de profils intérieurs différents.
@n Contrairement à GLamPrism, la classe GLamSolid met en oeuvre un mécanisme
de tessellation triangulaire qui autorise les formes concaves. La liste de points
de définition du contour doit être fournie dans l'ordre anti-horaire (CCW).
@n Les solides peuvent être complétés par usinages (trous) et des bordures
intérieures et extérieures ; voir GLamProfile et GLamHole.

<hr><h2>Propriétés</h2>

<p>@anchor solid_shape @c shape : contour (plan 0XY).
- QList<QPointF> @b shape () const ;
- void @b setShape (QList<QPointF> @a shape ) ;

<p>@anchor solid_thickness @c thickness : épaisseur de matière.
- qreal @b thickness () const ;
- void @b setThickness (qreal @a thickness ) ;

<p>@anchor solid_profile @c profile : Profilé extérieur (habillage de la tranche
du solide).
- GLamProfile @b profile () const ;
- void @b setProfile (const GLamProfile* @a profile ) ;

<hr><h2>Exemple</h2>
<table border="0" width="100%" cellspacing=20px ><tr>
<td>@image html "glamsolid_example.png" width=400px </td>
<td>
@code
// attributs
    GLamSolid*  m_solid[3] ;

// construction
    QList<QPointF> pts ;

    pts = GLam2D::rectangularShape(QSizeF(2.4, 1.2 ) ) ;    // rectangle
    pts.insert(1, QPointF(-0.5, 0.6 ) ) ;                   // forme queue d'aronde
    pts.insert(1, QPointF(-0.6, 0.3 ) ) ;
    pts.insert(1, QPointF( 0.6, 0.3 ) ) ;
    pts.insert(1, QPointF( 0.5, 0.6 ) ) ;
    pts = GLam2D::roundedShape(pts, 1, 0.025 ) ;            // chanfreins

    m_solid[0] = new GLamSolid(pts, 0.25 ) ;
    m_solid[0]->defaultMatrix().translate(-1, 1.5 ) ;
    m_solid[0]->material().set( GLamMaterial::Ruby ) ;

    m_solid[1] = new GLamSolid( GLam2D::circularShape(1, 90 ), 0.6 ) ;
    m_solid[1]->defaultMatrix().translate(-1, -1 ) ;
    m_solid[1]->material().setColor(0.565, 0.933, 0.565 ) ;

    qreal hh = 2 ;                                          // coeur
    qreal ww = 2 ;
    qreal coeur[][2] = {
        {0,-2*hh/3}, {0,-2*hh/3}, {ww/8,-hh/3}, {ww/2,0},
        {3*ww/8,hh/3}, {ww/10,hh/3}, {0,hh/5}, {0,hh/5},
        {-ww/10,hh/3}, {-3*ww/8,hh/3}, {-ww/2,0}, {-ww/8,-hh/3},
        {0,-2*hh/3}, {0,-2*hh/3} } ;

    pts.clear() ;
    for ( int i = 0 ; i < 14 ; ++i )  pts << QPointF( coeur[i][0], coeur[i][1] ) ;

    m_solid[2] = new GLamSolid( GLam2D::bSplineShape(pts, 100 ), 0.5 ) ;
    m_solid[2]->material().setColor(1.0, 0.753, 0.796 ) ;
    m_solid[2]->defaultMatrix().translate(2, -0.5 ) ;
    m_solid[2]->defaultMatrix().rotate(-30, 0, 1, 0 ) ;

// projection
    for( int i = 0 ; i < 3 ; ++i )  m_solid[i]->draw() ;

@endcode
</td></tr>
</table>

@n
*/
