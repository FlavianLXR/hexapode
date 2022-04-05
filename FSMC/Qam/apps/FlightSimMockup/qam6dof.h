/*  ---------------------------------------------------------------------------
 *  filename    :   qam6dof.h
 *  description :   INTERFACE des classes Qam6Dof
 *
 *	project     :	Matrices 6x6 et vecteurs pour 6 degrés de liberté
 *  start date  :   octobre 2017
 *  ---------------------------------------------------------------------------
 *  Copyright 2017 by Alain Menu   <alain.menu@ac-creteil.fr>
 *
 *  This file is part of "FlightSim Mock-up"
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

#ifndef QAM6DOF_H
#define QAM6DOF_H

#include <QtMath>
#include <QGenericMatrix>

// 6-DOF (Six Degrees Of Liberty) tools...

class QamMatrix1x6 ;
class QamMatrix6x1 ;

// matrice 6x6

class QamMatrix6x6 : public QGenericMatrix<6, 6, float>
{
  public:
	QamMatrix6x6() ;

	void setRow(int r, float v0, float v1, float v2, float v3, float v4, float v5 ) ;
	void setRow(int r, float v[] ) ;
	bool inverted() ;
	QamMatrix1x6 mult(const QamMatrix1x6& v ) ;

  private:
	float& m_(int r, int c) { return this->operator() (r,c) ; }
	float  m_(int r, int c) const { return this->operator() (r,c) ; }
	void _rowSwap(int c, int r0, int r1 ) ;
	void _rowScale(int c, float scale, int r ) ;
	void _rowScaleAdd(int c, float scale, int r0, int r1 ) ;

} ;

// vecteur colonne x6

class QamMatrix1x6 : public QGenericMatrix<1, 6, float>
{
  public:
	QamMatrix1x6() ;
	void set(float v0, float v1, float v2, float v3, float v4, float v5 ) ;
	void set(float v[] ) ;
	float norm() const ;
	QamMatrix6x1 toRow() const ;

	float& operator() (int r ) { return QGenericMatrix<1, 6, float>::operator() (r,0) ; }
	float  operator() (int r ) const { return QGenericMatrix<1, 6, float>::operator() (r,0) ; }

  private:
	float& m_(int r) { return this->operator() (r) ; }
	float  m_(int r) const { return this->operator() (r) ; }
} ;

// vecteur ligne x6

class QamMatrix6x1 : public QGenericMatrix<6, 1, float>
{
  public:
	QamMatrix6x1() ;
	void set(float v0, float v1, float v2, float v3, float v4, float v5 ) ;
	void set(float v[] ) ;
	float norm() const ;
	QamMatrix1x6 toColumn() const ;

	QamMatrix6x1 toRadians() const ;
	QamMatrix6x1 toDegrees() const ;

	float& operator() (int c ) { return QGenericMatrix<6, 1, float>::operator() (0,c) ; }
	float  operator() (int c ) const { return QGenericMatrix<6, 1, float>::operator() (0,c) ; }

  private:
	float& m_(int c) { return this->operator() (c) ; }
	float  m_(int c) const { return this->operator() (c) ; }
} ;

#endif
