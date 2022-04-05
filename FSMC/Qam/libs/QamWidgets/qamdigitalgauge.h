/*  ---------------------------------------------------------------------------
 *  filename    :   qamdigitalgauge.h
 *  description :   INTERFACE de la classe QamDigitalGauge
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

#ifndef QAMDIGITALGAUGE_H
#define QAMDIGITALGAUGE_H

#include "qambargraph.h"

#ifdef QAM_WIDGET_PLUGIN
class QDESIGNER_WIDGET_EXPORT QamDigitalGauge : public QamBargraph
#else
class QamDigitalGauge : public QamBargraph
#endif
{
	Q_OBJECT
	Q_PROPERTY( int lowThreshold READ lowThreshold WRITE setLowThreshold )
	Q_PROPERTY( int highThreshold READ highThreshold WRITE setHighThreshold )
	Q_PROPERTY( QColor lowColor READ lowColor WRITE setLowColor )
	Q_PROPERTY( QColor normalColor READ normalColor WRITE setNormalColor )
	Q_PROPERTY( QColor highColor READ highColor WRITE setHighColor )
	
  public :
  	QamDigitalGauge(QWidget* parent = 0 ) ;
	virtual ~QamDigitalGauge() ;
  	
	int		lowThreshold() const ;
	int		highThreshold() const ;
	void	setLowThreshold(int low ) ;
	void	setHighThreshold(int high ) ;

	bool	isLowLevel() const ;
	bool	isHighLevel() const ;
	bool	isNormalLevel() const ;

	QColor	lowColor() const ;
	QColor	normalColor() const ;
	QColor	highColor() const ;
	void	setLowColor(const QColor& color ) ;
	void	setNormalColor(const QColor& color ) ;
	void	setHighColor(const QColor& color ) ;

	void 	setSize(int numLeds ) ;							// reimp
	QColor	customColor() const { return QColor() ; }		// reimp
	void 	setCustomColor(const QColor& /*color*/ ) {}		// reimp

  signals:
	void 	lowLevel() ;
	void 	normalLevel() ;
	void 	highLevel() ;

  protected :
	virtual bool isCustomColorDesignable() const { return false ; }

  private :
	void setColors() ;

  private :
	int		m_lowThreshold ;
	int		m_highThreshold ;
	QColor	m_lowColor ;
	QColor	m_normalColor ;
	QColor	m_highColor ;
} ;

#endif

