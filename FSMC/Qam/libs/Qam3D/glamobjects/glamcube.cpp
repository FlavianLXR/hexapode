/*  ---------------------------------------------------------------------------
 *  filename    :   glamcube.cpp
 *  description :   IMPLEMENTATION de la classe GLamCube
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

#include "glamcube.h"

/*! Constructeur par défaut.
 *  Fabrique un cube de coté 0.5 posé à l'origine sur le plan 0XY. */
GLamCube::GLamCube()
	: GLamPrism()
{
	setClassName("GLamCube") ;
    resize(0.5f, 0.5f, 0.5f ) ;
}

/*! Constructeur d'un cube de coté @a side. */
GLamCube::GLamCube(qreal side )
	: GLamPrism()
{
	setClassName("GLamCube") ;
	resize(side, side, side ) ;
}

/*! Constructeur d'un parallépipède de section @a size et
 *  de hauteur @a thickness. */
GLamCube::GLamCube(QSize size, qreal thickness )
	: GLamPrism()
{
	setClassName("GLamCube") ;
	resize(size.width(), size.height(), thickness ) ;
}

/*! Constructeur d'un parallépipède de dimensions @a size. */
GLamCube::GLamCube(QVector3D size )
	: GLamPrism()
{
	setClassName("GLamCube") ;
	resize(size.x(), size.y(), size.z() ) ;
}

/*! Constructeur d'un parallépipède de dimensions @a x, @a y, @a z. */
GLamCube::GLamCube(qreal x, qreal y, qreal z )
	: GLamPrism()
{
	setClassName("GLamCube") ;
	resize(x, y, z ) ;
}

/*! Destructeur. */
GLamCube::~GLamCube()
{
}

