/*  ---------------------------------------------------------------------------
 *  filename    :   glamobjmtl.cpp
 *  description :   IMPLEMENTATION de la classe GLamObjMtl
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

#include "glamobjmtl.h"
#include "../_VERSION"
#include <QFile>
#include <QFileInfo>

/*! Constructeur par défaut, génère la célèbre @a Utah @a teapot. */

GLamObjMtl::GLamObjMtl()
	: GLamGroup()
{
	setClassName("GLamObjMtl") ;
	setName("teapot") ;

	m_hasExternalVerticesProvider = true ;
	load(":/res/teapot.obj") ;
	updateNeeded() ;
}

/*! Construction par chargement du fichier OBJ de chemin @a filename.
 * La procédure prend en charge la lecture du MTL associé le cas échéant.
 */

GLamObjMtl::GLamObjMtl(const QString& filename )
	: GLamGroup()
{
	setClassName("GLamObjMtl") ;
	QFileInfo fi( filename ) ;
	setName( fi.baseName() ) ;

	m_hasExternalVerticesProvider = true ;
	load( filename ) ;
	updateNeeded() ;
}

/*! Construction à partir d'un objet de la hiérarchie GLamObject. */

GLamObjMtl::GLamObjMtl(GLamObject* obj, const QString& name )
	: GLamGroup()
{
	setClassName("GLamObjMtl") ;
	if ( !name.isNull() ) setName( name ) ;
	addObject( (GLamMesh*)obj ) ;
	updateNeeded() ;
}

/*! Construction à partir d'un groupement d'objets de la hiérarchie GLamObject. */

GLamObjMtl::GLamObjMtl(GLamGroup* obj, const QString& name )
	: GLamGroup()
{
	setClassName("GLamObjMtl") ;
	if ( !name.isNull() ) setName( name ) ;
	addObject( (GLamMesh*)obj ) ;
	for ( int i = 0 ; i < obj->numObjects() ; ++i ) {
		this->addObject( obj->object(i) ) ;
	}
	updateNeeded() ;
}

/*! Constructeur de copie. */

GLamObjMtl::GLamObjMtl(const GLamObjMtl& obj )
	: GLamGroup()
{
	*this = obj ;
}

/*! Surcharge de l'opérateur d'affectation. */

GLamObjMtl& GLamObjMtl::operator =(const GLamObjMtl& obj )
{
	*(GLamGroup*)this = obj ;
	updateNeeded() ;
	return *this ;
}

/*! Méthode utilitaire de redimensionnement des texels d'un matériau.
 */

void GLamObjMtl::stretchTexels(const QString& mtlName, QVector2D vmin, QVector2D vmax )
{
	for ( int i = 0 ; i < numObjects() ; ++i ) {
		GLamMaterial mat = object(i)->material() ;
		if ( mat.name() == mtlName ) {
			QVector2D tmin = object(i)->m_texels.at(0) ;
			QVector2D tmax = tmin ;
			for ( int t = 1 ; t < object(i)->m_texels.size() ; ++t ) {
				QVector2D v = object(i)->m_texels.at(t) ;
				if ( v.x() < tmin.x() )	tmin.setX( v.x() ) ;
				if ( v.x() > tmax.x() )	tmax.setX( v.x() ) ;
				if ( v.y() < tmin.y() )	tmin.setY( v.y() ) ;
				if ( v.y() > tmax.y() )	tmax.setY( v.y() ) ;
			}
			float rx = ( vmax.x() - vmin.x() ) / ( tmax.x() - tmin.x() ) ;
			float ry = ( vmax.y() - vmin.y() ) / ( tmax.y() - tmin.y() ) ;
			for ( int t = 0 ; t < object(i)->m_texels.size() ; ++t ) {
				QVector2D v = object(i)->m_texels.at(t) ;
				v.setX( v.x() * rx + vmin.x() - tmin.x() * rx ) ;
				v.setY( v.y() * ry + vmin.y() - tmin.y() * ry ) ;
				object(i)->m_texels[t].setX( v.x() ) ;
				object(i)->m_texels[t].setY( v.y() ) ;
			}
		}
	}
}

/*! Méthode utilitaire de miroir des texels d'un matériau.
 */

void GLamObjMtl::reflectTexels(const QString& mtlName, bool acrossXaxis, bool acrossYaxis )
{
	for ( int i = 0 ; i < numObjects() ; ++i ) {
		GLamMaterial mat = object(i)->material() ;
		if ( mat.name() == mtlName ) {
			QVector2D tmin = object(i)->m_texels.at(0) ;
			QVector2D tmax = tmin ;
			for ( int t = 1 ; t < object(i)->m_texels.size() ; ++t ) {
				QVector2D v = object(i)->m_texels.at(t) ;
				if ( v.x() < tmin.x() )	tmin.setX( v.x() ) ;
				if ( v.x() > tmax.x() )	tmax.setX( v.x() ) ;
				if ( v.y() < tmin.y() )	tmin.setY( v.y() ) ;
				if ( v.y() > tmax.y() )	tmax.setY( v.y() ) ;
			}
			float dx = tmax.x() + tmin.x() ;
			float dy = tmax.y() + tmin.y() ;
			for ( int t = 0 ; t < object(i)->m_texels.size() ; ++t ) {
				QVector2D v = object(i)->m_texels.at(t) ;
				if ( acrossYaxis )	v.setX( dx - v.x() ) ;
				if ( acrossXaxis )	v.setY( dy - v.y() ) ;
				object(i)->m_texels[t].setX( v.x() ) ;
				object(i)->m_texels[t].setY( v.y() ) ;
			}
		}
	}
}

// ---------------------------------------------------------------------------
// lecture de fichiers OBJ/MTL
// ---------------------------------------------------------------------------

/*! Lecture du fichier Wavefront dont le chemin est spécifié par @a filename ;
 * et de son ou ses fichiers MTL associés le cas échéant.
 * Les éléments trouvés sont ajoutés au groupement (GLamObjMtl est dérivé de
 * GLamGroup) sous forme d'objets GLamMesh.
 */

bool GLamObjMtl::load(const QString& filename )
{
	QFile file( filename ) ;
	if ( !file.open(QIODevice::ReadOnly | QIODevice::Text) ) {
		error("load: Wavefront OBJ file not found, use default..." ) ;
		load(":/res/teapot.obj") ;
		return true ;
	}

	info( QString("loading %1...").arg(filename) ) ;

	this->eraseAllDatas() ;

	int invalidFaces1 = 0 ;	// face with fewer than 3 vertices counter
	int invalidFaces2 = 0 ;	// face with identical vertex indices counter

	QFileInfo	fi( file ) ;
	QString path = fi.absolutePath() ;

	m_matList.clear() ;
	m_matList << GLamMaterial() ;	// 1 matériau par défaut

	m_obj = new GLamMesh() ;		// 1 objet par défaut

	QTextStream stream( &file ) ;

	QString line = stream.readLine() ;

	while ( !line.isNull() ) {
//		QStringList tokens = line.split(' ', QString::SkipEmptyParts ) ;
        QStringList tokens = line.split(' ', Qt::SkipEmptyParts ) ;

		if ( line.isEmpty() || tokens.at(0).isEmpty() || tokens.at(0).startsWith('#') ) {
			line = stream.readLine() ;
			continue ;
		}
		if      ( tokens.at(0) == "mtllib" ) {
			if ( tokens.size() > 1 ) {
				QString fmtl( path ) ;
				fmtl += "/" ;
				fmtl += tokens.at(1) ;
				loadMtl(fmtl ) ;
			}
		}
		else if ( tokens.at(0) == "usemtl" ) {
			if ( tokens.size() > 1 ) {
				int i = 0 ;
				for ( ; i < m_matList.size() ; ++i ) {
					if ( m_matList.at(i).name() == tokens.at(1) )	break ;
				}
				if ( i >= m_matList.size() )	i = 0 ;

				if ( m_obj->numTrimesh() )	this->addObject( m_obj ) ;
				delete m_obj ;
				m_obj = new GLamMesh() ;

				m_obj->material() = m_matList.at(i) ;
			}
		}
		else if ( tokens.at(0) == "g" ) {
			if ( tokens.size() > 1 ) {
				m_obj->setName( tokens.at(1) ) ;
			}
		}
		else if ( tokens.at(0) == "o" ) {
			if ( tokens.size() > 1 )	this->setName( tokens.at(1) ) ;
		}
		else if ( tokens.at(0) == "v" ) {
			float x = ( tokens.size() > 1 ? tokens.at(1).toFloat() : 0 ) ;
			float y = ( tokens.size() > 2 ? tokens.at(2).toFloat() : 0 ) ;
			float z = ( tokens.size() > 3 ? tokens.at(3).toFloat() : 0 ) ;
			m_v << QVector3D(x, y, z ) ;
		}
		else if ( tokens.at(0) == "vt" ) {
			float x = ( tokens.size() > 1 ? tokens.at(1).toFloat() : 0 ) ;
			float y = ( tokens.size() > 2 ? tokens.at(2).toFloat() : 0 ) ;
			m_t << QVector2D( x, y ) ;
		}
//		else if ( tokens.at(0) == "vn" ) {
//		}
		else if ( tokens.at(0) == "s" ) {	// experimental...
			if ( tokens.size() > 1 ) {
				m_obj->setShadingEffect( tokens.at(1) != "off" ? Phong : Flat ) ;
			}
		}
		else if ( tokens.at(0) == "f" ) {

			if ( tokens.size() < 4 ) {
				invalidFaces1++ ;
				//	qDebug() << "face with fewer than 3 vertices" ;
			}
			else {

				while ( tokens.size() < 4 )	tokens << tokens.last() ;

				QList<int> ind1 = parseFaceElement( tokens.at(1) ) ;

				int index = 3 ;
				while ( index < tokens.size() ) {
					QList<int> ind2 = parseFaceElement( tokens.at(index-1) ) ;
					QList<int> ind3 = parseFaceElement( tokens.at(index) ) ;

					if (( ind1.at(0) == ind2.at(0) )||( ind1.at(0) == ind3.at(0) )||( ind2.at(0) == ind3.at(0) )) {
						invalidFaces2++ ;
					//	qDebug() << "face with identical vertex indices" ;
					}
//					m_obj->trimesh(m_v.at( ind1.at(0) ), m_v.at( ind2.at(0) ), m_v.at( ind3.at(0) ) ) ;
					m_obj->triangle(m_v.at( ind1.at(0) ), m_v.at( ind2.at(0) ), m_v.at( ind3.at(0) ) ) ;

                    //
                    m_mesh.triangle(m_v.at( ind1.at(0) ), m_v.at( ind2.at(0) ), m_v.at( ind3.at(0) ) ) ;
                    //

                    if ( !m_t.isEmpty() )	m_obj->triTex( m_t.at( ind1.at(1) ), m_t.at( ind2.at(1) ), m_t.at( ind3.at(1) ) ) ;
					index++ ;
				}
			}
		}
		line = stream.readLine() ;
	}

	this->addObject( m_obj ) ;

	if ( invalidFaces1 )	warning(QString("%1 face(s) with fewer than 3 vertices, ignored").arg( invalidFaces1 ) ) ;
	if ( invalidFaces2 )	warning(QString("%1 face(s) with identical vertex indices, ignored").arg( invalidFaces2 ) ) ;

	file.close() ;
	return true ;
}

// [private] lecture d'un fichier MTL

bool GLamObjMtl::loadMtl(const QString& filename )
{
	QFile file( filename ) ;
	if ( !file.open(QIODevice::ReadOnly | QIODevice::Text) ) {
		error("load: MTL file not found" ) ;
		return false ;
	}

	info( QString("loading %1...").arg(filename) ) ;

	QFileInfo	fi( file ) ;
	QString path = fi.absolutePath() ;

	QTextStream stream( &file ) ;

	QString line = stream.readLine() ;

	while ( !line.isNull() ) {
//		QStringList tokens = line.split(' ', QString::SkipEmptyParts ) ;
        QStringList tokens = line.split(' ', Qt::SkipEmptyParts ) ;
        if ( line.isEmpty() || tokens.at(0).isEmpty() || tokens.at(0).startsWith('#') ) {
			line = stream.readLine() ;
			continue ;
		}

		if      ( tokens.at(0) == "newmtl" ) {
			QString newmtl = ( tokens.size() > 1 ? tokens.at(1) : "unknown" ) ;
			GLamMaterial mat ;
			mat.setSimpleColor( false ) ;
			mat.setName( newmtl ) ;
			m_matList << mat ;
		}
		else if ( tokens.at(0) == "Ns" ) {	// specular exponent (0..1000)
			if ( tokens.size() > 1 ) {
				float v =  tokens.at(1).toFloat() ;
				m_matList.last().setShininess( 128.0 * v / 1000.0 ) ;
			}
		}
		// note: les valeurs Ka, Kd et Ks sont écrétées à 1.0
		else if ( tokens.at(0) == "Ka" ) {	// ambient reflectivity (r,g,b = 0.0 .. 1.0)
			float r = ( tokens.size() > 1 ? tokens.at(1).toFloat() : 0 ) ;
			float g = ( tokens.size() > 2 ? tokens.at(2).toFloat() : r ) ;
			float b = ( tokens.size() > 3 ? tokens.at(3).toFloat() : r ) ;
			m_matList.last().setAmbient( QVector3D(r, g, b ) ) ;
		}
		else if ( tokens.at(0) == "Kd" ) {	// diffuse reflectivity (r,g,b = 0.0 .. 1.0)
			float r = ( tokens.size() > 1 ? tokens.at(1).toFloat() : 0 ) ;
			float g = ( tokens.size() > 2 ? tokens.at(2).toFloat() : r ) ;
			float b = ( tokens.size() > 3 ? tokens.at(3).toFloat() : r ) ;
			m_matList.last().setDiffuse( QVector3D(r, g, b ) ) ;
		}
		else if ( tokens.at(0) == "Ks" ) {	// specular reflectivity (r,g,b = 0.0 .. 1.0)
			float r = ( tokens.size() > 1 ? tokens.at(1).toFloat() : 0 ) ;
			float g = ( tokens.size() > 2 ? tokens.at(2).toFloat() : r ) ;
			float b = ( tokens.size() > 3 ? tokens.at(3).toFloat() : r ) ;
			m_matList.last().setSpecular( QVector3D(r, g, b ) ) ;
		}
		else if ( tokens.at(0) == "d" ) {	// dissolve factor (default 1.0 = fully opaque)
			if ( tokens.size() > 1 ) {
				float v =  tokens.at(1).toFloat() ;
				m_matList.last().setTransparency( v ) ;
			}
		}
		else if ( tokens.at(0) == "Tr" ) {	// transparency factor, Tr = 1 - d
			if ( tokens.size() > 1 ) {
				float v =  tokens.at(1).toFloat() ;
				m_matList.last().setTransparency( 1.0 - v ) ;
			}
		}

		else if ( tokens.at(0) == "map_Kd" ) {	// texture file name
			if ( tokens.size() > 1 ) {
				QString ftex( path ) ;
				ftex += '/' ;
				ftex += tokens.at(1) ;
				if ( QFile::exists( ftex ) ) {
					m_matList.last().setTexture( ftex ) ;
				}
				else	warning( QString("texture %1 not found!").arg(ftex) ) ;
			}
		}

		line = stream.readLine() ;
	}

	file.close() ;
	return true ;
}

// [private]
// in:  face element = "v", "v/vt", "v//vn" ou "v/vt/vn"
// out: indices corrigés dans m_v, m_t (ou nuls si absents)

QList<int> GLamObjMtl::parseFaceElement(QString faceElement )
{
	QList<int> indices ;
	QStringList	face = faceElement.split('/') ;

	int iv = face.at(0).toInt() ;
	int ivt = 0 ;
	int ivn = 0 ;
	if ( face.size() == 2 )
		ivt = face.at(1).toInt() ;
	if ( face.size() == 3 ) {
		ivt = face.at(1).toInt() ;	// can be 0...
		ivn = face.at(2).toInt() ;
	}

	// correction
	if ( iv < 0 )	iv = m_v.size() + iv ;
	else			iv -= 1 ;
	if ( ivt ) {
		if ( ivt < 0 )	ivt = m_t.size() + ivt ;
		else			ivt -= 1 ;
	}
//	if ( ivn ) {
//		if ( ivn < 0 )	ivn = m_n.size() + ivn ;
//		else			ivn -= 1 ;
//	}

	indices << iv << ivt << ivn ;
	return indices ;
}

// ---------------------------------------------------------------------------
// écriture de fichiers OBJ/MTL
// ---------------------------------------------------------------------------

/*! Fabrication des fichiers Wavefront OBJ/MTL à partir des éléments
 * courants du groupement (GLamObjMtl est une spécialisation de GLamGroup).
 * Le chemin @a filename spécifié est celui retenu pour le fichier OBJ ;
 * l'extension est automatiquement changée en .mtl pour le fichier associé.
 * @n Dans le cas d'objets texturés, charge à l'utilisateur de provisionner
 * les images utilisées pour le texturage ; seul leur nom et leur extension
 * sont spécifiés dans le fichier MTL, sans chemin.
 */

bool GLamObjMtl::save(const QString& filename )
{
	QFile file( filename ) ;
	if ( !file.open(QIODevice::WriteOnly | QIODevice::Text) ) {
		error("save: can't open OBJ file" ) ;
		return false ;
	}

	QFileInfo	fi( file ) ;
	QString mtlpath( fi.absolutePath() ) ;
	mtlpath += "/" ;
	mtlpath += fi.completeBaseName() ;
	mtlpath += ".mtl" ;
	saveMtl(mtlpath ) ;

	info( QString("saving %1...").arg(filename) ) ;

	QTextStream stream( &file ) ;

	stream << QString("# Qam3D/GLam v%1 OBJ file: %2\n" ).arg(QAM3D_VERSION).arg( GLamObject::name() ) ;
	fi.setFile( mtlpath ) ;
	stream << QString("\nmtllib %1\n").arg( fi.fileName() ) ;

	QList<QVector3D> vList ;
//	QList<QVector3D> tList ;
    QList<QVector2D> tList ;    // à vérifier...
    int itList = 0 ;
	QStringList		 fList ;

	for ( int i = 0 ; i < numObjects() ; ++i ) {
		QString oname = object(i)->name() ;
		oname = oname.simplified() ;
		oname.replace(" ", "_" ) ;
		stream << QString("\ng %1\n").arg( oname ) ;

		QMatrix4x4 md ;//= object(i)->defaultMatrix() * this->defaultMatrix() ;

		for ( int v = 2 ; v < object(i)->m_iVertex.count() ; v += 3 ) {

//			QVector3D ve1 = md * object(i)->m_vertices.at( object(i)->m_iVertex[v-2] ) ;	// OBJ = CW !
            QVector3D ve1 = md.map( object(i)->m_vertices.at( object(i)->m_iVertex[v-2] ) ) ;	// OBJ = CW !
            int iv1 = vList.lastIndexOf(ve1) ;
			if ( iv1 < 0 ) {
				vList << ve1 ;
				iv1 = vList.size() - 1 ;
				stream <<  QString("v %1\n").arg( v2s(ve1) ) ;
			}

//			QVector3D ve2 = md * object(i)->m_vertices.at( object(i)->m_iVertex[v-1] ) ;
            QVector3D ve2 = md.map( object(i)->m_vertices.at( object(i)->m_iVertex[v-1] ) ) ;
            int iv2 = vList.lastIndexOf(ve2) ;
			if ( iv2 < 0 ) {
				vList << ve2 ;
				iv2 = vList.size() - 1 ;
				stream <<  QString("v %1\n").arg( v2s(ve2) ) ;
			}

//			QVector3D ve3 = md * object(i)->m_vertices.at( object(i)->m_iVertex[v] ) ;
            QVector3D ve3 = md.map( object(i)->m_vertices.at( object(i)->m_iVertex[v] ) ) ;
            int iv3 = vList.lastIndexOf(ve3) ;
			if ( iv3 < 0 ) {
				vList << ve3 ;
				iv3 = vList.size() - 1 ;
				stream <<  QString("v %1\n").arg( v2s(ve3) ) ;
			}
//			QVector3D norm = QVector3D::normal(ve1, ve2, ve3 ) ;
//			int in = nList.lastIndexOf(norm) ;
//			if ( in < 0 ) {
//				nList << norm ;
//				in = nList.size() - 1 ;
//			}

			if ( object(i)->material().hasTexture() ) {

				QVector2D vt1 = object(i)->m_texels.at( object(i)->m_iTexel[v-2] ) ;
				int it1 = tList.lastIndexOf(vt1) ;
				if ( it1 < 0 ) {
					tList << vt1 ;
					it1 = tList.size() - 1 ;
				}
				QVector2D vt2 = object(i)->m_texels.at( object(i)->m_iTexel[v-1] ) ;
				int it2 = tList.lastIndexOf(vt2) ;
				if ( it2 < 0 ) {
					tList << vt2 ;
					it2 = tList.size() - 1 ;
				}

				QVector2D vt3 = object(i)->m_texels.at( object(i)->m_iTexel[v] ) ;
				int it3 = tList.lastIndexOf(vt3) ;
				if ( it3 < 0 ) {
					tList << vt3 ;
					it3 = tList.size() - 1 ;
				}
				fList << QString("f %1/%2 %3/%4 %5/%6\n")
						 .arg(iv1+1).arg(it1+1)
						 .arg(iv2+1).arg(it2+1)
						 .arg(iv3+1).arg(it3+1) ;
			}
			else {
				fList << QString("f %1 %2 %3\n")
						 .arg(iv1+1)
						 .arg(iv2+1)
						 .arg(iv3+1) ;
			}
		}
//		stream << QString("\n") ;
//		for ( int n = inList ; n < nList.size() ; ++n ) {
//			stream << QString("vn %1\n").arg( v2s( nList.at(n) ) ) ;
//		}
//		inList = nList.size() ;

		if ( object(i)->material().hasTexture() ) {
			stream << QString("\n") ;
			for ( int n = itList ; n < tList.size() ; ++n ) {
				stream << QString("vt %1 %2\n").arg( f2s( tList.at(n).x() ) )
											   .arg( f2s( tList.at(n).y() ) ) ;
			}
			itList = tList.size() ;
		}


		GLamMaterial mat = object(i)->material() ;
		QString mname = mat.name() ;
		mname = mname.simplified() ;
		mname.replace(" ", "_" ) ;
		stream << QString("\nusemtl " + mname + "\n") ;
		for ( int f = 0 ; f < fList.size() ; ++f ) {
			stream << fList.at(f) ;
		}
		fList.clear() ;
	}

	file.close() ;
	return true ;
}

// [private]

bool GLamObjMtl::saveMtl(const QString& filename )
{
	QFile file( filename ) ;
	if ( !file.open(QIODevice::WriteOnly | QIODevice::Text) ) {
		error("save: can't open MTL file" ) ;
		return false ;
	}

	info( QString("saving %1...").arg(filename) ) ;

	QTextStream stream( &file ) ;
	QStringList	matList ;

	stream << QString("# Qam3D/GLam v%1 MTL file: %2\n" ).arg(QAM3D_VERSION).arg( GLamObject::name() ) ;

	for ( int i = 0 ; i < numObjects() ; ++i ) {
		GLamMaterial mat = object(i)->material() ;

		QString mname = mat.name() ;
		mname = mname.simplified() ;
		mname.replace(" ", "_" ) ;

		if ( matList.contains(mname) )	continue ;

		matList << mname ;
		stream << QString("\nnewmtl " + mname + "\n") ;
		stream << QString("Ka %1\n").arg( v2s(mat.ambient() ) ) ;
		stream << QString("Kd %1\n").arg( v2s(mat.diffuse() ) ) ;
		stream << QString("Ks %1\n").arg( v2s(mat.specular() ) ) ;
		stream << QString("Ns %1\n").arg( f2s(mat.shininess() * 1000 / 128.0f ) ) ;
		stream << QString("d %1\n").arg(  f2s(mat.transparency() ) ) ;
		// FIXME
		stream << QString("illum 2\n") ;
		//
		if ( mat.hasTexture() ) {
			QFileInfo fi( mat.textureFilename() ) ;
			stream << QString("map_Kd %1\n").arg( fi.fileName() ) ;
		}
	}

	stream << QString("\n# Material count: %1\n").arg( matList.count() ) ;

	file.close() ;
	return true ;
}

// [private] float format = x.xxxxxx

QString GLamObjMtl::f2s(float v )
{
	return QString("%1").arg(v, 0, 'f', 6 ) ;
}

// [private] vector format = x.xxxxxx y.yyyyyy z.zzzzzz

QString GLamObjMtl::v2s(QVector3D v )
{
	return QString("%1 %2 %3").arg(v.x(), 0, 'f', 6 ).arg(v.y(), 0, 'f', 6 ).arg(v.z(), 0, 'f', 6 ) ;
}
