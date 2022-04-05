/*  ---------------------------------------------------------------------------
 *  filename    :   glamxplaneobj.h
 *  description :   INTERFACE de la classe GLamXplaneObj
 *
 *	project     :	Qam3D/GLam: 3D object collection
 *  start date  :   décembre 2009
 *  ---------------------------------------------------------------------------
 *  Copyright 2009-2018 by Alain Menu   <alain.menu@ac-creteil.fr>
 *
 *  This file is part of "Qam3D/GLam Qt Library"
 *
 *  This program is free software ;  you can  redistribute it and/or  modify it
 *  under the terms of the  GNU General Public License as published by the Free
 *  Software Foundation ; either version 3 of the License, or  (at your option)
 *  any later version.
 *
 *  This program is distributed in the hope that it will be useful, but WITHOUT
 *  ANY WARRANTY ; without even the  implied  warranty  of  MERCHANTABILITY  or
 *  FITNESS FOR  A PARTICULAR PURPOSE. See the  GNU General Public License  for
 *  more details.
 *
 *	You should have  received  a copy of the  GNU General Public License  along
 *	with this program. If not, see <http://www.gnu.org/licenses/>.
 *  ---------------------------------------------------------------------------
 */

// GLamObject to/from Laminar Research XPlane OBJ8

#ifndef GLAMXPLANEOBJ_H
#define	GLAMXPLANEOBJ_H

#include "glamgroup.h"
#include "glammesh.h"

class GLamXplaneObj : public GLamObject
{
  public:
	GLamXplaneObj() ;
	GLamXplaneObj(const QString& filename ) ;
	GLamXplaneObj(GLamObject* obj, const QString& name = 0 ) ;
	GLamXplaneObj(GLamGroup* obj, const QString& name = 0 ) ;
	GLamXplaneObj(const GLamXplaneObj& obj ) ;

	GLamXplaneObj& operator =(const GLamXplaneObj& obj ) ;

	bool load(const QString& filename ) ;
	bool save(const QString& filename ) ;

  protected:
	virtual void create() ;

  private:
	QString f2s(float v ) ;
	QString v2s(QVector3D v ) ;

  private:
	QList<QVector3D>	m_v ;
	QList<QVector2D>	m_t ;
	QList<int>			m_i ;
} ;

#endif
