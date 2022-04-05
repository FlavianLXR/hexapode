/*  ---------------------------------------------------------------------------
 *  filename    :   dice.cpp
 *  description :   IMPLEMENTATION de la classe Dice
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

#include "dice.h"

Dice::Dice()
	: GLamCSG()
{
	// cube de base
	qreal diceSize = 4.0f ;
	GLamCube dice( diceSize ) ;
	dice.setZCentered() ;
	this->merge(&dice) ;

	// coins arrondis
//	GLamSphere diceSph( diceSize * 1.5 ) ;
	GLamSphere diceSph( diceSize * 1.45 ) ;
//	diceSph.setNumFaces(90) ;	// très gourmant en temps !!!
//	diceSph.setNumFaces(45) ;

	this->intersect( &diceSph ) ;	// A

	// chanfreins sur arètes vives
	GLamCylinder diceCyl( diceSize * 1.38 / 2, 8 ) ;
	diceCyl.setZCentered() ;
	this->intersect( &diceCyl ) ;
	diceCyl.defaultMatrix().rotate(90, QVector3D(0, 1, 0 ) ) ;
	this->intersect( &diceCyl ) ;
	diceCyl.defaultMatrix().rotate(90, QVector3D(1, 0, 0 ) ) ;

	this->intersect( &diceCyl ) ;	// B

	// note: avec les points c'est A ou B, plante quand A et B !!!

	// matériau du dé
	GLamMaterial mat ;
	mat.set( GLamMaterial::Ruby ) ;
	this->setMaterial( mat ) ;

	// point de marquage
	GLamRevolution dicePoint(QVector4D(0, -0.3, 0.3, 0.2 ), QVector4D(0, 0.3, 0.3, -0.2 ) ) ;
//	dicePoint.setNumFaces(16) ;
//	dicePoint.setNumSectors(16) ;
	dicePoint.setNumFaces(6) ;

	dicePoint.material().setColor(Qt::white ) ;

	// 3
//	qDebug() << "- side 3..." ;
	dicePoint.defaultMatrix().translate(0, 0, diceSize / 2 ) ;
	this->subtract( &dicePoint ) ;
	dicePoint.defaultMatrix().translate(1,1,0) ;
	this->subtract( &dicePoint ) ;
	dicePoint.defaultMatrix().translate(-2,-2,0) ;
	this->subtract( &dicePoint ) ;

	// 5
//	qDebug() << "- side 5..." ;
	dicePoint.defaultMatrix().setToIdentity() ;
	dicePoint.defaultMatrix().rotate(90, 0, 1, 0 ) ;
	dicePoint.defaultMatrix().translate(0, 0, diceSize / 2 ) ;
	this->subtract( &dicePoint ) ;
	dicePoint.defaultMatrix().translate(1, 1, 0 ) ;
	this->subtract( &dicePoint ) ;
	dicePoint.defaultMatrix().translate(0, -2, 0 ) ;
	this->subtract( &dicePoint ) ;
	dicePoint.defaultMatrix().translate(-2, 0, 0 ) ;
	this->subtract( &dicePoint ) ;
	dicePoint.defaultMatrix().translate(0, 2, 0 ) ;
	this->subtract( &dicePoint ) ;

	// 4
//	qDebug() << "- side 4..." ;
	dicePoint.defaultMatrix().setToIdentity() ;
	dicePoint.defaultMatrix().rotate(180, 0, 1, 0 ) ;
	dicePoint.defaultMatrix().translate(0, 0, diceSize / 2 ) ;
	dicePoint.defaultMatrix().translate(1, 1, 0 ) ;
	this->subtract( &dicePoint ) ;
	dicePoint.defaultMatrix().translate(0, -2, 0 ) ;
	this->subtract( &dicePoint ) ;
	dicePoint.defaultMatrix().translate(-2, 0, 0 ) ;
	this->subtract( &dicePoint ) ;
	dicePoint.defaultMatrix().translate(0, 2, 0 ) ;
	this->subtract( &dicePoint ) ;

	// 2
//	qDebug() << "- side 2..." ;
	dicePoint.defaultMatrix().setToIdentity() ;
	dicePoint.defaultMatrix().rotate(-90, 0, 1, 0 ) ;
	dicePoint.defaultMatrix().translate(0, 0, diceSize / 2 ) ;
	dicePoint.defaultMatrix().translate(1, 1, 0 ) ;
	this->subtract( &dicePoint ) ;
	dicePoint.defaultMatrix().translate(-2, -2, 0 ) ;
	this->subtract( &dicePoint ) ;

	// 1
//	qDebug() << "- side 1..." ;
	dicePoint.defaultMatrix().setToIdentity() ;
	dicePoint.defaultMatrix().rotate(90, 1, 0, 0 ) ;
	dicePoint.defaultMatrix().translate(0, 0, diceSize / 2 ) ;
	this->subtract( &dicePoint ) ;

	// 6
//	qDebug() << "- side 6..." ;
	dicePoint.defaultMatrix().setToIdentity() ;
	dicePoint.defaultMatrix().rotate(-90, 1, 0, 0 ) ;
	dicePoint.defaultMatrix().translate(0, 0, diceSize / 2 ) ;
	dicePoint.defaultMatrix().translate(1, 1, 0 ) ;
	this->subtract( &dicePoint ) ;
	dicePoint.defaultMatrix().translate(0, -1, 0 ) ;
	this->subtract( &dicePoint ) ;
	dicePoint.defaultMatrix().translate(0, -1, 0 ) ;
	this->subtract( &dicePoint ) ;
	dicePoint.defaultMatrix().translate(-2, 0, 0 ) ;
	this->subtract( &dicePoint ) ;
	dicePoint.defaultMatrix().translate(0, 1, 0 ) ;
	this->subtract( &dicePoint ) ;
	dicePoint.defaultMatrix().translate(0, 1, 0 ) ;
	this->subtract( &dicePoint ) ;
}
