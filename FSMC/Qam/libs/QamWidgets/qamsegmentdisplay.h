/*  ---------------------------------------------------------------------------
 *  filename    :   qamsegmentdisplay.h
 *  description :   INTERFACE de la classe QamSegmentDisplay
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

#ifndef QAMSEGMENTDISPLAY_H
#define QAMSEGMENTDISPLAY_H

#include "qamabstractdigital.h"
#include <QWheelEvent>

#ifdef QAM_WIDGET_PLUGIN
class QDESIGNER_WIDGET_EXPORT QamSegmentDisplay : public QamAbstractDigital
#else
class QamSegmentDisplay : public QamAbstractDigital
#endif
{
	Q_OBJECT
	Q_PROPERTY(int value READ value WRITE setValue )
	Q_PROPERTY(bool decimalPoint READ asDecimalPoint WRITE setDecimalPoint )
	Q_PROPERTY(bool colon READ asColon WRITE setColon )
	Q_PROPERTY(bool minus READ isMinus WRITE setMinus )
	Q_PROPERTY(bool bold READ isBold WRITE setBold )
	Q_PROPERTY(bool wheelControl READ asWheelControl WRITE setWheelControl )

  public :
  	QamSegmentDisplay( QWidget* parent = 0 ) ;

	int		value() const ;

	bool	asDecimalPoint() const ;
	void	setDecimalPoint(bool dp = true ) ;
	
	bool	isMinus() const ;
	void	setMinus(bool minus = true ) ;
	
	bool	asColon() const ;
	void	setColon(bool colon = true ) ;

	bool	isBold() const ;
	void	setBold(bool bold = true ) ;

	bool	asWheelControl() const ;
	void	setWheelControl(bool wheelCtrl = true ) ;
	
	void	setCustomDisplay(bool custom, const QString& segments = "" ) ;

  public slots :
	void	setValue(int value ) ;
  	
  protected:
  	void drawBackground(QPainter& painter ) ;
  	void drawForeground(QPainter& painter ) ;
  	
	virtual void wheelEvent(QWheelEvent* we ) ;

  private :
  	int		m_value ;
  	bool	m_dp ;
  	bool	m_minus ;
	bool	m_colon ;
	bool	m_bold ;
	bool	m_wheel ;
	bool	m_custom ;
	QString	m_segms ;
} ;

#endif

