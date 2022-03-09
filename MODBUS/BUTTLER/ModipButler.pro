#  ---------------------------------------------------------------------------
#   filename    :   ModipButler.pro
#   description :   Qt project file
#  
#  	project     :	ModipButler: Outil de test haut niveau Esclave Modbus/IP
#   start date  :   janvier 2014
#   ---------------------------------------------------------------------------
#   Copyright 2014-2016 by Alain Menu   <alain.menu@ac-creteil.fr>
#  
#   This file is part of "Qam Modbus over IP Project"
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

QT       += core gui widgets network

# Mac OS X : pour cout, cerr ...
macx {
	QMAKE_CFLAGS_X86_64 += -mmacosx-version-min=10.7
	QMAKE_CXXFLAGS_X86_64 = $$QMAKE_CFLAGS_X86_64
}

unix:!macx {
	QMAKE_LFLAGS += '-Wl,-rpath,\'\$$ORIGIN/libs\''
}

# application icon

macx:ICON = ../../../_am/icons/qam47.icns

# QamSockets / QamModbusMap libraries

include(../../libs/QamSockets/qamsockets.pri)
include(../../libs/QamModbusMap/qammodbusmap.pri)

# ModipButler files

INCLUDEPATH += .

HEADERS   += dialog.h

SOURCES   += main.cpp \
			 dialog.cpp

FORMS     += dialog.ui

RESOURCES += modipbutler.qrc

DISTFILES  += _ABOUT \
			 _CHANGES \
			 _VERSION
			
#   ---------------------------------------------------------------------------

UI_DIR		= .uic
RCC_DIR		= .uic
MOC_DIR		= .moc
OBJECTS_DIR	= .obj

CONFIG	 -= debug debug_and_release debug_and_release_target
CONFIG   += qt warn_on release

TEMPLATE = app
DESTDIR	 = release
TARGET 	 = ModipButler
