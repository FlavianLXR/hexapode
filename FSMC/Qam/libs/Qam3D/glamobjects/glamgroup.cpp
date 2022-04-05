/*  ---------------------------------------------------------------------------
 *  filename    :   glamgroup.cpp
 *  description :   IMPLEMENTATION de la classe GLamGroup
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

#include <GLamGroup>
#include <mechanical/GLamMechanical>

/*! constructeur (mode Composition par défaut). */

GLamGroup::GLamGroup(LifeCycle lifeCycle )
	: GLamObject()
	, m_lifeCycle( lifeCycle )
	, m_materialChanged( false )
{
	setClassName("GLamGroup") ;
	m_obj.clear() ;
}

/*! Constructeur de copie. */

GLamGroup::GLamGroup(const GLamGroup& obj )
{
	*this = obj ;
}

/*! Destructeur. */

GLamGroup::~GLamGroup()
{
	removeAllObjects() ;
}

/*! Surcharge de l'opérateur d'affectation. */

GLamGroup& GLamGroup::operator =(const GLamGroup& obj )
{
	*(GLamObject*)this = obj ;
	m_materialChanged = obj.m_materialChanged ;

	removeAllObjects() ;

	m_lifeCycle = obj.m_lifeCycle ;

	for ( int i = 0 ; i < obj.m_obj.size() ; ++i ) {
		GLamObject* p = obj.m_obj.at(i) ;
		addObject( p ) ;
	}
	return *this ;
}

/*! Pour concrétisation de la classe... */

void GLamGroup::create()
{
}

/*! Méthode d'ajout d'un objet dans le groupement.
 * Retourne l'indice de l'objet dans le groupe ou -1 en cas d'erreur.
*/

int GLamGroup::addObject(GLamObject* obj )
{
	QString cl = obj->className() ;

	if      ( cl == "GLamMesh" )		{ GLamMesh* p = dynamic_cast<GLamMesh*>( obj ) ; m_obj << ( m_lifeCycle == Composition ? new GLamMesh( *p ) : p ) ; }
	else if ( cl == "GLamGrid" )		{ GLamGrid* p = dynamic_cast<GLamGrid*>( obj ) ; m_obj << ( m_lifeCycle == Composition ? new GLamGrid( *p ) : p ) ; }
	else if ( cl == "GLamPrism" )		{ GLamPrism* p = dynamic_cast<GLamPrism*>( obj ) ; m_obj << ( m_lifeCycle == Composition ? new GLamPrism( *p ) : p ) ; }
	else if ( cl == "GLamCube" )		{ GLamCube* p = dynamic_cast<GLamCube*>( obj ) ; m_obj << ( m_lifeCycle == Composition ? new GLamCube( *p ) : p ) ; }
	else if ( cl == "GLamRevolution" )	{ GLamRevolution* p = dynamic_cast<GLamRevolution*>( obj ) ; m_obj << ( m_lifeCycle == Composition ? new GLamRevolution( *p ) : p ) ; }
	else if ( cl == "GLamCylinder" )	{ GLamCylinder* p = dynamic_cast<GLamCylinder*>( obj ) ; m_obj << ( m_lifeCycle == Composition ? new GLamCylinder( *p ) : p ) ; }
	else if ( cl == "GLamPipe" )		{ GLamPipe* p = dynamic_cast<GLamPipe*>( obj ) ; m_obj << ( m_lifeCycle == Composition ? new GLamPipe( *p ) : p ) ; }
	else if ( cl == "GLamSphere" )		{ GLamSphere* p = dynamic_cast<GLamSphere*>( obj ) ; m_obj << ( m_lifeCycle == Composition ? new GLamSphere( *p ) : p ) ; }
	else if ( cl == "GLamTorus" )		{ GLamTorus* p = dynamic_cast<GLamTorus*>( obj ) ; m_obj << ( m_lifeCycle == Composition ? new GLamTorus( *p ) : p ) ; }
	else if ( cl == "GLamVector" )		{ GLamVector* p = dynamic_cast<GLamVector*>( obj ) ; m_obj << ( m_lifeCycle == Composition ? new GLamVector( *p ) : p ) ; }
	else if ( cl == "GLamSolid" )		{ GLamSolid* p = dynamic_cast<GLamSolid*>( obj ) ; m_obj << ( m_lifeCycle == Composition ? new GLamSolid( *p ) : p ) ; }
	else if ( cl == "GLamProfile" )		{ GLamProfile* p = dynamic_cast<GLamProfile*>( obj ) ; m_obj << ( m_lifeCycle == Composition ? new GLamProfile( *p ) : p ) ; }
	else if ( cl == "GLamHole" )		{ GLamHole* p = dynamic_cast<GLamHole*>( obj ) ; m_obj << ( m_lifeCycle == Composition ? new GLamHole( *p ) : p ) ; }
	else if ( cl == "GLamCurve" )		{ GLamCurve* p = dynamic_cast<GLamCurve*>( obj ) ; m_obj << ( m_lifeCycle == Composition ? new GLamCurve( *p ) : p ) ; }
	else if ( cl == "GLamWire" )		{ GLamWire* p = dynamic_cast<GLamWire*>( obj ) ; m_obj << ( m_lifeCycle == Composition ? new GLamWire( *p ) : p ) ; }
	else if ( cl == "GLamSpring" )		{ GLamSpring* p = dynamic_cast<GLamSpring*>( obj ) ; m_obj << ( m_lifeCycle == Composition ? new GLamSpring( *p ) : p ) ; }
	else if ( cl == "GLamGear" )		{ GLamGear* p = dynamic_cast<GLamGear*>( obj ) ; m_obj << ( m_lifeCycle == Composition ? new GLamGear( *p ) : p ) ; }
	else if ( cl == "GLamHelicalGear" )	{ GLamHelicalGear* p = dynamic_cast<GLamHelicalGear*>( obj ) ; m_obj << ( m_lifeCycle == Composition ? new GLamHelicalGear( *p ) : p ) ; }
	else if ( cl == "GLamGroup" )		{ GLamGroup* p = dynamic_cast<GLamGroup*>( obj ) ; m_obj << ( m_lifeCycle == Composition ? new GLamGroup( *p ) : p ) ; }
	else if ( cl == "GLamCSG" )			{ GLamCSG* p = dynamic_cast<GLamCSG*>( obj ) ; m_obj << ( m_lifeCycle == Composition ? new GLamCSG( *p ) : p ) ; }
    //
    else if ( cl == "GLamText" )		{ GLamText* p = dynamic_cast<GLamText*>( obj ) ; m_obj << ( m_lifeCycle == Composition ? new GLamText( *p ) : p ) ; }
    //
	else {
#ifdef QAMDEBUG
		qDebug() << "GLamGroup::addObject(): unknown class" << cl ;
#endif
		return -1 ;
	}

	return m_obj.size() - 1 ;
}

/*! Lien générique vers objet du groupement.
 * L'indice utilisé doit être celui retourné par la méthode d'ajout addObject().
 * Retourne l'adresse de l'objet GLamGroup si @a id incorrect.
 */

GLamObject*	GLamGroup::object(int id ) const
{
	if (( id < 0 )||( id >= m_obj.size() ))	return (GLamObject*)this ;
	return m_obj.at(id) ;
}

/*! Lien générique vers objet du groupement.
 * Le critère de recherche est la propriété @ref obj_name "name" héritée
 * de GLamObject.
 * Retourne l'adresse de l'objet GLamGroup si @a name incorrect.
 */

GLamObject*	GLamGroup::object(const QString& name ) const
{
	for ( int i = 0 ; i < m_obj.size() ; ++i ) {
		if ( m_obj.at(i)->name() == name )	return m_obj.at(i) ;
	}
	return (GLamObject*)this ;
}

/*! Retrait de tous les objets du groupement, mais
 * conservation des attributs globaux : default/transform matrix, material...
 */

void GLamGroup::removeAllObjects()
{
	if ( m_lifeCycle == Composition ) {
		while ( !m_obj.isEmpty() )	delete m_obj.takeLast() ;
	}
	m_obj.clear() ;
}

/*! Retourne le nom du groupement suivi de la liste des composants
 * Le nom seul du groupe peut être obtenu par GLamObject::name().
 */

QString GLamGroup::name() const
{
	QString s = GLamObject::name() ;
	if ( m_obj.size() )	s += " [ " ;
	for ( int i = 0 ; i < m_obj.size() ; ++i ) {
		if ( m_obj.at(i)->className() == "GLamGroup" )
				s += m_obj.at(i)->GLamObject::name() ;
		else	s += m_obj.at(i)->name() ;
		if ( i < m_obj.size() - 1 )	s += " + " ;
		else s += " ]" ;
	}
	return s ;
}

/*! Calcul cumulé des triangles du groupement. */

int GLamGroup::numTriangles() const
{
	int res = 0 ;
	for ( int i = 0 ; i < m_obj.size() ; ++i )	res += m_obj.at(i)->numTriangles() ;
	return res ;
}

/*! Calcul cumulé des vertices du groupement. */

int GLamGroup::numVertices() const
{
	int res = 0 ;
	for ( int i = 0 ; i < m_obj.size() ; ++i )	res += m_obj.at(i)->numVertices() ;
	return res ;
}

/*! Projection du groupement d'objets avec prise en compte des matrices
 * @ref obj_defaultMatrix "defaultMatrix" et @ref obj_transformMatrix
 * "transformMatrix" globales (maintenues par le groupement).
 */

void GLamGroup::GLamGroup::draw(GLamObject* parent )
{
	Q_UNUSED( parent ) ;

	for ( int i = 0 ; i < m_obj.size() ; ++i ) {

		if ( m_materialChanged )	m_obj.at(i)->setMaterial( GLamObject::material() ) ;

		QMatrix4x4 md = m_obj.at(i)->defaultMatrix() ;

		m_obj.at(i)->defaultMatrix() = this->defaultMatrix() * this->transformMatrix() * md ;
		m_obj.at(i)->draw( this ) ;
		m_obj.at(i)->defaultMatrix() = md ;
	}
	m_materialChanged = false ;
	m_countDrawing++ ;
}

/*! Modification forcée de tous les objets du groupe. */

void GLamGroup::setWireFrame(bool wireFrame )
{
	GLamObject::setWireFrame( wireFrame ) ;
	for ( int i = 0 ; i < m_obj.size() ; ++i )	m_obj.at(i)->setWireFrame(wireFrame ) ;
}

/*! Modification forcée de tous les objets du groupe. */

void GLamGroup::setShadingEffect(ShadingEffect effect )
{
	GLamObject::setShadingEffect( effect ) ;
	for ( int i = 0 ; i < m_obj.size() ; ++i )	m_obj.at(i)->setShadingEffect(effect ) ;
}

/*! Modification forcée de tous les objets du groupe. */

void GLamGroup::setMaterial(GLamMaterial material )
{
	GLamObject::setMaterial( material ) ;
	for ( int i = 0 ; i < m_obj.size() ; ++i )	m_obj.at(i)->setMaterial(material ) ;
}

/*! Retourne le matériau générique du groupement. */

GLamMaterial& GLamGroup::material()
{
	m_materialChanged = true ;
	return GLamObject::material() ;
}

/*! Retourne le vertex du groupe dont les coordonnées signées x, y et z
 * sont les plus petites (coin inférieur arrière gauche du parallépipède
 * enveloppe) ; en tenant compte de la matrice de
 * transformation @ref obj_defaultMatrix "defaultMatrix".
 * @sa max(), dim()
 */

QVector3D GLamGroup::min()
{
	if ( !m_obj.size() )	m_min = QVector3D() ;
	else m_min = m_obj.at(0)->min() ;

	for ( int i = 1 ; i < m_obj.size() ; ++i ) {
		QVector3D	v = m_obj.at(i)->min() ;
		if ( v.x() < m_min.x() )	m_min.setX( v.x() ) ;
		if ( v.y() < m_min.y() )	m_min.setY( v.y() ) ;
		if ( v.z() < m_min.z() )	m_min.setZ( v.z() ) ;
	}
	return defaultMatrix().map( m_min ) ;
}

/*! Retourne le vertex du groupe dont les coordonnées signées x, y et z
 * sont les plus grandes (coin supérieur avant droit du parallépipède
 * enveloppe) ; en tenant compte de la matrice de
 * transformation @ref obj_defaultMatrix "defaultMatrix".
 * @sa min(), dim()
 */

QVector3D GLamGroup::max()
{
	if ( !m_obj.size() )	m_max = QVector3D() ;
	else m_max = m_obj.at(0)->max() ;

	for ( int i = 1 ; i < m_obj.size() ; ++i ) {
		QVector3D	v = m_obj.at(i)->max() ;
		if ( v.x() > m_max.x() )	m_max.setX( v.x() ) ;
		if ( v.y() > m_max.y() )	m_max.setY( v.y() ) ;
		if ( v.z() > m_max.z() )	m_max.setZ( v.z() ) ;
	}
	return defaultMatrix().map( m_max ) ;
}
