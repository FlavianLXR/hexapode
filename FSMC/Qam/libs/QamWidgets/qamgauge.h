/*  ---------------------------------------------------------------------------
 *  filename    :   qamgauge.h
 *  description :   INTERFACE de la classe QamGauge
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

#ifndef QAMGAUGE_H
#define QAMGAUGE_H

#include "qamabstractanalog.h"

#ifdef QAM_WIDGET_PLUGIN
class QDESIGNER_WIDGET_EXPORT QamGauge : public QamAbstractAnalog
#else
class QamGauge : public QamAbstractAnalog
#endif
{
    Q_OBJECT
    Q_PROPERTY( bool valueVisible READ valueVisible WRITE setValueVisible )
	Q_PROPERTY( int needleThickness READ needleThickness WRITE setNeedleThickness )
	Q_PROPERTY( QColor needleColor READ needleColor WRITE setNeedleColor )

  public :
	QamGauge( QWidget* parent = 0 ) ;
	virtual ~QamGauge() ;

	bool	valueVisible() const ;
	void	setValueVisible(bool visible ) ;
	
	int		needleThickness() const ;
	QColor	needleColor() const ;
	
	void	setNeedleThickness(int needleThickness ) ;
	void	setNeedleColor(const QColor& color ) ;

  protected :
	void drawBackground(QPainter& painter ) ;
	void drawForeground(QPainter& painter ) ;

  private :
	bool	m_valueVisible ;
	int		m_needleThickness ;
	QColor	m_needleColor ;
} ;

#endif

