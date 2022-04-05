/*  ---------------------------------------------------------------------------
 *  filename    :   glammaterial.h
 *  description :   INTERFACE de la classe GLamMaterial
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
 
#ifndef GLAMMATERIAL_H
#define GLAMMATERIAL_H

#include <QVector3D>
#include <QColor>
#include <QOpenGLTexture>

class GLamMaterial
{
  public:
	enum Material {
		Emerald, Jade, Obsidian, Pearl, Ruby, Turquoise,
		Brass, Bronze, Chrome, Copper, Gold, Silver, Pewter,
		BlackPlastic, CyanPlastic, GreenPlastic, RedPlastic, WhitePlastic, YellowPlastic,
		BlackRubber, CyanRubber, GreenRubber, RedRubber, WhiteRubber, YellowRubber
	} ;

	GLamMaterial() ;
	GLamMaterial(const QColor& color, bool shiny = false ) ;
	GLamMaterial(Material material ) ;
//	GLamMaterial(const GLamMaterial& material ) ;
	~GLamMaterial() ;

//	GLamMaterial& operator =(const GLamMaterial& material ) ;

	QString name() const ;
	bool isShiny() const ;
	bool isSimpleColor() const ;
	QColor color() const ;
	QVector3D ambient() const ;
	QVector3D diffuse() const ;
	QVector3D specular() const ;
	float shininess() const ;
	float transparency() const ;

	void setName(const QString& name ) ;

	void setShiny(bool shiny = true ) ;
	void setSimpleColor(bool simpleColor = true ) ;

	void setColor(QColor color ) ;
	void setColor(QVector3D color ) ;
	void setColor(float r, float g, float b ) ;

	void setAmbient(QVector3D ka ) ;
	void setAmbient(float r, float g, float b ) ;
	void setDiffuse(QVector3D kd ) ;
	void setDiffuse(float r, float g, float b ) ;
	void setSpecular(QVector3D ks ) ;
	void setSpecular(float r, float g, float b ) ;
	void setShininess(float s ) ;
	void setTransparency(float t ) ;

// Texture option
	bool hasTexture() const ;
	QOpenGLTexture* texture() const ;
	void setTexture(const QString& filename ) ;
	QString textureFilename() const ;
//
	void set(Material material ) ;
	void set(const GLamMaterial& material ) ;

  private:
	void setADS(float Ra, float Ga, float Ba, float Rd, float Gd, float Bd, float Rs, float Gs, float Bs, float shininess ) ;
	QVector3D clamp(QVector3D value ) ;
	float clamp(float value, float vmax = 1.0f ) ;

  private:
	QString		m_name ;
	bool		m_isSimpleColor ;
	bool		m_isShiny ;
	QVector3D	m_color ;
	QVector3D	m_ambient ;
	QVector3D	m_diffuse ;
	QVector3D	m_specular ;
	float		m_shininess ;
	float		m_transparency ;
// Texture option
	QString			m_texFilename ; // used by operator =
	QOpenGLTexture*	m_texture ;
//
} ;

bool operator ==(const GLamMaterial& m1, const GLamMaterial& m2 ) ;
bool operator !=(const GLamMaterial& m1, const GLamMaterial& m2 ) ;

#endif // GLAMMATERIAL_H

/*! @class GLamMaterial
	@brief Matériau défini par ses caractéristiques de réflexion.

La classe GLamMaterial encapsule l'ensemble des propriétés qui
caractérisent la réflexion ADS (Ambiante-Diffuse-Spéculaire), le coefficient
de brillance et la transparence d'un matériau.

Les réflexions ADS sont maintenues chacune par des proportions RGB
(Red-Green-Blue) comprises entre 0.0 et 1.0.
La brillance (propriété @ref mat_shininess "shininess") doit être dans l'intervalle 0..128.
La transparence (propriété @ref mat_transparency "transparency") est définie entre 0.0 (invisible)
et 1.0 (opaque).

GLamMaterial propose une palette de matériaux prédéfinis (voir énumération
GLamMaterial::Material) ; chaque matériau peut être associé à un nom (propriété @ref mat_name "name").
Un matériau prédéfini peut être décidé à la construction ou à postériori
grâce à la méthode set().

GLamMaterial assure aussi la création d'une texture au sens OpenGL à partir
d'un fichier image ; cette fonctionnalité est destinée à être exploitée par
les objets de la hiérarchie GLamObject qui disposent d'un accès à un
matériau par composition.

<b>Mode couleur simple</b> : C'est le mode par défaut dans lequel les proportions RGB
de la couleur sont maintenues par la propriété @ref mat_color "color" et la
brillance de surface par la propriété @ref mat_shiny "shiny".

<b>Mode ADS</b> : Ce mode est activé automatiquement par le constructeur dédié,
par setSimpleColor() avec un argument @a false ou par l'appel à la méthode set() qui permet de
sélectionné un matériau prédéfini.

<h2 class="groupheader">Propriétés</h2>

<p>@anchor mat_name @b name : nom associé au matériau.
- QString @b %name() const ;
- void @b %setName(const QString& @a name ) ;

<p>@anchor mat_ambient @b ambient : réflexion ambiante,
sous forme de 3 composantes RGB comprises entre 0.0 et 1.0.
La valeur par défaut est fixée à (0.05, 0.05, 0.0).
La réflexion ambiante retournée est (0.0,0.0,0.0) en mode couleur simple.
- QVector3D @b %ambient() const ;
- void @b %setAmbient(QVector3D @a ka ) ;
- void @b %setAmbient(float @a r, float @a g, float @a b ) ;

<p>@anchor mat_diffuse @b diffuse : réflexion diffuse,
sous forme de 3 composantes RGB comprises entre 0.0 et 1.0.
La valeur par défaut est fixée à (0.5, 0.5, 0.4).
La réflexion diffuse retournée vaut @ref mat_color "color" en mode couleur simple.
- QVector3D @b %diffuse() const ;
- void @b %setDiffuse(QVector3D @a kd ) ;
- void @b %setDiffuse(float @a r, float @a g, float @a b ) ;

<p>@anchor mat_specular @b specular : réflexion spéculaire,
sous forme de 3 composantes RGB comprises entre 0.0 et 1.0.
La valeur par défaut est fixée à (0.7, 0.7, 0.04).
En mode couleur simple, la réflexion spéculaire retournée vaut (0.0,0.0,0.0) ou (1.0,1.0,1.0) suivant la valeur
de @ref mat_shiny "shiny".
- QVector3D @b %specular() const ;
- void @b %setSpecular(QVector3D @a ks ) ;
- void @b %setSpecular(float @a r, float @a g, float @a b ) ;

<p>@anchor mat_shininess @b shininess : coefficient de brillance,
sous forme d'une valeur réelle dans l'intervalle 0.0 à 128.0 (fixée à 10.0
par défaut).
- float @b %shininess() const ;
- void @b %setShininess(float @a s ) ;

<p>@anchor mat_transparency @b transparency : transparence du matériau,
comprise entre 0.0 (invisible) et 1.0 (opaque, valeur par défaut).
- float @b %transparency() const ;
- void @b %setTransparency(float @a t ) ;

<p>@anchor mat_simpleColor @b simpleColor : sélecteur de mode couleur
simple (défaut), mode ADS sinon. L'usage de la méthode set() bascule
automatiquement en mode ADS.
- bool @b %isSimpleColor() const ;
- void @b %setSimpleColor(bool @a simpleColor = true ) ;

<p>@anchor mat_color @b color : Couleur simple, fixée à gris moyen par défaut.
- QColor @b %color() const ;
- void @b %setColor(QColor @a color ) ;
- void @b %setColor(QVector3D @a color ) ;
- void @b %setColor(float @a r, float @a g, float @a b ) ;

<p>@anchor mat_shiny @b shiny : Indicateur de brillance en mode couleur simple
(fixé à @a false par défaut).
- bool @b %isShiny() const ;
- void @b %setShiny(bool @a shiny = true ) ;

*/
/*! @enum GLamMaterial::Material
	@brief Matériaux prédéfinis par GLamMaterial.

	Les matériaux proposés regroupent des minéraux, des métaux ou encore des
	rendus plastiques mats et brillants.
	@n La première colonne représente à la fois le mnémonique de l'énumération
	et le nom attribué au matériau.
	@n Les proportions RGB (Red-Green-Blue) des réflexions ADS sont comprises
	entre 0.0 et 1.0. La brillance est dans l'intervalle 0..128.

<table border="0" cellpadding="5">
<tr bgcolor="#e8eef2">
	<th>@ref mat_name "name"</th>
	<th colspan="3">@ref mat_ambient "ambient"</th>
	<th colspan="3">@ref mat_diffuse "diffuse"</th>
	<th colspan="3">@ref mat_specular "specular"</th>
	<th>@ref mat_shininess "shininess"</th>
	<th></th>
</tr>

<tr align="center">
	<td>Emerald</td>
	<td>0.0215</td><td>0.1745</td><td>0.0215</td>
	<td>0.07568</td><td>0.61424</td><td>0.07568</td>
	<td>0.633</td><td>0.727811</td><td>0.633</td>
	<td>76.8</td><td></td>
</tr>
<tr align="center">
	<td>Jade</td>
	<td>0.135</td><td>0.2225</td><td>0.1575</td>
	<td>0.54</td><td>0.89</td><td>0.63</td>
	<td>0.316228</td><td>0.316228</td><td>0.316228</td>
	<td>12.8</td><td></td>
</tr>
<tr align="center">
	<td>Obsidian</td>
	<td>0.05375</td><td>0.05</td><td>0.06625</td>
	<td>0.18275</td><td>0.17</td><td>0.22525</td>
	<td>0.332741</td><td>0.328634</td><td>0.346435</td>
	<td>38.4</td><td></td>
</tr>
<tr align="center">
	<td>Pearl</td>
	<td>0.25</td><td>0.20725</td><td>0.20725</td>
	<td>1</td><td>0.829</td><td>0.829</td>
	<td>0.296648</td><td>0.296648</td><td>0.296648</td>
	<td>11.264</td><td></td>
</tr>
<tr align="center">
	<td>Ruby</td>
	<td>0.1745</td><td>0.01175</td><td>0.01175</td>
	<td>0.61424</td><td>0.04136</td><td>0.04136</td>
	<td>0.727811</td><td>0.626959</td><td>0.626959</td>
	<td>76.8</td><td></td>
</tr>
<tr align="center">
	<td>Turquoise</td>
	<td>0.1</td><td>0.18725</td><td>0.1745</td>
	<td>0.396</td><td>0.74151</td><td>0.69102</td>
	<td>0.297254</td><td>0.30829</td><td>0.306678</td>
	<td>12.8</td><td></td>
</tr>
<tr align="center" bgcolor="#ffffe7">
	<td>Brass</td>
	<td>0.329412</td><td>0.223529</td><td>0.027451</td>
	<td>0.780392</td><td>0.568627</td><td>0.113725</td>
	<td>0.992157</td><td>0.941176</td><td>0.807843</td>
	<td>27.8974</td><td></td>
</tr>
	<tr align="center" bgcolor="#ffffe7">
	<td>Bronze</td>
	<td>0.2125</td><td>0.1275</td><td>0.054</td>
	<td>0.714</td><td>0.4284</td><td>0.18144</td>
	<td>0.393548</td><td>0.271906</td><td>0.166721</td>
	<td>25.6</td><td></td>
</tr>
	<tr align="center" bgcolor="#ffffe7">
	<td>Chrome</td>
	<td>0.25</td><td>0.25</td><td>0.25</td>
	<td>0.4</td><td>0.4</td><td>0.4</td>
	<td>0.774597</td><td>0.774597</td><td>0.774597</td>
	<td>76.8</td><td></td>
</tr>
<tr align="center" bgcolor="#ffffe7">
	<td>Copper</td>
	<td>0.19125</td><td>0.0735</td><td>0.0225</td>
	<td>0.7038</td><td>0.27048</td><td>0.0828</td>
	<td>0.256777</td><td>0.137622</td><td>0.086014</td>
	<td>12.8</td><td></td>
</tr>
<tr align="center" bgcolor="#ffffe7">
	<td>Gold</td>
	<td>0.24725</td><td>0.1995</td><td>0.0745</td>
	<td>0.75164</td><td>0.60648</td><td>0.22648</td>
	<td>0.628281</td><td>0.555802</td><td>0.366065</td>
	<td>51.2</td><td></td>
</tr>
<tr align="center" bgcolor="#ffffe7">
	<td>Silver</td>
	<td>0.19225</td><td>0.19225</td><td>0.19225</td>
	<td>0.50754</td><td>0.50754</td><td>0.50754</td>
	<td>0.508273</td><td>0.508273</td><td>0.508273</td>
	<td>51.2</td><td></td>
</tr>
<tr align="center" bgcolor="#ffffe7">
	<td>Pewter</td>
	<td>0.105882</td><td>0.058824</td><td>0.113725</td>
	<td>0.427451</td><td>0.470588</td><td>0.541176</td>
	<td>0.333333</td><td>0.333333</td><td>0.521569</td>
	<td>9.84615</td><td></td>
</tr>
<tr align="center">
	<td>BlackPlastic</td>
	<td>0.0</td><td>0.0</td><td>0.0</td>
	<td>0.01</td><td>0.01</td><td>0.01</td>
	<td>0.50</td><td>0.50</td><td>0.50</td>
	<td>32.0</td><td></td>
</tr>
<tr align="center">
	<td>CyanPlastic</td>
	<td>0.0</td><td>0.1</td><td>0.06</td>
	<td>0.0</td><td>0.50980392</td><td>0.50980392</td>
	<td>0.50196078</td><td>0.50196078</td><td>0.50196078</td>
	<td>32.0</td><td></td>
</tr>
<tr align="center">
	<td>GreenPlastic</td>
	<td>0.0</td><td>0.0</td><td>0.0</td>
	<td>0.1</td><td>0.35</td><td>0.1</td>
	<td>0.45</td><td>0.55</td><td>0.45</td>
	<td>32.0</td><td></td>
</tr>
<tr align="center">
	<td>RedPlastic</td>
	<td>0.0</td><td>0.0</td><td>0.0</td>
	<td>0.5</td><td>0.0</td><td>0.0</td>
	<td>0.7</td><td>0.6</td><td>0.6</td>
	<td>32.0</td><td></td>
</tr>
<tr align="center">
	<td>WhitePlastic</td>
	<td>0.0</td><td>0.0</td><td>0.0</td>
	<td>0.55</td><td>0.55</td><td>0.55</td>
	<td>0.70</td><td>0.70</td><td>0.70</td>
	<td>32.0</td><td></td>
</tr>
<tr align="center">
	<td>YellowPlastic</td>
	<td>0.0</td><td>0.0</td><td>0.0</td>
	<td>0.5</td><td>0.5</td><td>0.0</td>
	<td>0.60</td><td>0.60</td><td>0.50</td>
	<td>32.0</td><td></td>
</tr>
<tr align="center" bgcolor="#ffffe7">
	<td>BlackRubber</td>
	<td>0.02</td><td>0.02</td><td>0.02</td>
	<td>0.01</td><td>0.01</td><td>0.01</td>
	<td>0.4</td><td>0.4</td><td>0.4</td>
	<td>10.0</td><td></td>
</tr>
<tr align="center" bgcolor="#ffffe7">
	<td>CyanRubber</td>
	<td>0.0</td><td>0.05</td><td>0.05</td>
	<td>0.4</td><td>0.5</td><td>0.5</td>
	<td>0.04</td><td>0.7</td><td>0.7</td>
	<td>10.0</td><td></td>
</tr>
<tr align="center" bgcolor="#ffffe7">
	<td>GreenRubber</td>
	<td>0.0</td><td>0.05</td><td>0.0</td>
	<td>0.4</td><td>0.5</td><td>0.4</td>
	<td>0.04</td><td>0.7</td><td>0.04</td>
	<td>10.0</td><td></td>
</tr>
<tr align="center" bgcolor="#ffffe7">
	<td>RedRubber</td>
	<td>0.05</td><td>0.0</td><td>0.0</td>
	<td>0.5</td><td>0.4</td><td>0.4</td>
	<td>0.7</td><td>0.04</td><td>0.04</td>
	<td>10.0</td><td></td>
</tr>
<tr align="center" bgcolor="#ffffe7">
	<td>WhiteRubber</td>
	<td>0.05</td><td>0.05</td><td>0.05</td>
	<td>0.5</td><td>0.5</td><td>0.5</td>
	<td>0.7</td><td>0.7</td><td>0.7</td>
	<td>10.0</td><td></td>
</tr>
<tr align="center" bgcolor="#ffffe7">
	<td>YellowRubber</td>
	<td>0.05</td><td>0.05</td><td>0.0</td>
	<td>0.5</td><td>0.5</td><td>0.4</td>
	<td>0.7</td><td>0.7</td><td>0.04</td>
	<td>10.0</td><td></td>
</tr>
<tr align="center"><td colspan="11">
<i>These names and numbers come from the OpenGL teapots.c demo,
© Silicon Graphics, Inc., © 1994, Mark J. Kilgard</i>
</td></tr>
</table>
*/
