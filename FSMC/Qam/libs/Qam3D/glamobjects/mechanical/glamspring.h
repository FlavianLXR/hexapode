/*  ---------------------------------------------------------------------------
 *  filename    :   glamspring.h
 *  description :   INTERFACE de la classe GLamSpring
 *
 *	project     :	Qam3D/GLam: 3D object collection
 *  start date  :   décembre 2009
 *  ---------------------------------------------------------------------------
 *  Copyright 2009-2020 by Alain Menu   <alain.menu@ac-creteil.fr>
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

// ressort cylindrique de compression défini par :
//	- internalDiameter: diamètre intérieur (e.g. utilisé pour guidage)
//	- wireDiameter: diamètre du fil
//	- restingLength: longueur au repos
//	- spins: nombre de spires ( toujours >= 3 )
//	- slices: nombre de facette par tour
// le ressort est posé sur le plan 0XY en élévation suivant 0Z

#ifndef GLAMSPRING_H
#define GLAMSPRING_H

#include "glamcurve.h"

class GLamSpring : public GLamCurve
{
  public:
	GLamSpring() ;
	GLamSpring(qreal internalDiameter, qreal wireDiameter, qreal restingLength, int spins, int numSlices = 120 ) ;
	GLamSpring(const GLamSpring& obj ) ;
	~GLamSpring() ;
	GLamSpring& operator =(const GLamSpring& obj ) ;

	qreal length() const ;
	qreal restingLength() const ;
	qreal minLength() const ;
	bool setLength(qreal length ) ;

  private:
	QList<QVector3D> arcShape(qreal angle, qreal dz ) ;

  private:
	qreal		m_iRadius ;
	qreal		m_wRadius ;
	qreal		m_rLength ;
	int			m_spins ;
	int			m_slices ;
	qreal		m_length ;

	qreal		m_a0 ;
	qreal		m_z0 ;
} ;

#endif	// GLAMSPRING_H

/*! class GLamSpring
 */
