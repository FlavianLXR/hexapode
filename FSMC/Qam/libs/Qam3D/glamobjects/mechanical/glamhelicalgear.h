/*  ---------------------------------------------------------------------------
 *  filename    :   glamhelicalgear.h
 *  description :   INTERFACE de la classe GLamHelicalGear
 *
 *	project     :	Qam3D/GLam: 3D object collection
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

#ifndef GLAMHELICALGEAR_H
#define GLAMHELICALGEAR_H

#include "glamgear.h"

class GLamHelicalGear : public GLamGear
{
  public:
	GLamHelicalGear() ;
	GLamHelicalGear(qreal innerRadius, qreal module, uint toothCount, qreal helixAngle, qreal thickness = 1.0f ) ;
	GLamHelicalGear(const GLamHelicalGear& obj, uint toothCount = 0 ) ;
	~GLamHelicalGear() ;

	GLamHelicalGear& operator =(const GLamHelicalGear& obj ) ;

	qreal helixAngle() const ;

	qreal meshTo(const GLamHelicalGear& gear, QVector2D& direction ) ;

  protected:
	virtual void create() ;

  private:
	qreal	m_helixAngle ;
} ;

#endif // GLAMHELICALGEAR_H
