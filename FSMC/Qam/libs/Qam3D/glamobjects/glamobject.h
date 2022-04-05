/*  ---------------------------------------------------------------------------
 *  filename    :   glamobject.h
 *  description :   INTERFACE de la classe GLamObject
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

#ifndef GLAMOBJECT_H
#define GLAMOBJECT_H

#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>

#include "../glamtools/glam2d.h"
#include "../glamtools/glammaterial.h"

#include "../_VERSION"

class Qam3D {
  public:
    static QString version() ;
} ;

// GLamObject user properties
// ---------------------------------------------------------------------------
// numVertices		int				RO
// verticesCounter	quint32			RO [static]
// glamWidget		GLamWidget*		WO [static]
// id				int				RO
// className		QString			RO
// name				QString			R/W
// wireFrame		bool			R/W
// shadingEffect	ShadingEffect	R/W
// material			GLamMaterial	R/W
// defaultMatrix	QMatrix4x4		R/W
// transformMatrix	QMatrix4x4		R/W
// ---------------------------------------------------------------------------

class GLamWidget ;
class GLamObject ;
class GLamMesh ;

// algorithme de tesselation
// ---------------------------------------------------------------------------

#ifdef USE_GLULIBTESS

#include "../libtess/tess.h"			// interface avec 'libtess' (GLU like)

void glamTessNew(GLamObject* wrapper ) ;
void glamTessBeginContour() ;
void glamTessVertex(double pt[3] ) ;
void glamTessEndContour() ;
void glamTessDelete() ;

#else

#include "../libtess2/tesspolygon.h"	// interface avec 'libtess2'

#endif

// ---------------------------------------------------------------------------

// GLam abstract base class

class GLamObject : protected QOpenGLFunctions
{
  public:
	/*! Modèle d'illumination de l'objet. */
	enum  ShadingEffect{
		Flat,		/*!< modèle constant */
		Gouraud,	/*!< modèle de Gouraud, interpolation des intensités */
		Phong,		/*!< modèle de Phong, interpolation des normales */
		Toon		/*!< Cartoon-Looking Rendering */
	} ;

	GLamObject() ;
	virtual ~GLamObject() ;

	void operator =(const GLamObject& obj ) ;

	GLamMesh& toTrimesh() ;

	// scène 3D de projection
	static void setGLamWidget(const GLamWidget* glamWidget ) ;
	const GLamWidget* widget() const ;

	// statistiques globales
	static void resetVerticesCounter() ;
	static quint32 verticesCounter() ;
	static quint32 trianglesCounter() ;
	static QString drawnObjets() ;

	/*! Numéro d'identifiant attribué à l'objet. */
	inline int id() const { return m_id ; }

	/*! Sélecteur de la propriété @ref obj_name "name". */
	virtual QString name() const { return m_name ; }
	/*! Modificateur de la propriété @ref obj_name "name". */
	inline void setName(const QString& name ) { m_name = name ; }

	/*! Sélecteur de la propriété @ref obj_className "className". */
	inline QString className() const { return m_class ; }

	/*! Accesseur de la propriété @ref obj_defaultMatrix "defaultMatrix". */
	inline QMatrix4x4& defaultMatrix() { return m_defaultMatrix ; }
	/*! Sélecteur de la propriété @ref obj_defaultMatrix "defaultMatrix". */
	inline QMatrix4x4  defaultMatrix() const { return m_defaultMatrix ; }

	/*! Accesseur de la propriété @ref obj_transformMatrix "transformMatrix". */
	inline QMatrix4x4& transformMatrix() { return m_transformMatrix ; }
	/*! Sélecteur de la propriété @ref obj_transformMatrix "transformMatrix". */
	inline QMatrix4x4  transformMatrix() const { return m_transformMatrix ; }

	/*! Sélecteur de la propriété @ref obj_wireFrame "wireFrame". */
	inline bool isWireFrame() const { return m_isWireFrame ; }
	/*! Modificateur de la propriété @ref obj_wireFrame "wireFrame". */
	virtual void setWireFrame(bool wireFrame = true ) { m_isWireFrame = wireFrame ; updateNeeded() ; }

	/*! Sélecteur de la propriété @ref obj_material "material". */
	inline GLamMaterial  material() const { return m_material ; }
	/*! Accesseur de la propriété @ref obj_material "material". */
	virtual GLamMaterial& material() { return m_material ; }
	/*! Modificateur de la propriété @ref obj_material "material". */
	virtual void setMaterial(GLamMaterial material ) { m_material = material ; }

	/*! Sélecteur de la propriété @ref obj_shadingEffect "shadingEffect". */
	inline ShadingEffect shadingEffect() const { return m_shadingEffect ; }
	/*! Modificateur de la propriété @ref obj_shadingEffect "shadingEffect". */
	virtual void setShadingEffect(ShadingEffect effect ) { m_shadingEffect = effect ; }

	virtual void draw(GLamObject* parent = 0 ) ;

	virtual QVector3D min() ;
	virtual QVector3D max() ;
	virtual QVector3D dim() ;

  private:
	void calcDim() ;

  protected:
	/*! Méthode virtuelle pure. Les classes descendantes sont concrétisées par
		implémentation de cette méthode qui doit assurer la création des vertices
		et texels de l'objet.
		- ressources de création des vertices : triangle(), quad() xExtrude(), yExtrude(), zExtrude()
		- ressources de création des texels : triTex(), quadTex()
	*/
	virtual void create() = 0 ;

	virtual void update() ;

	/*! Méthode à invoquer pour signaler un changement de géométrie de l'objet
	 * nécessitant une mise à jour des listes de vertices et indices (QOpenGLBuffer)
	 * à destination des shaders.
	 */

	inline void updateNeeded() { m_updateNeeded = true ; }


  protected:
	/*! Remise à zéro des listes de mémorisation des vertices, normales et texels. */
//	inline void clearAllDatas() { clearVertices() ; clearNormals() ; clearTexels() ; }
	inline void eraseAllDatas() { clearVertices() ; clearNormals() ; clearTexels() ; }
  private:
	inline void clearVertices() { m_vertices.clear() ; m_iVertex.clear() ; m_iWire.clear() ; }
	inline void clearNormals() { m_flatNormals.clear() ; m_iFlatNormal.clear() ; m_smoothNormals.clear(); m_iSmoothNormal.clear() ; }
	inline void clearTexels() { m_texels.clear() ; m_iTexel.clear() ; }

  protected:
	/*! Modificateur de la propriété @ref className. */
	inline void setClassName(const QString& className ) { m_class = className ; }

	// Ressources liées aux vertices et normales
  public:
	/*! Retourne le nombre de triangles utilisés pour construire l'objet. */
	virtual int numTriangles() const { return m_iVertex.count() / 3 ; }
	/*! Retourne le nombre de vertices utilisés pour construire l'objet. */
	virtual int numVertices() const { return m_vertices.count() ; }

#ifndef USE_GLULIBTESS
  protected:
	void triangle(QVector3D v1, QVector3D v2, QVector3D v3 ) ;
#else
  public:
	void triangle(QVector3D v1, QVector3D v2, QVector3D v3 ) ;
#endif
  protected:
	void quad(QVector3D v1, QVector3D v2, QVector3D v3, QVector3D v4, qreal thickness = 0.0f ) ;
	void xExtrude(QVector3D v1, QVector3D v2 ) ;
	void yExtrude(QVector3D v1, QVector3D v2 ) ;
	void zExtrude(QVector3D v1, QVector3D v2 ) ;
	int numVertexIndices() const ;
	QVector3D vertex(int index ) const ;
  private:
	void quadToTriangles(QVector3D v1, QVector3D v2, QVector3D v3, QVector3D v4 ) ;
	int addVertex(QVector3D vertex ) ;
	int addNormal(QVector3D normal ) ;
	bool addLine(int iv1, int iv2 ) ;

	// Ressources liées aux texels
  protected:
	void triTex(QVector2D t1, QVector2D t2, QVector2D t3 ) ;
	void quadTex(QVector2D t1, QVector2D t2, QVector2D t3, QVector2D t4 ) ;
	int numTexelIndices() const ;
	QVector2D texel(int index ) const ;
  private:
	int addTexel(QVector2D texel ) ;

	// Ressources de remontée de messages vers GLamWidget
  protected:
	void info(const QString& mess ) ;
	void warning(const QString& mess ) ;
	void error(const QString& mess ) ;
	void fatal(const QString& mess ) ;

	// External vertices consumer/provider
	friend class GLamMesh ;
	friend class GLamGroup ;
	friend class GLamCSG ;
	friend class GLamStL ;
	friend class GLamObjMtl ;
	friend class GLamXplaneObj ;

  private:
	int						m_id ;
	const GLamWidget*		m_glamWidget ;
	QString					m_name ;			// propriétés
	QString					m_class ;
	bool					m_isWireFrame ;
	ShadingEffect			m_shadingEffect ;
	GLamMaterial			m_material ;
	QMatrix4x4				m_defaultMatrix ;
	QMatrix4x4				m_transformMatrix ;

	bool					m_updateNeeded ;
	bool					m_dimUpdateNeeded ;

	int						m_countDrawing ;
//protected:
	bool					m_hasExternalVerticesProvider ;	// update() don't need create()
	QVector<QVector3D>		m_vertices ;		// vertices
	QVector<int>			m_iVertex ;
	QVector<QVector3D>		m_flatNormals ;		// normals
	QVector<int>			m_iFlatNormal ;
//private:
	QVector<QVector3D>		m_smoothNormals ;
	QVector<int>			m_iSmoothNormal ;
	QVector<QVector2D>		m_texels ;			// texels
	QVector<int>			m_iTexel ;
	QVector<int>			m_iWire ;			// line indices

	QVector3D				m_min ;				// enveloppe 3D
	QVector3D				m_max ;

	QOpenGLBuffer			m_vbo ;				// vertices data buffer
	QOpenGLBuffer			m_ibo ;				// indices buffer

	QOpenGLShader*			m_vertexShader ;	// shaders
	QOpenGLShader*			m_fragmentShader ;
	QOpenGLShaderProgram*	m_shaderProgram ;
	bool					m_shaderLinked ;
	int						m_vertexAttr ;
	int						m_normalAttr ;
	int						m_texelAttr ;

#ifndef USE_GLULIBTESS
	// Tesselation : interface avec 'libtess2'
  protected:
	void glamTessPolygon(QList<QPointF> contour ) ;
	void glamTessHole(QList<QPointF> contour ) ;
	QList<QPointF> glamTessTriangulate() ;
  private:
	TessPolygon*			m_tessPolygon ;
#endif

} ;

#endif // GLAMOBJECT_H

/*!
  \class	GLamObject
  \brief	Classe de base abstraite

La classe GLamObject est une classe abstraite servant de modèle de développement
pour l'ensemble des classes d'objets 3D de la bibliothèque GLam.

@b Géométrie
@n La classe GLamObject fournit les ressources permettant de définir la 
géométrie d'un objet 3D sous forme d'une liste de triangles. Chaque triangle est 
construit par un triplet de vertices ; chaque vertex est défini par un triplet
de coordonnées x,y,z.
@n Une classe dérivée de GLamObject est concrétisée par l'implémentation de la 
méthode virtuelle pure create() ; les primitives de construction disponibles 
sont : triangle(), quad(), xExtrude(), yExtrude(), zExtrude().

@b Aspect
@n GLamObject permet la gestion de la couleur, de la texture et de la brillance 
de l'objet. Les propriétés associées sont : @b wireFrame, @b material et 
@b shadingEffect.
@n La texture de l'objet est maintenue par la propriété @b material, elle est 
plaquée sur la géométrie par une liste de texels associés aux vertices de 
construction. La texture est définie dans la concrétisation de create() au moyen
des primitives triTex() et quadTex().

@b Attitude @b spatiale
@n La position et l'orientation initiales dans l'espace d'un objet sont définies
par les valeurs des vertices de ses triangles ; cette attitude spatiale peut 
être corrigée par la propriété @b defaultMatrix.
@n La cinématique de l'objet (translations, rotations...) peut ensuite évoluer 
au cours du temps en agissant sur la propriété @b transformMatrix.

@b Projection @b sur @b la @b scène
@n La scène 3D est associée aux objets grâce à la méthode statique
setGLamWidget() ; chaque objet est ensuite projeté sur la scène, avec prise en
compte des matrices de transformation, au moyen de la méthode publique draw().

<h2 class="groupheader">Propriétés</h2>

<p>@anchor obj_className @b className : classe de l'objet,
par défaut "GLamObject". Cette propriété doit être renseignée
par les constructeurs des classes dérivées.
- virtual QString @b %className() const ;
- void @b %setClassName( const QString& @a className ) ;

<p>@anchor obj_name @b name : nom de l'objet, construit par défaut
à partir de son identifiant - voir méthode id() - sous la forme "NNNN".
- virtual QString @b %name() const ;
- void @b %setName( const QString& @a name ) ;

<p>@anchor obj_defaultMatrix @b defaultMatrix : matrice de transformation
spatiale pour spécifier la position et l'orientation par défaut de l'objet.
Cette matrice vaut par défaut la matrice identité.
@n Les valeurs des vertices de l'objet fournissent sa géométrie de construction
dans le repère 3D ; defaultMatrix agit sur ces valeurs, par exemple pour placer
l'objet au sein d'un groupe.

- QMatrix4x4& @b %defaultMatrix() ;
- QMatrix4x4 @b %defaultMatrix() const ;

<p>@anchor obj_transformMatrix @b transformMatrix : matrice de transformation
spatiale pour modifier les position et orientation courantes de l'objet.
Cette matrice vaut par défaut la matrice identité ; c'est elle qui doit
notamment être utilisée par animer l'objet au cours du temps. Cette matrice
est combinée avec @ref defaultMatrix pour devenir la matrice de modèle
(@e Model @e Matrix) qui permet de calculer l'attitude de l'objet sur la
scène 3D (@e world @e coordinates).
- QMatrix4x4& @b %transformMatrix() ;
- QMatrix4x4 @b %transformMatrix() const ;

<p>@anchor obj_wireFrame @b wireFrame : Mode de rendu "fil de fer" dans lequel les
polygones (triangle ou quad) sont montrés simplement par leurs lignes de
construction.
- bool @b %isWireFrame() const ;
- virtual void @b %setWireFrame( bool @a wireFrame = true )

<p>@anchor obj_material @b material : matériau de l'objet : couleur, réflexion ADS
(ambiante, diffuse et spéculaire) et transparence.
- GLamMaterial @b %material() const ;
- virtual GLamMaterial& @b %material() ;
- virtual void @b %setMaterial( GLamMaterial @a material ) ;

<p>@anchor obj_shadingEffect @b shadingEffect : modèle d'illumination de l'objet.
@n L'intensité de la lumière pour chaque sommet d'une face (triangle) est
déterminée par la propriété @ref material. L'intensité des autres points de
la face est déterminée par le modèle d'illumination (ou d'ombrage) utilisé.
@n Avec Qam3D, un seul @ref material est attribué à l'ensemble d'un objet.
@n Le modèle constant (GLamObject::Flat) est le plus simple et le plus rapide :
il attribue la même intensité à tous les points de la face. L'objet apparait donc facettisé.
@n Le modèle de Gouraud (GLamObject::Gouraud) a pour principe de calculer l'intensité de la lumière pour
les sommets du triangle et d'interpoler linéairement l'intensité aux sommets pour
déterminer celle des autres points de la face. L'interpolation linéaire se fait
sur le polygône projeté en 2D. Cette technique permet d'avoir un lissage qui
"gomme" les frontières entre les faces visibles avec un ombrage plat
@n L'ombrage de Phong (GLamObject::Phong) est assez similaire à Gouraud à la différence
près que ce n'est pas l'intensité lumineuse des sommets qui est interpolée
linéairement sur le triangle 3D mais les normales des sommets. Cette technique est plus
coûteuse que celle de Gouraud mais permet un meilleur rendu
(à noter que le modèle de Gouraud ne permet pas de voir un point lumineux qui serait au
centre d'une face car seule la lumière aux sommets est calculée ; alors que Phong le permet...).
- ShadingEffect @b %shadingEffect() const ;
- virtual void @b %setShadingEffect( ShadingEffect @a effect ) ;
*/
