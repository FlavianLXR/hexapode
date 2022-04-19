QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

include(../../libs/QamSockets/qamsockets.pri)
include(../../libs/QamModbusMap/qammodbusmap.pri)

SOURCES += \
    main.cpp \
    modipslave.cpp \
    #qamabstractserver.cpp \
    #qammodbusdata.cpp \
    #qammodbusmap.cpp \
    #qamtcpconnection.cpp \
    #qamtcpserver.cpp \
    hexapodmgi.cpp\
    qam6dof.cpp \
    trdatagram.cpp

HEADERS += \
    modipslave.h \
    #qamabstractserver.h \
    #qammodbusdata.h \
    #qammodbusmap.h \
    #qamtcpconnection.h \
    #qamtcpserver.h \
    hexapodmgi.h\
    qam6dof.h \
    trdatagram.h

FORMS += \
    trdatagram.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    csv.qrc \
    hexa.png

DISTFILES += \
    diagramme_de_classe.qmodel

UI_DIR		= .uic
RCC_DIR		= .uic
MOC_DIR		= .moc
OBJECTS_DIR	= .obj

DESTDIR = release

