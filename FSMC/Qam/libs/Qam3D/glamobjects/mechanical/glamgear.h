/*  ---------------------------------------------------------------------------
 *  filename    :   glamgear.h
 *  description :   INTERFACE de la classe GLamGear
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

#ifndef GLAMGEAR_H
#define GLAMGEAR_H

#include "../glamobject.h"

// TODO: BevelGear

class GLamGear : public GLamObject
{
  public:
	GLamGear() ;
	GLamGear(qreal innerRadius, qreal module, uint toothCount, qreal thickness = 1.0f ) ;
	GLamGear(const GLamGear& obj, uint toothCount = 0 ) ;
	~GLamGear() ;

	GLamGear& operator =(const GLamGear& obj ) ;

	inline qreal module() const			{ return m_module ; }
	inline uint  toothCount() const		{ return m_toothCount ; }
	inline qreal innerRadius() const	{ return m_innerRadius ; }
	inline qreal pitchRadius() const	{ return ( m_module * m_toothCount ) / 2.0f ; }
	inline qreal toothSize() const		{ return m_module * 2.25f ; }
	inline qreal outerRadius() const	{ return pitchRadius() + m_module ; }
	inline qreal meshAngle() const		{ return qRadiansToDegrees( m_meshAngle ) ; }
	inline qreal ratio() const			{ return m_ratio ; }
	inline qreal thickness() const		{ return m_thickness ; }

	void setInnerRadius(qreal radius ) ;
	void setThickness(qreal thickness ) ;

	qreal meshTo(const GLamGear& gear, QVector2D& direction ) ;

  protected:
	virtual void create() ;

	qreal	m_thickness ;
	qreal	m_meshAngle ;

  private :
	qreal	m_innerRadius ;
	qreal	m_module ;
	uint	m_toothCount ;
	qreal	m_ratio ;
} ;

#endif // GLAMGEAR_H
