/*  ---------------------------------------------------------------------------
 *  filename    :   glamgroup.h
 *  description :   INTERFACE de la classe GLamGroup
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

#ifndef GLAMGROUP_H
#define GLAMGROUP_H

#include <GLamObjects>

class GLamGroup : public GLamObject
{
  public:
	/*! Cycle de vie des éléments du groupe. */
	enum LifeCycle {
		Aggregation ,	/*!< couplage faible, liens vers les éléments. */
		Composition		/*!< couplage fort, copie des éléments. */
	} ;

	GLamGroup(LifeCycle lifeCycle = Composition ) ;
	~GLamGroup() ;
	GLamGroup(const GLamGroup& obj ) ;
	GLamGroup& operator =(const GLamGroup& obj ) ;

	/*! Retourne le mode (agrégation ou composition) du groupement. */
	inline LifeCycle lifeCycle() const { return m_lifeCycle ; }

  public:
	int addObject(GLamObject* obj ) ;
	GLamObject*	object(int id ) const ;
	GLamObject*	object(const QString& name ) const ;
	/*! Retourne le nombre d'objets dans le groupement. */
	inline int numObjects() const { return m_obj.size() ; }

	QString name() const ;

	virtual int numTriangles() const ;
	virtual int numVertices() const ;

	void setWireFrame(bool wireFrame = true ) ;
	void setShadingEffect(ShadingEffect effect ) ;

    virtual void setMaterial(GLamMaterial material ) ;
    virtual GLamMaterial& material() ;

	virtual void draw(GLamObject* parent = 0 ) ;

	virtual QVector3D min() ;
	virtual QVector3D max() ;

  protected:
	virtual void create() ;
	void removeAllObjects() ;

  private:
	LifeCycle			m_lifeCycle ;
	QList<GLamObject*>	m_obj ;
	bool				m_materialChanged ;
} ;

#endif // GLAMGROUP_H

/*!	@class GLamGroup
	@brief Regroupement d'objets de la hiérarchie GLamObject.

La classe GLamGroup permet de créer une composition ou agrégation
d'objets afin de créer un système "assemblé" en agissant sur l'attitude
spatiale par défaut de chaque élément. Le système est ensuite susceptible
de subir des modifications globales :
- propriété @ref obj_defaultMatrix "GLamObject::defaultMatrix" ;
- propriété @ref obj_transformMatrix "GLamObject::transformMatrix" ;
- propriété @ref obj_material "GLamObject::material" ;
- propriété @ref obj_wireFrame "GLamObject::wireFrame" ;
- propriété @ref obj_shadingEffect "GLamObject::shadingEffect" ;

Le cycle de vie des éléments (agrégation ou composition) ne peut être décidé
qu'à la construction. En mode agrégation, la méthode addObject() ajoute les
objets au groupe par mémorisation de leur adresse ; en mode composition,
c'est une copie de l'objet qui est intégrée.
*/
