/*  ---------------------------------------------------------------------------
 *  filename    :   tesspoint.h
 *  description :   INTERFACE de la classe TessPoint
 *
 *	project     :	Qam3D/GLam tesselation useful resources
 *  start date  :   décembre 2009
 *  ---------------------------------------------------------------------------
 *  Copyright 2009-2017 by Alain Menu   <alain.menu@ac-creteil.fr>
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

#ifndef TESSPOINT_H
#define TESSPOINT_H

#include "tessdefs.h"
#include "splay.h"

enum  TessType {
	UNKNOWN,
	INPUT,
	INSERT,
	START, END,
	MERGE, SPLIT,
	REGULAR_UP, REGULAR_DOWN
} ;

// base class for tesselator points

class TessPoint
{
  public:
	TessPoint() {}
	TessPoint(const TessPoint& p ) ;
	TessPoint(double x, double y, TessType type = UNKNOWN )
		: m_id(0), m_x(x), m_y(y), m_type( type ) {}
	TessPoint(int id, double x, double y, TessType type = UNKNOWN )
		: m_id(id), m_x(x), m_y(y), m_type( type ) {}

	friend bool operator ==(const TessPoint&, const TessPoint& ) ;
	friend bool operator !=(const TessPoint&, const TessPoint& ) ;
	friend bool operator >(const TessPoint&, const TessPoint& ) ;
	friend bool operator <(const TessPoint&, const TessPoint& ) ;

	unsigned int	m_id ;		// id of point
	double			m_x, m_y ;	// coordinates
	TessType		m_type ;	// type of points
	bool			m_left ;	// left chain or not
} ;

#endif	// TESSPOINT_H
