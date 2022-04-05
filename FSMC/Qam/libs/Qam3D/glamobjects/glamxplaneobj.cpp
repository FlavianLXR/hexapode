/*  ---------------------------------------------------------------------------
 *  filename    :   glamxplaneobj.cpp
 *  description :   IMPLEMENTATION de la classe GLamXplaneObj
 *
 *	project     :	Qam3D/GLam: 3D object collection
 *  start date  :   décembre 2009
 *  ---------------------------------------------------------------------------
 *  Copyright 2009-2018 by Alain Menu   <alain.menu@ac-creteil.fr>
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

#include "glamxplaneobj.h"
#include <QFile>
#include <QFileInfo>
#include <QRegularExpression>

/*! Constructeur par défaut. */

GLamXplaneObj::GLamXplaneObj()
	: GLamObject()
{

}

/*! Construction par chargement d'un fichier OBJ externe. */

GLamXplaneObj::GLamXplaneObj(const QString& filename )
	: GLamObject()
{
	m_hasExternalVerticesProvider = true ;
	load( filename ) ;
	updateNeeded() ;

	setClassName("GLamXplaneObj") ;
	QFileInfo fi( filename ) ;
	setName( fi.baseName() ) ;
}

/*! Construction à partir d'un objet de la hiérarchie GLamObject. */

GLamXplaneObj::GLamXplaneObj(GLamObject* obj, const QString& name )
	: GLamObject()
{
	setClassName("GLamXplaneObj") ;
	*this = obj ;
	if ( !name.isNull() ) setName( name ) ;
	updateNeeded() ;
}

/*! Construction à partir d'un groupement d'objets de la hiérarchie GLamObject. */

GLamXplaneObj::GLamXplaneObj(GLamGroup* obj, const QString& name )
	: GLamObject()
{
	setClassName("GLamXplaneObj") ;
	for ( int i = 0 ; i < obj->numObjects() ; ++i ) {
		*this = obj->object(i) ;
	}
	if ( !name.isNull() ) setName( name ) ;
	updateNeeded() ;
}

/*! Constructeur de copie. */

GLamXplaneObj::GLamXplaneObj(const GLamXplaneObj& obj )
	: GLamObject()
{
	*this = obj ;
}

/*! Surcharge de l'opérateur d'affectation. */

GLamXplaneObj& GLamXplaneObj::operator =(const GLamXplaneObj& obj )
{
	*(GLamObject*)this = obj ;
	updateNeeded() ;
	return *this ;
}

bool GLamXplaneObj::load(const QString& filename )
{
	QFile file( filename ) ;
	if ( !file.open(QIODevice::ReadOnly | QIODevice::Text) ) {
		error("load: XPLane OBJ file not found" ) ;
		return false ;
	}

	info( QString("loading %1...").arg(filename) ) ;

	this->eraseAllDatas() ;

	int tris = 0 ;
	int indices = 0 ;

	QFileInfo	fi( file ) ;
	QString path = fi.absolutePath() ;

	QTextStream stream( &file ) ;

	QString line = stream.readLine() ;

	while ( !line.isNull() ) {
//		QStringList tokens = line.split(QRegExp("\\s+"), QString::SkipEmptyParts ) ;
        QStringList tokens = line.split(QRegularExpression("\\s+"), Qt::SkipEmptyParts ) ;

		if ( line.isEmpty() || tokens.at(0).isEmpty() || tokens.at(0).startsWith('#') ) {
			line = stream.readLine() ;
			continue ;
		}
//qDebug() << line ;
		if      ( tokens.at(0) == "A" ) {
		}
		else if ( tokens.at(0) == "800" ) {
		}
		else if ( tokens.at(0) == "OBJ" ) {
		}
		else if ( tokens.at(0) == "TEXTURE" ) {
			if ( tokens.size() > 1 ) {
				QString ftex( path ) ;
				ftex += "/" ;
				ftex += tokens.at(1) ;
				this->material().setTexture(ftex) ;
			}
		}
		else if ( tokens.at(0) == "POINT_COUNTS" ) {
//			qDebug() << line ;
			tris = ( tokens.size() > 1 ? tokens.at(1).toInt() : 0 ) ;
			indices = ( tokens.size() > 4 ? tokens.at(4).toInt() : 0 ) ;
		}
		else if ( tokens.at(0) == "VT" ) {
			float x = ( tokens.size() > 1 ? tokens.at(1).toFloat() : 0 ) ;
			float y = ( tokens.size() > 2 ? tokens.at(2).toFloat() : 0 ) ;
			float z = ( tokens.size() > 3 ? tokens.at(3).toFloat() : 0 ) ;
			m_v << QVector3D(x, y, z ) ;
//			float xn = ( tokens.size() > 4 ? tokens.at(4).toFloat() : 0 ) ;
//			float yn = ( tokens.size() > 5 ? tokens.at(5).toFloat() : 0 ) ;
//			float zn = ( tokens.size() > 6 ? tokens.at(6).toFloat() : 0 ) ;
			float s = ( tokens.size() > 7 ? tokens.at(7).toFloat() : 0 ) ;
			float t = ( tokens.size() > 8 ? tokens.at(8).toFloat() : 0 ) ;
			m_t << QVector2D( s, t ) ;
		}
		else if ( tokens.at(0) == "IDX10" ) {
			for ( int i = 1 ; i <= 10 ; ++i ) {
				if ( tokens.size() > i )	m_i << tokens.at(i).toInt() ;
			}
		}
		else if ( tokens.at(0) == "IDX" ) {
			if ( tokens.size() > 1 )	m_i << tokens.at(1).toInt() ;
		}

		line = stream.readLine() ;
	}

//	qDebug() << "GLamXplaneObj::load()" ;
//	qDebug() << "m_v" << m_v.count() ;
//	qDebug() << "m_t" << m_t.count() ;
//	qDebug() << "m_i" << m_i.count() ;

	if ( m_v.count() != tris )		qDebug() << "unexpected tris number!" ;
	if ( m_i.count() != indices )	qDebug() << "unexpected indices number!" ;

//	if ( m_i.size() < 3 )	return false ;

	for ( int i = 2 ; i < m_i.size() ; i += 3 ) {
		triangle(m_v.at(m_i.at(i-2)), m_v.at(m_i.at(i-1)), m_v.at(m_i.at(i)) ) ;
		if ( !m_t.isEmpty() )	triTex(m_t.at(m_i.at(i-2)), m_t.at(m_i.at(i-1)), m_t.at(m_i.at(i)) ) ;
	}

//	qDebug() << "vertices n/i" << m_vertices.size() << "/" << m_iVertex.size() ;
//	qDebug() << "normals  n/i" << m_flatNormals.size() << "/" << m_iFlatNormal.size() ;
//	qDebug() << "texels   n/i" << m_texels.size() << "/" << m_iTexel.size() ;

	file.close() ;
	return true ;
}

bool GLamXplaneObj::save(const QString& filename )
{
	QFile file( filename ) ;
	if ( !file.open(QIODevice::WriteOnly | QIODevice::Text) ) {
		error("save: can't open XPlane OBJ file" ) ;
		return false ;
	}

	info( QString("saving %1...").arg(filename) ) ;
	QTextStream stream( &file ) ;

	QString ftex = this->material().textureFilename() ;
	QFileInfo fi( ftex ) ;

	stream << QString("A\n") ;
	stream << QString("800\n") ;
	stream << QString("OBJ\n") ;
	stream << QString("\n") ;
	stream << QString("TEXTURE %1\n").arg( fi.fileName() ) ;
	stream << QString("POINT_COUNTS %1 0 0 %2\n")
			  .arg( this->numVertices() ).arg( this->numVertexIndices() ) ;

	qDebug() << "GLamXplaneObj::save()" ;
	qDebug() << "vertices n/i" << m_vertices.size() << "/" << m_iVertex.size() ;
	qDebug() << "normals  n/i" << m_flatNormals.size() << "/" << m_iFlatNormal.size() ;
	qDebug() << "texels   n/i" << m_texels.size() << "/" << m_iTexel.size() ;

	for ( int i = 0 ; i < numVertices() ; ++i ) {
		stream << QString("VT %1 %2 %3 %4\n")
				  .arg( v2s(vertex(i)) )
				  .arg( v2s(m_flatNormals.at(m_iFlatNormal[i]) ) )
				  .arg( f2s(texel(i).x()) )
				  .arg( f2s(texel(i).y()) ) ;
	}
	int div = numVertexIndices() / 10 ;
	int mod = numVertexIndices() % 10 ;
//	qDebug() << numVertexIndices() << "/" << div * 10 + mod ;
	for ( int i = 0 ; i < div ; ++i ) {
		stream << QString("IDX10") ;
		for ( int j = 0 ; j < 10 ; ++j )	stream << QString(" %1").arg( m_iVertex.at(i*10+j)) ;
		stream << QString("\n") ;
	}
	for ( int i = 0 ; i < mod ; ++i ) {
		stream << QString("IDX %1\n").arg(m_iVertex.at(div*10+i) ) ;
	}

	file.close() ;
	return true ;
}

/*! Pour concrétisation de la classe... */

void GLamXplaneObj::create()
{
}

// [private] float format = x.xxxxxx

QString GLamXplaneObj::f2s(float v )
{
	return QString("%1").arg(v, 0, 'f', 6 ) ;
}

// [private] vector format = x.xxxxxx y.yyyyyy z.zzzzzz

QString GLamXplaneObj::v2s(QVector3D v )
{
	return QString("%1 %2 %3").arg(v.x(), 0, 'f', 6 ).arg(v.y(), 0, 'f', 6 ).arg(v.z(), 0, 'f', 6 ) ;
}
