/*  ---------------------------------------------------------------------------
 *  filename    :   glammesh.cpp
 *  description :   IMPLEMENTATION de la classe GLamMesh
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

#include "glammesh.h"
#include <QFile>
#include <QFileInfo>

/*! Constructeur par défaut. */
// objet vide impératif (composant de base de GLamObjMtl)

GLamMesh::GLamMesh()
	: GLamObject()
{
	setClassName("GLamMesh") ;
	m_hasExternalVerticesProvider = true ;	// create() n.u.
}

/*! Construction à partir d'un fichier RAW.
 * @sa load().
 */

GLamMesh::GLamMesh(const QString& filename )
	: GLamObject()
{
	setClassName("GLamMesh") ;
	QFileInfo fi( filename ) ;
	setName( fi.baseName() ) ;
	load(filename ) ;

	m_hasExternalVerticesProvider = true ;	// create() n.u.
	updateNeeded() ;
}

/*! Construction à partir d'une liste de triplets de vertices. */

GLamMesh::GLamMesh(QVector<QVector3D> vertices )
	: GLamObject()
{
	setClassName("GLamMesh") ;

	int ntri = vertices.size() / 3 ;
	for ( int i = 0 ; i < ntri ; ++i ) {
		QVector3D v1 = vertices.at( i * 3 ) ;
		QVector3D v2 = vertices.at( i * 3 + 1 ) ;
		QVector3D v3 = vertices.at( i * 3 + 2 ) ;
		triangle( v1, v2, v3 ) ;
	}

	m_hasExternalVerticesProvider = true ;	// create() n.u.
	updateNeeded() ;
}

/*! Constructeur de copie. */

GLamMesh::GLamMesh(const GLamMesh& obj )
	: GLamObject()
{
	*this = obj ;
}

/*! Surcharge de l'opérateur d'affectation. */

GLamMesh& GLamMesh::operator =(const GLamMesh& obj )
{
	*(GLamObject*)this = obj ;
	setClassName("GLamMesh") ;
	setName( obj.name() ) ;
	m_hasExternalVerticesProvider = true ;	// create() n.u.
	updateNeeded() ;
	return *this ;
}

/*! Pour concrétisation de la classe... */

void GLamMesh::create()
{
}

/*! Retourne le nombre courant de triangles. */

int GLamMesh::numTrimesh() const
{
	return m_iVertex.count() / 3 ;
}

/*! Méthode de récupération d'un triangle (3 vertices).
 * Retourne 3 vecteurs nuls si l'@a index est incorrect.
 * @sa numTrimesh()
 */

QList<QVector3D> GLamMesh::trimesh(int index )
{
	QList<QVector3D> tri ;
	QVector3D v ;
	if ( index < numTrimesh() ) {
		tri << m_vertices.at( m_iVertex[index * 3] ) ;
		tri << m_vertices.at( m_iVertex[index * 3 + 1] ) ;
		tri << m_vertices.at( m_iVertex[index * 3 + 2] ) ;
	}
	else	tri << v << v << v ;
	return tri ;
}

// import/export de fichiers RAW
// ---------------------------------------------------------------------------

/*! Lecture d'un fichier RAW, avec raz de la defaultMatrix()
 * format reconnu : "x1 y1 z1 x2 y2 z2 x3 y3 z3 [x4 y4 z4]\n".
 * @n En cas d'échec d'ouverture du fichier @a filename, le solide généré est
 * un cube de taille 1 centré à l'origine.
 */

bool GLamMesh::load(const QString& filename )
{
	QFile file( filename ) ;
	if ( !file.open(QIODevice::ReadOnly | QIODevice::Text) ) {
		error("load: RAW file not found, use default..." ) ;
		load(":/res/cube.raw" ) ;
		return true ;
	}

	info( QString("loading %1...").arg(filename) ) ;

	this->eraseAllDatas() ;

	QTextStream stream( &file ) ;

	QString line = stream.readLine() ;
	while ( !line.isNull() ) {
//		QStringList tokens = line.split(' ', QString::SkipEmptyParts ) ;
        QStringList tokens = line.split(' ', Qt::SkipEmptyParts ) ;
        if ( line.isEmpty() || tokens.at(0).isEmpty() || tokens.at(0).startsWith('#') ) {
			line = stream.readLine() ;
			continue ;
		}
		if (( tokens.size() != 9 )&&( tokens.size() != 12 )) {
			warning( "bad line in RAW file" ) ;
			line = stream.readLine() ;
			continue ;
		}
		QVector3D v[4] ;
		for ( int i = 0 ; i < tokens.size() / 3 ; ++i ) {
			float x = tokens.at( i * 3 ).toFloat() ;
			float y = tokens.at( i * 3 + 1 ).toFloat() ;
			float z = tokens.at( i * 3 + 2 ).toFloat() ;
			v[i] = QVector3D(x, y ,z ) ;
		}

		triangle(v[0], v[1], v[2] ) ;
		if ( tokens.size() == 12 )	triangle(v[0], v[2], v[3] ) ;

		line = stream.readLine() ;
	}

	this->defaultMatrix().setToIdentity() ;

	file.close() ;
	return true ;
}

/*! Ecriture d'un fichier RAW format "x1 y1 z1 x2 y2 z2 x3 y3 z3\n"
 * avec prise en compte de la defaultMatrix() pour d'éventuels
 * ajustements d'échelle et d'attitude spatiale.
 */

bool GLamMesh::save(const QString& filename )
{
	QFile file( filename ) ;
	if ( !file.open(QIODevice::WriteOnly | QIODevice::Text) ) {
		error( "save: can't open RAW file" ) ;
		return false ;
	}

	info( QString("saving %1...").arg(filename) ) ;

	QMatrix4x4	matrix = this->defaultMatrix() ;

	QTextStream stream( &file ) ;

	for ( int i = 0 ; i < numTrimesh() ; ++i ) {
		for ( int j = 0 ; j < 3 ; ++j ) {
			QVector3D v = matrix.map( trimesh( i ).at( j ) ) ;
			stream << QString("%1 %2 %3 ").arg(v.x(), 0, 'f', 6 ).arg(v.y(), 0, 'f', 6 ).arg(v.z(), 0, 'f', 6 ) ;
		}
		stream << '\n' ;
	}

	file.close() ;
	return true ;
}
