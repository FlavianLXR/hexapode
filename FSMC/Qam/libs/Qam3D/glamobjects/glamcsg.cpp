/*  ---------------------------------------------------------------------------
 *  filename    :   glamcsg.cpp
 *  description :   IMPLEMENTATION de la classe GLamCSG
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

#include "glamcsg.h"

/*! Constructeur par défaut. */

GLamCSG::GLamCSG()
	: GLamGroup(GLamGroup::Composition)
{
	setClassName("GLamCSG") ;
}

/*! Construction à l'aide d'une primitive de base spécifiée par @a obj. */

GLamCSG::GLamCSG(const GLamObject* obj )
	: GLamGroup(GLamGroup::Composition)
{
	setClassName("GLamCSG") ;
	if ( addPrimitive( obj ) != -1 ) this->create() ;
}

/*! Constructeur de copie. */

GLamCSG::GLamCSG(const GLamCSG& obj )
	: GLamGroup(GLamGroup::Composition)
{
	*this = obj ;
}

/*! Surcharge de l'opérateur d'affectation. */

GLamCSG& GLamCSG::operator =(const GLamCSG& obj )
{
	*(GLamObject*)this = obj ;
	for ( int i = 0 ; i < obj.numObjects() ; ++i ) {
		GLamObject* child = obj.object(i) ;
		addPrimitive( child ) ;
	}
	m_models = obj.m_models ;

	this->create() ;

	return *this ;
}

/*! Opérateur de type OU (union) avec l'objet @a obj passé en argument,
 * Retourne l'objet courant après opération.
 * Si l'objet courant est vide, @a obj devient sa primitive de base.
 */

GLamCSG* GLamCSG::merge(const GLamObject* obj )
{
	int id = addPrimitive( obj ) ;
	if ( id > 0 )	m_models[0] = csgUnion(m_models.at(0), m_models.at(id) ) ;
	if ( id != -1 )	this->create() ;
	return this ;
}

/*! Opérateur de type MOINS (soustraction) avec l'objet @a obj passé en argument,
 * Retourne l'objet courant après opération.
 * Si l'objet courant est vide, @a obj devient sa primitive de base.
 */

GLamCSG* GLamCSG::subtract(const GLamObject* obj )
{
	int id = addPrimitive( obj ) ;
	if ( id > 0 )	m_models[0] = csgDifference(m_models.at(0), m_models.at(id) ) ;
	if ( id != -1 )	this->create() ;
	return this ;
}

/*! Opérateur de type ET (intersection) avec l'objet @a obj passé en argument,
 * Retourne l'objet courant après opération.
 * Si l'objet courant est vide, @a obj devient sa primitive de base.
 */

GLamCSG* GLamCSG::intersect(const GLamObject* obj )
{
	int id = addPrimitive( obj ) ;
	if ( id > 0 )	m_models[0] = csgIntersection(m_models.at(0), m_models.at(id) ) ;
	if ( id != -1 )	this->create() ;
	return this ;
}

// modifie liste des vertices et normales des objets de la liste
// après opérations booléennes, pour chaque objet de la liste
// les defaultMatrix() des objets sont pris en compte pour la projection
// il faut donc appliquer la matrice inverse sur les vertices...

/*! Pour concrétisation de la classe... */

void GLamCSG::create()
{
	for ( int i = 0 ; i < numObjects() ; ++i )	object(i)->eraseAllDatas() ;

	QVector<CSGPolygon> plist = csgModelToPolygons( m_models.at(0) ) ;

	for ( int p = 0 ; p < plist.size() ; ++p ) {
		CSGPolygon poly = plist[p] ;
		for ( int i = 2 ; i < poly.vertices.size() ; i += 3 ) {
			QVector3D v1 = poly.vertices.at(i-2).pos ;
			QVector3D v2 = poly.vertices.at(i-1).pos ;
			QVector3D v3 = poly.vertices.at(i).pos ;
			QMatrix4x4 m = object( poly.solidId )->defaultMatrix().inverted() ;
//			object( poly.solidId )->triangle(m * v1, m * v2, m * v3 ) ;
            object( poly.solidId )->triangle(m.map(v1), m.map(v2), m.map(v3) ) ;
        }
	}
}

// [private]
// ajout d'une nouvelle primitive (nouvel objet de classe de base créé)
// composants possibles : 	GLamPrism, GLamCube
//							GLamRevolution, GLamCylinder, GLamPipe, GLamSphere, GLamTorus
// ATTENTION :
// ces composants disposent d'une méthode polygons(), cf. this->modelFromPolygons()

// la primitive initiale et chaque primitive ajoutée donnent
// lieu à création d'un nouvel objet dans la liste GLamGroup

// les positions spatiales relatives des primitives sont
// définies par le client via leur defaultMatrix()

int GLamCSG::addPrimitive(const GLamObject* obj )
{
	// transformation spatiale de la primitive
	QMatrix4x4 transform = obj->defaultMatrix() ;

	QString cl = obj->className() ;
	int id = -1 ;

	// base 'GlamPrism'
	if ( cl == "GLamPrism" )			id = addObject( new GLamPrism( *(GLamPrism*)obj ) ) ;
	else if ( cl == "GLamCube" )		id = addObject( new GLamCube( *(GLamCube*)obj ) ) ;
	if ( id != -1 ) {
		object(id)->m_hasExternalVerticesProvider = true ;
		m_models << modelFromPolygons(dynamic_cast<GLamPrism*>( object(id) )->polygons( transform ), id ) ;
		return id ;
	}
	// base 'GLamRevolution'
	if ( cl == "GLamRevolution" )		id = addObject( new GLamRevolution( *(GLamRevolution*)obj ) ) ;
	else if ( cl == "GLamCylinder" )	id = addObject( new GLamCylinder( *(GLamCylinder*)obj ) ) ;
	else if ( cl == "GLamPipe" )		id = addObject( new GLamPipe( *(GLamPipe*)obj ) ) ;
	else if ( cl == "GLamSphere" )		id = addObject( new GLamSphere( *(GLamSphere*)obj ) ) ;
	else if ( cl == "GLamTorus" )		id = addObject( new GLamTorus( *(GLamTorus*)obj ) ) ;

	if ( id != -1 ) {
		object(id)->m_hasExternalVerticesProvider = true ;
		m_models << modelFromPolygons(dynamic_cast<GLamRevolution*>( object(id) )->polygons( transform ), id ) ;
		return id ;
	}
	return id ;
}

// [private]
// conversion QList<GLamPolygon> --> QVector<CSGPolygon> --> QVector<CSGVertex>

CSGModel GLamCSG::modelFromPolygons(const QList<GLamPolygon>& polygons, int id )
{
	QVector<CSGPolygon> plist ;

	for ( int i = 0 ; i < polygons.size() ; ++i ) {
		QVector<CSGVertex> vlist ;
		for ( int p = 0 ; p < polygons.at(i).points().size() ; ++p ) {
			CSGVertex vertex ;
			vertex.pos = polygons.at(i).points().at(p) ;
			vertex.normal = polygons.at(i).normal() ;
			vertex.solidId = id ;
			vlist.push_back( vertex ) ;
		}
		plist.push_back( CSGPolygon( vlist, id ) ) ;
	}

	return csgModelFromPolygons( plist ) ;
}
