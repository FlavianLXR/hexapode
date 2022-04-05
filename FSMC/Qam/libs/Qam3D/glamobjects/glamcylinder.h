/*  ---------------------------------------------------------------------------
 *  filename    :   glamcylinder.h
 *  description :   INTERFACE de la classe GLamCylinder
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

#ifndef GLAMCYLINDER_H
#define GLAMCYLINDER_H

#include "glamrevolution.h"

class GLamCylinder : public GLamRevolution
{
  public:
	GLamCylinder() ;
	GLamCylinder(qreal radius, qreal length = 0.0f ) ;
	GLamCylinder(const GLamCylinder& obj ) ;
	~GLamCylinder() ;

	GLamCylinder& operator =(const GLamCylinder& obj ) ;

    /*! Propriété @ref glamcylinder_radius "radius". */
	qreal radius() const ;
    /*! Propriété @ref glamcylinder_radius "radius". */
    void setRadius(qreal radius ) ;

    /*! Propriété @ref glamcylinder_length "length". */
    qreal length() const ;
    /*! Propriété @ref glamcylinder_length "length". */
    void setLength(qreal length ) ;

	void resize(qreal radius, qreal length ) ;

  private:
	void setHalfProfile() ;

  private:
	qreal			m_radius ;
	qreal			m_length ;
} ;

#endif // GLAMCYLINDER_H

/*!
  @class	GLamCylinder
  @brief	Solide de forme cylindrique.

La classe GLamCylinder assure la construction d'un élément 3D dont la section
est un polygone régulier ; l'élément est posé sur le plan 0XY et centré sur l'origine.
Le nombre de facettes du polygone de la section est maintenu par la classe de base.
@n GLamCylinder permet aussi la création de disques 2D en fixant la
longueur à 0.0.

<hr><h2>Propriétés</h2>

<p>@anchor glamcylinder_radius @c radius : Rayon du cylindre.
- qreal @b radius () const ;
- void @b setRadius (qreal @a radius ) ;

<p>@anchor glamcylinder_length @c length : Longueur 0Z du solide.
- qreal @b length () const ;
- void @b setLength (qreal @a length ) ;

<hr><h2>Exemple</h2>
<table border="0" width="100%" cellspacing=20px ><tr>
<td>@image html "glamcylinder_example.png" width=400px </td>
<td>
@code
// attributs
    GLamCylinder*   m_cylinder[8] ;

// construction
    int cylsect[8] = {3, 4, 5, 6, 8, 12, 30, 60 } ;
    for ( int i = 0 ; i < 8 ; ++i ) {
        m_cylinder[i] = new GLamCylinder(0.20, 0.1 * i ) ;
        m_cylinder[i]->setNumSectors( cylsect[i] ) ;
        float x = ( -0.75 + (i % 4) * 0.5 ) * ( i > 3 ? -1 : 1 ) ;
        float y =   -0.25 + (i / 4) * 0.5 ;
        m_cylinder[i]->defaultMatrix().translate(x, y, 0 ) ;
        m_cylinder[i]->material() = GLamMaterial( (GLamMaterial::Material)(i) ) ;
    }

// projection
    for (int i = 0 ; i < 8 ; ++i )  m_cylinder[i]->draw() ;

@endcode
</td></tr>
</table>

@n
*/
