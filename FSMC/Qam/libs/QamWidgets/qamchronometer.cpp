/*  ---------------------------------------------------------------------------
 *  filename    :   qamchronometer.cpp
 *  description :   IMPLEMENTATION de la classe QamChronoMeter
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

#include "qamchronometer.h"

QamChronoMeter::QamChronoMeter(QWidget* parent )
	: QamDigitalMeter(parent )
	, m_it( NULL )
	, m_period( 1000 )
	, m_chronoType( QamWidget::Clock )
	, m_chronoSize( QamWidget::Second )
	, m_pause( false )
	, m_inc( true )
{
	m_it = new QTimer( this ) ;
	connect( m_it, SIGNAL( timeout() ), SLOT( timeout() ) ) ;

	setChronoType( QamWidget::Clock ) ;
	setChronoSize( QamWidget::Second ) ;
}

QamChronoMeter::~QamChronoMeter()
{
	m_it->stop() ;
	delete m_it ;
}

QamWidget::ChronoType QamChronoMeter::chronoType() const
{
	return m_chronoType ;
}

void QamChronoMeter::setChronoType( QamWidget::ChronoType chronoType )
{
	if (	( chronoType == QamWidget::Clock )
		  &&( chronoSize() == QamWidget::Hundredth ) ) {
		setChronoSize( QamWidget::Second ) ;
	}
	m_chronoType = chronoType ;

	if ( m_chronoType == QamWidget::Clock )  {		// mode Clock
		m_value = QTime::currentTime() ;
		m_it->start( m_period ) ;
	}
	else {											// mode Timer
		m_value = QTime(0, 0 ) ;
		m_it->stop() ;
	}
}

QamWidget::ChronoSize QamChronoMeter::chronoSize() const
{
	return m_chronoSize ;
}

void QamChronoMeter::setChronoSize( QamWidget::ChronoSize chronoSize )
{
	if (	( chronoSize == QamWidget::Hundredth )
		  &&( chronoType() == QamWidget::Clock ) ) {
		chronoSize = QamWidget::Second ;
	}
	m_chronoSize = chronoSize ;

	switch( m_chronoSize ) {
		case QamWidget::Second :	
			QamDigitalMeter::setSize(6) ; m_period = 1000 ;	break ;
		case QamWidget::Hundredth :	
			QamDigitalMeter::setSize(8) ; m_period = 10 ;	break ;
		default :					
			QamDigitalMeter::setSize(4) ; m_period = 60000 ;
	}
	for (int i = 0 ; i < this->size() ; ++i ) {
		m_digits[i].setDecimalPoint(false ) ;
		m_digits[i].setValue(0) ;
	}
	m_digits[1].setColon() ;
	if ( this->size() > 4 )	m_digits[3].setColon() ;
	if ( this->size() > 6 )	m_digits[5].setDecimalPoint() ;	
	
	m_it->stop() ;
	if ( chronoType() == QamWidget::Clock )	{
		timeout() ; 							// force affichage de suite...
		m_it->start( m_period ) ;
	}
}

QTime QamChronoMeter::chrono() const { return m_value ; }

QTime QamChronoMeter::alarmTime() const { return m_alarm ; }

void QamChronoMeter::setAlarmTime(const QTime& alarmTime )
{
	m_alarm = alarmTime ;
}

// mode Timer / comptage si m_value nulle, sinon décomptage

void QamChronoMeter::start()
{
	if ( m_chronoType == QamWidget::Clock )	return ;

    m_inc = ( m_value == QTime(0,0) ? true : false ) ;
    m_pause = false ;
	m_it->start( m_period ) ;
}

// mode Timer / décomptage

void QamChronoMeter::start(const QString& time )
{
	if ( m_chronoType == QamWidget::Clock )	return ;

	start( QTime::fromString(time, "hh:mm:ss" ) ) ;
}

void QamChronoMeter::start(const QTime& time )
{
	if ( m_chronoType == QamWidget::Clock )	return ;

	m_value = time ;
	m_inc = false ;
	m_pause = false ;
	m_it->start( m_period ) ;
}

bool QamChronoMeter::isRunning() const { return m_it->isActive() ; }

// mode Timer / pause

void QamChronoMeter::pause(bool on )						// slot
{
	if ( m_chronoType == QamWidget::Clock )	return ;

	if ( on )	m_it->stop() ;
	else		m_it->start( m_period ) ;

	m_pause = on ;
}

// mode Timer / stop

void QamChronoMeter::stop()									// slot
{
	if ( m_chronoType == QamWidget::Clock )	return ;
	
	m_it->stop() ;
	m_pause = false ;
}

// size = précision = 4 | 6 | 8

void QamChronoMeter::setSize(int numDigits )				// overloading
{
	if ( numDigits == 4 )	setChronoSize(QamWidget::Minute ) ;
	if ( numDigits == 6 )	setChronoSize(QamWidget::Second ) ;
	if ( numDigits == 8 )	setChronoSize(QamWidget::Hundredth ) ;
}

int	QamChronoMeter::precision() const						// overloading
{
	return size() ;
}

void QamChronoMeter::setPrecision(int precision )			// overloading
{
	setSize(precision ) ;
}

// retourne valeur du chrono en secondes avec 3 décimales (ms)

double QamChronoMeter::value() const						// overloading
{
	double res = m_value.hour() * 3600.0 ;
	res += m_value.minute() * 60.0 ;
	res += m_value.second() ;
	res += m_value.msec() / 1000.0 ;
	return res ;
}

void QamChronoMeter::setValue(double value )				// overloading
{
	if ( value < 0.0 )	value = 0.0 ;
	int ms = (int)( ( value - (int)value ) * 1000 ) ;
	int hh = (int)value / 3600 ;
	value = (int)value % 3600 ;
	int mm = (int)value / 60 ;
	value = (int)value % 60 ;
	int ss = (int)value ;
	m_value.setHMS(hh, mm, ss, ms ) ;
	refresh() ;
}

// mise à jour de l'affichage

void QamChronoMeter::refresh()								// slot
{
	m_digits[0].setValue( m_value.hour() / 10 ) ;
	m_digits[1].setValue( m_value.hour() % 10 ) ;
	m_digits[2].setValue( m_value.minute() / 10 ) ;
	m_digits[3].setValue( m_value.minute() % 10 ) ;
	if ( size() > 4 ) {
		m_digits[4].setValue( m_value.second() / 10 ) ;
		m_digits[5].setValue( m_value.second() % 10 ) ;
	}
	if ( size() > 6 ) {
		m_digits[6].setValue( m_value.msec() / 100 ) ;
		m_digits[7].setValue( ( m_value.msec() / 10 ) % 10 ) ;
	}
	update() ;
}

void QamChronoMeter::timeout()								// slot
{
	if ( m_chronoType == QamWidget::Clock )  {
		m_value = QTime::currentTime() ;
	}
	else {
		if ( m_inc )	m_value = m_value.addMSecs( m_period ) ;
		else			m_value = m_value.addMSecs( -m_period ) ;
	}	

	if ( m_value.isNull() )			emit ended() ;
	else {
		if ( m_value == m_alarm )	emit alarm() ;
	}
	refresh() ;
}

// pause/reprise par clic

void QamChronoMeter::mouseReleaseEvent(QMouseEvent* /*pe*/ )
{
	if ( m_chronoType == QamWidget::Clock )	return ;
	pause( m_pause ? false : true ) ;
}

