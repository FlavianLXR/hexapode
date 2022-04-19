#  ---------------------------------------------------------------------------
#   filename    :   flightsimmockup.pro
#   description :   Qt project file
#  
#  	project     :	Scène 3D LLF Maquette éch. 1:1 FlightSim
#   start date  :   septembre 2017
#   ---------------------------------------------------------------------------
#   Copyright 2017 by Alain Menu   <alain.menu@ac-creteil.fr>
#  
#   This file is part of "FlightSim Mock-up"
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

QT       += core gui widgets openglwidgets

message(Using mkspecs: $${QMAKESPEC})
message(For platform: $${QMAKE_PLATFORM})
message(Output directory: $${OUT_PWD})

# Application & libraries source files
# ----------------------------------------------------------------------------

# Qam3D/GLam lib
include(../../libs/Qam3D/glam.pri)

# Qam3D/GLam examples
include(../../libs/Qam3D/examples/glamexamples.pri)

# QamLed
include(../../libs/QamWidgets/qamled.pri)

# QamSockets / QamModbusMap libraries
include(../../libs/QamSockets/qamsockets.pri)
include(../../libs/QamModbusMap/qammodbusmap.pri)

SOURCES += \
    fsmucollective.cpp \
    fsmuconsole.cpp \
    fsmuhandle.cpp \
    fsmurudder.cpp \
    fsmuseat.cpp \
	main.cpp\
    mainwindow.cpp \
	qam6dof.cpp \
    flightsimmockup.cpp \ 
    fsmuframe.cpp \
	hexapod.cpp \
    hexapodconfigurator.cpp


HEADERS  += \
    fsmucollective.h \
    fsmuconsole.h \
    fsmuhandle.h \
    fsmurudder.h \
    fsmuseat.h \
	mainwindow.h \
	qam6dof.h \
	flightsimmockup.h \
    fsmuframe.h \
	hexapod.h \
    hexapodconfigurator.h


FORMS    += mainwindow.ui

DISTFILES += _ABOUT _CHANGES _VERSION \
    _model/fsmockup.qmodel

# Application icon
# ----------------------------------------------------------------------------

macx:ICON = ./_deployment/desktop/qam47.icns

# note iOS: générer une première fois l'app
# puis copier le fichier Info.plist obtenu dans ./ios
# adapter le contenu du fichier...

ios {
	ios_icon.files = $$files($$PWD/_deployment/ios/icon/AppIcon*.png)
	QMAKE_BUNDLE_DATA += ios_icon
	ios_launchimage.files = $$files($$PWD/_deployment/ios/launchimage/LaunchImage*.png)
	QMAKE_BUNDLE_DATA += ios_launchimage
	QMAKE_INFO_PLIST = $$PWD/_deployment/ios/Info.plist
}

# note Android: générer une première fois l'Apk Android
# puis copier le fichier AndroidManifest.xml obtenu dans ./android
# ajouter la ligne android:icon="@drawable/ic_launcher"
# dans la balise <application>...
# et modifier éventuellement le nom du paquet et la version
# dans la balise <manifest>...

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/_deployment/android
OTHER_FILES += _deployment/android/AndroidManifest.xml

# Target (shadow build must be used!)
# ----------------------------------------------------------------------------

UI_DIR      = $${OUT_PWD}/.uic
RCC_DIR     = $${OUT_PWD}/.uic
MOC_DIR     = $${OUT_PWD}/.moc
OBJECTS_DIR = $${OUT_PWD}/.obj

TEMPLATE 	= app
CONFIG	   -= debug debug_and_release debug_and_release_target
CONFIG     += qt warn_on release

DESTDIR		= release
TARGET	 	= FlightSimMockup

RESOURCES += docFS.qrc \

