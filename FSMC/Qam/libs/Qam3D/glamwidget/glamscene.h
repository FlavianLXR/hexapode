/*  ---------------------------------------------------------------------------
 *  filename    :   glamscene.h
 *  description :   classe GLamScene (toutes méthodes inline)
 *
 *	project     :	Qam3D/GLam: widget "3D scene"
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

/*
	GLamScene: Encapsulation des paramètres courants d'une scène 3D :
	- taille
	- couleur de fond
	- facteur d'échelle
	- angles de rotation
	- translations
	- position de l'éclairage
*/

#ifndef GLAMSCENE_H
#define GLAMSCENE_H

#include <QVector3D>

class GLamScene
{
  public:
	GLamScene()
		: m_size( 2 )
		, m_background(1.0f, 1.0f, 1.0f )
		, m_scale( 1.0f, 0.1f, 10.0f )
		, m_rotate(0.0f, 0.0f, 0.0f )
		, m_translate(0.0f, 0.0f, 0.0f )
		, m_light(0.0f, 0.0f, 0.0f )
		, m_positional( true )
	{
	}

	// taille de la scène

	/*! Sélecteur de la propriété @ref scn_size "size". */
	int size() const { return m_size ; }
	/*! Modificateur de la propriété @ref scn_size "size". */
	void setSize(int size ) { m_size = size ; }

	// couleur de fond

	/*! Sélecteur de la propriété @ref scn_background "background". */
	QVector3D backgroundColor() const { return m_background ; }

	/*! Modificateur de la propriété @ref scn_background "background". 
	 * Les proportions rgb peuvent être spécifiées dans l'intervalle 0..1
	 * ou sous forme de valeurs entières 0, 2..255.
	 */
	void setBackgroundColor(float r, float g, float b )
	{
		if ( r <= 0 )	r = 0 ;
		if ( g <= 0 )	g = 0 ;
		if ( b <= 0 )	b = 0 ;
		if ( r > 1.0 )	r = ( r > 255 ? 1.0 : r / 255 ) ;
		if ( g > 1.0 )	g = ( g > 255 ? 1.0 : g / 255 ) ;
		if ( b > 1.0 )	b = ( b > 255 ? 1.0 : b / 255 ) ;
		m_background = QVector3D(r, g, b ) ;
	}

	// facteur d'échelle

	/*! Sélecteur de la propriété @ref scn_scale "scale". */
	float scale() const { return m_scale.x() ; }
	/*! Sélecteur de la propriété @ref scn_scale "scale". */
	float minScale() const { return m_scale.y() ; }
	/*! Sélecteur de la propriété @ref scn_scale "scale". */
	float maxScale() const { return m_scale.z() ; }

	/*! Modificateur de la propriété @ref scn_scale "scale". */
	void setScale(float scale )
	{
		if ( scale < minScale() )	scale = minScale() ;
		if ( scale > maxScale() )	scale = maxScale() ;
		m_scale.setX( scale ) ;
	}
	/*! Modificateur de la propriété @ref scn_scale "scale". */
	void setScaleRange(float min, float max )
	{
		if ( min > max )	return ;
		m_scale.setY( min ) ;
		m_scale.setZ( max ) ;
	}
	/*! Fixe les valeurs par défaut pour le zoom et sa plage de validité. */
	void resetScale()
	{
		m_scale = QVector3D( 1.0, 0.1, 10.0 ) ;
	}

	// rotations

	/*! Sélecteur de la propriété @ref scn_rotation "rotation". */
	QVector3D rotation() const { return m_rotate ; }
	/*! Sélecteur de la propriété @ref scn_rotation "rotation". */
	float rotationX() const { return m_rotate.x() ; }
	/*! Sélecteur de la propriété @ref scn_rotation "rotation". */
	float rotationY() const { return m_rotate.y() ; }
	/*! Sélecteur de la propriété @ref scn_rotation "rotation". */
	float rotationZ() const { return m_rotate.z() ; }

	/*! Modificateur de la propriété @ref scn_rotation "rotation". */
	void setRotation(const QVector3D& pov ) { m_rotate = pov ; }
	/*! Modificateur de la propriété @ref scn_rotation "rotation". */
	void setRotation(float xAngle, float yAngle, float zAngle ) { m_rotate = QVector3D(xAngle, yAngle, zAngle ) ; }
	/*! Modificateur de la propriété @ref scn_rotation "rotation". */
	void setRotationX(float angle ) { m_rotate.setX(angle ) ; }
	/*! Modificateur de la propriété @ref scn_rotation "rotation". */
	void setRotationY(float angle ) { m_rotate.setY(angle ) ; }
	/*! Modificateur de la propriété @ref scn_rotation "rotation". */
	void setRotationZ(float angle ) { m_rotate.setZ(angle ) ; }

	// translations

	/*! Sélecteur de la propriété @ref scn_translation "translation". */
	QVector3D translation() const { return m_translate ; }
	/*! Sélecteur de la propriété @ref scn_translation "translation". */
	float translationX() const { return m_translate.x() ; }
	/*! Sélecteur de la propriété @ref scn_translation "translation". */
	float translationY() const { return m_translate.y() ; }
	/*! Sélecteur de la propriété @ref scn_translation "translation". */
	float translationZ() const { return m_translate.z() ; }

	/*! Modificateur de la propriété @ref scn_translation "translation". */
	void setTranslation(const QVector3D& pos ) { m_translate = pos ; }
	/*! Modificateur de la propriété @ref scn_translation "translation". */
	void setTranslation(float x, float y, float z ) { m_translate = QVector3D(x, y, z ) ; }
	/*! Modificateur de la propriété @ref scn_translation "translation". */
	void setTranslationX(float x ) { m_translate.setX( x ) ; }
	/*! Modificateur de la propriété @ref scn_translation "translation". */
	void setTranslationY(float y ) { m_translate.setY( y ) ; }
	/*! Modificateur de la propriété @ref scn_translation "translation". */
	void setTranslationZ(float z ) { m_translate.setZ( z ) ; }

	// position de l'éclairage

	/*! Sélecteur de la propriété @ref scn_lightPosition "lightPosition". */
	QVector3D lightPosition() const { return m_light ; }
	/*! Sélecteur de la propriété @ref scn_lightPosition "lightPosition". */
	bool isLightPositional() const { return m_positional ; }

	/*! Modificateur de la propriété @ref scn_lightPosition "lightPosition". */
	void setLightPosition(const QVector3D& pos, bool positional = true )
	{
		m_light = pos ;
		m_positional = positional ;
	}
	/*! Modificateur de la propriété @ref scn_lightPosition "lightPosition". */
	void setLightPosition(float x, float y, float z, bool positional = true )
	{
		setLightPosition( QVector3D(x, y, z ), positional ) ;
	}
	/*! Modificateur de la propriété @ref scn_lightPosition "lightPosition". */
	void addLightPositionX(float val ) { m_light.setX(m_light.x() + val ) ; }
	/*! Modificateur de la propriété @ref scn_lightPosition "lightPosition". */
	void addLightPositionY(float val ) { m_light.setY(m_light.y() + val ) ; }
	/*! Modificateur de la propriété @ref scn_lightPosition "lightPosition". */
	void addLightPositionZ(float val ) { m_light.setZ(m_light.z() + val ) ; }

  private:
	int			m_size ;		// taille de la scène
	QVector3D	m_background ;	// proportions RGB couleur de fond
	QVector3D	m_scale ;		// zoom, zoom min, zoom max
	QVector3D	m_rotate ;		// rotations
	QVector3D	m_translate ;	// translations
	QVector3D	m_light ;		// position de l'éclairage
	bool		m_positional ;	// éclairage ponctuel / directionnel
} ;

#endif // GLAMSCENE_H

/*!
  @class GLamScene
  @brief Classe d'encapsulation des paramètres courants d'une scène 3D.

La classe GLamScene maintient les propriétés de scène 3D suivantes :
- taille
- couleur de fond
- facteur d'échelle
- angles de rotation
- translations
- position de l'éclairage

<h2 class="groupheader">Propriétés</h2>

<p>@anchor scn_size @b size : taille de la scène en multiple entier de U
(unité arbitraire OpenGL). Cette taille égale à 2 par défaut
(NDC: @e Normalized @e Device @e Coordinates) détermine
l'amplitude de vue pour la plus petite dimension entre la largeur et
la hauteur de la fenêtre de scène (pour un zoom de 100%, @ref scale = 1).
- int @b %size() const ;
- void @b %setSize(int @a size ) ;

<p>@anchor scn_background @b background : couleur de fond.
La couleur de fond de scène est maintenue sous forme de 3 proportions R, V et B
dans l'intervalle 0.0 à 1.0. La valeur par défaut est fixée à
[1.0, 1.0, 1.0] (blanc).
- QVector3D @b %backgroundColor() const ;
- void @b %setBackgroundColor(float @a r, float @a g, float @a b ) ;

<p>@anchor scn_scale @b scale : zoom.
Le zoom initial est fixé à 1 (100%), les valeurs minimale et
maximale par défaut sont respectivement égales à 0.1 (10%) et 10.0 (1000%).
- float @b %scale() const ;
- void @b %setScale(float @a scale ) ;
- float @b %minScale() const ;
- float @b %maxScale() const ;
- void @b %setScaleRange(float @a min, float @a max ) ;
- void @b %resetScale() ;

<p>@anchor scn_translation @b translation : décalages du point de vue.
Les translations Tx, Ty et Tz, décalages par rapport à l'origine, peuvent être
manipulés séparément ou globalement sous forme d'un vecteur 3D.
- QVector3D @b %translation() const ;
- void @b %setTranslation(const QVector3D& @a pos ) ;
- void @b %setTranslation(float @a x, float @a y, float @a z ) ;
- float @b %translationX() const ;
- void @b %setTranslationX(float @a x ) ;
- float @b %translationY() const ;
- void @b %setTranslationY(float @a y ) ;
- float @b %translationZ() const ;
- void @b %setTranslationZ(float @a z ) ;

<p>@anchor scn_rotation @b rotation : angles du point de vue.
Les angles de rotation Rx, Ry et Rz, exprimés en degrés, peuvent être manipulés
séparément ou globalement sous forme d'un vecteur 3D.
- QVector3D @b %rotation() const ;
- void @b %setRotation(const QVector3D& @a pov ) ;
- void @b %setRotation(float @a xAngle, float @a yAngle, float @a zAngle ) ;
- float @b %rotationX() const ;
- void @b %setRotationX(float @a angle ) ;
- float @b %rotationY() const ;
- void @b %setRotationY(float @a angle ) ;
- float @b %rotationZ() const ;
- void @b %setRotationZ(float @a angle ) ;

<p>@anchor scn_lightPosition @b lightPosition : pilotage de la position spatiale
de la source d'éclairage de la scène.
- QVector3D @b %lightPosition() const ;
- bool @b %isLightPositional() const
- void @b %setLightPosition(const QVector3D& @a pos, bool @a positional = true ) ;
- void @b %setLightPosition(float @a x, float @a y, float @a z, bool @a positional = true ) ;
- void @b %addLightPositionX(float @a val ) ;
- void @b %addLightPositionY(float @a val ) ;
- void @b %addLightPositionZ(float @a val ) ;
*/

