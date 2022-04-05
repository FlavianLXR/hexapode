#  ---------------------------------------------------------------------------
#   filename    :   qamsockets.pri
#   description :   Qt project file
#
#  	project     :	QamSockets Library
#   start date  :   février 2006
#   ---------------------------------------------------------------------------
#   Copyright 2006-2016 by Alain Menu   <alain.menu@ac-creteil.fr>
#
#   This file is part of "QamSockets Library"
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

QT	+= core gui network

VERSION = $$system(cat $$system(pwd)/_VERSION | cut -f 2 -d \'\"\' )
message(Using QamSockets version $${VERSION}) 

INCLUDEPATH += $$PWD

CPPHEADERS	+= \
	$$PWD/QamTcpSocket \
	$$PWD/QamUdpSocket

HEADERS	+=	\
	$$PWD/qamabstractserver.h \
	$$PWD/qamtcpclient.h \
	$$PWD/qamtcpconnection.h \
	$$PWD/qamtcpserver.h \
	$$PWD/qamudpsocket.h
				
SOURCES	+=	\
	$$PWD/qamabstractserver.cpp \
	$$PWD/qamtcpclient.cpp \
	$$PWD/qamtcpconnection.cpp \
	$$PWD/qamtcpserver.cpp \
	$$PWD/qamudpsocket.cpp

DISTFILES += \
	$${CPPHEADERS} \
	$$system(pwd)/_ABOUT $$system(pwd)/_CHANGES $$system(pwd)/_VERSION
