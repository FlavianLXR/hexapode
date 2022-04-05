/*  ---------------------------------------------------------------------------
 *  filename    :   wiresample.h
 *  description :   INTERFACE de la classe WireSample
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

// exemples de construction de courbes 3D
// par enveloppe de Bézier/B-Spline

#ifndef WIRESAMPLE_H
#define WIRESAMPLE_H

#include <GLamGroup>
#include <glamwire.h>

class WireSample : public GLamGroup
{
  public:
	WireSample() ;

private:
	GLamGrid*	m_grid ;
	GLamWire*	m_wire[3] ;
	qreal		m_diameter ;
} ;

#endif	// WIRESAMPLE_H
