/*  ---------------------------------------------------------------------------
 *  filename    :   glamwidget.cpp
 *  description :   IMPLEMENTATION de la classe GLamWidget
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

#include "../_ABOUT"
#include "glamwidget.h"
#include <QTimer>
#include <QPainter>
#include <QMouseEvent>

//#ifdef QAMDEBUG
	#include <QDebug>
//#endif

/*! Constructeur.
 * @n Ce constructeur invoque la méthode statique
 * GLamObject::setGLamWidget() pour s'identifier en tant
 * que scène de projection pour les futurs objets de la
 * hiérarchie GLamObject.
 */

GLamWidget::GLamWidget(QWidget* parent )
	: QOpenGLWidget( parent )
	, m_isFrustum( false )
	, m_paintOverEnabled( true )
	, m_projectionChanged( true )
	, m_animation( false )
	, m_speed( 1 )
	, m_zAnimate( 0.0f )
{
	m_rotateFlags[0] = 0 ;
	m_rotateFlags[1] = 0 ;
	m_rotateFlags[2] = 0 ;
	m_lightPosFlags[0] = 0 ;
	m_lightPosFlags[1] = 0 ;
	m_lightPosFlags[2] = 0 ;

	m_scene = new GLamScene ;

	this->setFocusPolicy( Qt::StrongFocus ) ;

	m_timer = new QTimer(this ) ;
	connect(m_timer, SIGNAL( timeout() ), this, SLOT( animate() ) ) ;
	m_timer->start( 20 ) ;

	GLamObject::setGLamWidget( this ) ;
}

/*! Destructeur. */

GLamWidget::~GLamWidget()
{
	m_timer->stop() ;

	delete m_scene ;
/*
	makeCurrent() ;
	deleteObjects() ;	// in destructor, it does not call derived version !!!
	doneCurrent() ;
*/
}

/*! Accès aux paramètres de la scène 3D courante. */

GLamScene& GLamWidget::scene() const
{
	return *m_scene ;
}

/*! @internal */

void GLamWidget::setScene(const GLamScene& scene )
{
	*m_scene = scene ;
	emit scaleChanged() ;	// pour màj slider...
}

/*! Indique si la matrice de projection est de type perspective
 * (orthographique sinon).
 */

bool GLamWidget::isFrustum() const
{
	return m_isFrustum ;
}

/*! Fixe le mode de projection, perspective par défaut,
 * orthographique sinon. */

void GLamWidget::setFrustum(bool enabled )
{
	m_isFrustum = enabled ;
	m_projectionChanged = true ;
}

/*! Méthode d'activation ou non de la sur-impression 2D
 * sur la scène courante.
 * @sa paintOver()
 */

bool GLamWidget::paintOverEnabled(bool enabled )
{
	bool currentState = m_paintOverEnabled ;
	m_paintOverEnabled = enabled ;
	return currentState ;
}

/*! Matrice de projection courante. */

QMatrix4x4	GLamWidget::projectionMatrix() const
{
	return m_projectionMatrix ;
}

/*! Matrice de vue courante. */

QMatrix4x4	GLamWidget::viewMatrix() const
{
	return m_viewMatrix ;
}

/*! Ressource d'export de la scène courante au format d'image PNG,
 * vers le fichier @a filename spécifié en argument.
 * La taille de l'image est équivalente à celle du widget de visualisation 3D.
 * @n Retourne @a false en cas d'échec d'enregistrement du fichier.
 */

bool GLamWidget::exportToPng(const QString& filename )
{
	QPixmap pixmap( this->size() ) ;
	makeCurrent() ;
	this->render( &pixmap ) ;
	doneCurrent() ;

	QString path = filename ;
	if ( !path.endsWith(".png", Qt::CaseInsensitive) )	path += ".png" ;
	return pixmap.save( path ) ;
}

// ---------------------------------------------------------------------------

// [private] slot activé périodiquement

void GLamWidget::animate()
{
	if ( m_rotateFlags[0] )	m_scene->setRotationX( m_scene->rotationX() + m_rotateFlags[0] ) ;
	if ( m_rotateFlags[1] )	m_scene->setRotationY( m_scene->rotationY() + m_rotateFlags[1] ) ;
	if ( m_rotateFlags[2] )	m_scene->setRotationZ( m_scene->rotationZ() + m_rotateFlags[2] ) ;
	QVector3D lp = m_scene->lightPosition() ;
	lp += QVector3D(m_lightPosFlags[0], m_lightPosFlags[1], m_lightPosFlags[2] ) ;
	m_scene->setLightPosition( lp ) ;
	animateObjects() ;
	update() ;
	// test 20/02/2018
//	emit animateChanged() ;
}

// ---------------------------------------------------------------------------
// modèle de spécialisation

/*! Méthode du modèle de spécialisation.
 * @n Cette méthode doit être surchargée par la classe dérivée de GLamWidget
 * afin d'assurer la création des objets 3D (issus de la hiérarchie GLamObject)
 * et leur positionnement spatiale initial (voir propriété @ref defaultMatrix).
 */

void GLamWidget::createObjects()
{
}

/*! Méthode du modèle de spécialisation.
 * @n Cette méthode doit être surchargée par la classe dérivée de GLamWidget
 * afin de projeter les objets 3D sur la scène au moyen de leur méthode
 * GLamObject::draw() après application de leur matrice de transformation
 * (voir propriété @ref transformMatrix).
 */

void GLamWidget::drawObjects()
{
}

/*! Méthode du modèle de spécialisation.
 * @n Cette méthode peut être surchargée par la classe dérivée de GLamWidget
 * pour animer les objets de la scène 3D en modifiant leur matrice de
 * transformation (voir propriété @ref transformMatrix).
 * @n Cette routine d'animation est déclenchée périodiquement toutes les 20ms.
 */

void GLamWidget::animateObjects()
{
}

/*! Ressource de sur-impression 2D sur le fenêtre de la scène 3D.
 * Le copyright de la bibliothèque Qam3D est affiché par défaut en bas
 * à gauche de la vue.
 * @n Cette méthode peut être surchargée par les classes de spécialisation.
 * @sa paintOverEnabled()
 */

void GLamWidget::paintOver(QPainter* painter )
{
	painter->setPen( Qt::gray ) ;
//	painter->drawText(10, height() - 10, QAM3D_COPYRIGHT ) ;

	QString txt( QAM3D_COPYRIGHT ) ;
	txt += QString(" - GLSL: %1").arg( (char*)glGetString(GL_SHADING_LANGUAGE_VERSION) ) ;
	painter->drawText(10, height() - 10, txt ) ;
}

/*! Méthode de traitement par défaut des messages en provenance de la
 * hiérarchie des objets GLamObject.
 * Le type de message est spécifié par @a level :
 * 0=info, 1=warning, 2=error, 3=fatal
 * @n Par défaut, cette méthode se contente d'émettre le signal message()
 * sans tenir compte du @a level ; elle peut être surchargée par la
 * classe de spécialisation de GLamWidget afin de mettre en place
 * un traitement plus fin des messages.
 */

void GLamWidget::messageFromObjects(const QString& mess, int level ) const
{
	Q_UNUSED( level ) ;
	emit message( mess ) ;
}

// test 19/02/2018 - version utilisable par QML...
void GLamWidget::initPaint()
{
	initializeOpenGLFunctions() ;
//	createObjects() ;
	qDebug("GLamWidget::initializeGL(): OpenGL version: %s GLSL: %s",
	glGetString(GL_VERSION), glGetString(GL_SHADING_LANGUAGE_VERSION));
//	resizeGL(width(), height() ) ;
	emit initialized() ;
}

void GLamWidget::resizePaint(QSize size )
{
	resizeGL(size.width(), size.height() ) ;
}

void GLamWidget::paint(/*QPainter& painter*/ )
{

//	painter.begin(this) ;

//	painter.setRenderHint(QPainter::HighQualityAntialiasing ) ;

	// raw OpenGL commands...
//	painter.beginNativePainting() ;

	if ( m_projectionChanged )	resizeGL(width(), height() ) ;

	// background
	QVector3D	bg = m_scene->backgroundColor() ;
	glClearColor(bg.x(), bg.y(), bg.z(), 1.0f ) ;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ) ;

	// Z-buffer
	glEnable(GL_DEPTH_TEST ) ;

	// Transparency option
	glEnable( GL_BLEND ) ;
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA ) ;

	// View Matrix
	m_viewMatrix.setToIdentity() ;
	m_viewMatrix.translate(0.0f, 0.0f, m_scene->translationZ() ) ;

	m_viewMatrix.rotate(m_scene->rotationX(), 1.0f, 0.0f, 0.0f ) ;
	m_viewMatrix.rotate(m_scene->rotationY(), 0.0f, 1.0f, 0.0f ) ;

	float zAngle = m_scene->rotationZ() ;
	if ( m_animation ) {
		m_zAnimate = normalizedAngle( m_zAnimate + m_speed * 0.1f ) ;
		zAngle = m_zAnimate ;
	}
	m_viewMatrix.rotate(zAngle, 0.0f, 0.0f, 1.0f ) ;

	m_viewMatrix.scale(m_scene->scale() ) ;

	// projection des objets sur la scène
	GLamObject::resetVerticesCounter() ;	// init. compteur de vertices
	drawObjects() ;

	// Transparency option
	glDisable(GL_DEPTH_TEST ) ;
	glDisable(GL_CULL_FACE ) ;

	// painter drawing...
//	painter.endNativePainting() ;
//	if ( m_paintOverEnabled )	paintOver( &painter ) ;

//	painter.end() ;
}

// ---------------------------------------------------------------------------

// [private] initialisation du contexte OpenGL
// --> invoque createObjects() du modèle de dév.

void GLamWidget::initializeGL()
{
	initializeOpenGLFunctions() ;

	createObjects() ;

	qDebug("GLamWidget::initializeGL(): OpenGL version: %s GLSL: %s",
	glGetString(GL_VERSION), glGetString(GL_SHADING_LANGUAGE_VERSION));

	emit initialized() ;
}

// [private] dimensionnement de la vue OpenGL
// méthode déclenchée automatiquement lorsque le widget est retaillé
// ou sur demande par paintGL() quand m_scene->translation() est modifié.
// --> mise à jour de la matrice de projection

void GLamWidget::resizeGL(int w, int h )
{
	// rapports de projection sur plan 2D sans déformation
	float rapx = float(w)/float(h) ;
	float rapy = 1.0f ;
	if ( w < h ) {
		rapx = 1.0f ;
		rapy = float(h)/float(w) ;
	}

	// décalage du POV
	float dx = m_scene->translationX() ;
	float dy = m_scene->translationY() ;

	// prise en compte de la taille de scène courante
	rapx *= m_scene->size() / 2.0 ;
	rapy *= m_scene->size() / 2.0 ;
	dx *= m_scene->size() ;
	dy *= m_scene->size() ;

	// projection en perspective (par défaut) ou orthographique
	m_projectionMatrix.setToIdentity() ;
	if ( m_isFrustum )
			m_projectionMatrix.frustum(-rapx + dx, rapx + dx, -rapy + dy, rapy + dy, 0.0f, 100.0f ) ;
	else	m_projectionMatrix.ortho(  -rapx + dx, rapx + dx, -rapy + dy, rapy + dy, 0.0f, 100.0f ) ;

	// caméra à l'origine du monde...
	m_viewMatrix.setToIdentity() ;
	// ... il faut donc prendre un peu de recul !
	m_scene->setTranslationZ(-50.0) ;

	m_projectionChanged = false ;
}

// [private] rafraichissement de la vue OpenGL.
// --> invoque drawObjects() + paintOver() du modèle de dév.

void GLamWidget::paintGL()
{
	QPainter painter ;
	painter.begin(this) ;

//	painter.setRenderHint(QPainter::HighQualityAntialiasing ) ;
    painter.setRenderHint(QPainter::Antialiasing ) ;

	painter.beginNativePainting() ;

	if ( m_projectionChanged )	resizeGL(width(), height() ) ;

	// background
	QVector3D	bg = m_scene->backgroundColor() ;
	glClearColor(bg.x(), bg.y(), bg.z(), 1.0f ) ;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT ) ;

	// Z-buffer
	glEnable(GL_DEPTH_TEST ) ;

	// Transparency option
	glEnable( GL_BLEND ) ;
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA ) ;

	// View Matrix
	m_viewMatrix.setToIdentity() ;
	m_viewMatrix.translate(0.0f, 0.0f, m_scene->translationZ() ) ;

	m_viewMatrix.rotate(m_scene->rotationX(), 1.0f, 0.0f, 0.0f ) ;
	m_viewMatrix.rotate(m_scene->rotationY(), 0.0f, 1.0f, 0.0f ) ;

	float zAngle = m_scene->rotationZ() ;
	if ( m_animation ) {
		m_zAnimate = normalizedAngle( m_zAnimate + m_speed * 0.1f ) ;
		zAngle = m_zAnimate ;
	}
	m_viewMatrix.rotate(zAngle, 0.0f, 0.0f, 1.0f ) ;

	m_viewMatrix.scale(m_scene->scale() ) ;

	// projection des objets sur la scène
	GLamObject::resetVerticesCounter() ;	// init. compteur de vertices
	drawObjects() ;

	// Transparency option
	glDisable(GL_DEPTH_TEST ) ;
	glDisable(GL_CULL_FACE ) ;

	// painter drawing...
	painter.endNativePainting() ;
	if ( m_paintOverEnabled )	paintOver( &painter ) ;

	painter.end() ;
}

// ---------------------------------------------------------------------------
// interface utilisateur
// -----------------------------------------------------------------------

/*! Intercepteur des événements liés au clavier (test de la valeur
 * de touche, sans tenir compte des modificateurs éventuels...).
 * Cette méthode peut être chainée par une surcharge mise en place
 * dans la classe de spécialisation de GLamWidget.
 */

void GLamWidget::keyPressEvent(QKeyEvent* event )
{
	float dx = m_scene->translationX() ;
	float dy = m_scene->translationY() ;
	float offs = 0.05f ;	// décalage scène par pas de 0.05*size()

	switch( event->key() ) {
		case Qt::Key_Left :		dx += offs ; break ;
		case Qt::Key_Right :	dx -= offs ; break ;
		case Qt::Key_Down :		dy += offs ; break ;
		case Qt::Key_Up :		dy -= offs ; break ;
		case Qt::Key_Home :		dx = dy = 0.0f ; break ;
		case Qt::Key_A :
			m_animation = !m_animation ;
			if ( m_animation )	m_zAnimate = m_scene->rotationZ() ;
			else				m_scene->setRotationZ( m_zAnimate ) ;
			break ;
		case Qt::Key_Plus :		m_speed++ ; break ;
		case Qt::Key_Minus :	m_speed-- ;	break ;
	}
	m_scene->setTranslationX( dx ) ;
	m_scene->setTranslationY( dy ) ;
	m_projectionChanged = true ;
}

/* [private] */

void GLamWidget::mousePressEvent(QMouseEvent* event )
{
	m_lastMousePos = event->pos() ;
}

/* [private] */

void GLamWidget::mouseMoveEvent(QMouseEvent* event )
{
//	int dx = event->x() - m_lastMousePos.x() ;
//	int dy = event->y() - m_lastMousePos.y() ;
    int dx = event->position().x() - m_lastMousePos.x() ;
    int dy = event->position().y() - m_lastMousePos.y() ;

	QVector3D r = m_scene->rotation() ;

	if ( event->buttons() & Qt::LeftButton ) {
		m_scene->setRotationX( normalizedAngle( r.x() + dy / 2.0f ) ) ;
		m_scene->setRotationY( normalizedAngle( r.y() + dx / 2.0f ) ) ;
	}
	else if ( event->buttons() & Qt::RightButton ) {
		m_scene->setRotationX( normalizedAngle( r.x() + dy / 2.0f ) ) ;
		m_scene->setRotationZ( normalizedAngle( r.z() + dx / 2.0f ) ) ;
	}
	m_lastMousePos = event->pos() ;
}

/* [private] */

void GLamWidget::wheelEvent(QWheelEvent* event )
{
	float scale = m_scene->scale() ;
//	scale += ( event->delta() > 0 ? scale * 0.1f : -scale * 0.1f ) ;
    scale += ( event->angleDelta().y() > 0 ? scale * 0.1f : -scale * 0.1f ) ;
    m_scene->setScale( scale ) ;

	emit scaleChanged() ;	// pour màj slider...
}

/* [private] Méthode utilitaire de normalisation d'un @a angle
 * en degré dans l'intervalle 0..360.
 */

float GLamWidget::normalizedAngle(float angle )
{
	while ( angle < 0.0f )		angle += 360.0f ;
	while ( angle > 360.0f )	angle -= 360.0f ;
	return angle ;
}

// -----------------------------------------------------------------------
// accesseurs pour GLamWidgetUi
// -----------------------------------------------------------------------

/*! @internal.
 * Requête de rotation du point de vue.
 * Cette méthode est invoquée par l'interface GLamWidgetUi
 * pilotée par l'utilisateur.
 * @n Les angles spécifiés (en degrés entiers) sont cumulés aux valeurs
 * courantes d'angles du POV de la scène ; les valeurs courantes peuvent être
 * remises à zéro en spécifiant @a rx, @a ry et @a rz à -1 (valeurs par
 * défaut des arguments).
 * @n Des valeurs nulles d'arguments ne remettent pas les angles à zéro mais
 * stoppent toute éventuelle animation en rotation...
 */

void GLamWidget::setRotateFlags(int rx, int ry, int rz )
{
	if (( rx == -1 )&&( ry == -1 )&&( rz == -1 )) {
		rx = ry = rz = 0 ;
		m_scene->setRotation(0.0, 0.0, 0.0 ) ;
	}
	m_rotateFlags[0] = rx ;
	m_rotateFlags[1] = ry ;
	m_rotateFlags[2] = rz ;
}


/*! @internal.
 * Requête de décalage du point de vue dans le plan OXY.
 * Cette méthode est invoquée par l'interface GLamWidgetUi
 * pilotée par l'utilisateur.
 * @n Les décalages spécifiés sont cumulés aux valeurs
 * courantes de position du POV de la scène ; les valeurs courantes peuvent être
 * remises à zéro en spécifiant @a tx et @a ty à 0 (valeurs par
 * défaut des arguments).
 */

//void GLamWidget::setTranslateFlags(int tx, int  ty /*, int tz*/ )
void GLamWidget::setTranslateFlags(int tx, int  ty, float offs )
{
	if ( !(tx + ty) ) {
		m_scene->setTranslation(0.0, 0.0, 0.0 ) ;
		return ;
	}
//	float offs = 0.05f ;	// décalage scène par pas de 0.05*size()

	if ( tx )	m_scene->setTranslationX( m_scene->translationX() + tx * offs ) ;
	if ( ty )	m_scene->setTranslationY( m_scene->translationY() + ty * offs ) ;
//	if ( tz )	m_scene->setTranslation(0.0, 0.0, 0.0 ) ;
	m_projectionChanged = true ;
}


/*! @internal
 * Requête de modification du zoom (échelle).
 * Cette méthode est invoquée par l'interface GLamWidgetUi
 * pilotée par l'utilisateur.
 */

void GLamWidget::setScale(float scale )
{
	m_scene->setScale( scale ) ;
}


/*! @internal
 * Requête de déplacement de l'éclairage.
 * Cette méthode est invoquée par l'interface GLamWidgetUi
 * pilotée par l'utilisateur.
 */

void GLamWidget::setLightPositionFlags(int dx, int dy, int dz )
{
	m_lightPosFlags[0] = dx ;
	m_lightPosFlags[1] = dy ;
	m_lightPosFlags[2] = dz ;
}
