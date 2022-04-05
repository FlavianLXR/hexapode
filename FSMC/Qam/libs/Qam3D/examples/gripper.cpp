/*  ---------------------------------------------------------------------------
 *  filename    :   gripper.cpp
 *  description :   IMPLEMENTATION de la classe Gripper
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

#include "gripper.h"

Gripper::Gripper(qreal defaultScale )
{
	QList<QPointF>	gripperPts ;

	// perçage et rivet
	GLamHole hole( GLam2D::circularShape(1.5, 30 ) ) ;
	gripperPts << QPointF(1,-4 ) << QPointF(2,-4 ) << QPointF(2,-3.5 ) << QPointF(1.5,-3.5 ) ;
	gripperPts << QPointF(1.5,3.5 ) << QPointF(2,3.5 ) << QPointF(2,4 ) << QPointF(1,4 ) ;
	GLamRevolution rivet( gripperPts ) ;
	rivet.material().setColor(Qt::yellow ) ;

	// bras 1
	m_gripper1 = new GLamGroup ;	// groupement par 'composition'

	qreal ptsbr10[] = {  4,-7, 8,-3, 51,-3, 53,-1, 53,1, 51,3, -1,3,  -4,0,  -4,-7 } ;
	gripperPts = GLam2D::tabToList( ptsbr10, 18 ) ;

	GLamSolid gripper11( gripperPts, 4 ) ;
	gripper11.addHole(&hole ) ;
	gripper11.addHole(&hole, QPointF(50,0) ) ;
	gripper11.defaultMatrix().translate(0, 0, -2 ) ;

	qreal ptsbr11[] = { -4,-7, -4,-23, -3,-24, -2,-24, -2,-18, 0,-16,  2,-18,  2,-24, 3,-24, 4,-23,  4,-7 } ;
	gripperPts = GLam2D::tabToList( ptsbr11, 22 ) ;

	GLamSolid gripper12( gripperPts, 1.9 ) ;
	gripper12.defaultMatrix().translate(0, 0, -2 ) ;

	gripperPts.clear() ;
	gripperPts << QPointF(0,0)  << QPointF(-2.1,0) << QPointF(0,-4) ;

	GLamPrism gripper13( gripperPts, 8 ) ;
	gripper13.setZCentered() ;
	gripper13.defaultMatrix().translate(0, -7, -0.1 ) ;
	gripper13.defaultMatrix().rotate(90, 0, 1, 0 ) ;

	m_gripper1->addObject( &gripper11 ) ;
	m_gripper1->addObject( &gripper12 ) ;
	m_gripper1->addObject( &gripper13 ) ;

	// affecte tout le groupement (y compris objets futurs...)
//	m_gripper1->material().setColor(Qt::red) ;
	// affecte seulement les objets déjà regroupés
	m_gripper1->setMaterial( GLamMaterial( QColor(Qt::red) ) ) ;

	m_gripper1->addObject( &rivet ) ;

	// bras 2
	m_gripper2 = new GLamGroup ;	// groupement par 'composition'

	GLamSolid gripper21( GLam2D::translate( GLam2D::oblongShape(QSize(56,6), 16 ), 25, 0 ), 4 ) ;
	gripper21.addHole(&hole ) ;
	gripper21.addHole(&hole, QPointF( 50,0) ) ;
	gripper21.defaultMatrix().translate(0, 0, -2 ) ;

	gripper21.material().setColor(Qt::blue) ;

	m_gripper2->addObject( &gripper21 ) ;
	m_gripper2->addObject( &rivet ) ;
	m_gripper2->defaultMatrix().translate(0, 8, 0 ) ;

	// mors
	m_gripper3 = new GLamGroup ;	// groupement par 'composition'

	qreal ptsdg[] = { 0,-3.5, 10,-3.5, 10,0, 2,11, -1,11, -3,9, -3,-1 } ;
	gripperPts = GLam2D::tabToList( ptsdg, 14 ) ;

	GLamSolid gripper31(gripperPts, 1 ) ;
	gripper31.addHole(&hole ) ;
	gripper31.addHole(&hole, QPointF(0,8) ) ;

	GLamCube gripper32(10, 2, 7 ) ;

	gripper31.defaultMatrix().translate(0, 0, -3.5 ) ;
	m_gripper3->addObject(&gripper31 ) ;
	gripper31.defaultMatrix().translate(0, 0, 6 ) ;
	m_gripper3->addObject(&gripper31 ) ;
	gripper32.defaultMatrix().translate(5, -4.5, -3.5 ) ;
	m_gripper3->addObject(&gripper32 ) ;

	m_gripper3->setMaterial( GLamMaterial( QColor(Qt::green) ) ) ;

	m_gripper3->addObject( &rivet ) ;
	rivet.defaultMatrix().translate(0, 8, 0 ) ;
	m_gripper3->addObject( &rivet ) ;

	m_gripper3->defaultMatrix().translate(50, 0, 0 ) ;

	// doigts complets : groupements par agrégation

	m_finger[0] = new GLamGroup( GLamGroup::Aggregation ) ;
	m_finger[0]->addObject( m_gripper1 ) ;
	m_finger[0]->addObject( m_gripper2 ) ;
	m_finger[0]->addObject( m_gripper3 ) ;

	m_finger[1] = new GLamGroup( *m_finger[0] ) ;	// copie conforme !

	// parties fixes : carter + corps

	qreal ptsca[] = { 31,0, 31,17, 21,31, 16.5,31, 3,12, -3,12, -3,4, 27,4, 29,2, 29,0 } ;
	gripperPts = GLam2D::tabToList( ptsca, 20 ) ;

	GLamSolid carter(gripperPts, 1 ) ;

	GLamHole hole2(GLam2D::circularShape(1, 30 ) ) ;
	carter.addHole(&hole2, QPointF(0,6) ) ;
	carter.addHole(&hole2, QPointF(0,10) ) ;

	carter.addHole(&hole, QPointF(19,20) ) ;
	carter.addHole(&hole, QPointF(19,28) ) ;
	carter.defaultMatrix().translate(-19, 0, -3.5 ) ;

	m_body = new GLamGroup ;
	m_body->addObject(&carter) ;
	carter.defaultMatrix().rotate(180, 1, 0, 0 ) ;
	carter.defaultMatrix().translate(0, 0, -1 ) ;
	m_body->addObject(&carter) ;

	// auto-duplication pour l'autre face ?! ça marche !
	int side = m_body->addObject( m_body ) ;
	m_body->object(side)->defaultMatrix().translate(0,0,6) ;

	GLamCube carter2(1, 24, 5 ) ;
	carter2.defaultMatrix().translate(11.5, 0, -2.5 ) ;
	m_body->addObject(&carter2) ;

	m_body->setMaterial( GLamMaterial( QColor(Qt::magenta) ) ) ;

	GLamPipe body1(28, 12, 26 ) ;

	GLamSolid body2(GLam2D::rectangularShape(QSizeF(6,8)), 5 ) ;
	body2.addHole(&hole2, QPointF(0,-2) ) ;
	body2.addHole(&hole2, QPointF(0, 2) ) ;
	body2.defaultMatrix().rotate(90, 0, 1, 0 ) ;
	body2.defaultMatrix().translate(3, -8, -2.5 ) ;

	GLamGroup body ;
	body.addObject( &body1 ) ;
	body.addObject( &body2 ) ;
	body2.defaultMatrix().translate(0, 16, 0 ) ;
	body.addObject( &body2 ) ;
	body.defaultMatrix().rotate(-90, 0, 1, 0 ) ;
	body.defaultMatrix().translate( 0, 0, 22 ) ;

	m_body->addObject( &body ) ;

	GLamCylinder axis(1, 8 ) ;
	axis.material().setColor(Qt::yellow ) ;
	axis.defaultMatrix().translate(-19, 10, -4 ) ;
	m_body->addObject( &axis ) ;
	axis.defaultMatrix().translate(0, -4, 0 ) ;
	m_body->addObject( &axis ) ;
	axis.defaultMatrix().translate(0, -12, 0 ) ;
	m_body->addObject( &axis ) ;
	axis.defaultMatrix().translate(0, -4, 0 ) ;
	m_body->addObject( &axis ) ;

	// piston
	qreal ptspi[] = { -6,-3.5, 1.5,-3.5, 3,-2, 3,2, 1.5,3.5, -6,3.5 } ;
	gripperPts = GLam2D::tabToList( ptspi, 12 ) ;
	GLamSolid piston1(gripperPts, 1 ) ;
	piston1.addHole( new GLamHole( GLam2D::circularShape(2, 30) ) ) ;

	GLamCube piston2(3, 7, 7 ) ;
	piston2.defaultMatrix().translate(-7.5, 0, -3.5 ) ;

	GLamCylinder piston3(2, 32 ) ;
	piston3.setNumSectors(90) ;
	piston3.defaultMatrix().rotate(-90, 0, 1, 0 ) ;
	piston3.defaultMatrix().translate(0, 0, 9 ) ;

	m_piston = new GLamGroup ;

	piston1.defaultMatrix().translate(0, 0, -3.5 ) ;
	m_piston->addObject( &piston1 ) ;
	piston1.defaultMatrix().translate(0, 0, 6 ) ;
	m_piston->addObject( &piston1 ) ;

	m_piston->addObject( &piston2 ) ;
	m_piston->addObject( &piston3 ) ;

	m_piston->setMaterial( GLamMaterial( QColor(255, 153, 102) ) ) ;

	axis.resize(2, 8 ) ;
	axis.defaultMatrix().setToIdentity() ;
	axis.defaultMatrix().translate(0, 0, -4 ) ;
	m_piston->addObject( &axis ) ;

	// échelle par défaut

	m_body->defaultMatrix().scale(defaultScale) ;
	m_piston->defaultMatrix().scale(defaultScale) ;
	m_finger[0]->defaultMatrix().scale(defaultScale) ;
	m_finger[1]->defaultMatrix().scale(defaultScale) ;

	// placement spatial initial des doigts

	m_finger[0]->defaultMatrix().translate(0,  20, 0 ) ;
	m_finger[1]->defaultMatrix().translate(0, -20, 0 ) ;
	m_finger[1]->defaultMatrix().rotate(180, 1, 0, 0 ) ;
}

void Gripper::draw()
{
	m_body->draw() ;
	m_piston->draw() ;
	m_finger[0]->draw() ;
	m_finger[1]->draw() ;
}

void Gripper::animate()
{
	static qreal gripperDelta = 0 ;
	static int   gripperSens = 1 ;

	qreal alpha = qAtan( gripperDelta / 20 ) ;
	qreal x = 50.0 * qCos( alpha ) ;
	qreal y = 50.0 * qSin( alpha ) ;
	alpha = qRadiansToDegrees( alpha ) ;

	gripperDelta += gripperSens * 0.25 ;
	if ( gripperDelta >  6.0 ) { gripperDelta =  6.0 ; gripperSens *= -1 ; }
	if ( gripperDelta < -6.0 ) { gripperDelta = -6.0 ; gripperSens *= -1 ; }

	m_piston->transformMatrix().setToIdentity() ;
	m_piston->transformMatrix().translate(gripperDelta, 0, 0 ) ;

	m_gripper1->transformMatrix().setToIdentity() ;
	m_gripper1->transformMatrix().rotate(alpha, 0, 0, 1 ) ;

	m_gripper2->transformMatrix().setToIdentity() ;
	m_gripper2->transformMatrix().rotate(alpha, 0, 0, 1 ) ;

	m_gripper3->transformMatrix().setToIdentity() ;
	m_gripper3->transformMatrix().translate(x - 50.0, y, 0 ) ;
}
