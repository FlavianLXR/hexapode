/*  ---------------------------------------------------------------------------
 *  filename    :   csgsample.cpp
 *  description :   IMPLEMENTATION de la classe CsgSample
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

#include <csgsample.h>
//#include <QDebug>

CsgSample::CsgSample(qreal defaultScale )
{
	m_primBox = new GLamCube(1.8) ;
	m_primBox->setZCentered() ;
	m_primBox->material().setColor(QColor(240, 120, 0 ) ) ;

	m_primSph = new GLamSphere(2.4) ;
	m_primSph->material().setColor(QColor(152, 255, 12 ) ) ;

	m_primCyl = new GLamCylinder(0.6, 2.4 ) ;
	m_primCyl->setZCentered() ;
	m_primCyl->material().setColor(QColor(72, 74, 250 ) ) ;

	for ( int i = 0 ; i < 6 ; ++i )	m_csg[i] = new GLamCSG( m_primBox ) ;

//	m_csg[0]->intersect( m_primSph ) ;
//	m_csg[1]->subtract( m_primSph ) ;
//	m_csg[2]->subtract( m_primCyl ) ;
	*m_csg[0] &= *m_primSph ;
	*m_csg[1] -= *m_primSph ;
	*m_csg[2] -= *m_primCyl ;

	GLamSphere* primSph = new GLamSphere( *m_primSph ) ;
	primSph->setDiameter(2.6) ;

	m_csg[3]->subtract( m_primSph )->intersect( primSph ) ;

	GLamCylinder* primCyl = new GLamCylinder( *m_primCyl ) ;

	m_csg[4]->subtract( primCyl ) ;
	primCyl->defaultMatrix().rotate(90, 0, 1, 0 ) ;

//	m_csg[4]->subtract( primCyl ) ;
//	m_csg[4]->intersect( primSph ) ;
	( *m_csg[4] -= *primCyl ) &= *primSph;

	primSph->defaultMatrix().translate(1, 1, 1 ) ;
	m_csg[5]->subtract( primSph ) ;
	primCyl->defaultMatrix().setToIdentity() ;
	primCyl->defaultMatrix().translate(-1, -1, 0 ) ;
	primCyl->defaultMatrix().rotate(-15, 1, 0, 0 ) ;
	m_csg[5]->subtract( primCyl ) ;

	delete primSph ;
	delete primCyl ;

	// au final, 9 objets dans le plan 0XY

    m_primBox->defaultMatrix().scale(defaultScale ) ;
    m_primSph->defaultMatrix().scale(defaultScale ) ;
    m_primCyl->defaultMatrix().scale(defaultScale ) ;
    for (int i = 0 ; i < 6 ; ++i )  m_csg[i]->defaultMatrix().scale(defaultScale ) ;

    qreal sep = 2.5f ;
	m_primBox->defaultMatrix().translate(-sep,  sep, 0 ) ;
	m_primSph->defaultMatrix().translate(-sep,    0, 0 ) ;
	m_primCyl->defaultMatrix().translate(-sep, -sep, 0 ) ;
	m_csg[0]->defaultMatrix().translate(   0,  sep, 0 ) ;
	m_csg[2]->defaultMatrix().translate(   0, -sep, 0 ) ;
	m_csg[3]->defaultMatrix().translate( sep,  sep, 0 ) ;
	m_csg[4]->defaultMatrix().translate( sep,    0, 0 ) ;
	m_csg[5]->defaultMatrix().translate( sep, -sep, 0 ) ;
}

void CsgSample::draw()
{
	m_primBox->draw() ;
	m_primSph->draw() ;
	m_primCyl->draw() ;
	for ( int i = 0 ; i < 6 ; ++i )	m_csg[i]->draw() ;
}
