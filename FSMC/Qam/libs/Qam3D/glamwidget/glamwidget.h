/*  ---------------------------------------------------------------------------
 *  filename    :   glamwidget.h
 *  description :   INTERFACE de la classe GLamWidget
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
	GLamWidget: Modèle de widget de type scène 3D :
	- maintien des matrices Projection et ModelView
	- pilotable par clavier/souris
	- animation par défaut (RZ)
	- export de la scène courante vers fichier PNG
	- interfaçable avec UI spécialisée GLamWidgetUi
	
	Spécialisation (* : minimale) :
	- (*) création des objets 3D de la scène (dérivés de GLamObject)
	- (*) dessin des objets
	-     animation des objets (périodicité 20 ms)
	- (*) destruction des objets
	-     surimpression 2D
	-     ajout d'intercepteurs clavier personnalisés
*/

#ifndef GLAMWIDGET_H
#define GLAMWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QTimer>
#include <QKeyEvent>
#include <QMatrix4x4>
#include <QPainter>

#include "glamscene.h"
#include "../glamobjects/glamobject.h"
#include "../glamtools/glam2d.h"

// Note :   Lors de la dérivation protected d'une classe de base,
//          les membres publiques et protégés de la classe de base deviennent
//          des membres protégés de la classe dérivée.

class GLamWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
	Q_OBJECT

  public:
	GLamWidget(QWidget* parent = Q_NULLPTR ) ;
	virtual ~GLamWidget() ;

	GLamScene& scene() const ;
	void setScene(const GLamScene& scene ) ;

	bool isFrustum() const ;
	void setFrustum(bool enabled = true ) ;

	QMatrix4x4	viewMatrix() const ;
	QMatrix4x4	projectionMatrix() const ;

	bool paintOverEnabled(bool enabled = true ) ;

	bool exportToPng(const QString& filename ) ;

	// accesseurs pour pilotage via GLamWidgetUi
	void setRotateFlags(int rx = -1, int ry = -1, int rz = -1 ) ;
//	void setTranslateFlags(int tx = 0, int ty = 0 /*, int tz = 0*/ ) ;
	void setTranslateFlags(int tx = 0, int ty = 0, float offs = 0.05 ) ;
	void setScale(float scale ) ;
	void setLightPositionFlags(int dx, int dy, int dz ) ;

	// messages en provenance de la hiérarchie GLamObject
	virtual void messageFromObjects(const QString& mess, int level = 0 ) const ;

	// test 19/02/2018 : interface QML
  public slots:
	void initPaint() ;
	void resizePaint(QSize size ) ;
	void paint(/*QPainter& painter*/ ) ;
	// test 20/02/2018
//signals:
//	void animateChanged() ;
	//

  signals:
	/*! Ce signal est émis pour indiquer un changement d'échelle
	 * provoqué par la molette de la souris ou la prise en compte d'une
	 * nouvelle @ref scene.
	 */
	void scaleChanged() ;

	/*! Ce signal est émis après l'appel à createObjects().
	 * Il permet au programme client d'être informé de la disponibilité
	 * des objets 3D de la scène. Toute tentative d'usage d'un objet de la
	 * hiérarchie GLamObject avant la réception de ce signal entraîne une
	 * interruption d'exécution...
	 * @n L'émission de ce signal est consécutive à l'affichage du message
	 * sur la sortie standard spécifiant les versions d'OpenGL/GLSL détectées.
	 */
	void initialized() ;

	/*! Signal émis par défaut par la méthode implicite messageFromObjects()
	 * de remontée des messages en provenance des objets 3D.
	 */
	void message(const QString& mess ) const ;

	// modèle de développement
  protected:
	virtual void createObjects() ;
	virtual void drawObjects() ;
	virtual void animateObjects() ;
//	virtual void deleteObjects() {}
	virtual void paintOver(QPainter* painter ) ;
	virtual void keyPressEvent(QKeyEvent* event ) ;

  private slots:
	void animate() ;

  private:
	void initializeGL() ;
	void resizeGL(int w, int h ) ;
	void paintGL() ;

	void mousePressEvent(QMouseEvent* event ) ;
	void mouseMoveEvent(QMouseEvent* event ) ;
	void wheelEvent(QWheelEvent* event ) ;

	float normalizedAngle(float angle ) ;

  private:
	GLamScene*	m_scene ;
	bool		m_isFrustum ;
	QTimer*		m_timer ;
	QPoint		m_lastMousePos ;

	bool		m_paintOverEnabled ;

	QMatrix4x4	m_projectionMatrix ;
	QMatrix4x4	m_viewMatrix ;
	bool		m_projectionChanged ;

	bool		m_animation ;		// key 'a'
	int			m_speed ;			// key '-' '+'
	float		m_zAnimate ;		// angle courant animation

	int			m_rotateFlags[3] ;
	int			m_lightPosFlags[3] ;
} ;

#endif // GLAMWIDGET_H
/*!
  @class GLamWidget
  @brief Interface d'exploitation du moteur Qam3D.

La classe GLamWidget interface la couche OpenGL afin de préparer la scène 3D
sous forme d'un widget au sens Qt (GLamWidget dérive de QOpenGLWidget qui hérite
de QWidget).
@n Elle propose un modèle de développement permettant de faire apparaître des
objets de la hiérarchie GLamObject sur la scène.
@n Ce modèle se résume principalement à 3 méthodes virtuelles protégées :
- virtual void @b createObjects() ;
- virtual void @b animateObjects() ;
- virtual void @b drawObjects().

Ces méthodes doivent être surchargées pour les classes de spécialisation de
GLamWidget afin de respectivment :
- créer dynamiquement les objets GLamObject et définir leur attitude spatiale
initiale (voir propriété @ref defaultMatrix) ;
- agir sur leur matrice de transformation, propriété @ref transformMatrix
 de GLamObject, pour animer les objets (période 20ms, facultatif) ;
- invoquer leur méthode GLamObject::draw() pour les projeter sur la scène 3D.

La destruction des objets créés dans createObjects() est à la charge du
destructeur de la classe dérivée.

La hiérarchie GLamObject prend en charge la matrice de modèle
(@e Model @e Matrix), première transformation spatiale déterminant la
visualisation de la scène 3D ; GLamWidget assure la gestion des 2 autres :
la matrice de vue (@e View @e Matrix) et la matrice de projection (@e Projection
@e Matrix). La projection par défaut est de type perspective (@e frustum).

Schéma traditionnel du système de projection 3D :
@code
 Model Coordinates : valeurs des vertices
 -----------------
		 |
		 | Model Matrix      ( GLamObject )
		 V
 World Coordinates : position et orientation spatiale
 -----------------
		 |
		 | View Matrix       ( GLamWidget )
		 V
 Camera Coordinates : position et direction du point de vue
 ------------------
		 |
		 | Projection Matrix ( GLamWidget )
		 V
 Homogeneous Coordinates : déformation cubique et plan 2D
 -----------------------
@endcode

@n La classe GLamWidget propose par défaut un jeu minimal de commandes de
manipulation de la scène au moyen du clavier et de la souris. Ces commandes
déterminent la position et la direction de la caméra, elles agissent donc sur
la matrice de vue :

<table border="0" cellpadding="5" align="center">
<tr bgcolor="#e8eef2">
	<th colspan=2>Fonction</th>
	<th colspan=2>Description</th>
</tr><tr bgcolor="#ffffe7">
	<td rowspan=2><b>Trackball virtuel</b></td>
	<td rowspan=2 align="center">souris</td>
	<td>mouvement + bouton gauche</td><td>rotations autour des axes X et Y</td>
</tr><tr bgcolor="#ffffe7">
	<td>mouvement + bouton droit</td><td>rotation autour de l'axe Z</td>
</tr><tr bgcolor="#ffffc7">
	<td><b>Zoom</b></td>
	<td align="center">souris</td>
	<td>rotation molette</td><td>réduction/agrandissement par pas de 10%</td>
</tr><tr bgcolor="#ffffe7">
	<td rowspan=5><b>Décalage<br>du point de vue</b></td>
	<td rowspan=5 align="center">clavier</td>
	<td>flèche gauche</td><td>décalage vers la gauche</td>
</tr><tr bgcolor="#ffffe7">
	<td>flèche droite</td><td>décalage vers la droite</td>
</tr><tr bgcolor="#ffffe7">
	<td>flèche haut</td><td>décalage vers le haut</td>
</tr><tr bgcolor="#ffffe7">
	<td>flèche bas</td><td>décalage vers le bas</td>
</tr><tr bgcolor="#ffffe7">
	<td>touche Home</td><td>vue centrée sur l'origine 0XYZ (défaut)</td>
</tr><tr bgcolor="#ffffc7">
	<td rowspan=3><b>Animation</b></td>
	<td rowspan=3 align="center">clavier</td>
	<td>touche A</td><td>[dés]activation de la rotation autour de l'axe Z</td>
</tr><tr bgcolor="#ffffc7">
	<td>touche +</td><td>augmentation de la vitesse de rotation</td>
</tr><tr bgcolor="#ffffc7">
	<td>touche -</td><td>diminution de la vitesse de rotation</td>
</tr>
</table>

*/
