/*  ---------------------------------------------------------------------------
 *  filename    :   glampipe.h
 *  description :   INTERFACE de la classe GLamPipe
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

#ifndef GLAMPIPE_H
#define GLAMPIPE_H

#include "glamrevolution.h"

class GLamPipe : public GLamRevolution
{
  public:
	GLamPipe() ;
	GLamPipe(qreal diameter, qreal thickness, qreal length = 0.0f ) ;
	GLamPipe(qreal baseDiameter, qreal topDiameter, qreal thickness, qreal length ) ;
	GLamPipe(const GLamPipe& obj ) ;
	~GLamPipe() ;

	GLamPipe& operator =(const GLamPipe& obj ) ;

    /*! Propriété @ref pipe_baseDiameter "baseDiameter". */
    qreal baseDiameter() const ;
    /*! Propriété @ref pipe_baseDiameter "baseDiameter". */
    void setBaseDiameter(qreal baseDiameter ) ;

    /*! Propriété @ref pipe_topDiameter "topDiameter". */
    qreal topDiameter() const ;
    /*! Propriété @ref pipe_topDiameter "topDiameter". */
    void setTopDiameter(qreal topDiameter ) ;

    /*! Propriété @ref pipe_length "length". */
    qreal length() const ;
    /*! Propriété @ref pipe_length "length". */
    void setLength(qreal length ) ;

    /*! Propriété @ref pipe_thickness "thickness". */
    qreal thickness() const ;
    /*! Propriété @ref pipe_thickness "thickness". */
    void setThickness(qreal thickness ) ;

  private:
	void setHalfProfile() ;

  private:
	qreal			m_baseRadius ;
	qreal			m_topRadius ;
	qreal			m_length ;
	qreal			m_thickness ;
} ;

#endif // GLAMPIPE_H

/*!
  @class GLamPipe
  @brief Elément tubulaire ou conique.

Cette classe nommée « tuyau » permet en réalité de construire autour de l'axe Z
une grande variété d'éléments 3D dont les coupes transversales sont des polygones
réguliers : des formes pleines, des tubes creux de sections diverses avec contrôle
de l'épaisseur de matière, des cônes ou troncs de cônes, …
@n Un objet de classe GLamPipe est construit en indiquant un diamètre de base, un
diamètre final, une longueur et une épaisseur de matière.
@n L'épaisseur correspond ici à la différence entre rayon extérieur et rayon
intérieur pour toute coupe transversale de l'objet.

<hr><h2>Propriétés</h2>

<p>@anchor pipe_baseDiameter @c baseDiameter : Diamètre extérieur du tube.
- qreal @b baseDiameter () const ;
- void @b setBaseDiameter (qreal @a baseDiameter ) ;

<p>@anchor pipe_topDiameter @c topDiameter : Diamètre extérieur final du solide,
différent de baseDiameter dans le cas d'un tronc de cône.
- qreal @b topDiameter () const ;
- void @b setTopDiameter (qreal @a topDiameter ) ;

<p>@anchor pipe_length @c length : Longueur 0Z du solide.
- qreal @b length () const ;
- void @b setLength (qreal @a length ) ;

<p>@anchor pipe_thickness @c thickness : épaisseur de matière (interne).
- qreal @b thickness () const ;
- void @b setThickness (qreal @a thickness ) ;

<hr><h2>Exemple</h2>
<table border="0" width="100%" cellspacing=20px ><tr>
<td>@image html "glampipe_example.png" width=400px </td>
<td>
@code
// attributs
    GLamPipe*   m_pipe[4] ;

// construction
    // tube par défaut
    m_pipe[0] = new GLamPipe ;
    m_pipe[0]->defaultMatrix().translate(-1,  1 ) ;

    // tube carré
    m_pipe[1] = new GLamPipe(1.6, 0.1, 1.5)  ;
    m_pipe[1]->setNumSectors(4) ;
    m_pipe[1]->material().setColor( 0.3f, 0.9f, 0.9f ) ;
    m_pipe[1]->defaultMatrix().translate( 1,  1 ) ;
    m_pipe[1]->defaultMatrix().rotate(45, 0, 0, 1 ) ;

    // tronc de cône
    m_pipe[2] = new GLamPipe(1.4, 0.8, 0.05, 1 ) ;
    m_pipe[2]->material().setColor( 0.9f, 0.9f, 0.3f ) ;
    m_pipe[2]->defaultMatrix().translate( 1, -1 ) ;
    m_pipe[2]->setAngle(270) ;

    // pointe pleine
    m_pipe[3] = new GLamPipe(1, 0, 0.5, 1.5 ) ; ;
    m_pipe[3]->material().setColor( 0.9f, 0.3f, 0.9f ) ;
    m_pipe[3]->defaultMatrix().translate(-1, -1 ) ;
    m_pipe[3]->setAngle(300) ;

// projection
    for ( int i = 0 ; i < 4 ; ++i ) m_pipe[i]->draw() ;

@endcode
</td></tr>
</table>

@n
*/
