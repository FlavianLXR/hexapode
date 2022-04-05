/*  ---------------------------------------------------------------------------
 *  filename    :   glammesh.h
 *  description :   INTERFACE de la classe GLamMesh
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

#ifndef GLAMMESH_H
#define GLAMMESH_H

#include "glamobject.h"

class GLamMesh : public GLamObject
{
  public:
	GLamMesh() ;
	GLamMesh(const QString& filename ) ;
	GLamMesh(QVector<QVector3D> vertices ) ;
	GLamMesh(const GLamMesh& obj ) ;

	GLamMesh& operator =(const GLamMesh& obj ) ;

	int numTrimesh() const ;
	QList<QVector3D> trimesh(int index ) ;

	bool load(const QString& filename ) ;
	bool save(const QString& filename ) ;

  protected:
	virtual void create() ;
} ;

#endif	// GLAMMESH_H

/*! @class GLamMesh
	@brief Vue "maillage triangulaire" (@e trimesh) de GLamObject.

La classe GLamMesh propose la vue d'un objet de la hiérarchie GLamObject
sous forme d'une suite de triplets de vertices.
@n La classe permet aussi l'import/export au format brut RAW
(@e Blender @e Raw @e Mesh @e Format).

Le format RAW est un format ASCII ou chaque ligne contient 3 ou 4 triplets
de valeurs réelles au format IEEE 754 ; valeurs qui représentent les sommets
d'un triangle ou d'un quad. Le format RAW ne contient aucune information de
couleur ou de texture.
*/
