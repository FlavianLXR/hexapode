/*  ---------------------------------------------------------------------------
 *  filename    :   graal.cpp
 *  description :   IMPLEMENTATION de la classe Graal
 *
 *	project     :	Qam3D/GLam example
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

#include "graal.h"

Graal::Graal()
	: GLamRevolution()
{
	setNumSectors(90) ;
	setClosed(false) ;

	QList<QPointF>	pts ;
	
	pts << QPointF(0, 0 ) ;
	pts << GLam2D::bezierShape(QVector4D(30,0,4,0), QVector4D(12,6,16,0), 60 ) ;
	pts << GLam2D::bezierShape(QVector4D(12,6,-12,0), QVector4D(14,64,-14,-5), 60 ) ;
	pts << GLam2D::bezierShape(QVector4D(14,64,14,5), QVector4D(42,104,0,-21), 60 ) ;
	pts << GLam2D::bezierShape(QVector4D(42,104,-0.5,1), QVector4D(40,104,0.5,1), 30 ) ;
	pts << GLam2D::translate( GLam2D::arcShape(40,0,-90,60), 0, 104 ) ;
	pts = GLam2D::noDuplicate( pts ) ;

	setPoints( pts ) ;

	material().set(GLamMaterial::Gold ) ;
	defaultMatrix().scale(0.01) ;
//	defaultMatrix().translate(QVector3D(0,0,-40)) ;
}
