/*  ---------------------------------------------------------------------------
 *  filename    :   qambargraph.h
 *  description :   INTERFACE de la classe QamBargraph
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

#ifndef QAMBARGRAPH_H
#define QAMBARGRAPH_H

#include "qamabstractdigital.h"
#include "qamled.h"
#include <QBoxLayout>

#ifdef QAM_WIDGET_PLUGIN
class QDESIGNER_WIDGET_EXPORT QamBargraph : public QamAbstractDigital
#else
class QamBargraph : public QamAbstractDigital
#endif
{
	Q_OBJECT
	Q_PROPERTY( Orientation orientation READ orientation WRITE setOrientation )
	Q_PROPERTY(int size READ size WRITE setSize )
	Q_PROPERTY(int value READ value WRITE setValue )
	
  public :
  	QamBargraph(QWidget* parent = 0 ) ;
	virtual ~QamBargraph() ;
  	
	int		size() const ;
	virtual void 	setSize(int numLeds ) ;
	
	int		value() const ;

	QamWidget::Orientation orientation() const ;
	void setOrientation(QamWidget::Orientation orientation ) ;

	QamWidget::State 	state() const ;
	void 	setState(QamWidget::State state ) ;
	bool 	isOn() const ;
	
	QamWidget::Blink 	blink() const ;
	void 	setBlink(QamWidget::Blink blink ) ;
	void	setFlash(QamWidget::State state, int ms = 500 ) ;

	bool 	isShining() const ;
	void 	setShining(bool shining = true ) ;

	virtual QColor	customColor() const ;
	virtual void 	setCustomColor(const QColor& color ) ;

	int		alphaOff() const ;
	void	setAlphaOff(int alphaOff ) ;
	
  public slots:
	void 	off() ;
	void 	on() ;
	void	toggle() ;
	void 	setValue(int value ) ;

  protected :
	virtual bool isBlinkDesignable() const { return false ; }

	void drawBackground(QPainter& painter ) ;
	void drawForeground(QPainter& painter ) ;
	
  private :
	QamWidget::Orientation m_orientation ;
  	int				m_size ;
  	int				m_value ;
  	QBoxLayout*		m_layout ;
  	QamLed			m_led ;			// mémo état
  	
  protected :
  	QamLed*			m_leds ;
} ;

#endif

