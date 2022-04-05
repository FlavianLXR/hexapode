/*  ---------------------------------------------------------------------------
 *  filename    :   gripper.h
 *  description :   INTERFACE de la classe Gripper
 *
 *	project     :	Qam3D/GLam robot gripper example
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

// modélisation d'une pince de robot avec animation
// (exemple d’animation de pièces non déformables)

// ressources GLam utilisées --------------------------------------------------

// 	GLamGroup					corps et carter (partie statique de la pince)
//		GLamSolid				flan du carter (x2)
//			GLamHole			perçage R1 (x2)
//			GLamHole			perçage R1,5 (x2)
//		GLamCube				base du carter 1x24x5
//		GLamGroup				corps complet
//			GLamPipe			corps cylindrique D28
//			GLamSolid			téton de fixation 6x8x5 (x2)
//				GLamHole		perçage R1 (x2)
//		GLamCylinder			rivet R1 (x4)

// 	GLamGroup					piston (partie dynamique)
//		GLamSolid				coté de la chape 9x7x1 (x2)
//			GLamHole			perçage R2
//		GLamCube				base de la chape 3x7x7
//		GLamCylinder			tige R2-L32

// 	GLamGroup					doigt (x2) (partie dynamique)
//		GLamGroup				bras 1
//			GLamSolid			partie longitudinale 57x10x4
//				GLamHole		perçage R1,5 (x2)
//			GLamSolid			fourche latérale 8x13x1,9
//			GLamPrism			partie chanfreinée 8x4
//			GLamRevolution		rivet
//		GLamGroup				bras 2
//			GLamSolid			bras 56x6x4
//				GLamHole		perçage R1,5 (x2)
//			GLamRevolution		rivet (x2)
//		GLamGroup				mors
//			GLamSolid			coté 13x14,5x1 (x2)
//				GLamHole		perçages R1,5
//			GLamCube			face d'appui 10x2x7

#ifndef GRIPPER_H
#define GRIPPER_H

#include <GLamGroup>
//#include <GLamObjects>

class Gripper
{
  public:
	Gripper(qreal defaultScale = 0.1 ) ;
	void draw() ;
	void animate() ;

  private:
	GLamGroup*	m_body ;
	GLamGroup*	m_piston ;
	GLamGroup*	m_finger[2] ;

	GLamGroup*	m_gripper1 ;
	GLamGroup*	m_gripper2 ;
	GLamGroup*	m_gripper3 ;
} ;

#endif	// GRIPPER_H
