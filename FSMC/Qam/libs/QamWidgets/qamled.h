/*  ---------------------------------------------------------------------------
 *  filename    :   qamled.h
 *  description :   INTERFACE de la classe QamLed
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

#ifndef QAMLED_H
#define QAMLED_H

#include "qamabstractdigital.h"

#ifdef QAM_WIDGET_PLUGIN
class QDESIGNER_WIDGET_EXPORT QamLed : public QamAbstractDigital
#else
class QamLed : public QamAbstractDigital
#endif
{
    Q_OBJECT
	Q_PROPERTY( Orientation orientation READ orientation WRITE setOrientation )
	Q_PROPERTY( QString text READ text WRITE setText )
	Q_PROPERTY( Shape shape READ shape WRITE setShape )
	Q_PROPERTY( int size READ size WRITE setSize )
	Q_PROPERTY( int line READ line WRITE setLine )
	Q_PROPERTY( bool autoToggle READ isAutoToggle WRITE setAutoToggle )

  public:
	QamLed( QWidget* parent = 0, const QString& label = ""  ) ;
	virtual ~QamLed() ;

	Orientation orientation() const ;
	void setOrientation(Orientation orientation ) ;

	QString text() const ;
	void setText(const QString& text ) ;

	QamWidget::Shape shape() const ;
	void setShape(QamWidget::Shape shape ) ;

	int size() const ;
	void setSize(int size ) ;

	int line() const ;
	void setLine(int thickness ) ;

	bool isAutoToggle() const ;
	void setAutoToggle(bool autoToggle = true ) ;

  signals:
	void clicked() ;

  protected:
  	void drawBackground(QPainter& painter ) ;
	void drawForeground(QPainter& painter ) ;
	void mousePressEvent( QMouseEvent* pe ) ;
	void mouseReleaseEvent( QMouseEvent* pe ) ;

  private:
	QamWidget::Orientation	m_orientation ;
	QamWidget::Shape		m_shape ;
	int			m_size ;
	int			m_line ;
	QString 	m_text ;
	bool		m_autoToggle ;
	QRect		m_rled ;
	bool		m_click ;
} ;

#endif

