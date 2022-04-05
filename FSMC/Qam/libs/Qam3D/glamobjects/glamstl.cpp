/*  ---------------------------------------------------------------------------
 *  filename    :   glamstl.cpp
 *  description :   IMPLEMENTATION de la classe GLamStL
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

#include "glamstl.h"
#include "glamgroup.h"
#include "../_VERSION"
#include <QFile>
#include <QFileInfo>

/*! Constructeur par défaut.
 * Génère la célèbre "Suzanne" de Blender...
 */

GLamStL::GLamStL()
	: GLamObject()
{
	m_hasExternalVerticesProvider = true ;
	load(":/res/suzanne.stl") ;
	updateNeeded() ;

	setClassName("GLamStL") ;
	setName("suzanne") ;

	defaultMatrix().rotate(-90, QVector3D( 1, 0, 0 ) ) ;
	defaultMatrix().scale(0.5) ;
}

/*! Construction par chargement d'un fichier STL externe. */

GLamStL::GLamStL(const QString& filename )
	: GLamObject()
{
	m_hasExternalVerticesProvider = true ;
	load( filename ) ;
	updateNeeded() ;

	setClassName("GLamStL") ;
	QFileInfo fi( filename ) ;
	setName( fi.baseName().left(16) ) ;
}

/*! Construction par copie des vertices d'un objet de la hiérarchie GLamObject.
 * Les transformations induites par la defaultMatrix() de l'objet
 * sont directement appliquées aux vertices ;
 * la defaultMatrix() de l'objet GLamStL est donc la matrice identité.
 */

GLamStL::GLamStL(GLamObject* obj, const QString& name )
	: GLamObject()
{
	// matrices de transformation, matériau, class name, name...
	*(GLamObject*)this = *obj ;

//	obj->update();
//	qDebug() << "GLamStL" << obj->numVertices()  << obj->numVertexIndices() ;

	eraseAllDatas() ;
	addFacetsFromObject( obj ) ;
	defaultMatrix().setToIdentity() ;

	m_hasExternalVerticesProvider = true ;
	updateNeeded() ;

	setWireFrame( false ) ;

	setClassName("GLamStL") ;
	if ( !name.isNull() )	setName( QString(name).left(16) ) ;
}

/*! Construction par copie des vertices d'un groupement d'objets de la
 * hiérarchie GLamObject.
 * Les transformations relatives à l'intérieur du groupement
 * sont fournies par les defaultMatrix() de chacun des objets
 * et appliquées directement aux vertices.
 * La matrice globale du GLamGroup est elle aussi appliquée
 * aux vertices du GLamStL ;
 * la defaultMatrix() de l'objet GLamStL est donc la matrice identité.
 */

GLamStL::GLamStL(GLamGroup* obj, const QString& name )
	: GLamObject()
{
	eraseAllDatas() ;
	for ( int i = 0 ; i < obj->numObjects() ; ++i ) {
		addFacetsFromObject( obj->object(i), obj->defaultMatrix() ) ;
	}

	defaultMatrix().setToIdentity() ;

	m_hasExternalVerticesProvider = true ;
	updateNeeded() ;

	setWireFrame( false ) ;

	setClassName("GLamStL") ;
	if ( !name.isNull() )	setName( QString(name).left(16) ) ;
}

/*! Constructeur de copie. */

GLamStL::GLamStL(const GLamStL& obj )
	: GLamObject()
{
	*this = obj ;
}

// destructeur
/*
GLamStL::~GLamStL()
{
}
*/
/*! Surcharge de l'opérateur d'affectation. */

GLamStL& GLamStL::operator =(const GLamStL& obj )
{
	*(GLamObject*)this = obj ;

	setName( obj.name() ) ;

	m_hasExternalVerticesProvider = true ;
	updateNeeded() ;

	return *this ;
}

// lecture/écriture de fichiers STL
// ---------------------------------------------------------------------------

/*! Lecture du fichier STL @a filename. */

bool GLamStL::load(const QString& filename )
{
	QFile file( filename ) ;
	if ( !file.open(QIODevice::ReadOnly) ) {
		error("load: STL file not found, use default..." ) ;
		load(":/res/suzanne.stl") ;
		return true ;
	}

	info( QString("loading %1...").arg(filename) ) ;
//
	this->eraseAllDatas() ;
//
	/*QByteArray header =*/ file.read(80) ;

	QDataStream	stream(&file) ;
	stream.setByteOrder(QDataStream::LittleEndian) ;
	stream.setFloatingPointPrecision(QDataStream::SinglePrecision) ;

	qint32	size ;
	stream >> size ;

	eraseAllDatas() ; //clearFacets() ;

	for ( qint32 i = 0 ; i < size ; ++i ) {

		QVector3D	/*norm,*/ v1, v2, v3 ;
		float x, y , z ;
		qint16	attr ;

		stream >> x >> y >> z ; //norm = QVector3D(x,y,z) ;
		stream >> x >> y >> z ; v1 = QVector3D(x,y,z) ;
		stream >> x >> y >> z ; v2 = QVector3D(x,y,z) ;
		stream >> x >> y >> z ; v3 = QVector3D(x,y,z) ;
		stream >> attr ;

		triangle(v1, v2, v3 ) ;
	}

	defaultMatrix().setToIdentity() ;

	file.close() ;
	return true ;
}

/*! Ecriture du fichier STL @a filename
 * avec prise en compte de la defaultMatrix() de l'objet GLamStL
 * (ce qui permet des ajustements d'échelle...).
 */

bool GLamStL::save(const QString& filename )
{
	QFile file( filename ) ;
	if ( !file.open(QIODevice::WriteOnly) ) {
		error( "save: can't open STL file" ) ;
		return false ;
	}

	info( QString("saving %1...").arg(filename) ) ;

	qint32	size = m_iVertex.count() / 3 ;

	QByteArray header ;
	header.append( "from Qam3D/GLam" ) ;
	header = header.leftJustified(16, ' ' ) ;
//	header.append( QString("v%1 by A.M.").arg(QAM3D_VERSION) ) ;
    header.append( QString("v%1 by A.M.").arg(QAM3D_VERSION).toLatin1() ) ;
    header = header.leftJustified(32, ' ' ) ;
//	header.append( name() ) ;
    header.append( name().toLatin1() ) ;
    header = header.leftJustified(48, ' ' ) ;
//	header.append( QString("%1 facets").arg(size) ) ;
    header.append( QString("%1 facets").arg(size).toLatin1() ) ;
    header = header.leftJustified(80, ' ' ) ;
	header[79] = '\n' ;

	QDataStream	stream(&file) ;

	stream.setByteOrder(QDataStream::LittleEndian) ;
	stream.setFloatingPointPrecision(QDataStream::SinglePrecision) ;

	stream.writeRawData(header.data(), 80 ) ;
	stream << size ;

	QMatrix4x4	md = this->defaultMatrix() ;

	for ( qint32 i = 2 ; i < numVertexIndices() ; i += 3 ) {
//	for ( qint32 i = 0 ; i < m_iVertex.count() ; i += 3 ) {
		QVector3D norm = m_flatNormals.at( m_iFlatNormal.at(i-2) ) ;
		norm.normalize() ;
//		QVector3D v1 = md.map( m_vertices.at( m_iVertex.at(i) ) ) ;
//		QVector3D v2 = md.map( m_vertices.at( m_iVertex.at(i+1) ) ) ;
//		QVector3D v3 = md.map( m_vertices.at( m_iVertex.at(i+2) ) ) ;
		QVector3D v1 = md.map( vertex(i-2) ) ;
		QVector3D v2 = md.map( vertex(i-1) ) ;
		QVector3D v3 = md.map( vertex(i) ) ;
		qint16	attr = 0 ;

		stream << norm.x() << norm.y() << norm.z() ;
		stream << v1.x() << v1.y() << v1.z() ;
		stream << v2.x() << v2.y() << v2.z() ;
		stream << v3.x() << v3.y() << v3.z() ;
		stream << attr ;
	}

	file.close() ;
	return true ;
}

// gestion de la liste des vertices/normales de l'objet 3D
// ---------------------------------------------------------------------------

/*! Pour concrétisation de la classe... */

void GLamStL::create()
{
}

// copie des vertices et normales d'un objet
// avec application de la matrice par défaut de l'objet
// et d'une matrice commune mg le cas échéant (groupement).

void GLamStL::addFacetsFromObject(GLamObject* obj, const QMatrix4x4& mg )
{
	obj->update() ;
	QMatrix4x4	md = obj->defaultMatrix() ;
	for ( int i = 2 ; i < obj->numVertexIndices() ; i += 3 ) {
//		QVector3D v1 = obj->m_vertices.at( obj->m_iVertex[i-2] ) ;
//		QVector3D v2 = obj->m_vertices.at( obj->m_iVertex[i-1] ) ;
//		QVector3D v3 = obj->m_vertices.at( obj->m_iVertex[i] ) ;
		QVector3D v3 = obj->vertex(i-2) ;	// ordre CCW avec ça ??
		QVector3D v2 = obj->vertex(i-1) ;
		QVector3D v1 = obj->vertex(i) ;
//		triangle( mg * md * v1, mg * md * v2, mg * md * v3 ) ;
        QMatrix4x4 m = mg * md ;
        triangle( m.map(v1), m.map(v2), m.map(v3) ) ;
    }
}
