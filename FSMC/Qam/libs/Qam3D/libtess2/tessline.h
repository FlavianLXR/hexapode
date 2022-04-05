/*  ---------------------------------------------------------------------------
 *  filename    :   tessline.h
 *  description :   INTERFACE de la classe TessLine
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

#ifndef TESSLINE_H
#define TESSLINE_H

#include "tesspoint.h"

// base class for tesselator polygon boundary
// (directed line segment with start/end point)

class TessLine
{
  public:
	TessLine() ;
	TessLine(TessPoint* sp, TessPoint* ep, TessType type ) ;
	TessLine(const TessLine& line ) ;

	unsigned int id() const { return m_id ; }
	TessPoint*   endPoint(int i) const { return m_endp[i] ; }
	TessType     type() const { return m_type ; }
	double       keyValue() const { return m_key ; }
	void         setKeyValue(double y ) ;
	void         increaseKeyValue(const double diff ) ;
	void         reverse() ;
	void         setHelper(unsigned int i ) { m_helper = i ; }
	unsigned int helper() { return m_helper ; }

  protected:
	unsigned int	m_id ;			// id of a line segment
	TessPoint*		m_endp[2] ;		// two end points

	TessType		m_type ;		// type of a line segement, input/insert
	double			m_key ;			// key of a line segment for splay tree searching
	unsigned int	m_helper ;		// helper of a line segemnt
} ;

#endif	// TESSLINE_H
