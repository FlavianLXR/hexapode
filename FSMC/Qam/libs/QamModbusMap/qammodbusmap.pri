#  ---------------------------------------------------------------------------
#   filename    :   qammodbusmap.pri
#   description :   Qt project file
#
#  	project     :	Qam Modbus over TCP/IP
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

QT	+= core gui widgets network

VERSION = $$system(cat $$system(pwd)/_VERSION | cut -f 2 -d \'\"\' )
message(Using QamModbusMap version $${VERSION}) 

INCLUDEPATH += $$PWD

CPPHEADERS	+= $$PWD/QamModbusMap

HEADERS	+= \
	$$PWD/qammodbusdata.h \
	$$PWD/qammodbusmap.h \
	$$PWD/qammodbusmapviewer.h

SOURCES += \
	$$PWD/qammodbusdata.cpp \
	$$PWD/qammodbusmap.cpp \
	$$PWD/qammodbusmapviewer.cpp

DISTFILES += \
	$${CPPHEADERS} \
	$$system(pwd)/_ABOUT $$system(pwd)/_CHANGES $$system(pwd)/_VERSION
