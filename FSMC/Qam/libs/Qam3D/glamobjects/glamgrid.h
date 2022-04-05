/*  ---------------------------------------------------------------------------
 *  filename    :   glamgrid.h
 *  description :   INTERFACE de la classe GLamGrid
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

#ifndef GLAMGRID_H
#define GLAMGRID_H

#include "glamobject.h"

class GLamGrid : public GLamObject
{
  public:
	GLamGrid() ;
	GLamGrid(int xSize, int ySize = 0 ) ;
	GLamGrid(const GLamGrid& obj ) ;
	~GLamGrid() ;

	GLamGrid& operator =(const GLamGrid& obj ) ;

    /*! Propriété @ref grid_size "size". */
	QSize size() const ;
    /*! Propriété @ref grid_size "size". */
    void setSize(int xSize, int ySize = 0 ) ;
    /*! Propriété @ref grid_size "size". */
    void setSize(const QSize& size ) ;
    /*! Propriété @ref grid_step "step". */
    qreal step() const ;
    /*! Propriété @ref grid_step "step". */
    void setStep(qreal step ) ;

	virtual void create() ;

  private:
	int		m_xSize ;
	int		m_ySize ;
	qreal	m_step ;
} ;

#endif // GLAMGRID_H

/*!
  @class	GLamGrid
  @brief	Grille de référence

La classe GLamGrid permet la fabrication d'une grille quadrillée destinée
à fournir une notion d'échelle sur la scène 3D.
@n La grille est dessinée dans le plan 0XY, elle est centrée sur l'origine.

<hr><h2>Propriétés</h2>

<p>@anchor grid_size @c size : Dimensions de la grille (défaut 1x1).
- QSize @b size () const ;
- void @b setSize (int @a xSize, int @a ySize = 0 ) ;
- void @b setSize (const QSize& @a size ) ;

<p>@anchor grid_step @c step : Pas des graduations (défaut 1.0)
- qreal @b step () const ;
- void @b setStep (qreal @a step ) ;

<hr><h2>Exemple</h2>
<table border="0" width="100%" cellspacing=20px ><tr>
<td>@image html "glamgrid_example.png" width=400px </td>
<td>
@code
// attributs
    GLamGrid* m_grid ;

// construction
    m_grid = new GLamGrid ;
    m_grid->material().setSimpleColor() ;

// projection
    float w = 5.0 ;
    float h = 3.0 ;

    // plan 0XY
    m_grid->setSize( (int)w, (int)h ) ;
    m_grid->transformMatrix().setToIdentity() ;
    m_grid->transformMatrix().translate(w / 2, h / 2, 0 ) ;
    m_grid->material().setColor(Qt::blue ) ;
    m_grid->draw() ;

    // plan 0XZ
    m_grid->transformMatrix().rotate(-90, 1, 0, 0 ) ;
    m_grid->transformMatrix().translate(0, -h / 2, -h / 2 ) ;
    m_grid->material().setColor(Qt::green ) ;
    m_grid->draw() ;

    // plan 0YZ
    m_grid->setSize(3) ;
    m_grid->transformMatrix().translate(-w / 2, 0.0, h / 2 ) ;
    m_grid->transformMatrix().rotate( 90, 0, 1, 0 ) ;
    m_grid->material().setColor(Qt::red ) ;
    m_grid->draw() ;

@endcode
</td></tr>
</table>

@n
*/
