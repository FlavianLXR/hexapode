/*  ---------------------------------------------------------------------------
 *  filename    :   glamwidgetui.cpp
 *  description :   IMPLEMENTATION de la classe GLamWidgetUi
 *
 *	project     :	Qam3D/GLam: widget "3D scene"
 *  start date  :   mars 2015 (GLam: décembre 2009)
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

#include "glamwidgetui.h"

#ifdef QAMDEBUG
	#include <QDebug>
#endif

/*! Constructeur. */

GLamWidgetUi::GLamWidgetUi(QWidget* parent )
	: QWidget(parent)
	, m_widget3D( 0 )
	, m_mode( Rotate )
	, m_hasComment( true )
{
	// sélecteur de mode

	m_rbuRotateMode = new QRadioButton(tr("Orientation (trackball)") ) ;
	m_rbuRotateMode->setChecked(true ) ;
	m_rbuTranslateMode = new QRadioButton(tr("Point de vue") ) ;
	m_rbuLightPosMode = new QRadioButton(tr("Position de l'éclairage") ) ;

	m_vboxMode = new QVBoxLayout ;
	m_vboxMode->addWidget(m_rbuRotateMode ) ;
	m_vboxMode->addWidget(m_rbuTranslateMode ) ;
	m_vboxMode->addWidget(m_rbuLightPosMode ) ;
	m_vboxMode->setSpacing(10) ;

	m_grpMode = new QGroupBox ;
	m_grpMode->setLayout(m_vboxMode ) ;

	QFont font = m_grpMode->font() ;
	font.setPointSize(10) ;
	m_grpMode->setFont( font ) ;

	// grille de boutons

	m_pbuLeft = new QPushButton(tr("X-") ) ;
	m_pbuRight = new QPushButton(tr("X+") ) ;
	m_pbuUp = new QPushButton(tr("Z+") ) ;
	m_pbuDown = new QPushButton(tr("Z-") ) ;
	m_pbuHome = new QPushButton(tr("Y-") ) ;
	m_pbuEnd = new QPushButton(tr("Y+") ) ;
	m_pbuReset = new QPushButton(tr("• ") ) ;

	QPalette pal = m_pbuLeft->palette() ;
	pal.setColor(QPalette::ButtonText, QColor(192, 0, 0 ) )   ;
	m_pbuLeft->setPalette(pal ) ;
	m_pbuRight->setPalette(pal ) ;
	pal.setColor(QPalette::ButtonText, QColor(0, 128, 0 ) )  ;
	m_pbuHome->setPalette(pal ) ;
	m_pbuEnd->setPalette(pal ) ;
	pal.setColor(QPalette::ButtonText, QColor(0, 0, 192 ) )  ;
	m_pbuUp->setPalette(pal ) ;
	m_pbuDown->setPalette(pal ) ;

	m_gridDir = new QGridLayout ;
	m_gridDir->addWidget(m_pbuLeft,  1, 0, 1, 1 ) ;
	m_gridDir->addWidget(m_pbuRight, 1, 2, 1, 1 ) ;
	m_gridDir->addWidget(m_pbuUp,    0, 1, 1, 1 ) ;
	m_gridDir->addWidget(m_pbuDown,  2, 1, 1, 1 ) ;
	m_gridDir->addWidget(m_pbuHome,  2, 0, 1, 1 ) ;
	m_gridDir->addWidget(m_pbuEnd,   0, 2, 1, 1 ) ;
	m_gridDir->addWidget(m_pbuReset, 1, 1, 1, 1 ) ;
	m_gridDir->setSpacing(2) ;

	m_wDir = new QWidget(this) ;
	m_wDir->setLayout(m_gridDir) ;

	// gestion de l'échelle

	m_labScale = new QLabel(tr("Éch. :") ) ;
	m_labValueScale = new QLabel(tr("?") ) ;
	m_labValueScale->setSizePolicy( QSizePolicy::MinimumExpanding, QSizePolicy::Minimum ) ;
	m_pbuScale1 = new QPushButton(tr("1:1") ) ;
	m_pbuScale1->setSizePolicy( QSizePolicy::Minimum, QSizePolicy::Minimum ) ;

	m_hboxScale = new QHBoxLayout ;
//	m_hboxScale->addWidget(m_labScale ) ;
	m_hboxScale->addWidget(m_labValueScale ) ;
	m_hboxScale->addWidget(m_pbuScale1 ) ;

	m_sliderScale = new QSlider( Qt::Horizontal ) ;

	m_vboxScale = new QVBoxLayout ;
	m_vboxScale->addLayout( m_hboxScale ) ;
	m_vboxScale->addWidget( m_sliderScale ) ;
//	m_vboxScale->addStretch() ;
	m_vboxScale->setSpacing(1) ;

	m_wScale = new QWidget(this) ;
	m_wScale->setLayout(m_vboxScale) ;

	// zone de texte (HTML), mode Paysage uniquement

	m_txtComment = new QTextEdit ;
	m_txtComment->setFont( font ) ;
	m_txtComment->setReadOnly( true ) ;
	m_txtComment->setMinimumHeight( 10 ) ;

	// conteneur général

//	m_boxMain = new QBoxLayout(QBoxLayout::LeftToRight ) ;
	m_boxMain = new QBoxLayout(QBoxLayout::TopToBottom ) ;
	m_boxMain->addWidget(m_grpMode ) ;
	m_boxMain->addWidget(m_wDir ) ;
	m_boxMain->addWidget(m_wScale ) ;
//    m_boxMain->addStretch() ;
	m_boxMain->addWidget(m_txtComment ) ;
//	m_boxMain->setDirection(QBoxLayout::TopToBottom ) ;
	m_boxMain->setSpacing(1) ;

	this->setLayout( m_boxMain ) ;

	// connexions

	connect(m_rbuRotateMode, SIGNAL( clicked() ), this, SLOT( rbuMode_clicked() ) ) ;
	connect(m_rbuTranslateMode, SIGNAL( clicked() ), this, SLOT( rbuMode_clicked() ) ) ;
	connect(m_rbuLightPosMode, SIGNAL( clicked() ), this, SLOT( rbuMode_clicked() ) ) ;
	connect(m_pbuLeft, SIGNAL( pressed() ), this, SLOT( pbuLeft_pressed() ) ) ;
	connect(m_pbuLeft, SIGNAL( released() ), this, SLOT( pbu_released() ) ) ;
	connect(m_pbuRight, SIGNAL( pressed() ), this, SLOT( pbuRight_pressed() ) ) ;
	connect(m_pbuRight, SIGNAL( released() ), this, SLOT( pbu_released() ) ) ;
	connect(m_pbuUp, SIGNAL( pressed() ), this, SLOT( pbuUp_pressed() ) ) ;
	connect(m_pbuUp, SIGNAL( released() ), this, SLOT( pbu_released() ) ) ;
	connect(m_pbuDown, SIGNAL( pressed() ), this, SLOT( pbuDown_pressed() ) ) ;
	connect(m_pbuDown, SIGNAL( released() ), this, SLOT( pbu_released() ) ) ;
	connect(m_pbuHome, SIGNAL( pressed() ), this, SLOT( pbuHome_pressed() ) ) ;
	connect(m_pbuHome, SIGNAL( released() ), this, SLOT( pbu_released() ) ) ;
	connect(m_pbuEnd, SIGNAL( pressed() ), this, SLOT( pbuEnd_pressed() ) ) ;
	connect(m_pbuEnd, SIGNAL( released() ), this, SLOT( pbu_released() ) ) ;
	connect(m_pbuReset, SIGNAL( pressed() ), this, SLOT( pbuReset_pressed() ) ) ;
	connect(m_pbuScale1, SIGNAL( pressed() ), this, SLOT( pbuScale1_pressed() ) ) ;
	connect(m_sliderScale, SIGNAL( valueChanged(int) ), this, SLOT( scaleChanged(int) ) ) ;
}

/*! Attachement de l'interface utilisateur avec une scène 3D GLamWidget. */

void GLamWidgetUi::bindTo(GLamWidget *widget3D )
{
	m_widget3D = widget3D ;
	if ( !m_widget3D )	return ;

	scaleChanged() ;	// init. slider
	connect(m_widget3D, SIGNAL( scaleChanged() ), this, SLOT( scaleChanged() ) ) ;
}

/*! Méthode de changement d'orientation portrait/paysage.
 * Les possibles sont Qt::Vertical (par défaut à la construction)
 * ou Qt::Horizontal. La zone de texte alimentée par setComment() et
 * addComment() n'est visible qu'en mode portrait.
 * @sa setCommentVisible()
 */

void GLamWidgetUi::setOrientation(Qt::Orientation orientation )
{
	if ( orientation == Qt::Vertical ) {
		m_boxMain->setDirection(QBoxLayout::TopToBottom ) ;
		m_txtComment->show() ;
	}
	else {
		m_boxMain->setDirection(QBoxLayout::LeftToRight ) ;
		m_txtComment->hide() ;
	}
}

/*! Méthode permettant de montrer/masquer la zone de commentaires.
 * @sa setOrientation()
*/

void GLamWidgetUi::setCommentVisible(bool visible )
{
	m_hasComment = visible ;
	if ( m_hasComment )	m_txtComment->show() ;
	else				m_txtComment->hide() ;
}

/*! Méthode de remplissage de la zone de texte libre (HTML).
 * L'éventuel contenu précédent est remplacé par @a text.
 * @sa addComment()
 */

void GLamWidgetUi::setComment(const QString& text )
{
	m_txtComment->setHtml( text ) ;
}

/*! Méthode d'ajout dans la zone de texte libre (HTML).
 * @sa setComment()
 */

void GLamWidgetUi::addComment(const QString& text )
{
	m_txtComment->append( text ) ;
}

// --------------------------------------------------------------------------
//	changement de point de vue (rotation, décalage, position éclairage)
// --------------------------------------------------------------------------

// [private] sélecteur de mode

void GLamWidgetUi::rbuMode_clicked()
{
	pbu_released() ;
	if ( m_rbuRotateMode->isChecked() )		m_mode = Rotate ;
	if ( m_rbuTranslateMode->isChecked() )	m_mode = Translate ;
	if ( m_rbuLightPosMode->isChecked() )	m_mode = LightPosition ;
	m_pbuHome->setVisible( m_mode != Translate ) ;
	m_pbuEnd->setVisible( m_mode != Translate ) ;
	m_pbuReset->setVisible( m_mode != LightPosition ) ;
}

// [private] annulation de la rotation en cours

void GLamWidgetUi::pbu_released()
{
	if ( !m_widget3D )	return ;
	m_widget3D->setRotateFlags( 0, 0, 0 ) ;
	m_widget3D->setLightPositionFlags( 0, 0, 0 ) ;
}

// [private] annulation des rotations/décalages du point de vue

void GLamWidgetUi::pbuReset_pressed()
{
	if ( !m_widget3D )	return ;
	if ( m_mode == Rotate )			m_widget3D->setRotateFlags() ;
	if ( m_mode == Translate )		m_widget3D->setTranslateFlags() ;
    emit resetPressed() ;
}

// [private] actions sur les boutons directionnels de l'interface
// - rotations effectives tant que le bouton est enfoncé
// - translations ponctuelles lors de chaque appui
// - déplacements point d'éclairage ponctuels

void GLamWidgetUi::pbuLeft_pressed()
{
	if ( !m_widget3D )	return ;
	if ( m_mode == Rotate )			m_widget3D->setRotateFlags(-1, 0, 0 ) ;
	if ( m_mode == Translate )		m_widget3D->setTranslateFlags(+1, 0 ) ;
	if ( m_mode == LightPosition )	m_widget3D->setLightPositionFlags(-1, 0, 0 ) ; ;
}

void GLamWidgetUi::pbuRight_pressed()
{
	if ( !m_widget3D )	return ;
	if ( m_mode == Rotate )			m_widget3D->setRotateFlags(+1, 0, 0 ) ;
	if ( m_mode == Translate )		m_widget3D->setTranslateFlags(-1, 0 ) ;
	if ( m_mode == LightPosition )	m_widget3D->setLightPositionFlags(+1, 0, 0 ) ; ;
}

void GLamWidgetUi::pbuUp_pressed()
{
	if ( !m_widget3D )	return ;
	if ( m_mode == Rotate )			m_widget3D->setRotateFlags( 0, 0,+1 ) ;
	if ( m_mode == Translate )		m_widget3D->setTranslateFlags( 0,-1 ) ;
	if ( m_mode == LightPosition )	m_widget3D->setLightPositionFlags( 0, 0,+1 ) ; ;
}

void GLamWidgetUi::pbuDown_pressed()
{
	if ( !m_widget3D )	return ;
	if ( m_mode == Rotate )			m_widget3D->setRotateFlags( 0, 0,-1 ) ;
	if ( m_mode == Translate )		m_widget3D->setTranslateFlags( 0,+1 ) ;
	if ( m_mode == LightPosition )	m_widget3D->setLightPositionFlags( 0, 0,-1 ) ; ;
}

void GLamWidgetUi::pbuHome_pressed()
{
	if ( !m_widget3D )	return ;
	if ( m_mode == Rotate )			m_widget3D->setRotateFlags( 0,-1, 0 ) ;
	if ( m_mode == LightPosition )	m_widget3D->setLightPositionFlags( 0,-1, 0 ) ; ;
}

void GLamWidgetUi::pbuEnd_pressed()
{
	if ( !m_widget3D )	return ;
	if ( m_mode == Rotate )			m_widget3D->setRotateFlags( 0,+1, 0 ) ;
	if ( m_mode == LightPosition )	m_widget3D->setLightPositionFlags( 0,+1, 0 ) ; ;
}

// --------------------------------------------------------------------------
//	changement d'échelle
//	le slider est logarithmique ; pour mémoire, log10(x) = ln(x) / ln(10)
// --------------------------------------------------------------------------

// [private] échelle [1:1] = 100%

void GLamWidgetUi::pbuScale1_pressed()
{
	m_widget3D->setScale( 1.0 ) ;
	scaleChanged() ;
}

// [private] changement en provenance de la scène GLamWidget -> màj slider

void GLamWidgetUi::scaleChanged()
{
	if ( !m_widget3D )	return ;

	int scale    = 100 * m_widget3D->scene().scale() ;
	int minScale = 100 * m_widget3D->scene().minScale() ;
	int maxScale = 100 * m_widget3D->scene().maxScale() ;

	disconnect(m_sliderScale, 0, this, 0 ) ;

	m_sliderScale->setMinimum( minScale ) ;
	m_sliderScale->setMaximum( maxScale ) ;
	m_sliderScale->setValue(
//		minScale + ( qLn(scale)/qLn(10) - qLn(minScale)/qLn(10) ) * maxScale / ( qLn(maxScale/minScale)/qLn(10))
		minScale + ( qLn(scale) - qLn(minScale) ) / qLn(maxScale/minScale)  * maxScale
	) ;

	connect(m_sliderScale, SIGNAL( valueChanged(int) ), this, SLOT( scaleChanged(int) ) ) ;

	updateScale( scale ) ;
}

// [private] action sur le slider -> màj scène GLamWidget

void GLamWidgetUi::scaleChanged(int value )
{
	if ( !m_widget3D )	return ;

	int minScale = m_sliderScale->minimum() ;
	int maxScale = m_sliderScale->maximum() ;

	float scale = qPow(10, value * ( qLn( maxScale/minScale ) / qLn(10) ) / maxScale + qLn( minScale ) / qLn(10) ) ;

	m_widget3D->setScale( scale / 100.0 ) ;
	updateScale( (int)( 100 * m_widget3D->scene().scale() ) ) ;
}

// [private] mise à jour de l'affichage en pourcentage

void GLamWidgetUi::updateScale(int scale )
{
	m_labValueScale->setText( QString("%1 %").arg( scale ) ) ;
}
