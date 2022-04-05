/*  ---------------------------------------------------------------------------
 *  filename    :   glamlogo.h
 *  description :   INTERFACE de la classe GLamLogo
 *
 *	project     :	Qam3D/GLam: 3D object collection
 *  start date  :   février 2010
 *  ---------------------------------------------------------------------------
 *  Copyright 2010-2020 by Alain Menu   <alain.menu@ac-creteil.fr>
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

#ifndef GLAMLOGO_H
#define GLAMLOGO_H

#include <GLamGroup>
#include <_VERSION>

class GLamLogo : public GLamGroup
{
  public:
    GLamLogo(const QString& text = QString() ) ;
} ;

#endif // GLAMLOGO_H
