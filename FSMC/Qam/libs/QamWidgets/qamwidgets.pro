#	---------------------------------------------------------------------------
#   filename    :   qamwidgets.pro
#   description :   Qt project file
#  
#  	project     :	QamWidgets library
#   start date  :   jan 2004
#   ---------------------------------------------------------------------------
#   Copyright 2004-2016 by Alain Menu   <alain.menu@ac-creteil.fr>
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

TEMPLATE		 = lib
TARGET			 = QamWidgets
!win32:VERSION	 = 4.7

CCONFIG		-= debug debug_and_release debug_and_release_target
CONFIG		+= qt warn_on release

DESTDIR		 = release
MOC_DIR		 = .moc
OBJECTS_DIR	 = .obj

# source files
# -----------------------------------------------------------------------------

CPPHEADERS	 = 	QamWidget \
				QamAbstractDigital \
				QamLed \
				QamBargraph \
				QamSegmentDisplay \
				QamDigitalMeter \
				QamDigitalGauge \
				QamChronoMeter \
				QamAbstractAnalog \
				QamGauge \
				QamAnalogMeter \
				QamPotentioMeter

HEADERS		+= 	qamwidget.h \
				qamabstractdigital.h \
				qamled.h \
				qambargraph.h \
				qamsegmentdisplay.h \
				qamdigitalmeter.h \
				qamdigitalgauge.h \
				qamchronometer.h \
				qamabstractanalog.h \
				qamgauge.h \
				qamanalogmeter.h \
				qampotentiometer.h

SOURCES		+= 	qamabstractdigital.cpp \
				qamled.cpp \
				qambargraph.cpp \
				qamsegmentdisplay.cpp \
				qamdigitalmeter.cpp \
				qamdigitalgauge.cpp \
				qamchronometer.cpp \
				qamabstractanalog.cpp \
				qamgauge.cpp \
				qamanalogmeter.cpp \
				qampotentiometer.cpp

# MacOSX Framework
# -----------------------------------------------------------------------------

macx:!dylib {
	CONFIG					 += lib_bundle
	FRAMEWORK_HEADERS.version = Versions
	FRAMEWORK_HEADERS.files   = $${CPPHEADERS} $${HEADERS}
	FRAMEWORK_HEADERS.path    = Headers
	QMAKE_BUNDLE_DATA        += FRAMEWORK_HEADERS
}	 

# library installation
# -----------------------------------------------------------------------------

include(../libinstall.pri)
