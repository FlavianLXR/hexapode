/*  ---------------------------------------------------------------------------
 *  filename    :   fsmuframe.cpp
 *  description :   IMPLEMENTATION de la classe FSmuFrame
 *
 *	project     :	Scène 3D LLF Maquette éch. 1:1 FlightSim
 *  start date  :   novembre 2017
 *  ---------------------------------------------------------------------------
 *  Copyright 2017 by Alain Menu   <alain.menu@ac-creteil.fr>
 *
 *  This file is part of "FlightSim Mock-up"
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

#include "fsmuframe.h"

FSmuFrame::FSmuFrame()
	: GLamGroup()
{
	// plateau 15mm
	m_plateau = new GLamCube(1200, 760, 15 ) ;
	m_plateau->defaultMatrix().translate(0, 0, 90 ) ;
	m_plateau->material().setTransparency(0.3) ;

	// bati mécano-soudé tubes 30x30mm
	GLamCube* cube = new GLamCube(1200, 760, 180 ) ;
	cube->setZCentered() ;

	m_bati = new GLamCSG( cube ) ;

	*cube = GLamCube(1140, 700, 180 ) ;
	cube->setZCentered() ;
	m_bati->subtract( cube ) ;

	*cube = GLamCube(1200, 700, 120 ) ;
	cube->setZCentered() ;
	m_bati->subtract( cube ) ;

	*cube = GLamCube(555, 760, 120 ) ;
	cube->setZCentered() ;
	cube->defaultMatrix().translate(-300, 0, 0 ) ;
	m_bati->subtract( cube ) ;
	cube->defaultMatrix().translate(585, 0, 0 ) ;
	m_bati->subtract( cube ) ;

	m_bati->setMaterial( GLamMaterial::BlackPlastic ) ;

	addObject( m_bati ) ;
	addObject( m_plateau ) ;
}
