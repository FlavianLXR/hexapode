/*  ---------------------------------------------------------------------------
 *  filename    :   tessline.cpp
 *  description :   IMPLEMENTATION de la classe TessLine
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

#include "tessline.h"

long int l_id = 0 ;
long int p_id = 0 ;

TessLine::TessLine()
	: m_id( 0 )
	, m_type( UNKNOWN )
{
	m_endp[0] = 0 ;
	m_endp[1] = 0 ;
}

TessLine::TessLine(TessPoint* sp, TessPoint* ep, TessType type )
	: m_type( type )
{
	m_id = ++l_id ;
	m_endp[0] = sp ;
	m_endp[1] = ep ;
}

TessLine::TessLine(const TessLine& line )
{
	this->m_id = line.m_id ;
	this->m_endp[0] = line.m_endp[0] ;
	this->m_endp[1] = line.m_endp[1] ;
	this->m_type = line.m_type ;
	this->m_key = line.m_key ;
	this->m_helper = line.m_helper ;
}

// slightly increased the key to avoid duplicated key for searching tree

void TessLine::setKeyValue(double y )
{
	if( m_endp[1]->m_y == m_endp[0]->m_y )
			m_key = ( m_endp[0]->m_x < m_endp[1]->m_x ? m_endp[0]->m_x : m_endp[1]->m_x ) ;
	else    m_key = ( y - m_endp[0]->m_y ) * ( m_endp[1]->m_x - m_endp[0]->m_x ) / ( m_endp[1]->m_y - m_endp[0]->m_y ) + m_endp[0]->m_x ;
}

void TessLine::increaseKeyValue(const double diff )
{
	m_key += diff ;
}

// reverse a directed line segment; reversable only for inserted diagonals

void TessLine::reverse()
{
//	assert( m_type == INSERT ) ;
	TessPoint* tmp = m_endp[0] ;
	m_endp[0] = m_endp[1] ;
	m_endp[1] = tmp ;
}
