#  ---------------------------------------------------------------------------
#   filename    :   glam.pri
#   description :   Qt project file
#
#  	project     :	Qam3D/GLam Qt project file
#   start date  :   mars 2015
#   ---------------------------------------------------------------------------
#   Copyright 2009-2018 by Alain Menu   <alain.menu@ac-creteil.fr>
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

#	recognized CONFIG options: 
#   ---------------------------------------------------------------------------
#	NO_GLamWidgetUi:	without glamwidgetui.*
#	NO_GLamGroup:		without glamgroup.*, glamcsg.*, libcsg/
#	NO_GLamMechanical:	without glamobjects/mechanical/*
#	GLamGluTess			GLU libtess used (default is Poly2Tri, Wu Liang)
#   ---------------------------------------------------------------------------

QT	+= core gui widgets

VERSION = $$system(cat $$system(pwd)/_VERSION | cut -f 2 -d \'\"\' )
message(Using Qam3D/GLam version $${VERSION}) 

# 	GLam paths
#   ---------------------------------------------------------------------------

GLAMWIDGET		= $$system(pwd)/glamwidget
GLAMOBJECTS		= $$system(pwd)/glamobjects
GLAMMECHANICAL	= $$system(pwd)/glamobjects/mechanical
GLAMTOOLS		= $$system(pwd)/glamtools
GLAMRESOURCES	= $$system(pwd)/res
GLAMEXAMPLES	= $$system(pwd)/examples
LIBCSG			= $$system(pwd)/libcsg
LIBTESS			= $$system(pwd)/libtess
LIBTESS2		= $$system(pwd)/libtess2
SHADERS			= $$system(pwd)/shaders

# 	GLam OpenGL ES Widget
#   ---------------------------------------------------------------------------

INCLUDEPATH	+= $${GLAMWIDGET}
CPPHEADERS	+= $${GLAMWIDGET}/GLamWidget

HEADERS	+= \
	$${GLAMWIDGET}/glamscene.h \
	$${GLAMWIDGET}/glamwidget.h

SOURCES	+= \
	$${GLAMWIDGET}/glamwidget.cpp

# 	GLam 3D Objects
#   ---------------------------------------------------------------------------

INCLUDEPATH	+= $${GLAMOBJECTS}
CPPHEADERS	+= $${GLAMOBJECTS}/GLamObjects

RESOURCES	+=	$$system(pwd)/glam.qrc

HEADERS	+= \
	$${GLAMOBJECTS}/glamobject.h \
	$${GLAMOBJECTS}/glammesh.h \
	$${GLAMOBJECTS}/glamgrid.h \
	$${GLAMOBJECTS}/glamsolid.h \
	$${GLAMOBJECTS}/glamprofile.h \
	$${GLAMOBJECTS}/glamhole.h \
	$${GLAMOBJECTS}/glamprism.h \
	$${GLAMOBJECTS}/glamcube.h \
	$${GLAMOBJECTS}/glamrevolution.h \
	$${GLAMOBJECTS}/glamcylinder.h \
	$${GLAMOBJECTS}/glampipe.h \
	$${GLAMOBJECTS}/glamsphere.h \
	$${GLAMOBJECTS}/glamvector.h \
	$${GLAMOBJECTS}/glamtorus.h \
        $${GLAMOBJECTS}/glamcurve.h \
        $${GLAMOBJECTS}/glamphoto.h
	
SOURCES	+= \
	$${GLAMOBJECTS}/glamobject.cpp \
	$${GLAMOBJECTS}/glammesh.cpp \
	$${GLAMOBJECTS}/glamgrid.cpp \
	$${GLAMOBJECTS}/glamsolid.cpp \
	$${GLAMOBJECTS}/glamprofile.cpp \
	$${GLAMOBJECTS}/glamhole.cpp \
	$${GLAMOBJECTS}/glamprism.cpp \
	$${GLAMOBJECTS}/glamcube.cpp \
	$${GLAMOBJECTS}/glamrevolution.cpp \
	$${GLAMOBJECTS}/glamcylinder.cpp \
	$${GLAMOBJECTS}/glampipe.cpp \
	$${GLAMOBJECTS}/glamsphere.cpp \
	$${GLAMOBJECTS}/glamvector.cpp \
	$${GLAMOBJECTS}/glamtorus.cpp \
        $${GLAMOBJECTS}/glamcurve.cpp \
        $${GLAMOBJECTS}/glamphoto.cpp

# 	GLam 3D Objects Tesselation Library
#   ---------------------------------------------------------------------------

GLamGluTess {

warning(GLam GLU libtess used.)
DEFINES += USE_GLULIBTESS

HEADERS	+= \
    $${LIBTESS}/dict-list.h \
    $${LIBTESS}/dict.h \
    $${LIBTESS}/geom.h \
    $${LIBTESS}/gluos.h \
    $${LIBTESS}/memalloc.h \
    $${LIBTESS}/mesh.h \
    $${LIBTESS}/normal.h \
    $${LIBTESS}/priorityq-heap.h \
    $${LIBTESS}/priorityq-sort.h \
    $${LIBTESS}/priorityq.h \
    $${LIBTESS}/render.h \
    $${LIBTESS}/sweep.h \
    $${LIBTESS}/tess.h \
    $${LIBTESS}/tessmono.h

SOURCES	+= \
    $${LIBTESS}/dict.c \
    $${LIBTESS}/geom.c \
    $${LIBTESS}/memalloc.c \
    $${LIBTESS}/mesh.c \
    $${LIBTESS}/normal.c \
    $${LIBTESS}/priorityq-heap.c \
    $${LIBTESS}/priorityq.c \
    $${LIBTESS}/render.c \
    $${LIBTESS}/sweep.c \
    $${LIBTESS}/tess.c \
    $${LIBTESS}/tessmono.c

} else {

warning(GLam Poly2Tri libtess used.)

HEADERS	+= \
	$${LIBTESS2}/tessdefs.h \
	$${LIBTESS2}/splay.h \
	$${LIBTESS2}/tesspoint.h \
	$${LIBTESS2}/tessline.h \
	$${LIBTESS2}/tesspolygon.h

SOURCES	+= \
	$${LIBTESS2}/predicates.cc \
	$${LIBTESS2}/tesspoint.cpp \
	$${LIBTESS2}/tessline.cpp \
	$${LIBTESS2}/tesspolygon.cpp
}

# 	GLamWidget User Interface Option
#   ---------------------------------------------------------------------------

!NO_GLamWidgetUi {
	CPPHEADERS	+=	$${GLAMWIDGET}/GLamWidgetUi
	HEADERS		+=	$${GLAMWIDGET}/glamwidgetui.h
	SOURCES		+=	$${GLAMWIDGET}/glamwidgetui.cpp
} else {
	warning(GLam configured without User Interface.)
}

# 	GLamGroup/GLamCSG/GLamStL/GLamObjMtl Option
#   ---------------------------------------------------------------------------

!NO_GLamGroup {
	CPPHEADERS	+=	$${GLAMOBJECTS}/GLamGroup
	HEADERS		+=	$${GLAMOBJECTS}/glamgroup.h \
					$${GLAMOBJECTS}/glamcsg.h \
    				$${GLAMTOOLS}/glamlogo.h \
					$${LIBCSG}/csg.h \
					$${GLAMOBJECTS}/glamtext.h \
					$${GLAMOBJECTS}/glamstl.h \
					$${GLAMOBJECTS}/glamobjmtl.h \
					$${GLAMOBJECTS}/glamxplaneobj.h
	SOURCES		+=	$${GLAMOBJECTS}/glamgroup.cpp \
					$${GLAMOBJECTS}/glamcsg.cpp \
    				$${GLAMTOOLS}/glamlogo.cpp \
					$${LIBCSG}/csg.cpp \
					$${GLAMOBJECTS}/glamtext.cpp \
					$${GLAMOBJECTS}/glamstl.cpp \
					$${GLAMOBJECTS}/glamobjmtl.cpp \
					$${GLAMOBJECTS}/glamxplaneobj.cpp
} else {
	warning(GLam configured without Group/CSG or import/export capabilities.)
}

# 	GLam Mechanical Objects Option
#   ---------------------------------------------------------------------------

!NO_GLamMechanical{
	INCLUDEPATH	+=	$${GLAMMECHANICAL}
	CPPHEADERS	+=	$${GLAMMECHANICAL}/GLamMechanical
	HEADERS		+=	$${GLAMMECHANICAL}/glamwire.h \
					$${GLAMMECHANICAL}/glamspring.h \
					$${GLAMMECHANICAL}/glamgear.h \
					$${GLAMMECHANICAL}/glamhelicalgear.h
	SOURCES		+=	$${GLAMMECHANICAL}/glamwire.cpp \
					$${GLAMMECHANICAL}/glamspring.cpp \
					$${GLAMMECHANICAL}/glamgear.cpp \
					$${GLAMMECHANICAL}/glamhelicalgear.cpp
} else {
	warning(GLam configured without mechanical objects collection.)
}

# 	GLam Tools
#   ---------------------------------------------------------------------------

INCLUDEPATH	+= $${GLAMTOOLS}
CPPHEADERS	+= $${GLAMTOOLS}/GLamTools

HEADERS	+= \
#    $${GLAMTOOLS}/glamlogo.h \	<-- dépend de GLamGroup
	$${GLAMTOOLS}/glam2d.h \
	$${GLAMTOOLS}/glampolygon.h \
	$${GLAMTOOLS}/glammaterial.h

SOURCES	+= \
#    $${GLAMTOOLS}/glamlogo.cpp \  <-- dépend de GLamGroup
	$${GLAMTOOLS}/glam2d.cpp \
	$${GLAMTOOLS}/glampolygon.cpp \
	$${GLAMTOOLS}/glammaterial.cpp

#   ---------------------------------------------------------------------------

DISTFILES += \
	$${CPPHEADERS} \
	$${GLAMRESOURCES}/*.* \
	$${GLAMEXAMPLES}/*.* \
	$$system(pwd)/_ABOUT $$system(pwd)/_CHANGES $$system(pwd)/_VERSION
