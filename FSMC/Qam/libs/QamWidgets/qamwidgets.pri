#  ---------------------------------------------------------------------------
#   filename    :   qamwidgets.pri
#   description :   Qt project file
#
#  	project     :	QamWidgets collection
#   start date  :   janvier 2004
#   ---------------------------------------------------------------------------
#   Copyright 2004-2017 by Alain Menu   <alain.menu@ac-creteil.fr>
#
#   This file is part of "QamWidgets Library"
#
#   This program is free software ;  you can  redistribute it and/or  modify it
#   under the terms of the  GNU General Public License as published by the Free
#   Software Foundation ; either version 3 of the License, or  (at your option)
#   any later version.
#
#   This program is distributed in the hope that it will be useful, but WITHOUT
#   ANY WARRANTY ; without even the  implied  warranty  of  MERCHANTABILITY  or
#   FITNESS FOR  A PARTICULAR PURPOSE. See the  GNU General Public License  for
#   more details.
#
#  	You should have  received  a copy of the  GNU General Public License  along
#  	with this program. If not, see <http://www.gnu.org/licenses/>.
#   ---------------------------------------------------------------------------

QT			+= core widgets

VERSION = $$system(cat $$system(pwd)/_VERSION | cut -f 2 -d \'\"\' )
message(Using QamWidgets version $${VERSION}) 

INCLUDEPATH += $$PWD

CPPHEADERS	+= \
	$$PWD/QamWidget \
	$$PWD/QamAbstractDigital \
	$$PWD/QamLed \
	$$PWD/QamSegmentDisplay \
	$$PWD/QamBargraph \
	$$PWD/QamDigitalGauge \
	$$PWD/QamDigitalMeter \
	$$PWD/QamChronoMeter \
	$$PWD/QamAbstractAnalog \
	$$PWD/QamGauge \
	$$PWD/QamAnalogMeter \
	$$PWD/QamPotentioMeter

HEADERS	+= \
	$$PWD/qamwidget.h \
	$$PWD/qamabstractdigital.h \
	$$PWD/qamled.h \
	$$PWD/qambargraph.h \
	$$PWD/qamsegmentdisplay.h \
	$$PWD/qamdigitalmeter.h \
	$$PWD/qamdigitalgauge.h \
	$$PWD/qamchronometer.h \
	$$PWD/qamabstractanalog.h \
	$$PWD/qamgauge.h \
	$$PWD/qamanalogmeter.h \
	$$PWD/qampotentiometer.h

SOURCES += \
	$$PWD/qamabstractdigital.cpp \
	$$PWD/qamled.cpp \
	$$PWD/qambargraph.cpp \
	$$PWD/qamsegmentdisplay.cpp \
	$$PWD/qamdigitalmeter.cpp \
	$$PWD/qamdigitalgauge.cpp \
	$$PWD/qamchronometer.cpp \
	$$PWD/qamabstractanalog.cpp \
	$$PWD/qamgauge.cpp \
	$$PWD/qamanalogmeter.cpp \
	$$PWD/qampotentiometer.cpp

DISTFILES += \
	$${CPPHEADERS} \
	$$system(pwd)/_ABOUT $$system(pwd)/_CHANGES $$system(pwd)/_VERSION
