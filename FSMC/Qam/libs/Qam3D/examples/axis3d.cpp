/*  ---------------------------------------------------------------------------
 *  filename    :   axis3d.cpp
 *  description :   IMPLEMENTATION de la classe Axis3D
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

#include "axis3d.h"
#include "glamvector.h"

Axis3D::Axis3D(qreal size, qreal diameter )
{
	// objets locaux
	GLamVector xAxis( QVector3D(size,0,0) ) ;
    xAxis.setDiameter( diameter ) ;
	xAxis.material().setColor(Qt::red ) ;
	
	GLamVector yAxis( QVector3D(0,size,0) ) ;
    yAxis.setDiameter( diameter ) ;
    yAxis.material().setColor(Qt::green ) ;

	// regroupement (composition)
	this->addObject( &xAxis ) ;
	this->addObject( &yAxis ) ;

	// regroupement par objet temporaire
	int id = this->addObject( new GLamVector( QVector3D(0,0,size) ) ) ;
	
	// accès aux propriétés ... maintenues par GLamObject
	this->object(id)->material().setColor(Qt::blue ) ;
	
	// accès aux propriétés ... propres à la classe de l'objet, ici GLamVector
	GLamVector* p = dynamic_cast<GLamVector*>( this->object(id) ) ;
    p->setDiameter( diameter ) ;
	p->setOutLine( 0.04 ) ;
}
