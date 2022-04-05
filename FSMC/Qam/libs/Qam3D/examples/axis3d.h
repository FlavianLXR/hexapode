/*  ---------------------------------------------------------------------------
 *  filename    :   axis3d.h
 *  description :   INTERFACE de la classe Axis3D
 *
 *	project     :	Qam3D/GLam: repère 3D orthonormé
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

// modélisation d'un trièdre (repère 3D)

// ressources GLam utilisées --------------------------------------------------

// 	GLamGroup					classe de base de la classe Axis3D
// 		GLamVector				axes orientés

#ifndef AXIS3D_H
#define AXIS3D_H

#include <GLamGroup>

class Axis3D : public GLamGroup
{
  public:
    Axis3D(qreal size = 1.0, qreal diameter = 0.05 ) ;
} ;

#endif	// AXIS3D_H
