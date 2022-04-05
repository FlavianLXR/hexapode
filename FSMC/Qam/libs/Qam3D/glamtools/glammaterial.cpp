/*  ---------------------------------------------------------------------------
 *  filename    :   glammaterial.cpp
 *  description :   IMPLEMENTATION de la classe GLamMaterial
 *
 *	project     :	Qam3D/GLam useful resources
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
 
#include "glammaterial.h"

/*! Constructeur par défaut.
 * Le matériau par défaut est de type couleur simple mate opaque grise.
 */

GLamMaterial::GLamMaterial()
	: m_name( "default" )
	, m_isSimpleColor( true )
	, m_isShiny( false )
	, m_color(0.7, 0.7, 0.7 )
	, m_ambient(0.05, 0.05, 0.0 )
	, m_diffuse(0.5, 0.5, 0.4 )
	, m_specular(0.7, 0.7, 0.04 )
	, m_shininess( 10.0f )
	, m_transparency( 1.0f )
	, m_texture( 0 )
{
}

/*! Constructeur mode couleur simple.
 * La couleur (opaque par défaut) est spécifiée par @a color et la brillance
 * éventuelle par @a shiny.
 */

GLamMaterial::GLamMaterial(const QColor& color, bool shiny )
	: m_name( "default" )
	, m_isSimpleColor( true )
	, m_isShiny( shiny )
	, m_shininess( 10.0f )
	, m_transparency( 1.0f )
	, m_texture( 0 )
{
	setColor( color ) ;
}

/*! Constructeur mode ADS avec un matériau prédéfini.
 */

GLamMaterial::GLamMaterial(Material material )
	: m_name( "default" )
	, m_isSimpleColor( false )
	, m_isShiny( false )
	, m_shininess( 10.0f )
	, m_transparency( 1.0f )
	, m_texture( 0 )
{
	set( material ) ;
}
/*
GLamMaterial::GLamMaterial(const GLamMaterial& material )
{
	*this = material ;
}
*/
/*! @internal. */

GLamMaterial::~GLamMaterial()
{
//	if ( m_texture )	delete m_texture ;
//	m_texture = 0 ;
}
/*
GLamMaterial& GLamMaterial::operator =(const GLamMaterial& material )
{
	m_name = material.m_name ;
	m_isSimpleColor = material.m_isSimpleColor ;
	m_isShiny = material.m_isShiny ;
	m_color = material.m_color ;
	m_ambient = material.m_ambient ;
	m_diffuse = material.m_diffuse ;
	m_specular = material.m_specular ;
	m_shininess = material.m_shininess ;
	m_transparency = material.m_transparency ;
	m_texFilename = material.m_texFilename ;
	if ( material.hasTexture() )	setTexture( material.m_texFilename ) ;
	return *this ;
}
*/
// material name
// ---------------------------------------------------------------------------

/*! Sélecteur de la propriété @ref mat_name "name". */
QString GLamMaterial::name() const { return m_name ; }
/*! Modificateur de la propriété @ref mat_name "name". */
void GLamMaterial::setName(const QString& name ) { m_name = name ; }

// simple color mode
// ---------------------------------------------------------------------------

/*! Sélecteur de la propriété @ref mat_shiny "shiny". */
bool GLamMaterial::isShiny() const { return m_isShiny ; }
/*! Modificateur de la propriété @ref mat_shiny "shiny". */
void GLamMaterial::setShiny(bool shiny ) { m_isShiny = shiny ; }

/*! Sélecteur de la propriété @ref mat_simpleColor "simpleColor". */
bool GLamMaterial::isSimpleColor() const { return m_isSimpleColor ; }
/*! Modificateur de la propriété @ref mat_simpleColor "simpleColor". */
void GLamMaterial::setSimpleColor(bool simpleColor ) { m_isSimpleColor = simpleColor ; }

/*! Sélecteur de la propriété @ref mat_color "color". */
QColor GLamMaterial::color() const
{
	QColor color ;
	color.setRgbF(m_color.x(), m_color.y(), m_color.z() ) ;
	return color ;
}

/*! Modificateur de la propriété @ref mat_color "color". */
void GLamMaterial::setColor(QColor color )
{
	m_color = QVector3D(color.redF(), color.greenF(), color.blueF() ) ;
}

/*! Modificateur de la propriété @ref mat_color "color". */
void GLamMaterial::setColor(QVector3D color )
{
	m_color = clamp( color ) ;
}

/*! Modificateur de la propriété @ref mat_color "color". */
void GLamMaterial::setColor(float r, float g, float b )
{
	m_color = QVector3D(clamp(r), clamp(g), clamp(b) ) ;
}

// ADS (Ambient Diffuse Specular) components
// ---------------------------------------------------------------------------

/*! Sélecteur de la propriété @ref mat_ambient "ambient". */
QVector3D GLamMaterial::ambient() const
{
	if ( m_isSimpleColor )	return QVector3D( 0.0f, 0.0f, 0.0f ) ;
	return m_ambient ;
}

/*! Modificateur de la propriété @ref mat_ambient "ambient". */
void GLamMaterial::setAmbient(QVector3D ka ) { m_ambient = clamp( ka ) ; }

/*! Modificateur de la propriété @ref mat_ambient "ambient". */
void GLamMaterial::setAmbient(float r, float g, float b )
{
	m_ambient = QVector3D(clamp(r), clamp(g), clamp(b) ) ;
}

/*! Sélecteur de la propriété @ref mat_diffuse "diffuse". */
QVector3D GLamMaterial::diffuse() const
{
	if ( m_isSimpleColor )	return m_color ;
	return m_diffuse ;
}

/*! Modificateur de la propriété @ref mat_diffuse "diffuse". */
void GLamMaterial::setDiffuse(QVector3D kd ) { m_diffuse = clamp( kd ) ; }

/*! Modificateur de la propriété @ref mat_diffuse "diffuse". */
void GLamMaterial::setDiffuse(float r, float g, float b )
{
	m_diffuse = QVector3D(clamp(r), clamp(g), clamp(b) ) ;
}

/*! Sélecteur de la propriété @ref mat_specular "specular". */
QVector3D GLamMaterial::specular() const
{
	if ( m_isSimpleColor ) {
		if ( m_isShiny )	return QVector3D( 1.0f, 1.0f, 1.0f ) ;
		else				return QVector3D( 0.0f, 0.0f, 0.0f ) ;
	}
	return m_specular ;
}

/*! Modificateur de la propriété @ref mat_specular "specular". */
void GLamMaterial::setSpecular(QVector3D ks ) { m_specular = clamp( ks ) ; }

/*! Modificateur de la propriété @ref mat_specular "specular". */
void GLamMaterial::setSpecular(float r, float g, float b )
{
	m_specular = QVector3D(clamp(r), clamp(g), clamp(b) ) ;
}

/*! Sélecteur de la propriété @ref mat_shininess "shininess". */
float GLamMaterial::shininess() const
{
	if ( m_isSimpleColor ) {
		if ( m_isShiny )	return 32.0f ;
		else				return  1.0f ;
	}
	return m_shininess ;
}

/*! Modificateur de la propriété @ref mat_shininess "shininess". */
void GLamMaterial::setShininess(float s ) { m_shininess = clamp(s, 128.0f ) ; }

// global transparency
// ---------------------------------------------------------------------------

/*! Sélecteur de la propriété @ref mat_transparency "transparency". */
float GLamMaterial::transparency() const { return m_transparency ; }
/*! Modificateur de la propriété @ref mat_transparency "transparency". */
void GLamMaterial::setTransparency(float t ) { m_transparency = clamp( t ) ; }

// texture file path
// ---------------------------------------------------------------------------

/*! Retourne @e true si un fichier image est associé au matériau. */
bool GLamMaterial::hasTexture() const { return m_texture != 0 ; }

/*! Fourni la texture au sens OpenGL. */
QOpenGLTexture* GLamMaterial::texture() const { return m_texture ; }

/*! Fixe le fichier image @a filename en tant que texture associée
 * au matériau.
 */
void GLamMaterial::setTexture(const QString& filename )
{
	if ( m_texture )	{ delete m_texture ; m_texture = 0 ; }

	m_texFilename = filename ;
	m_texture = new QOpenGLTexture( QImage(m_texFilename).mirrored() ) ;
	m_texture->setMinificationFilter( QOpenGLTexture::LinearMipMapLinear ) ;
	m_texture->setMagnificationFilter( QOpenGLTexture::Linear ) ;
}

/*! Retourne le chemin du fichier texture associé, ou une chaine
 * vide s'il n'est pas défini.
 */

QString GLamMaterial::textureFilename() const
{
	return ( hasTexture() ? m_texFilename : QString() ) ;
}

// predefined materials
// ---------------------------------------------------------------------------

/*! Attribution des caractéristiques de réflexion d'un matériau prédéfini.
 * Cette méthode révoque le mode couleur simple.
 */

void GLamMaterial::set(Material material )
{
	switch( material ) {
	case Emerald :
		setName("Emerald") ;
		setADS(0.0215, 0.1745, 0.0215, 0.07568, 0.61424, 0.07568, 0.633, 0.727811, 0.633, 76.8 ) ;
		break ;
	case Jade :
		setName("Jade") ;
		setADS(0.135, 0.2225, 0.1575, 0.54, 0.89, 0.63, 0.316228, 0.316228, 0.316228, 12.8 ) ;
		break ;
	case Obsidian :
		setName("Obsidian") ;
		setADS(0.05375, 0.05, 0.06625, 0.18275, 0.17, 0.22525, 0.332741, 0.328634, 0.346435, 38.4 ) ;
		break ;
	case Pearl :
		setName("Pearl") ;
		setADS(0.25, 0.20725, 0.20725, 1.0, 0.829, 0.829, 0.296648, 0.296648, 0.296648, 11.264 ) ;
		break ;
	case Ruby :
		setName("Ruby") ;
		setADS(0.1745, 0.01175, 0.01175, 0.61424, 0.04136, 0.04136, 0.727811, 0.626959, 0.626959, 76.8 ) ;
		break ;
	case Turquoise :
		setName("Turquoise") ;
		setADS(0.1, 0.18725, 0.1745, 0.396, 0.74151, 0.69102, 0.297254, 0.30829, 0.306678, 12.8 ) ;
		break ;
	case Brass :
		setName("Brass") ;
		setADS(0.329412, 0.223529, 0.027451, 0.780392, 0.568627, 0.113725, 0.992157, 0.941176, 0.807843, 27.8974 ) ;
		break ;
	case Bronze :
		setName("Bronze") ;
		setADS(0.2125, 0.1275, 0.054, 0.714, 0.4284, 0.18144, 0.393548, 0.271906, 0.166721, 25.6 ) ;
		break ;
	case Chrome :
		setName("Chrome") ;
		setADS(0.25, 0.25, 0.25, 0.4, 0.4, 0.4, 0.774597, 0.774597, 0.774597, 76.8 ) ;
		break ;
	case Copper :
		setName("Copper") ;
		setADS(0.19125, 0.0735, 0.0225, 0.7038, 0.27048, 0.0828, 0.256777, 0.137622, 0.086014, 12.8 ) ;
		break ;
	case Gold :
		setName("Gold") ;
		setADS(0.24725, 0.1995, 0.0745, 0.75164, 0.60648, 0.22648, 0.628281, 0.555802, 0.366065, 51.2 ) ;
		break ;
	case Silver :
		setName("Silver") ;
		setADS(0.19225, 0.19225, 0.19225, 0.50754, 0.50754, 0.50754, 0.508273, 0.508273, 0.508273, 51.2 ) ;
		break ;
	case Pewter :
		setName("Pewter") ;
		setADS(0.105882, 0.058824, 0.113725, 0.427451, 0.470588, 0.541176, 0.333333, 0.333333, 0.521569, 9.84615 ) ;
		break ;
	case BlackPlastic :
		setName("BlackPlastic") ;
		setADS(0.0, 0.0, 0.0, 0.01, 0.01, 0.01, 0.50, 0.50, 0.50, 32.0 ) ;
		break ;
	case CyanPlastic :
		setName("CyanPlastic") ;
		setADS(0.0, 0.1, 0.06, 0.0, 0.50980392, 0.50980392, 0.50196078, 0.50196078, 0.50196078, 32.0 ) ;
		break ;
	case GreenPlastic :
		setName("GreenPlastic") ;
		setADS(0.0, 0.0, 0.0, 0.1, 0.35, 0.1, 0.45, 0.55, 0.45, 32.0 ) ;
		break ;
	case RedPlastic :
		setName("RedPlastic") ;
		setADS(0.0, 0.0, 0.0, 0.5, 0.0, 0.0, 0.7, 0.6, 0.6, 32.0 ) ;
		break ;
	case WhitePlastic :
		setName("WhitePlastic") ;
		setADS(0.0, 0.0, 0.0, 0.55, 0.55, 0.55, 0.70, 0.70, 0.70, 32.0 ) ;
		break ;
	case YellowPlastic :
		setName("YellowPlastic") ;
		setADS(0.0, 0.0, 0.0, 0.5, 0.5, 0.0, 0.60, 0.60, 0.50, 32.0 ) ;
		break ;
	case BlackRubber :
		setName("BlackRubber") ;
		setADS(0.02, 0.02, 0.02, 0.01, 0.01, 0.01, 0.4, 0.4, 0.4, 10.0 ) ;
		break ;
	case CyanRubber :
		setName("CyanRubber") ;
		setADS(0.0, 0.05, 0.05, 0.4, 0.5, 0.5, 0.04, 0.7, 0.7, 10.0 ) ;
		break ;
	case GreenRubber :
		setName("GreenRubber") ;
		setADS(0.0, 0.05, 0.0, 0.4, 0.5, 0.4, 0.04, 0.7, 0.04, 10.0 ) ;
		break ;
	case RedRubber :
		setName("RedRubber") ;
		setADS(0.05, 0.0, 0.0, 0.5, 0.4, 0.4, 0.7, 0.04, 0.04, 10.0 ) ;
		break ;
	case WhiteRubber :
		setName("WhiteRubber") ;
		setADS(0.05, 0.05, 0.05, 0.5, 0.5, 0.5, 0.7, 0.7, 0.7, 10.0 ) ;
		break ;
	case YellowRubber :
		setName("YellowRubber") ;
		setADS(0.05, 0.05, 0.0, 0.5, 0.5, 0.4, 0.7, 0.7, 0.04, 10.0 ) ;
		break ;
	}
	setSimpleColor( false ) ;
}

/*! @internal. */

void GLamMaterial::set(const GLamMaterial& material )
{
	setName( material.name() ) ;
	setSimpleColor( material.isSimpleColor() ) ;
	setShiny( material.isShiny() ) ;
	setColor( material.color() ) ;
	setAmbient( material.ambient() ) ;
	setDiffuse( material.diffuse() ) ;
	setSpecular( material.specular() ) ;
	setShininess( material.shininess() ) ;
	setTransparency( material.transparency() ) ;
//
	if ( material.hasTexture() )	setTexture( material.m_texFilename ) ;
}

// private methods
// ---------------------------------------------------------------------------

void GLamMaterial::setADS(float Ra, float Ga, float Ba, float Rd, float Gd, float Bd, float Rs, float Gs, float Bs, float shininess )
{
	setAmbient(Ra, Ga, Ba ) ;
	setDiffuse(Rd, Gd, Bd ) ;
	setSpecular(Rs, Gs, Bs ) ;
	setShininess(shininess ) ;
}

QVector3D GLamMaterial::clamp(QVector3D value )
{
	return QVector3D( clamp(value.x()), clamp(value.y()), clamp(value.z()) ) ;
}

float GLamMaterial::clamp(float value, float vmax )
{
	return ( value < 0.0f ? 0.0f : ( value > vmax ? vmax : value ) ) ;
}

// related non-members
// ---------------------------------------------------------------------------

/*! @internal. */

bool operator ==(const GLamMaterial& m1, const GLamMaterial& m2 )
{
	if ( m1.name() != m2.name() )					return false ;
	if ( m1.isShiny() != m2.isShiny() )				return false ;
	if ( m1.isSimpleColor() != m2.isSimpleColor() )	return false ;
	if ( m1.color() != m2.color() )					return false ;
	if ( m1.ambient() != m2.ambient() )				return false ;
	if ( m1.diffuse() != m2.diffuse() )				return false ;
	if ( m1.specular() != m2.specular() )			return false ;
	if ( m1.shininess() != m2.shininess() )			return false ;
	if ( m1.transparency() != m2.transparency() )	return false ;

	if ( m1.texture() != m2.texture() )				return false ;

	return true ;
}

/*! @internal. */

bool operator !=(const GLamMaterial& m1, const GLamMaterial& m2 )
{
	return !( m1 == m2 ) ;
}
