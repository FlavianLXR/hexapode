/*  ---------------------------------------------------------------------------
 *  filename    :   glamobjmtl.h
 *  description :   INTERFACE de la classe GLamObjMtl
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

#ifndef GLAMOBJMTL_H
#define GLAMOBJMTL_H

#include "glamgroup.h"
#include "glammesh.h"

class GLamObjMtl : public GLamGroup
{
  public:
	GLamObjMtl() ;
	GLamObjMtl(const QString& filename ) ;
	GLamObjMtl(GLamObject* obj, const QString& name = 0 ) ;
	GLamObjMtl(GLamGroup* obj, const QString& name = 0 ) ;
	GLamObjMtl(const GLamObjMtl& obj ) ;

	GLamObjMtl& operator =(const GLamObjMtl& obj ) ;

    // new 26/11/2020
    GLamMesh& toTrimesh() { return m_mesh ; }
    //

    bool load(const QString& filename ) ;
	bool save(const QString& filename ) ;

	void stretchTexels(const QString& mtlName, QVector2D vmin = QVector2D(0.0,0.0), QVector2D vmax = QVector2D(1.0,1.0) ) ;
	void reflectTexels(const QString& mtlName, bool acrossXaxis, bool acrossYaxis ) ;
  private:
	bool loadMtl(const QString& filename ) ;
	bool saveMtl(const QString& filename ) ;

	QList<int> parseFaceElement(QString faceElement ) ;
	QString f2s(float v ) ;
	QString v2s(QVector3D v ) ;

  private:
	QList<QVector3D>	m_v ;		//
	QList<QVector2D>	m_t ;		// membres temporaires utilisés seulement
	GLamMesh*			m_obj ;		// en phase de chargement d'un fichier...
	QList<GLamMaterial>	m_matList ;	//

    // new 26/11/2020
    GLamMesh			m_mesh ;    // objet unique pour toTrimesh()...
} ;

#endif	// GLAMOBJMTL_H

/*!	@class GLamObjMtl
	@brief Classe d'import/export de fichiers Wavefront OBJ/MTL (format ASCII).

La classe GLamObjMtl permet de charger un couple de fichiers OBJ/MTL sous
forme d'un groupement GLamGroup. Elle permet aussi à l'inverse la sauvegarde
du groupement vers des fichiers Wavefront.
@n Le constructeur par défaut permet d'obtenir la célèbre théière (@a Utah @a teapot) !
@n Un objet GLamObjMtl peut aussi être construit à partir d'un descendant de
GLamObject ou d'un groupement de classe GLamGroup.
Quel que soit la solution de construction utilisée, l'objet peut ensuite être
enrichi par ajouts grâce à la méthode addObject().

La classe reconnait les entrées suivantes du format de fichier Wavefront
(les entrées @a vn sont cependant ignorées, les normales étant recalculées en interne pour
chaque triangle en fonction du @ref obj_shadingEffect "shadingEffect") :
@code
//
// OBJ recognized entries
// ---------------------------------------------------------------------------
// external mtl file name:	                    mtllib file_name
// material selector:	                        usemtl material_name
// object name:	                                o object_name
// polygon group name:	                        g group_name
// vertex coordinates:                          v x y z
// texture coordinates:                         vt x y
// vertex normal:	                            vn x y z
// polygonal face element:                      f v/vt/vn ...
//
// MTL (Material Template Library) recognized entries
// ---------------------------------------------------------------------------
// material name:								newmtl material_name
// specular exponent (exponent = 0.0 .. 1000):	Ns exponent
// ambient reflectivity (r,g,b = 0.0 .. 1.0):	Ka r g b
// diffuse reflectivity (r,g,b = 0.0 .. 1.0):	Kd r g b
// specular reflectivity (r,g,b = 0.0 .. 1.0):	Ks r g b
// dissolve factor (factor = 0.0 .. 1.0):		d factor
// Transparency factor (Tr = 1 - d):			Tr factor
// Texture file:								map_Kd filename
//
@endcode
*/
