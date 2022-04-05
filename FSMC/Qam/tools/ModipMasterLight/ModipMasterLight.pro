QT       += core network
QT       -= gui

CONFIG   += console

macx:CONFIG -= app_bundle

MOC_DIR		 = .moc
OBJECTS_DIR  = .obj

QAMSOCKETS	 = ../../libs/QamSockets
QAMMODBUSMAP = ../../libs/QamModbusMap

INCLUDEPATH +=	. $${QAMSOCKETS} $${QAMMODBUSMAP}

SOURCES  += main.cpp \
			modipmaster.cpp \
			$${QAMSOCKETS}/qamtcpclient.cpp \
			$${QAMSOCKETS}/qamabstractserver.cpp \
			$${QAMMODBUSMAP}/qammodbusmap.cpp \
			$${QAMMODBUSMAP}/qammodbusdata.cpp

HEADERS  += modipmaster.h \
			$${QAMSOCKETS}/qamtcpclient.h \
			$${QAMSOCKETS}/qamabstractserver.h \
			$${QAMMODBUSMAP}/qammodbusmap.h \
			$${QAMMODBUSMAP}/qammodbusdata.h
			
TEMPLATE  = app
DESTDIR	  =	release
TARGET	  = modipmasterlight
