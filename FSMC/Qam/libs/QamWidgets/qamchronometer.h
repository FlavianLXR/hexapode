/*  ---------------------------------------------------------------------------
 *  filename    :   qamchronometer.h
 *  description :   INTERFACE de la classe QamChronoMeter
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

#ifndef QAMCHRONOMETER_H
#define QAMCHRONOMETER_H

#include "qamdigitalmeter.h"
#include <QTimer>
#include <QTime>

#ifdef QAM_WIDGET_PLUGIN
class QDESIGNER_WIDGET_EXPORT QamChronoMeter : public QamDigitalMeter
#else
class QamChronoMeter : public QamDigitalMeter
#endif
{
	Q_OBJECT
	Q_PROPERTY(ChronoType chronoType READ chronoType WRITE setChronoType )
	Q_PROPERTY(ChronoSize chronoSize READ chronoSize WRITE setChronoSize )
	Q_PROPERTY(QTime alarmTime READ alarmTime WRITE setAlarmTime )
	
  public :
	QamChronoMeter(QWidget* parent = 0 ) ;
	virtual ~QamChronoMeter() ;
	
	// properties
	
	QamWidget::ChronoType chronoType() const ;
	void 	setChronoType( QamWidget::ChronoType chronoType ) ;

	QamWidget::ChronoSize chronoSize() const ;
	void 	setChronoSize( QamWidget::ChronoSize chronoSize ) ;

	QTime	chrono() const ;
	
	QTime 	alarmTime() const ;
	void 	setAlarmTime(const QTime& alarmTime ) ;

	void 	setSize(int numDigits ) ;				// overloading
	double	value() const ;							// overloading
	int		precision() const ;						// overloading
	void	setPrecision(int precision ) ;			// overloading
	
	// timer control
	
	void 	start() ;
	void 	start(const QString& time ) ;
	void 	start(const QTime& time ) ;
	bool 	isRunning() const ;
	
  public slots :
	void 	pause(bool on = true ) ;
	void 	stop() ;
	void 	setValue(double value ) ;				// overloading
	
  signals:
	void 	ended() ;			// timer or clock = 00:00:00.000
	void 	alarm() ;			// timer or clock = alarm time

  protected:
	void 	mouseReleaseEvent(QMouseEvent* pe ) ;
	
  private slots:
  	void	refresh() ;
	void 	timeout() ;

  private :
  	QTimer*	m_it ;
  	int		m_period ;
  	QTime	m_value ;
  	QamWidget::ChronoType	m_chronoType ;
  	QamWidget::ChronoSize	m_chronoSize ;
  	bool	m_pause ;
  	bool	m_inc ;
  	QTime	m_alarm ;
} ;

#endif

