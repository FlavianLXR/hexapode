QT       += core network
QT       -= gui

CONFIG   += console

macx:CONFIG -= app_bundle

MOC_DIR		 = .moc
OBJECTS_DIR	 = .obj

QAMSOCKETS	 = ../../libs/QamSockets
QAMMODBUSMAP = ../../libs/QamModbusMap

INCLUDEPATH +=	. $${QAMSOCKETS} $${QAMMODBUSMAP}

SOURCES  += main.cpp \
			modipslave.cpp \
			$${QAMSOCKETS}/qamtcpserver.cpp \
			$${QAMSOCKETS}/qamtcpconnection.cpp \
			$${QAMSOCKETS}/qamabstractserver.cpp \
			$${QAMMODBUSMAP}/qammodbusmap.cpp \
			$${QAMMODBUSMAP}/qammodbusdata.cpp

HEADERS  += modipslave.h \
			$${QAMSOCKETS}/qamtcpserver.h \
			$${QAMSOCKETS}/qamtcpconnection.h \
			$${QAMSOCKETS}/qamabstractserver.h \
			$${QAMMODBUSMAP}/qammodbusmap.h \
			$${QAMMODBUSMAP}/qammodbusdata.h

TEMPLATE  = app
DESTDIR	  =	release
TARGET	  = modipslavelight
