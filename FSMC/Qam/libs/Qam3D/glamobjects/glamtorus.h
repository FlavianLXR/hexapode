/*  ---------------------------------------------------------------------------
 *  filename    :   glamtorus.h
 *  description :   INTERFACE de la classe GLamTorus
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

#ifndef GLAMTORUS_H
#define GLAMTORUS_H

#include "glamrevolution.h"

class GLamTorus : public GLamRevolution
{
  public:
	GLamTorus() ;
	GLamTorus(qreal diameter, qreal thickness ) ;
	GLamTorus(qreal diameter, QSizeF size ) ;
	GLamTorus(const GLamTorus& obj ) ;
	~GLamTorus() ;

	GLamTorus& operator =(const GLamTorus& obj ) ;

    /*! Propriété @ref torus_diameter "diameter". */
	qreal diameter() const ;
    /*! Propriété @ref torus_diameter "diameter". */
	void setDiameter(qreal diameter ) ;

    /*! Propriété @ref torus_thickness "thickness". */
	qreal thickness() const ;
    /*! Propriété @ref torus_thickness "thickness". */
	void setThickness(qreal thickness ) ;

    /*! Propriété @ref torus_size "size". */
    QSizeF size() const ;
    /*! Propriété @ref torus_size "size". */
    void setSize(QSizeF size ) ;

	void setNumSlices(uint numSlices ) ;
	void setNumFaces(uint numFaces ) ;

  private:
	void setHalfProfile() ;

  private:
	qreal	m_diameter ;
	qreal	m_thickness ;
	qreal	m_height ;
} ;

#endif // GLAMTORUS_H

/*!
  @class GLamTorus
  @brief Elément torique.

La classe GlamTorus permet de réaliser des tores (toroïdes de section
circulaire). Un tore est défini par son diamètre nomimal et le diamètre de
sa section ; cette dernière caractéristique est maintenue par la propriété
@c thickness.
@n Le diamètre nominal est la distance séparant les axes centraux de deux
sections opposées.
@n La section du tore est elliptique  si sa largeur est rendue différente
de sa hauteur au moyen de la propriété @c size.


<hr><h2>Propriétés</h2>

<p>@anchor torus_diameter @c diameter : diamètre nominal du tore.
- qreal @b diameter () const ;
- void @b setDiameter (qreal @a diameter ) ;

<p>@anchor torus_thickness @c thickness : diamètre de la section du tore.
- qreal @b thickness () const ;
- void @b setThickness (qreal @a thickness ) ;

<p>@anchor torus_size @c size : rectangle d'inscription de la section elliptique.
- QSizeF @b size () const ;
- void @b setSize (QSizeF @a size ) ;

<hr><h2>Exemple</h2>
<table border="0" width="100%" cellspacing=20px ><tr>
<td>@image html "glamtorus_example.png" width=400px </td>
<td>
@code
// attributs
    GLamTorus*      m_torus[3] ;

// construction
    m_torus[0] = new GLamTorus ;

    m_torus[1] = new GLamTorus(1.5, 0.2 ) ;
    m_torus[1]->setNumSectors(90) ;

    m_torus[2] = new GLamTorus(2.3, QSizeF(0.4, 0.2 ) ) ;
    m_torus[2]->material().setColor(0.5, 0.7, 0.4 ) ;
    m_torus[2]->setAngle(240) ;

// projection
    for (int i = 0 ; i < 3 ; ++i )  m_torus[i]->draw() ;

@endcode
</td></tr>
</table>

@n
*/
