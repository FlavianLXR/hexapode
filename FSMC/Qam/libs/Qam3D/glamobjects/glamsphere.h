/*  ---------------------------------------------------------------------------
 *  filename    :   glamsphere.h
 *  description :   INTERFACE de la classe GLamSphere
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

#ifndef GLAMSPHERE_H
#define GLAMSPHERE_H

#include "glamrevolution.h"

class GLamSphere : public GLamRevolution
{
  public:
	GLamSphere() ;
	GLamSphere(qreal diameter, qreal thickness = 0.0f ) ;
	GLamSphere(QSizeF size, qreal thickness = 0.0f ) ;
	GLamSphere(const GLamSphere& obj ) ;
	~GLamSphere() ;

	GLamSphere& operator =(const GLamSphere& obj ) ;

    /*! Propriété @ref sphere_diameter "diameter". */
	qreal diameter() const ;
    /*! Propriété @ref sphere_diameter "diameter". */
    void setDiameter(qreal diameter ) ;
    /*! Propriété @ref sphere_height "height". */
    qreal height() const ;
    /*! Propriété @ref sphere_height "height". */
    void setHeight(qreal height ) ;
    /*! Propriété @ref sphere_size "size". */
    QSizeF size() const ;
    /*! Propriété @ref sphere_size "size". */
    void setSize(QSizeF size ) ;

    /*! Propriété @ref sphere_thickness "thickness". */
    qreal thickness() const ;
    /*! Propriété @ref sphere_thickness "thickness". */
    void setThickness(qreal thickness ) ;

    /*! Propriété @ref sphere_southAngle "southAngle". */
    qreal southAngle() const ;
    /*! Propriété @ref sphere_southAngle "southAngle". */
    void setSouthAngle(qreal southAngle ) ;
    /*! Propriété @ref sphere_northAngle "northAngle". */
    qreal northAngle() const ;
    /*! Propriété @ref sphere_northAngle "northAngle". */
    void setNorthAngle(qreal northAngle ) ;

	void setLatitudes(qreal southAngle, qreal northAngle ) ;

	void setNumSlices(uint numSlices ) ;
	void setNumFaces(uint numFaces ) ;

  private:
	void setHalfProfile() ;

  private:
	qreal			m_diameter ;
	qreal			m_height ;
	qreal			m_thickness ;
	qreal			m_southAngle ;
	qreal			m_northAngle ;
} ;

#endif // GLAMSPHERE_H

/*!
@class GLamSphere
@brief Sphère ou portion sphérique.

La classe GLamSphere permet la création de sphères ou de portions sphériques ;
avec pour centre l'origine 0XYZ.
@n La portion de sphère est définie d'une part par l'angle de révolution autour
de l'axe Z (propriété @c angle de GLamRevolution), et d'autre part par
les amplitudes positive northAngle (défaut +90°) et négative southAngle (défaut +90°)
par rapport à l'axe X. Les valeurs par défaut permettent d'obtenir une sphère
complète.

<hr><h2>Propriétés</h2>

<p>@anchor sphere_diameter @c diameter : Diamètre de la sphère (plan XY).
- qreal @b diameter () const ;
- void @b setDiameter (qreal @a diameter ) ;

<p>@anchor sphere_height @c height : Hauteur de la sphère (plan XZ),
égale par défaut au diamètre.
- qreal @b height () const ;
- void @b setHeight (qreal @a height ) ;

<p>@anchor sphere_size @c size : Diamètre (propriété @c width de QSizeF) et
hauteur (propriété @c height de QSizeF) de la sphère.
- QSizeF @b size () const ;
- void @b setSize (QSizeF @a size ) ;

<p>@anchor sphere_northAngle @c northAngle : Latitude Nord de la portion sphèrique
en degrés (+90° par défaut).
- qreal @b northAngle () const ;
- void @b setNorthAngle (qreal &a northAngle ) ;

<p>@anchor sphere_southAngle @c southAngle : Latitude Sud de la portion sphèrique
en degrés (+90° par défaut).
- qreal @b southAngle () const ;
- void @b setSouthAngle (qreal &a southAngle ) ;

<p>@anchor sphere_thickness @c thickness : épaisseur de matière (interne).
- qreal @b thickness () const ;
- void @b setThickness (qreal &a thickness ) ;

<hr><h2>Exemple</h2>
<table border="0" width="100%" cellspacing=20px ><tr>
<td>@image html "glamsphere_example.png" width=400px </td>
<td>
@code
// attributs
    GLamSphere* m_sphere[4] ;

// construction
    m_sphere[0] = new GLamSphere( 1.2 ) ;
    m_sphere[1] = new GLamSphere( *m_sphere[0] ) ;
    m_sphere[2] = new GLamSphere( *m_sphere[0] ) ;
    m_sphere[3] = new GLamSphere( *m_sphere[0] ) ;

    m_sphere[0]->setName("1:WireFrame" ) ;
    m_sphere[0]->defaultMatrix().translate(-0.75, 0.75 ) ;
    m_sphere[0]->setWireFrame() ;

    m_sphere[1]->setName("2:Flat" ) ;
    m_sphere[1]->setNumFaces(30) ;
    m_sphere[1]->defaultMatrix().translate(0.75, 0.75 ) ;
    m_sphere[1]->material().set(GLamMaterial::GreenRubber) ;
    m_sphere[1]->setShadingEffect(GLamObject::Flat ) ;
    m_sphere[1]->setThickness(0.1) ;
    m_sphere[1]->setAngle(240) ;
    m_sphere[1]->setNorthAngle(45) ;

    m_sphere[2]->setName("3:Phong" ) ;
    m_sphere[2]->defaultMatrix().translate(0.75, -0.75 ) ;
    m_sphere[2]->material().set(GLamMaterial::Bronze) ;
    m_sphere[2]->setShadingEffect(GLamObject::Phong ) ;

    m_sphere[3]->setName("4:Textured" ) ;
    m_sphere[3]->defaultMatrix().translate(-0.75, -0.75 ) ;
    m_sphere[3]->material().setColor(Qt::white) ;
    m_sphere[3]->material().setSimpleColor() ;
    m_sphere[3]->setTexture("<path_to>/earth_16k.jpg") ;
    m_sphere[3]->setShadingEffect(GLamObject::Phong ) ;

// projection
    for (int i = 0 ; i < 4 ; ++i )  m_sphere[i]->draw() ;

// animation
    m_sphere[3]->transformMatrix().rotate(0.25, 0, 0, -1 ) ;

@endcode
</td></tr>
</table>

@n
*/
