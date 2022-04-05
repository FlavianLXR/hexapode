/*  ---------------------------------------------------------------------------
 *  filename    :   qamwidget.h
 *  description :   INTERFACE de la classe QamWidget
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

#ifndef QAMWIDGET_H
#define QAMWIDGET_H

#include <QWidget>

#ifdef QAM_WIDGET_PLUGIN
#include <QtUiPlugin/QDesignerExportWidget>
class QDESIGNER_WIDGET_EXPORT QamWidget : public QWidget
#else
class QamWidget : public QWidget
#endif
{
	Q_OBJECT
	Q_ENUMS( Orientation State Shape Blink ChronoType ChronoSize )
	
  public :
  	QamWidget(QWidget* parent = 0 ) : QWidget( parent ) {}
  	
	enum Orientation { Left, Top, Right, Bottom } ;
 	enum State { Off, Green, Yellow, Red, Custom } ;
	enum Shape { Circular, Flat, Square, Plain } ;
	enum Blink { Fixed, Slow, Fast } ;
	enum ChronoType { Clock, Timer } ;
	enum ChronoSize { Minute, Second, Hundredth } ;
} ;

#endif
