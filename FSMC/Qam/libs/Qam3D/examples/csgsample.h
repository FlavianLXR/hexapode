/*  ---------------------------------------------------------------------------
 *  filename    :   csgsample.h
 *  description :   INTERFACE de la classe CsgSample
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

// exemple de composition d'objets GLamCSG

// ressources GLam utilisées --------------------------------------------------

//	GLamCube		volume de base A (affiché)
//	GLamSphere		volume de base B (affiché)
//	GLamCylinder	volume de base C (affiché)
//	GLamSphere		volume de base D (interne), copie de B
//	GLamCylinder	volume de base E (interne), copie de C
//	GLamCSG			tableau de 6 objets
//					fabriqués par opérations entre A, B, C, D et E

#ifndef CSGSAMPLE_H
#define CSGSAMPLE_H

#include <GLamGroup>
#include <GLamObjects>

class CsgSample
{
  public:
    CsgSample(qreal defaultScale ) ;
	void draw() ;

  private:
	GLamCube*		m_primBox ;
	GLamSphere*		m_primSph ;
	GLamCylinder*	m_primCyl ;
	GLamCSG*		m_csg[6] ;
} ;

#endif	// CSGSAMPLE_H
