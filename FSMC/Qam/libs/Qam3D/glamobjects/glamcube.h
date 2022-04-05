/*  ---------------------------------------------------------------------------
 *  filename    :   glamcube.h
 *  description :   INTERFACE de la classe GLamCube
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

#ifndef GLAMCUBE_H
#define GLAMCUBE_H

#include "glamprism.h"

class GLamCube : public GLamPrism
{
  public:
	GLamCube() ;
	GLamCube(qreal side ) ;
	GLamCube(QSize size, qreal thickness ) ;
	GLamCube(QVector3D size ) ;
	GLamCube(qreal x, qreal y, qreal z ) ;
//	GLamCube(const GLamCube& obj ) ;
	~GLamCube() ;
//	GLamCube& operator =(const GLamCube& obj ) ;
} ;

#endif // GLAMCUBE_H

/*!
  @class GLamCube
  @brief Parallépipède rectangle.

La classe GLamCube spécialise la classe GLamPrism pour la construction de solides
de section constante carrée ou rectangulaire, posés sur le plan 0XY.

<hr><h2>Exemple</h2>
<table border="0" width="100%" cellspacing=20px ><tr>
<td>@image html "glamcube_example.png" width=400px </td>
<td>
@code
// attributs
    GLamCube*  m_cube[3] ;

// construction
    m_cube[0] = new GLamCube ;
    m_cube[0]->setZCentered() ;
    m_cube[0]->defaultMatrix().translate(-1, 1 ) ;

    m_cube[1] = new GLamCube(1.5, 1.2, 0.5 ) ;
    m_cube[1]->defaultMatrix().translate( 1.5, 1 ) ;
    m_cube[1]->material().setColor(0.8, 0.1, 0.0 ) ;

    m_cube[2] = new GLamCube( *m_cube[1] ) ;
    m_cube[2]->setThickness(0.02) ;
    m_cube[2]->defaultMatrix().setToIdentity() ;
    m_cube[2]->defaultMatrix().translate( -0.5, -1 ) ;
    m_cube[2]->defaultMatrix().rotate(-30, 1, 0, 1 ) ;
    m_cube[2]->material().setColor(0.0, 0.8, 0.2 ) ;

// projection
    for ( int i = 0 ; i < 3 ; ++i ) m_cube[i]->draw() ;

@endcode
</td></tr>
</table>

@n
*/
