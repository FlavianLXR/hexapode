/*  ---------------------------------------------------------------------------
 *  filename    :   wiresample.cpp
 *  description :   IMPLEMENTATION de la classe WireSample
 *
 *	project     :	Qam3D/GLam: GLamWire examples
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

#include "wiresample.h"

WireSample::WireSample()
	: GLamGroup(GLamGroup::Aggregation)
	, m_diameter( 0.05 )
{
	m_grid = new GLamGrid(4) ;
	addObject( m_grid ) ;

	qreal	E = 1.0 ;
	qreal	e = E / 10 ;

	QVector3D	p0( E, -E, 0 ) ;
	QVector3D	v0(-E, 0, 0 ) ;
	QVector3D	p1( 0, E, E ) ;
	QVector3D	v1( E, 0, 0 ) ;

	m_wire[1] = new GLamWire(BezierCurve(p0,v0,p1,v1), m_diameter ) ;
	m_wire[1]->material().setColor(Qt::red) ;
//	m_wire[1]->setSketched() ;
	addObject( m_wire[1] ) ;

	QList<QVector3D>	curve ;
	curve << p0 << p0 + v0 << p1 + v1 << p1 ;
	m_wire[0] = new GLamWire(curve, m_diameter ) ;
	m_wire[0]->defaultMatrix().translate(0, e, 0 ) ;
	m_wire[0]->material().setColor(Qt::blue) ;
//	m_wire[0]->setSketched() ;
	addObject( m_wire[0] ) ;

	m_wire[2] = new GLamWire( *m_wire[0] ) ;
	m_wire[2]->defaultMatrix().translate(0, -2*e, 0 ) ;
	m_wire[2]->material().setColor(Qt::yellow ) ;
	m_wire[2]->setColoredStripe( Qt::darkGreen ) ;
//	m_wire[2]->setSketched() ;
	addObject( m_wire[2] ) ;
}
