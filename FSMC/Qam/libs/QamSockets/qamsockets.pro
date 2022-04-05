#  ---------------------------------------------------------------------------
#   filename    :   qamsockets.pro
#   description :   Qt project file
#  
#  	project     :	QamSerial : librairie Qt sockets UDP/TCP
#   start date  :   fév 2006
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

QT				+= network
TEMPLATE		 = lib
TARGET			 = QamSockets
!win32:VERSION	 = 3.2

CCONFIG		-= debug debug_and_release debug_and_release_target
CONFIG		+= qt warn_on release

DESTDIR		 = release
MOC_DIR		 = .moc
OBJECTS_DIR	 = .obj

# source files
# -----------------------------------------------------------------------------

CPPHEADERS	 = 	QamTcpSocket \
				QamUdpSocket

HEADERS		+=	qamabstractserver.h \
				qamtcpclient.h \
				qamtcpconnection.h \
				qamtcpserver.h \
				qamudpsocket.h
				
SOURCES		+=	qamabstractserver.cpp \
				qamtcpclient.cpp \
				qamtcpconnection.cpp \
				qamtcpserver.cpp \
				qamudpsocket.cpp

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
