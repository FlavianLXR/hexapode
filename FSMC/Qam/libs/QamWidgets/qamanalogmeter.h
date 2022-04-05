/*  ---------------------------------------------------------------------------
 *  filename    :   qamanalogmeter.h
 *  description :   INTERFACE de la classe QamAnalogMeter
 *
 *	project     :	Qam Custom Widgets Collection
 *  start date  :   jan 2004
 *  ---------------------------------------------------------------------------
 *  Copyright 2004-2016 by Alain Menu   <alain.menu@ac-creteil.fr>
 *
 *  This file is part of "QamWidgets Library"
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

#ifndef QAMANALOGMETER_H
#define QAMANALOGMETER_H

#include "qamabstractanalog.h"

#ifdef QAM_WIDGET_PLUGIN
class QDESIGNER_WIDGET_EXPORT QamAnalogMeter : public QamAbstractAnalog
#else
class QamAnalogMeter : public QamAbstractAnalog
#endif
{
	Q_OBJECT
	Q_PROPERTY( Shape shape READ shape WRITE setShape )
    Q_PROPERTY( bool valueVisible READ valueVisible WRITE setValueVisible )
    Q_PROPERTY( int stepNumber READ stepNumber WRITE setStepNumber )
    Q_PROPERTY( int stepMark READ stepMark WRITE setStepMark )
	Q_PROPERTY( QFont stepFont READ stepFont WRITE setStepFont )
	Q_PROPERTY( QColor needleColor READ needleColor WRITE setNeedleColor )

  public :
	QamAnalogMeter( QWidget* parent = 0 ) ;
	virtual ~QamAnalogMeter() ;

	QamWidget::Shape shape() const ;
	void setShape(QamWidget::Shape shape ) ;

	bool	valueVisible() const ;
	void	setValueVisible(bool visible ) ;
	
	int		stepNumber() const ;
	void	setStepNumber(int stepNumber ) ;

	int		stepMark() const ;
	void	setStepMark(int stepMark ) ;
	
	QFont	stepFont() const ;
	void	setStepFont(const QFont& stepFont ) ;
	
	QColor	needleColor() const ;
	void	setNeedleColor(const QColor& color ) ;
	
  protected:
	virtual bool isOrientationDesignable() const { return false ; }

	void drawBackground(QPainter& painter ) ;
	void drawForeground(QPainter& painter ) ;

  private :
  	void	setDrawingArea(QPainter& painter ) ;
  	QRect	drawFrame(QPainter& painter, QRect rdraw, QBrush fillColor ) ;
	
  private :
	QamWidget::Shape	m_shape ;
	bool	m_valueVisible ;		// affichage valeur instantanée
	QColor	m_needleColor ;			// couleur de l'aiguille
	int		m_angle ;				// étendue min-max, en degrés
	int 	m_ringThickness ;		// épaisseur cadran, en % du rayon
	int		m_zoneThickness ; 		// épaisseur zones colorées, en % du rayon
	QColor	m_ringColor ;			// couleur de base du cadran

	int		m_stepNumber ;			// nombre de pas de graduation
	int		m_stepMark ;			// intervalle graduations avec valeurs
	QFont	m_stepFont ;			// police des graduations
	
	int		m_grid ;
	int		m_radius ;
} ;

#endif

