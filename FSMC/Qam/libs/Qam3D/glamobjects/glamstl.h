/*  ---------------------------------------------------------------------------
 *  filename    :   glamstl.h
 *  description :   INTERFACE de la classe GLamStL
 *
 *	project     :	Qam3D/GLam: 3D object collection
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

// GLamObject to/from StereoLithography file (binary format)

#ifndef GLAMSTL_H
#define GLAMSTL_H

#include "glamobject.h"

class GLamGroup ;

class GLamStL : public GLamObject
{
  public:
	GLamStL() ;
	GLamStL(const QString& filename ) ;
	GLamStL(GLamObject* obj, const QString& name = 0 ) ;
	GLamStL(GLamGroup* obj, const QString& name = 0 ) ;
	GLamStL(const GLamStL& obj ) ;
//	~GLamStL() ;

	GLamStL& operator =(const GLamStL& obj ) ;

	bool load(const QString& filename ) ;
	bool save(const QString& filename ) ;

  protected:
	virtual void create() ;

  private:
	void addFacetsFromObject(GLamObject* obj, const QMatrix4x4& mg = QMatrix4x4() ) ;
} ;

#endif // GLAMSTL_H

/*! @class GLamStL
	@brief Classe d'import/export StL (@e StereoLithography @e binary @e format).

Le format de fichier STL ne décrit que la géométrie de surface d'un objet 3D.
Ce format ne comporte notamment pas d'informations concernant la couleur ou
la texture.

Un fichier STL binaire est architecturé de la façon suivante :
- les 80 premiers octets sont un commentaire (texte libre) ;
- les 4 octets suivants forment un entier codé sur 32 bits, qui représentent
le nombre de triangles présents dans le fichier ;
- chaque triangle est ensuite codé sur 50 octets, selon la décomposition suivante :
	- 3 fois 4 octets (3 x @e float), chaque paquet de 4 octets représente un nombre à virgule
flottante correspondant respectivement aux coordonnées (x,y,z) de la direction
normale au triangle ;
	- 3 paquets de 3 fois 4 octets (9 x @e float), chaque groupe de 4 octets représente un
nombre à virgule flottante correspondant respectivement aux coordonnées (x,y,z)
de chacun des sommets du triangle ;
	- 2 octets (1 x @e short @e int) représentant un mot de contrôle (e.g. 0).

Les nombres en virgule flottante sont représentés conformément à la spécification
IEEE 754 (en mode @e little-endian).
The direction of the normal is outward.
The vertices are listed in counterclockwise order when looking at the object
from the outside (right-hand rule).
*/
