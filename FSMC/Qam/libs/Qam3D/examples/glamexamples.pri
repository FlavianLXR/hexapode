#  ---------------------------------------------------------------------------
#   filename    :   glamexamples.pri
#   description :   Qt project file
#
#  	project     :	Qam3D/GLam examples Qt project file
#   start date  :   mars 2015
#   ---------------------------------------------------------------------------
#   Copyright 2009-2016 by Alain Menu   <alain.menu@ac-creteil.fr>
#
#   This file is part of "Qam3D/GLam Qt Library"
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

GLAMEXAMPLES = $$system(pwd)

INCLUDEPATH	+= $${GLAMEXAMPLES}

CPPHEADERS	+= \
	$${GLAMEXAMPLES}/GLamExamples
	
HEADERS	+= \
	$${GLAMEXAMPLES}/graal.h \
	$${GLAMEXAMPLES}/axis3d.h \
	$${GLAMEXAMPLES}/wbwheel.h \
	$${GLAMEXAMPLES}/gripper.h \
	$${GLAMEXAMPLES}/curvesample.h \
	$${GLAMEXAMPLES}/wiresample.h \
	$${GLAMEXAMPLES}/csgsample.h \
	$${GLAMEXAMPLES}/thingy.h \
	$${GLAMEXAMPLES}/dice.h

SOURCES	+= \
	$${GLAMEXAMPLES}/graal.cpp \
	$${GLAMEXAMPLES}/axis3d.cpp \
	$${GLAMEXAMPLES}/wbwheel.cpp \
	$${GLAMEXAMPLES}/gripper.cpp \
	$${GLAMEXAMPLES}/curvesample.cpp \
	$${GLAMEXAMPLES}/wiresample.cpp \
	$${GLAMEXAMPLES}/csgsample.cpp \
	$${GLAMEXAMPLES}/thingy.cpp \
	$${GLAMEXAMPLES}/dice.cpp

DISTFILES += \
	$${CPPHEADERS}
