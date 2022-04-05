/*  ---------------------------------------------------------------------------
 *  filename    :   glamwidgetui.h
 *  description :   INTERFACE de la classe GLamWidgetUi
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

#ifndef GLAMWIDGETUI_H
#define GLAMWIDGETUI_H

#include <QWidget>
#include "glamwidget.h"
#include <QVBoxLayout>
#include <QGroupBox>
#include <QLabel>
#include <QRadioButton>
#include <QPushButton>
#include <QCheckBox>
#include <QSlider>
#include <QTextEdit>

class GLamWidgetUi : public QWidget
{
	Q_OBJECT

  public:
	explicit GLamWidgetUi(QWidget* parent = 0 ) ;

	void setOrientation(Qt::Orientation orientation ) ;
	void setCommentVisible(bool visible = true ) ;

	void bindTo(GLamWidget* widget3D ) ;

  public slots:
	void setComment(const QString& text ) ;
	void addComment(const QString& text ) ;

  signals:
    void resetPressed() ;

  private slots:
	void rbuMode_clicked() ;
	void pbu_released() ;
	void pbuReset_pressed() ;
	void pbuLeft_pressed() ;
	void pbuRight_pressed() ;
	void pbuUp_pressed() ;
	void pbuDown_pressed() ;
	void pbuHome_pressed() ;
	void pbuEnd_pressed() ;
	void pbuScale1_pressed() ;
	void scaleChanged() ;
	void scaleChanged(int ) ;

  private:
	enum Mode { Rotate, Translate, LightPosition } ;
	void updateScale(int scale ) ;

  private:
	GLamWidget*		m_widget3D ;		// scène 3D associée
	Mode			m_mode ;			// mode commande en cours

	bool			m_hasComment ;		// avec/sans zone de texte

	QBoxLayout*		m_boxMain ;			// conteneur général

	QGroupBox*		m_grpMode ;			// widget 1
	QVBoxLayout*	m_vboxMode ;
	QRadioButton*	m_rbuRotateMode ;
	QRadioButton*	m_rbuTranslateMode ;
	QRadioButton*	m_rbuLightPosMode ;

	QWidget*		m_wDir ;			// widget 2
	QGridLayout*	m_gridDir ;
	QPushButton*	m_pbuLeft ;
	QPushButton*	m_pbuRight ;
	QPushButton*	m_pbuUp ;
	QPushButton*	m_pbuDown ;
	QPushButton*	m_pbuHome ;
	QPushButton*	m_pbuEnd ;
	QPushButton*	m_pbuReset ;

	QWidget*		m_wScale ;			// widget 3
	QHBoxLayout*	m_hboxScale ;
	QVBoxLayout*	m_vboxScale ;
	QLabel*			m_labScale ;
	QLabel*			m_labValueScale ;
	QPushButton*	m_pbuScale1 ;

	QSlider*		m_sliderScale ;		// widget 4

	QTextEdit*		m_txtComment ;		// widget 5
} ;

#endif // GLAMWIDGETUI_H

/*!
  @class GLamWidgetUi
  @brief Interface Utilisateur de pilotage d'une scène 3D GLamWidget.

La classe GLamWidgetUi propose une interface graphique destinée au pilotage
d'une scène 3D GLamWidget, elle permet de controler les rotations et
décalages de point de vue, la position de l'éclairage et le facteur d'échelle
de la scène.

La classe GLamWidgetUi est dérivée de QWidget. Elle peut donc directement
remplacer par promotion un élément de type QWidget positionné sur une
interface construite avec Qt Designer. Elle peut aussi être positionné sur une
interface utilisateur d'application au même titre que tout autre objet issu de
la hiérarchie QWidget.

L'objet GLamWidgetUi doit être associé à un objet dérivé de GLamWidget au moyen
de la méthode publique bindTo().

L'interface est constituée d'un sélecteur de mode, d'une grille de boutons
directionnels, d'un sélecteur d'échelle (zoom logarithmique entre 10 et 1000%)
et d'une zone de texte libre alimentée par les ressources publiques setComment()
et addComment().

La classe permet de choisir entre une disposition de type portrait (par défaut)
ou paysage ; dans ce dernier mode, la zone de texte n'est pas visible.

<hr><h2>Exemple</h2>
Comment créer une application dont l'interface principale est une
scène 3D avec ses commandes de pilotage (zoom, rotations...) ?

<table border="0" width="100%" ><tr>
<td>
1. Point d'entrée : main.cpp

@code
        #include "mainwindow.h"
        #include <QApplication>

        int main(int argc, char* argv[] )
        {
            QApplication a(argc, argv ) ;
            MainWindow w ;
            w.show() ;      // projection de la fenêtre principale

            return a.exec() ;
        }
@endcode
2. Fenêtre principale de classe MainWindow dérivée de QMainWindow

@code
    // Membres privés

        QBoxLayout*			m_layout ;
        MyGLamWidget*		m_glamWidget ;
        GLamWidgetUi*		m_glamWidgetUi ;

    // Construction

    MainWindow::MainWindow(QWidget* parent )
        : QMainWindow(parent)
    {
        // ...
        // setupUi( this ) ;
        // ...

        // construction de la scène 3D
        m_glamWidget = new MyGLamWidget ;

        // construction d'une IHM de pilotage et attachement à la scène
        m_glamWidgetUi = new GLamWidgetUi( this ) ;
        m_glamWidgetUi->bindTo( m_glamWidget ) ;

        // création de l'agencement de la fenêtre
        QWidget* centralWidget = new QWidget( this ) ;
        m_layout = new QBoxLayout(QBoxLayout::LeftToRight, centralWidget ) ;
        m_layout->addWidget( m_glamWidget ) ;
        m_layout->addWidget( m_glamWidgetUi ) ;
        m_glamWidget->setMinimumSize(300, 300 ) ;
        setCentralWidget( centralWidget ) ;
    }

@endcode
3.	Objet(s) 3D = membres privés de MyGLamWidget (dérivée de GLamWidget)

@code
        Axis3D*			m_axis ;
        GLamCube*		m_cube ;
        float			m_angle ;

@endcode
4. Création des objets par surcharge de GLamWidget::createObjects()

@code
    void MyGLamWidget::createObjects()
    {
        m_axis = new Axis3D ;

        m_cube = new GLamCube(5) ;
        m_cube->defaultMatrix().translate(4, 4, 3 ) ;
        m_angle = 0 ;
    }

@endcode
5. Projection des objets par surcharge de GLamWidget::drawObjects()

@code
    void MyGLamWidget::drawObjects()
    {
        m_axis->draw() ;
        m_cube->transformMatrix().setToIdentity() ;
        m_cube->transformMatrix().rotate(m_angle, 1, 0, 0 ) ;
        m_cube->draw() ;
    }

@endcode
6. Animation de objets par surcharge de GLamWidget::animateObjects()

@code
    void MyGLamWidget::animateObjects()				// périodicité 20 ms
    {
        static int i = 0 ;
        m_angle = 6 * ( i++ * 0.02 ) ;				// 1 tr/min = 6°/s
        while ( m_angle > 360 )	m_angle -= 360 ;	// modulo 360
    }
@endcode
</td></tr>
</table>

@n
*/
