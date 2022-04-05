/*  ---------------------------------------------------------------------------
 *  filename    :   thingy.h
 *  description :   INTERFACE de la classe Thingy
 *
 *	project     :	Qam3D/GLam CSG primitives
 *  start date  :   décembre 2009
 *  ---------------------------------------------------------------------------
 *  Copyright 2009-2016 by Alain Menu   <alain.menu@ac-creteil.fr>
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

// exemple de spécialisation de la classe GLamCSG

// ressources GLam utilisées --------------------------------------------------

//	GLamPrism			matière de base avant usinage
//	GLamCylinder		matière de base avant usinage
//	GLamPrism			embase de fixation
//	GLamCylinder		perçage sur embase
//	GLamCylinder		trou central du collier
//	GLamCube			séparation en 2 éléments
//	GLamCylinder		perçage pour accouplement (x2)
//	GLamCylinder		logement hexagonal

#ifndef THINGY_H
#define THINGY_H

#include <GLamGroup>
//#include <GLamObjects>

class Thingy : public GLamCSG
{
  public:
	Thingy() ;
} ;

#endif	// THINGY_H
