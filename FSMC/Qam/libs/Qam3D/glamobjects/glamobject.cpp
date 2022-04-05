/*  ---------------------------------------------------------------------------
 *  filename    :   glamobject.cpp
 *  description :   IMPLEMENTATION de la classe GLamObject
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

//	codage des vertices
//	-------------------
//	QVector<QVector3D> m_vertices :	(x,y,z) = vertex
//	QVector<QVector3D> m_normals :  (x,y,z) = normale (xn,yn,zn)
//	QVector<QVector2D> m_texels :	(x,y) = coord. texture (s,t)
//
//	codage du VBO
//	-------------
//	8 x sizeof(float) par VertexData :	x, y, z, xn, yn, zn, s, t

#include "glamobject.h"
#include "../glamwidget/glamwidget.h"
#include "glammesh.h"

#ifdef QAMDEBUG
	#include <QDebug>
#endif

QString Qam3D::version() { return QString( QAM3D_VERSION ) ; } ;

// ---------------------------------------------------------------------------
// statistiques globales
// ---------------------------------------------------------------------------

static quint32 drawnVerticesCounter = 0 ;
static quint32 drawnTrianglesCounter = 0 ;
static int idProvider = 0 ;
QMap<int, GLamObject* >	objectsMap ;

/*! Initialise la collecte d'informations sur les objets projetés.
 * Cette méthode statique est invoquée avant la projection de la scène
 * par GLamWidget::paintGL().
 * @sa verticesCounter(), trianglesCounter(), drawnObjets()
 */

void GLamObject::resetVerticesCounter()
{
	drawnVerticesCounter = 0 ;
	drawnTrianglesCounter = 0 ;
	QMapIterator<int, GLamObject* > it( objectsMap ) ;
	while( it.hasNext() ) {
		it.next() ;
		it.value()->m_countDrawing = 0 ;
	}
}

/*! Retourne la valeur du compteur cumulatif de vertices
 * mis à jour par GLamObject::draw() au fil des demandes
 * de projection d'objets.
 * @sa resetVerticesCounter(), trianglesCounter(), drawnObjets()
 */

quint32 GLamObject::verticesCounter()
{
	return drawnVerticesCounter ;
}

/*! Retourne la valeur du compteur cumulatif de triangles
 * mis à jour par GLamObject::draw() au fil des demandes
 * de projection d'objets.
 * @sa resetVerticesCounter(), verticesCounter(), drawnObjets()
 */

quint32 GLamObject::trianglesCounter()
{
	return drawnTrianglesCounter ;
}

// liste des objets projetés : nom, classe, nombre de vertices
// m_countDrawing est mis à jour par GLamObject::draw()
/*! Retourne, sous forme @e human @e readable, la liste des objets
 * projetés sur la scène 3D. Chaque objet est présenté par son nom
 * (propriété @ref obj_name "name"), sa classe (propriété @ref obj_className "className"), son
 * nombre de vertices et son nombre d'occurrence (un même objet peut
 * être projeté plusieurs fois, avec des matrices de transformation
 * différentes).
 * @sa resetVerticesCounter(), verticesCounter(), trianglesCounter()
 */

QString GLamObject::drawnObjets()
{
	QString objList ;

	QMapIterator<int, GLamObject* > it( objectsMap ) ;
	while( it.hasNext() ) {
		it.next() ;
		if ( it.value()->m_countDrawing ) {
			objList += it.value()->name() + " " ;
			objList += it.value()->className() + " " ;
			objList += QString("%1/%2 (x%3)").arg( it.value()->numVertices() ).arg( it.value()->numTriangles() ).arg( it.value()->m_countDrawing ) + "\n" ;
		}
	}
	return objList ;
}

// ---------------------------------------------------------------------------
// scène 3D de projection maintenue par var. statique privée currentGLamWidget.
// GLamObject::draw() doit accéder à la scène courante pour configurer les shaders ;
// le membre privé m_glamWidget est renseigné par la liste d'initialisation
// du constructeur de GLamObject...

static const GLamWidget* currentGLamWidget = 0 ;

/*! Méthode normalement invoquée par le constructeur de GLamWidget
 * (ou de sa classe de spécialisation) pour s'identifier en tant
 * sur scène 3D de projection pour tous les objets dérivés de GLamObject
 * créés ultérieurement.
 * @sa widget()
 */

void GLamObject::setGLamWidget(const GLamWidget* glamWidget )
{
	currentGLamWidget = glamWidget ;
}

/*! Retourne la scène 3D courante utilisée par les objets dérivés
 * de GLamObject.
 * @sa setGLamWidget()
 */

const GLamWidget* GLamObject::widget() const
{
	return m_glamWidget ;
}

// ---------------------------------------------------------------------------
// Constructeur
// ---------------------------------------------------------------------------
/*! Constructeur.*/
GLamObject::GLamObject()
	: m_id( ++idProvider )
	, m_glamWidget( currentGLamWidget )
	, m_isWireFrame( false )
	, m_shadingEffect( Flat )
	, m_updateNeeded( true )
	, m_dimUpdateNeeded( true )
    , m_countDrawing( 0 )
    , m_hasExternalVerticesProvider( false )
	, m_ibo(QOpenGLBuffer::IndexBuffer)

#ifndef USE_GLULIBTESS
	, m_tessPolygon( NULL )	// interface avec 'libtess2'
#endif
{
	m_class = QString("GLamObject") ;
	m_defaultMatrix.setToIdentity() ;
	m_transformMatrix.setToIdentity() ;

	initializeOpenGLFunctions() ;

//	QString GLversion((const char*)glGetString(GL_VERSION) ) ;
//	QString GLSLversion((const char*)glGetString(GL_SHADING_LANGUAGE_VERSION) ) ;
//	qDebug() << "GLamObject::GLamObject():" << "GL" << GLversion << ", GLSL" << GLSLversion ;

	m_vertexShader = new QOpenGLShader( QOpenGLShader::Vertex ) ;
	m_fragmentShader = new QOpenGLShader( QOpenGLShader::Fragment ) ;

/*
// Q_OS_MACOS | Q_OS_LINUX | Q_OS_WIN
	QString vertexFile(":/shaders/vertex120.glsl") ;
	QString fragmentFile(":/shaders/fragment120.glsl") ;

#ifdef Q_OS_IOS
	vertexFile = QString(":/shaders/vertex300es.glsl") ;
	fragmentFile = QString(":/shaders/fragment300es.glsl") ;
#endif
#ifdef Q_OS_ANDROID
	vertexFile = QString(":/shaders/vertex300es.glsl") ;
	fragmentFile = QString(":/shaders/fragment300es.glsl") ;
#endif
*/

	// new v0.85
	QString vertexFile(":/shaders/vertex.glsl") ;
	QString fragmentFile(":/shaders/fragment.glsl") ;
	//

	m_vertexShader->compileSourceFile( vertexFile ) ;
	m_fragmentShader->compileSourceFile( fragmentFile ) ;

	m_shaderProgram = new QOpenGLShaderProgram ;
	m_shaderProgram->addShader( m_vertexShader ) ;
	m_shaderProgram->addShader( m_fragmentShader ) ;

	m_shaderLinked = m_shaderProgram->link() ;

	m_vertexAttr = m_shaderProgram->attributeLocation("VertexPosition" ) ;
	m_normalAttr = m_shaderProgram->attributeLocation("VertexNormal" ) ;
	m_texelAttr  = m_shaderProgram->attributeLocation("TexelCoord" ) ;

	objectsMap.insert(m_id, this ) ;

	m_name = QString("%1").arg(m_id, 4, 10, QChar('0') ) ;
}

/*! Destructeur.*/
GLamObject::~GLamObject()
{
#ifdef QAMDEBUG
	qDebug() << "GLamObject destructor for " << id() << className() << name() ;
#endif
	delete m_shaderProgram ;
	delete m_vertexShader ;
	delete m_fragmentShader ;
	m_vbo.destroy() ;
	m_ibo.destroy() ;

	QMap<int,GLamObject*>::iterator it = objectsMap.find( m_id ) ;
	if ( it != objectsMap.end() )	objectsMap.erase( it ) ;
}

// copie des attributs (e.g. après init. par le constructeur)
// la copie à son propre id, différent de celui de l'objet source
// et son propre nom par défaut construit à partir de l'id
/*! surcharge de l'opérateur d'affectation. */

void GLamObject::operator =(const GLamObject& obj )
{
	setWireFrame( obj.isWireFrame() ) ;
	setShadingEffect( obj.shadingEffect() ) ;
	setMaterial( obj.material() ) ;
	m_defaultMatrix = obj.defaultMatrix() ;
	m_transformMatrix = obj.transformMatrix() ;
	m_class = obj.m_class ;
//	m_name = obj.m_name ;
//	m_hasExternalVerticesProvider = obj.m_hasExternalVerticesProvider ;
	m_vertices = obj.m_vertices ;
	m_iVertex = obj.m_iVertex ;
	m_flatNormals = obj.m_flatNormals ;
	m_iFlatNormal = obj.m_iFlatNormal ;
	if ( isWireFrame() )	m_iWire = obj.m_iWire ;
	m_texels = obj.m_texels ;
	m_iTexel = obj.m_iTexel ;
}

// vue "maillage triangulaire"
/*!
 * \brief vue "maillage triangulaire"
 * \return maillage
 */
GLamMesh& GLamObject::toTrimesh()
{
	update() ;
	return *(GLamMesh*)this ;
}

// ---------------------------------------------------------------------------
// 'Vertex Data' --> shaders
// ---------------------------------------------------------------------------

// initialisation du VBO et de la géométrie

struct VertexData
{
	QVector3D p ;	// position
	QVector3D n ;	// normale
	QVector2D t ;	// texel
} ;

/*! Méthode invoquée automatiquement par draw() lorsque une mise à jour
 * des listes de données associées aux vertices et indices (QOpenGLBuffer)
 * à destination des shaders est requise.
 * Pour chaque vertex, les données enregistrées sont la position (vecteur 3D),
 * la normale (vecteur 3D) et les coordonnées de texture (point 2D) ; soit un
 * total de 8 @a float.
 * @n Cette méthode tient compte de l'état de la propriété @ref obj_wireFrame "wireFrame"
 * et recalcule les normales pour chaque vertex en fonction
 * du modèle d'illumination @ref obj_shadingEffect "shadingEffect" sélectionné (mode "flat" ou "smooth").
 * @sa updateNeeded()
 */

void GLamObject::update()
{
	if ( !m_hasExternalVerticesProvider ) {
		clearVertices() ;
		clearNormals() ;
//		clearTexels() ;
		create() ;
	}

	if ( !m_vbo.isCreated()	)	m_vbo.create() ;
	m_vbo.bind() ;
	if ( !m_ibo.isCreated()	)	m_ibo.create() ;
	m_ibo.bind() ;

	// mode 'smooth': calcul des normales moyennes pour chaque vertex

	if ( m_shadingEffect != Flat && !m_isWireFrame ) {
		m_iSmoothNormal = m_iFlatNormal ;

		for ( int i = 0 ; i < m_vertices.count() ; ++i ) {
			QVector<int> v ;
			v.clear() ;
			QVector3D	n ;
			for ( int j = 0 ; j < m_iVertex.count() ; ++j ) {
				if ( m_iVertex.at(j) == i )	{
					v << j ;
					n += m_flatNormals.at( m_iFlatNormal.at(j) ) ;
				}
			}
			n.normalize() ;		// n /= (float)(v.size()) ;
			m_smoothNormals << n ;
			for ( int j = 0 ; j < v.count() ; ++j ) {
				m_iSmoothNormal[v.at(j)] = m_smoothNormals.size() - 1 ;
			}
		}
	}

	// alimentation du VBO 0 : vertices data buffer
	// alimentation du VBO 1 : indices buffer

	QVector<VertexData>	  vbo0 ;
	QVector<unsigned int> vbo1 ;

	for ( int i = 0 ; i < m_iVertex.count() ; ++i ) {
		VertexData data ;
		data.p = m_vertices[ m_iVertex.at(i) ] ;
		if ( m_shadingEffect == Flat || m_isWireFrame )
				data.n = m_flatNormals[ m_iFlatNormal.at(i) ] ;
		else	data.n = m_smoothNormals[ m_iSmoothNormal.at(i) ] ;
		if ( i >= m_iTexel.count() )
				data.t = QVector2D() ;
		else	data.t = m_texels[ m_iTexel.at(i) ] ;

		vbo0 << data ;
		vbo1 << (unsigned int)( i ) ;
	}

	m_vbo.allocate(vbo0.constData(), vbo0.count() * sizeof(VertexData) ) ;
	m_vbo.release() ;

	// correction VBO1 si mode filaire : 1 triangle --> 3 lignes
	// ou m_iWire pré-rempli

	if ( m_isWireFrame ) {
		vbo1.clear() ;
		if ( m_iWire.isEmpty() ) {
			for ( int i = 0 ; i < m_iVertex.count() ; i += 3 ) {
				addLine(i,     i + 1 ) ;
				addLine(i + 1, i + 2 ) ;
				addLine(i + 2, i ) ;
			}
		}
		for ( int i = 0 ; i < m_iWire.count() ; ++i ) {
			vbo1 << (unsigned int)( m_iWire.at(i) ) ;
		}
	}

	m_ibo.allocate(vbo1.constData(), vbo1.count() * sizeof(unsigned int) ) ;
	m_ibo.release() ;
/*
	// mise à jour de la géométrie (enveloppe de l'objet)

	if ( m_vertices.isEmpty() )	{
		m_max = m_min = QVector3D() ;
	}
	else {
		m_max = m_min = m_vertices.at(0) ;
		for ( int i = 1 ; i < m_vertices.size() ; ++i ) {
			QVector3D v = m_vertices.at(i) ;
			if ( v.x() < m_min.x() )	m_min.setX( v.x() ) ;
			if ( v.y() < m_min.y() )	m_min.setY( v.y() ) ;
			if ( v.z() < m_min.z() )	m_min.setZ( v.z() ) ;
			if ( v.x() > m_max.x() )	m_max.setX( v.x() ) ;
			if ( v.y() > m_max.y() )	m_max.setY( v.y() ) ;
			if ( v.z() > m_max.z() )	m_max.setZ( v.z() ) ;
		}
	}
*/
	m_dimUpdateNeeded = true ;
	m_updateNeeded = false ;
}

/*! Méthode de projection de l'objet sur la scène 3D avec
 * prise en compte des matrices de transformation de la scène
 * et paramétrage des shaders.
 */

void GLamObject::draw(GLamObject* parent )
{
	Q_UNUSED( parent )

	if ( !m_shaderLinked )	return ;

	if ( !m_glamWidget ) return ;

	if ( m_updateNeeded )	update() ;

	// ModelView Matrix = View * Model
	QMatrix4x4 mv = m_glamWidget->viewMatrix() ;
	mv *= m_defaultMatrix ;
	mv *= m_transformMatrix ;

	// ModelViewProjection Matrix = Projection * View * Model
	QMatrix4x4 mvp = m_glamWidget->projectionMatrix() ;
//	mvp *= m_glamWidget->viewMatrix() ;
//	mvp *= m_defaultMatrix ;
//	mvp *= m_transformMatrix ;
	mvp *= mv ;

	m_shaderProgram->bind() ;

	m_shaderProgram->setUniformValue("hasTexture", m_material.hasTexture() ) ;
	m_shaderProgram->setUniformValue("shadingEffect", (int)m_shadingEffect ) ;
	m_shaderProgram->setUniformValue("ModelViewMatrix", mv ) ;
	m_shaderProgram->setUniformValue("NormalMatrix", mv.normalMatrix() ) ;
	m_shaderProgram->setUniformValue("MvpMatrix", mvp ) ;

	m_shaderProgram->setUniformValue("LightPosition", m_glamWidget->scene().lightPosition() ) ;

	m_shaderProgram->setUniformValue("Light.La", QVector3D( 1.0f, 1.0f, 1.0f ) ) ;
	m_shaderProgram->setUniformValue("Light.Ld", QVector3D( 1.0f, 1.0f, 1.0f ) ) ;
	m_shaderProgram->setUniformValue("Light.Ls", QVector3D( 1.0f, 1.0f, 1.0f ) ) ;

	m_shaderProgram->setUniformValue("Material.Ka", m_material.ambient() ) ;
	m_shaderProgram->setUniformValue("Material.Kd", m_material.diffuse() ) ;
	m_shaderProgram->setUniformValue("Material.Ks", m_material.specular() ) ;
	m_shaderProgram->setUniformValue("Material.Shininess", m_material.shininess() ) ;
	m_shaderProgram->setUniformValue("Material.Transparency", m_material.transparency() ) ;

	m_shaderProgram->enableAttributeArray(m_vertexAttr ) ;
	m_shaderProgram->enableAttributeArray(m_normalAttr ) ;
	m_shaderProgram->enableAttributeArray(m_texelAttr ) ;

	m_vbo.bind() ;
	m_ibo.bind() ;

	m_shaderProgram->setAttributeBuffer(m_vertexAttr, GL_FLOAT, 0, 3, sizeof(VertexData) ) ;
	m_shaderProgram->setAttributeBuffer(m_normalAttr, GL_FLOAT, sizeof(QVector3D), 3, sizeof(VertexData) ) ;
	m_shaderProgram->setAttributeBuffer(m_texelAttr,  GL_FLOAT, 2 * sizeof(QVector3D), 2, sizeof(VertexData) ) ;
	m_vbo.release() ;

	if ( m_material.hasTexture() )	m_material.texture()->bind() ;

	if ( m_isWireFrame )
			glDrawElements(GL_LINES, m_iWire.count(),  GL_UNSIGNED_INT, 0 ) ;
	else	glDrawElements(GL_TRIANGLES, m_iVertex.count(),  GL_UNSIGNED_INT, 0 ) ;

	if ( m_material.hasTexture() )	m_material.texture()->release() ;

	m_ibo.release() ;

	m_shaderProgram->disableAttributeArray(m_normalAttr ) ;
	m_shaderProgram->disableAttributeArray(m_vertexAttr ) ;
	m_shaderProgram->disableAttributeArray(m_texelAttr ) ;

	m_shaderProgram->release() ;

	drawnVerticesCounter += this->numVertices() ;
	drawnTrianglesCounter += this->numTriangles() ;
	m_countDrawing++ ;
}

// ---------------------------------------------------------------------------
// géométrie de l'enveloppe de l'objet 3D
// ---------------------------------------------------------------------------

/*! Retourne le vertex de l'objet dont les coordonnées signées x, y et z
 * sont les plus petites (coin inférieur arrière gauche du parallépipède
 * enveloppe) ; en tenant compte de la matrice de
 * transformation @ref obj_defaultMatrix "defaultMatrix".
 * @sa max(), dim()
 */

QVector3D GLamObject::min()
{
	if ( m_dimUpdateNeeded ) calcDim() ;
	return defaultMatrix().map( m_min ) ;
}

/*! Retourne le vertex de l'objet dont les coordonnées signées x, y et z
 * sont les plus grandes (coin supérieur avant droit du parallépipède
 * enveloppe) ; en tenant compte de la matrice de
 * transformation @ref obj_defaultMatrix "defaultMatrix".
 * @sa min(), dim()
 */

QVector3D GLamObject::max()
{
	if ( m_dimUpdateNeeded ) calcDim() ;
	return defaultMatrix().map( m_max ) ;
}

/*! Retourne les dimensions suivant OX,OY et OZ de l'objet ; en tenant
 * compte de la matrice de transformation @ref obj_defaultMatrix "defaultMatrix".
 * @sa min(), max()
 */

QVector3D GLamObject::dim()
{
	return max() - min() ;
}

// [private] mise à jour de la géométrie (enveloppe de l'objet)

void GLamObject::calcDim()
{
	if ( m_updateNeeded )	update() ;

	if ( m_vertices.isEmpty() )	{
		m_max = m_min = QVector3D() ;
	}
	else {
		m_max = m_min = m_vertices.at(0) ;
		for ( int i = 1 ; i < m_vertices.size() ; ++i ) {
			QVector3D v = m_vertices.at(i) ;
			if ( v.x() < m_min.x() )	m_min.setX( v.x() ) ;
			if ( v.y() < m_min.y() )	m_min.setY( v.y() ) ;
			if ( v.z() < m_min.z() )	m_min.setZ( v.z() ) ;
			if ( v.x() > m_max.x() )	m_max.setX( v.x() ) ;
			if ( v.y() > m_max.y() )	m_max.setY( v.y() ) ;
			if ( v.z() > m_max.z() )	m_max.setZ( v.z() ) ;
		}
	}
	m_dimUpdateNeeded = false ;
}

// ---------------------------------------------------------------------------
// Ressources liées aux vertices et normales
// ---------------------------------------------------------------------------

/*!
 * Méthode d'ajout du triangle défini par les 3 vertices
 * @a v1, @a v2, et @a v3 (ordre CCW).
 * @n Les vertices et leur normale sont mémorisés par des listes sans doublons
 * et accessibles en interne au moyen de listes d'indices.
 * La méthode protégée eraseAllDatas() permet d'effacer ces listes.
 * @n 3 vertices = 1 triangle | 3 lignes en mode @ref obj_wireFrame "wireFrame"
 * \sa quad(), numVertexIndices(), vertex()
 */
void GLamObject::triangle(QVector3D v1, QVector3D v2, QVector3D v3 )
{
	int in  = addNormal( QVector3D::normal(v1, v2, v3 ) ) ;
	int iv1 = addVertex( v1 ) ;
	int iv2 = addVertex( v2 ) ;
	int iv3 = addVertex( v3 ) ;

	int iVertex = m_iVertex.count() ;

	m_iVertex     << iv1 << iv2 << iv3 ;
	m_iFlatNormal << in  << in  << in ;

	if ( m_isWireFrame ) {
		addLine(iVertex,     iVertex + 1 ) ;	// ligne 1-2
		addLine(iVertex + 1, iVertex + 2 ) ;	// ligne 2-3
		addLine(iVertex + 2, iVertex ) ;		// ligne 3-1
	}
}

/*!
 * Méthode d'ajout de la surface "front face" définie par les 4 vertices
 * @a v1, @a v2, @a v3 et @a v4 (ordre CCW).
 * @n Le quad est converti en 2 triangles (v1,v2,v4),(v2,v3,v4)
 * ou en 4 lignes (v1,v2),(v2,v3),(v3,v4),(v4,v1) en mode @ref obj_wireFrame "wireFrame".
 * @n Si l'épaisseur @a thickness est non nulle, une deuxième surface "back face" est
 * ajoutée avec l'ordre inversé CW et un décalage de @a thickness suivant l'axe OZ.
 * @n Les vertices et leur normale sont mémorisés par des listes sans doublons
 * et accessibles en interne au moyen de listes d'indices.
 * La méthode protégée eraseAllDatas() permet d'effacer ces listes.
 * @n 4 vertices = 2 triangles | 4 lignes en mode @ref obj_wireFrame "wireFrame"
 * @n 4 vertices + épaisseur = 4 triangles | 8 lignes en mode @ref obj_wireFrame "wireFrame"
 * \sa triangle(), xExtrude(), yExtrude(), zExtrude()
 */

void GLamObject::quad(QVector3D v1, QVector3D v2, QVector3D v3, QVector3D v4, qreal thickness )
{
	// optimisation lorsque les vertices d'extrémité sont identiques
	// sauf en cas de plaquage de texture ou des facettes à 4 sommets
	// doivent être conservées...
#ifdef TEXTUREDQUADTO4TRIANGLES
	if ( material().hasTexture() ) {
		// note: GL_QUADS non dispo. sous OpenGLES
		// un 5e point central est ici ajouté
		// pour le texturage d'un quad afin de limiter les déformations
		// ( 1 quad --> 4 triangles )

		QVector3D v5 = ( v1 + v2 + v3 + v4 ) / 4 ;
		triangle(v1, v2, v5 ) ;
		triangle(v2, v3, v5 ) ;
		triangle(v3, v4, v5 ) ;
		triangle(v4, v1, v5 ) ;
		return ;
	}
#else
	if ( material().hasTexture() ) {
		if ( v1 == v2 )	v2 += QVector3D(0,0,0.000001 ) ;
		if ( v3 == v4 )	v4 += QVector3D(0,0,0.000001 ) ;
	}
#endif

	quadToTriangles(v1, v2, v3, v4 ) ;

	if ( !thickness )	return ;

	v1 += QVector3D(0, 0, thickness ) ;
	v2 += QVector3D(0, 0, thickness ) ;
	v3 += QVector3D(0, 0, thickness ) ;
	v4 += QVector3D(0, 0, thickness ) ;

	quadToTriangles(v4, v3, v2, v1 ) ;
}

/*!
 * Méthode d'ajout d'une face définie par
 * extrusion de 2 vertices @a v1 et @a v2 suivant l'axe OX
 * (les 2 vertices définissent la diagonale d'un rectangle
 * parallèle à l'axe OX).
 * @n 2 vertices = 2 triangles | 4 lignes en mode @ref obj_wireFrame "wireFrame"
 * \sa quad() avec @a thickness = 0
 */

void GLamObject::xExtrude(QVector3D v1, QVector3D v2 )
{
	QVector3D v3 = QVector3D(v2.x(), v1.y(), v1.z() ) ;
	QVector3D v4 = QVector3D(v1.x(), v2.y(), v2.z() ) ;
	quad(v1, v3, v2, v4 ) ;
}

/*!
 * Méthode d'ajout d'une face définie par
 * extrusion de 2 vertices @a v1 et @a v2 suivant l'axe OY
 * (les 2 vertices définissent la diagonale d'un rectangle
 * parallèle à l'axe OY).
 * @n 2 vertices = 2 triangles | 4 lignes en mode @ref obj_wireFrame "wireFrame"
 * \sa quad() avec @a thickness = 0
 */

void GLamObject::yExtrude(QVector3D v1, QVector3D v2 )
{
	QVector3D v3 = QVector3D(v1.x(), v2.y(), v1.z() ) ;
	QVector3D v4 = QVector3D(v2.x(), v1.y(), v2.z() ) ;
	quad(v1, v3, v2, v4 ) ;
}

/*!
 * Méthode d'ajout d'une face définie par
 * extrusion de 2 vertices @a v1 et @a v2 suivant l'axe OZ
 * (les 2 vertices définissent la diagonale d'un rectangle
 * parallèle à l'axe OZ).
 * @n 2 vertices = 2 triangles | 4 lignes en mode @ref obj_wireFrame "wireFrame"
 * \sa quad() avec @a thickness = 0
 */

void GLamObject::zExtrude(QVector3D v1, QVector3D v2 )
{
	QVector3D v3 = QVector3D(v1.x(), v1.y(), v2.z() ) ;
	QVector3D v4 = QVector3D(v2.x(), v2.y(), v1.z() ) ;
	quad(v1, v3, v2, v4 ) ;
}

// [private] 4 points --> 2 triangles | 4 lignes (avec gestion des v1==v2 et v3==v4)

void GLamObject::quadToTriangles(QVector3D v1, QVector3D v2, QVector3D v3, QVector3D v4 )
{
	// 4 sommets distincts : traitement particulier
	// pour ne pas montrer le quad sous forme de 2 triangles
	// en mode filaire...
	if (( v1 != v2 )&&( v3 != v4 )) {
		int in  = addNormal( QVector3D::normal(v1, v2, v4 ) ) ;
		int iv1 = addVertex( v1 ) ;
		int iv2 = addVertex( v2 ) ;
		int iv3 = addVertex( v3 ) ;
		int iv4 = addVertex( v4 ) ;

		int iVertex = m_iVertex.count() ;
		//               0      1      2      3      4      5
		m_iVertex     << iv1 << iv2 << iv4 << iv2 << iv3 << iv4 ;
		m_iFlatNormal << in  << in  << in  << in  << in  << in ;

		if ( m_isWireFrame ) {
			addLine(iVertex,     iVertex + 1 ) ;	// ligne 1-2
			addLine(iVertex + 1, iVertex + 4 ) ;	// ligne 2-3
			addLine(iVertex + 4, iVertex + 2 ) ;	// ligne 3-4
			addLine(iVertex + 2, iVertex ) ;		// ligne 4-1
		}
		return ;
	}

	if ( v1 != v2 )	triangle(v1, v2, v4 ) ;
	if ( v3 != v4 )	triangle(v2, v3, v4 ) ;
}

// [private] ajout du vertex spécifié en argument (sans doublons)

int GLamObject::addVertex(QVector3D vertex )
{
	int index = m_vertices.lastIndexOf( vertex ) ;
	if ( index < 0 ) {
		m_vertices << vertex ;
		index = m_vertices.size() - 1 ;
	}
	return index ;
}

// [private] ajout de la normale spécifiée en argument (sans doublons)

int GLamObject::addNormal(QVector3D normal )
{
	int index = m_flatNormals.lastIndexOf( normal ) ;
	if ( index < 0 ) {
		m_flatNormals << normal ;
		index = m_flatNormals.size() - 1 ;
	}
	return index ;
}

// [private] mode WireFrame: ajout d'une ligne dans m_iWire (sans doublons)
// m_iWire contient des indices dans m_iVertex
// ( accès au vertex : m_vertices[ m_iVertex[ m_iWire[i] ] ] )

bool GLamObject::addLine(int iv1, int iv2 )
{
	for ( int i = 1 ; i < m_iWire.count() ; i+= 2 ) {
		int i1 = m_iWire.at( i - 1 ) ;
		int i2 = m_iWire.at( i ) ;
		if (( i1 == iv1 )&&( i2 == iv2 ))	return false ;
		if (( i1 == iv2 )&&( i2 == iv1 ))	return false ;
	}
	m_iWire << iv1 << iv2 ;
	return true ;
}

/*! Retourne le nombre courant de vertices.
 * @sa triangle(), quad()
*/

int GLamObject::numVertexIndices() const
{
	return m_iVertex.count() ;
}

/*! Retourne le vertex d'indice @a index (si il existe)
 * ou un vecteur nul en cas d'échec.
 * @sa numVertexIndices()
 */

QVector3D GLamObject::vertex(int index ) const
{
//	if ( index < m_iVertex.count() )	return m_vertices[ index ] ;
	if ( index < m_iVertex.count() )	return m_vertices[ m_iVertex.at(index) ] ;
	return QVector3D() ;
}

// ---------------------------------------------------------------------------
// Ressources liées aux texels
// ---------------------------------------------------------------------------

/*!
 * Méthode d'ajout d'un triplet de texels (coordonnées 2D de texture).
 * @n Les texels sont mémorisés par une liste sans doublons
 * et accessibles en interne au moyen d'une liste d'indices.
 * La méthode protégée eraseAllDatas() permet d'effacer ces listes.
 * @sa quadTex(), numTexelIndices(), texel()
 */

void GLamObject::triTex(QVector2D t1, QVector2D t2, QVector2D t3 )
{
	int it1 = addTexel( t1 ) ;
	int it2 = addTexel( t2 ) ;
	int it3 = addTexel( t3 ) ;
	m_iTexel << it1 << it2 << it3 ;
}

/*!
 * Méthode d'ajout d'un quadruplet de texels (coordonnées 2D de texture).
 * @n Les texels sont mémorisés par une liste sans doublons
 * et accessibles en interne au moyen d'une liste d'indices.
 * La méthode protégée eraseAllDatas() permet d'effacer ces listes.
 * @sa triTex(), numTexelIndices(), texel()
 */

void GLamObject::quadTex(QVector2D t1, QVector2D t2, QVector2D t3, QVector2D t4 )
{
	int it1 = addTexel( t1 ) ;
	int it2 = addTexel( t2 ) ;
	int it3 = addTexel( t3 ) ;
	int it4 = addTexel( t4 ) ;

#ifndef TEXTUREDQUADTO4TRIANGLES
	m_iTexel << it1 << it2 << it4 << it2 << it3 << it4 ;
#else
	// note: GL_QUADS non dispo. sous OpenGLES
	// un 5e point central est ici ajouté
	// pour le texturage d'un quad afin de limiter les déformations
	// ( 1 quad --> 4 triangles )

	QVector2D t5 = ( t1 + t2 + t3 + t4 ) / 4 ;
	int it5 = addTexel( t5 ) ;
	m_iTexel << it1 << it2 << it5 ;
	m_iTexel << it2 << it3 << it5 ;
	m_iTexel << it3 << it4 << it5 ;
	m_iTexel << it4 << it1 << it5 ;
#endif
}

// [private] ajout du texel spécifié en argument (sans doublons)

int GLamObject::addTexel(QVector2D texel )
{
	int index = m_texels.lastIndexOf( texel ) ;
	if ( index < 0 ) {
		m_texels << texel ;
		index = m_texels.size() - 1 ;
	}
	return index ;
}

/*! Retourne le nombre courant de texels.
 * @sa triTex(), quadTex()
*/

int GLamObject::numTexelIndices() const
{
	return m_iTexel.count() ;
}

/*! Retourne le texel d'indice @a index (si il existe)
 * ou un vecteur nul en cas d'échec.
 * @sa numTexelIndices()
 */

QVector2D GLamObject::texel(int index ) const
{
//	if ( index < m_iTexel.count() )	return m_texels[ index ] ;
	if ( index < m_iTexel.count() )	return m_texels[ m_iTexel.at(index) ] ;
	return QVector2D() ;
}

// ---------------------------------------------------------------------------
// remontée des messages vers GLamWidget associé
// ---------------------------------------------------------------------------

/*! Remontée de messages vers la scène 3D par appel de la méthode
 * GLamWidget::messageFromObjects() avec un niveau 0.
 * @sa warning(), error(), fatal()
 */

void GLamObject::info(const QString& mess )
{
	if ( !m_glamWidget )	return ;
	m_glamWidget->messageFromObjects( mess ) ;
}

/*! Remontée de messages vers la scène 3D par appel de la méthode
 * GLamWidget::messageFromObjects() avec un niveau 1.
 * @sa info(), error(), fatal()
 */

void GLamObject::warning(const QString& mess )
{
	if ( !m_glamWidget )	return ;
	m_glamWidget->messageFromObjects( mess, 1 ) ;
}

/*! Remontée de messages vers la scène 3D par appel de la méthode
 * GLamWidget::messageFromObjects() avec un niveau 2.
 * @sa info(), warning(), fatal()
 */

void GLamObject::error(const QString& mess )
{
	if ( !m_glamWidget )	return ;
	m_glamWidget->messageFromObjects( mess, 2 ) ;
}

/*! Remontée de messages vers la scène 3D par appel de la méthode
 * GLamWidget::messageFromObjects() avec un niveau 3.
 * @sa info(), warning(), error()
 */

void GLamObject::fatal(const QString& mess )
{
	if ( !m_glamWidget )	return ;
	m_glamWidget->messageFromObjects( mess, 3 ) ;
}

#ifndef USE_GLULIBTESS

// ---------------------------------------------------------------------------
// interface avec 'libtess2'
// ---------------------------------------------------------------------------

/*! Facettisation (tesselation) d'une enveloppe extérieure CCW définie par la
 * liste de coordonnées 2D @a contour.
 * @n La liste résultante des sommets 2D est obtenue ensuite par glamTessTriangulate().
 */

void GLamObject::glamTessPolygon(QList<QPointF> contour )
{
	contour = GLam2D::noDuplicate( contour ) ;
	if ( GLam2D::isClockWise(contour) )	contour = GLam2D::invert(contour) ;

	if ( m_tessPolygon != NULL ) {
		delete m_tessPolygon ;
		m_tessPolygon = NULL ;
	}

	vector<double>	vd ;
	for ( int i = 0 ; i < contour.size() ; ++i ) {
		vd.push_back( contour[i].x() ) ;
		vd.push_back( contour[i].y() ) ;
	}

	m_tessPolygon = new TessPolygon( vd ) ;
}

/*! Facettisation (tesselation) d'une enveloppe intérieure CW définie par la
 * liste de coordonnées 2D @a contour. Cette méthode permet d'ajouter un "trou"
 * sur une surface définie au préalable par glamTessPolygon().
 * @n La liste résultante des sommets 2D est obtenue ensuite par glamTessTriangulate().
 */

void GLamObject::glamTessHole(QList<QPointF> contour )
{
	if ( !m_tessPolygon )	return ;

	contour = GLam2D::noDuplicate( contour ) ;
	if ( !GLam2D::isClockWise(contour) )	contour = GLam2D::invert(contour) ;

	vector<double>	vd ;
	for ( int i = 0 ; i < contour.size() ; ++i ) {
		vd.push_back( contour[i].x() ) ;
		vd.push_back( contour[i].y() ) ;
	}
	m_tessPolygon->addHole( vd ) ;
}

/*! Retourne la liste des sommets 2D des triangles CCW résultant d'une
 * facettisation (tesselation) initiée par un appel à glamTessPolygon() et
 * d'éventuels appels à glamTessHole().
 */

QList<QPointF> GLamObject::glamTessTriangulate()
{
	QList<QPointF> 	res ;

	if ( !m_tessPolygon )	return res ;

	m_tessPolygon->triangulation() ;

	Triangles	idTri = m_tessPolygon->triangles() ;
	PointMap	pts = m_tessPolygon->points() ;

	Triangles::iterator itt = idTri.begin() ;
	for ( ; itt != idTri.end() ; itt++ ) {
		TessPoint p1 = *pts[ (*itt)[0] ] ;
		TessPoint p2 = *pts[ (*itt)[1] ] ;
		TessPoint p3 = *pts[ (*itt)[2] ] ;
		QList<QPointF> lp ;
		lp.clear() ;
		lp << QPointF(p1.m_x, p1.m_y ) ;
		lp << QPointF(p2.m_x, p2.m_y ) ;
		lp << QPointF(p3.m_x, p3.m_y ) ;
		if ( GLam2D::isClockWise(lp) )	lp = GLam2D::invert(lp) ;
		res << lp ;
	}
	return res ;
}

#else	// #ifndef USE_GLULIBTESS

// ---------------------------------------------------------------------------
// interface avec 'libtess' (GLU)
// ---------------------------------------------------------------------------

#ifndef CALLBACK
#define CALLBACK
#endif

void CALLBACK _vertexCallback(void* vertex ) ;
void CALLBACK _beginCallback(GLenum prim ) ;
void CALLBACK _endCallback() ;
void CALLBACK _combineCallback( GLdouble coords[3], void* d[4], GLfloat w[4], void** dataOut ) ;
void CALLBACK _errorCallback(GLenum errorCode ) ;

GLamObject*			_wrapper ;
GLUtesselator*		_tess ;
GLenum				_glprim ;
QVector<QVector3D>	_vertices_triangles ;
QVector<QVector3D>	_vertices_triangle_fan ;
QVector<QVector3D>	_vertices_triangle_strip ;

void glamTessNew(GLamObject* wrapper )
{
	_wrapper = wrapper ;
	_tess = gluNewTess() ;
	gluTessCallback(_tess, GLU_TESS_VERTEX, (void(*)()) &_vertexCallback ) ;
	gluTessCallback(_tess, GLU_TESS_BEGIN,  (void(*)()) &_beginCallback ) ;
	gluTessCallback(_tess, GLU_TESS_END, (void(*)()) &_endCallback ) ;
	gluTessCallback(_tess, GLU_TESS_COMBINE, (void(*)()) &_combineCallback ) ;
	gluTessCallback(_tess, GLU_TESS_ERROR, (void(*)()) &_errorCallback ) ;

	gluTessProperty(_tess, GLU_TESS_WINDING_RULE, GLU_TESS_WINDING_ODD ) ;
	gluTessBeginPolygon(_tess, NULL ) ;
}

void glamTessBeginContour()
{
	gluTessBeginContour( _tess ) ;
}

void glamTessVertex(double pt[3] )
{
	//
//	double epsilon = 0.000001 ;
//	if ( fabs(pt[0]) < epsilon ) pt[0] = epsilon ;
//	if ( fabs(pt[1]) < epsilon ) pt[1] = epsilon ;
//	if ( fabs(pt[2]) < epsilon ) pt[2] = epsilon ;
	//
	gluTessVertex(_tess, pt, pt ) ;
}

void glamTessEndContour()
{
	gluTessEndContour( _tess ) ;
}

void glamTessDelete()
{
	gluTessEndPolygon( _tess ) ;
	gluDeleteTess( _tess ) ;
	_wrapper = 0 ;
}

// fonction déclarée amie de la classe GLamObject

void glamTessTriangle(QVector3D v1, QVector3D v2, QVector3D v3 )
{
	if ( _wrapper )	_wrapper->triangle(v1, v2, v3 ) ;
}

// new vertice provided by tesselator

void CALLBACK _vertexCallback(void* vertex )
{
	const GLdouble*	ptr = (const GLdouble*)vertex ;
	QVector3D v ;
	v.setX( *( ptr ) ) ;
	v.setY( *( ptr + 1 ) ) ;
	v.setZ( *( ptr + 2 ) ) ;

	if ( _glprim == GL_TRIANGLES )		_vertices_triangles << v ;
	if ( _glprim == GL_TRIANGLE_FAN )	_vertices_triangle_fan << v ;
	if ( _glprim == GL_TRIANGLE_STRIP )	_vertices_triangle_strip << v ;
}

// glBegin : prim = 4 | 5 | 6 (GL_TRIANGLE..)

void CALLBACK _beginCallback(GLenum prim )
{
	_glprim = prim ;
	_vertices_triangles.clear() ;
	_vertices_triangle_fan.clear() ;
	_vertices_triangle_strip.clear() ;
}

// glEnd

void CALLBACK _endCallback()
{
	// TRIANGLES
	for ( int i = 0 ; i < _vertices_triangles.size() - 2 ; i++ ) {
		glamTessTriangle(
			_vertices_triangles.at( i ),
			_vertices_triangles.at( i + 1 ),
			_vertices_triangles.at( i + 2 )
		) ;
	}

	// TRIANGLE_FAN -> TRIANGLES
	for ( int i = 1 ; i < _vertices_triangle_fan.size() - 1 ; i++ ) {
		glamTessTriangle(
			_vertices_triangle_fan.at( 0 ),
			_vertices_triangle_fan.at( i ),
			_vertices_triangle_fan.at( i + 1 )
		) ;
	}

	// TRIANGLE_STRIP -> TRIANGLES
	for ( int i = 0 ; i < _vertices_triangle_strip.size() - 2 ; i++ ) {
		glamTessTriangle(
			_vertices_triangle_strip.at( i ),
			_vertices_triangle_strip.at( i & 1 ? i + 2 : i + 1 ),
			_vertices_triangle_strip.at( i & 1 ? i + 1 : i + 2 )
		) ;
	}
}

// combine callback

void CALLBACK _combineCallback( GLdouble coords[3], void* d[4], GLfloat w[4], void** dataOut )
{
	Q_UNUSED( d ) ;
	Q_UNUSED( w ) ;

	GLdouble* vertex = new GLdouble[3] ;
	vertex[0] = coords[0] ;
	vertex[1] = coords[1] ;
	vertex[2] = coords[2] ;
	*dataOut = vertex ;
}

// error callback

void CALLBACK _errorCallback(GLenum errorCode )
{
	qDebug() << "[ERROR] libtess: " << errorCode ;
}

#endif	// #ifdef USE_LIBTESS
