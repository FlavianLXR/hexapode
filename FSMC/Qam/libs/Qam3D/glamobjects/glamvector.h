/*  ---------------------------------------------------------------------------
 *  filename    :   glamvector.h
 *  description :   INTERFACE de la classe GLamVector
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

#ifndef GLAMVECTOR_H
#define GLAMVECTOR_H

#include "glamrevolution.h"

class GLamVector : public GLamRevolution
{
  public:
	GLamVector() ;
	GLamVector(QVector3D vector ) ;
	GLamVector(const GLamVector& obj ) ;
	~GLamVector() ;

	GLamVector& operator =(const GLamVector& obj ) ;

    /*! Propriété @ref vector_diameter "diameter". */
	qreal diameter() const ;
    /*! Propriété @ref vector_diameter "diameter". */
    void setDiameter(qreal diameter ) ;

    /*! Propriété @ref vector_outLine "outLine". */
    qreal outLine() const ;
    /*! Propriété @ref vector_outLine "outLine". */
    void setOutLine(qreal outLine ) ;

  private:
	void setDirection() ;
	void setHalfProfile() ;

  private:
	QVector3D	m_vector ;
	qreal		m_diameter ;
	qreal		m_outLine ;
} ;

#endif // GLAMVECTOR_H

/*!
  @class GLamVector
  @brief Matérialisation d'un vecteur.

La classe GLamVector permet de matérialiser des flèches orientées dans l'espace
3D. Une flèche est définie principalement par un diamètre et un vecteur 3D ; ce
dernier maintient la direction et la longueur de la flèche.
La propriété @c outLine (défaut 0.03) permet de contrôler le débordement du
début de la flèche par rapport à son point d'origine. Ce débordement peut être
négatif, dans ce cas la flèche débute "au-dessus" de son point d'origine.

<hr><h2>Propriétés</h2>

<p>@anchor vector_diameter @c diameter : diamètre du corps de la flèche.
- qreal @b diameter () const ;
- void @b setDiameter (qreal @a diameter ) ;

<p>@anchor vector_outLine @c outLine : débordement par rapport à l'origine
(peut ètre négatif).
- qreal @b outLine () const ;
- void @b setOutLine (qreal @a outLine ) ;

<hr><h2>Exemple</h2>
<table border="0" width="100%" cellspacing=20px ><tr>
<td>@image html "glamvector_example.png" width=400px </td>
<td>
@code
// attributs
    GLamVector*     m_vector[5] ;

// construction
    m_vector[0] = new GLamVector ;
    for ( int i = 1 ; i <= 4 ; ++i ) {
        m_vector[i] = new GLamVector( QVector3D(2 - 0.75 * i, 0.5 * i, 0.25 * i ) ) ;
        m_vector[i]->setDiameter( 0.02 + 0.005 * i ) ;
        m_vector[i]->setOutLine( -0.1 * i ) ;
        m_vector[i]->material().setColor(0.7, 0.7, 0.25 * i ) ;
    }

// projection
    for (int i = 0 ; i < 5 ; ++i )  m_vector[i]->draw() ;

@endcode
</td></tr>
</table>

@n
*/
