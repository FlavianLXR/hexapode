/*  ---------------------------------------------------------------------------
 *  filename    :   qamabstractdigital.h
 *  description :   INTERFACE de la classe QamAbstractDigital
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

#ifndef QAMABSTRACTDIGITAL_H
#define QAMABSTRACTDIGITAL_H

#include "qamwidget.h"
#include <QTimer>
#include <QPixmap>
#include <QPainter>

#ifdef QAM_WIDGET_PLUGIN
class QDESIGNER_WIDGET_EXPORT QamAbstractDigital : public QamWidget
#else
class QamAbstractDigital : public QamWidget
#endif
{
	Q_OBJECT
	Q_PROPERTY( State state READ state WRITE setState )
//	Q_PROPERTY( Blink blink READ blink WRITE setBlink DESIGNABLE isBlinkDesignable )
    Q_PROPERTY( Blink blink READ blink WRITE setBlink DESIGNABLE true )
    Q_PROPERTY( bool shining READ isShining WRITE setShining )
//	Q_PROPERTY( QColor customColor READ customColor WRITE setCustomColor DESIGNABLE isCustomColorDesignable )
    Q_PROPERTY( QColor customColor READ customColor WRITE setCustomColor DESIGNABLE true )
    Q_PROPERTY( int alphaOff READ alphaOff WRITE setAlphaOff )
	
  public :
	QamAbstractDigital(QWidget* parent = 0 ) ;
	virtual ~QamAbstractDigital() ;

	virtual QamWidget::State 	state() const ;
	virtual void 	setState(QamWidget::State state ) ;
	virtual bool 	isOn() const ;

	virtual QamWidget::Blink 	blink() const ;
	virtual void 	setBlink(QamWidget::Blink blink ) ;
	virtual void	setFlash(QamWidget::State state, int ms = 500 ) ;

	virtual bool 	isShining() const ;
	virtual void 	setShining(bool shining = true ) ;
	
	virtual QColor	customColor() const ;
	virtual void 	setCustomColor(const QColor& color ) ;
	
	virtual int		alphaOff() const ;
	virtual void	setAlphaOff(int alphaOff ) ;

	QColor			onColor() const ;
	QColor			offColor() const ;

	void	draw(QPainter* painter ) ;	// new v5.0

  public slots:
	virtual void 	off() ;
	virtual void 	on() ;
	virtual void	toggle() ;

  signals:
	void 	flashEnded() ;

  private slots:
	void timeout() ;

  protected :
	virtual bool isBlinkDesignable() const { return true ; }
	virtual bool isCustomColorDesignable() const { return true ; }

  	virtual void drawBackground(QPainter& painter ) = 0 ;
	virtual void drawForeground(QPainter& painter ) = 0 ;	// new 5.0

	void	updateWithBackground() ;
	void 	showBackground() ;		// dessin du fond

	void paintEvent(QPaintEvent* pe ) ;

  private :
  	bool		m_redraw ;			// indique si le fond doit être redessiné
	QPixmap*	m_background ;		// image de fond
	
  private:
	QamWidget::State	m_state ;
	QamWidget::State	m_oldState ;
	QamWidget::Blink	m_blink ;
	QColor				m_customColor ;
	int					m_alphaOff ;
	bool				m_shining ;
	QTimer*				m_it ;
	bool				m_flash ;	
} ;

#endif
