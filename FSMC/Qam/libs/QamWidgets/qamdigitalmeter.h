/*  ---------------------------------------------------------------------------
 *  filename    :   qamdigitalmeter.h
 *  description :   INTERFACE de la classe QamDigitalMeter
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

#ifndef QAMDIGITALMETER_H
#define QAMDIGITALMETER_H

#include "qamabstractdigital.h"
#include "qamsegmentdisplay.h"
#include <QHBoxLayout>

#ifdef QAM_WIDGET_PLUGIN
class QDESIGNER_WIDGET_EXPORT QamDigitalMeter : public QamAbstractDigital
#else
class QamDigitalMeter : public QamAbstractDigital
#endif
{
	Q_OBJECT
	Q_PROPERTY(int size READ size WRITE setSize )
	Q_PROPERTY(int precision READ precision WRITE setPrecision )
	Q_PROPERTY(double value READ value WRITE setValue )
	Q_PROPERTY(bool bold READ isBold WRITE setBold )

  public :
  	QamDigitalMeter(QWidget* parent = 0 ) ;
	virtual ~QamDigitalMeter() ;
  	
	virtual int		size() const ;
	virtual void 	setSize(int numDigits ) ;
	
	virtual double	value() const ;

	virtual int		precision() const ;
	virtual void	setPrecision(int precision ) ;

	bool	isBold() const ;
	void	setBold(bool bold = true ) ;

	QamWidget::State 	state() const ;
	void 	setState(QamWidget::State state ) ;
	bool 	isOn() const ;
	
	QamWidget::Blink 	blink() const ;
	void 	setBlink(QamWidget::Blink blink ) ;
	void	setFlash(QamWidget::State state, int ms = 500 ) ;

	bool 	isShining() const ;
	void 	setShining(bool shining = true ) ;

	QColor	customColor() const ;
	void 	setCustomColor(const QColor& color ) ;

	int		alphaOff() const ;
	void	setAlphaOff(int alphaOff ) ;
	
    void 	setCustomDisplay(int digit, int value ) ;
    void 	setCustomDisplay(int digit, const QString& segments = "" ) ;
    
    bool	leadingZeros() const ;
    void	setLeadingZeros(bool leadingZeros = true ) ;
	
  public slots:
	void 	off() ;
	void 	on() ;
	void	toggle() ;
	void	setValue(int value ) ;
	virtual void 	setValue(double value ) ;

  protected:
  	void drawBackground(QPainter& painter ) ;
  	void drawForeground(QPainter& painter ) ;

  private :
  	int					m_size ;
  	int					m_precision ;
  	double				m_value ;
  	QHBoxLayout*		m_layout ;
  	QamSegmentDisplay	m_sd ;			// mémo état afficheurs
  	bool				m_leadingZeros ;

  protected :
  	QamSegmentDisplay*	m_digits ;
} ;

#endif

