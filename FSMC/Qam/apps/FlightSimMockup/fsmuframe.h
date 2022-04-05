/*  ---------------------------------------------------------------------------
 *  filename    :   fsmuframe.h
 *  description :   INTERFACE de la classe FSmuFrame
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

#ifndef FSMUFRAME_H
#define FSMUFRAME_H

#include <GLamObjects>
#include <GLamGroup>

// FlighSim Mock-up : support mécano-soudé

class FSmuFrame : public GLamGroup
{
  public:
	FSmuFrame() ;

  private:
	GLamCube*		m_plateau ;
	GLamCSG*		m_bati ;
} ;

#endif // FSMUFRAME_H
