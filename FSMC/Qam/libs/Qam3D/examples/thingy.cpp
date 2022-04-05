/*  ---------------------------------------------------------------------------
 *  filename    :   thingy.cpp
 *  description :   IMPLEMENTATION de la classe Thingy
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

#include "thingy.h"

Thingy::Thingy()
	: GLamCSG()
{
	// matière de base
	GLamPrism thingyPrism(GLam2D::roundedShape( GLam2D::rectangularShape(QSize(12,3)), 3, 0.5 ), 2 ) ;
	thingyPrism.setZCentered() ;
	this->merge(&thingyPrism) ;

	GLamCylinder thingyCyl(3, 3 ) ;
	thingyCyl.setZCentered() ;
	thingyCyl.defaultMatrix().rotate(90, 1, 0, 0 ) ;
	this->merge( &thingyCyl ) ;

	// embase de fixation avec perçage
	thingyPrism = GLamPrism( GLam2D::oblongShape(QSize(4,3), 24 ), 4 ) ;
	this->merge( &thingyPrism )->subtract( new GLamCylinder(0.7,6) ) ;

	// trou central du collier
	thingyCyl.resize(2, 3.2 ) ;
	this->subtract( &thingyCyl ) ;

	// séparation des 2 éléments
	GLamCube thingySpace(12.2, 3.2, 0.2 ) ;
	thingySpace.setZCentered() ;
	this->subtract( &thingySpace ) ;

	// perçages pour accouplement
	thingyCyl.setRadius(0.7) ;
	thingyCyl.defaultMatrix().setToIdentity() ;
	thingyCyl.defaultMatrix().translate(-4.5, 0, 0 ) ;
	this->subtract( &thingyCyl ) ;
	thingyCyl.defaultMatrix().translate( 9.0, 0, 0 ) ;
	this->subtract( &thingyCyl ) ;

	// logement hexagonal
	thingyCyl = GLamCylinder(1.3, 3 ) ;
	thingyCyl.setNumSectors(6) ;
	this->subtract( &thingyCyl ) ;

	// matériau
	GLamMaterial mat ;
	mat.set( GLamMaterial::Brass ) ;
	this->setMaterial( mat ) ;
	// accès direct à l'objet "trou central"
	this->object(4)->material().set(GLamMaterial::BlackRubber ) ;

	// positionnement spatial initial
//	this->defaultMatrix().scale(0.2 ) ;
//  this->defaultMatrix().rotate(105, 1, 0, 0 ) ;
//	this->defaultMatrix().rotate(30, 0, 0, 1 ) ;
}
