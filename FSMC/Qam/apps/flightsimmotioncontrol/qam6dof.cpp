/*  ---------------------------------------------------------------------------
 *  filename    :   qam6dof.cpp
 *  description :   IMPLEMENTATION des classes Qam6Dof
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

#include "qam6dof.h"

// ---------------------------------------------------------------------------
//	QamMatrix6x6 : matrice 6x6
// ---------------------------------------------------------------------------

QamMatrix6x6::QamMatrix6x6()
    : QGenericMatrix<6, 6, float>()
{
}

void QamMatrix6x6::setRow(int r, float v0, float v1, float v2, float v3, float v4, float v5 )
{
    if (( r < 0 )||( r >= 6 ))	return ;
    m_(r, 0) = v0 ;
    m_(r, 1) = v1 ;
    m_(r, 2) = v2 ;
    m_(r, 3) = v3 ;
    m_(r, 4) = v4 ;
    m_(r, 5) = v5 ;
}

void QamMatrix6x6::setRow(int r, float v[] )
{
    if (( r < 0 )||( r >= 6 ))	return ;
    for ( int c = 0 ; c < 6 ; ++c )	m_(r, c) = v[c] ;
}

// inverse the matrix using Gauss-Jordan elimination
// returns false if the matrix is singular, true otherwise.

bool QamMatrix6x6::inverted()
{
    QamMatrix6x6 inv ;
    float	eps = 10e-6 ;

    for ( int c = 0 ; c < 6 ; ++c ) {
        if ( qAbs( m_(c,c) ) <= 2 * eps ) {
            int r ;
            for ( r = c + 1 ; r < 6 ; ++r )
                if ( qAbs( m_(r,c) ) <= 2 * eps ) {
                    this->_rowSwap(6, c, r ) ;
                    inv._rowSwap(6, c, r ) ;
                    break ;
            }
            if ( r >= 6 )	return false ;
        }
        float scale = 1.0 / m_(c,c) ;
        this->_rowScale(6, scale, c ) ;
        inv._rowScale(6, scale, c ) ;
        for ( int r = 0 ; r < 6 ; ++r ) {
            if ( r != c ) {
                scale = -m_(r,c) ;
                this->_rowScaleAdd(6, scale, c, r ) ;
                inv._rowScaleAdd(6, scale, c, r ) ;
            }
        }
    }
    *this = inv ;
    return true ;
}

// column vector <- matrix * column vector

QamMatrix1x6 QamMatrix6x6::mult(const QamMatrix1x6& v )
{
    QamMatrix1x6 res ;
    res.fill(0.0) ;

    for ( int r = 0 ; r < 6 ; ++r ) {
        for ( int c = 0 ; c < 6 ; ++c ) {
            res(r) += m_(r,c) * v(c) ;
        }
    }
    return res ;
}

// swap 2 rows in a matrix

void QamMatrix6x6::_rowSwap(int c, int r0, int r1 )
{
    for ( int i = 0 ; i < c ; ++i ) {
        float tmp = m_(r0,i) ;
        m_(r0,i) = m_(r1,i) ;
        m_(r1,i) = tmp ;
    }
}

// scale a row in a matrix by a constant factor

void QamMatrix6x6::_rowScale(int c, float scale, int r )
{
    for ( int i = 0 ; i < c ; ++i )	m_(r,i) *= scale ;
}

// perform scale and add a row in a matrix to another row

void QamMatrix6x6::_rowScaleAdd(int c, float scale, int r0, int r1 )
{
    for ( int i = 0 ; i < c ; ++i )	m_(r1,i) += scale * m_(r0,i) ;
}

// ---------------------------------------------------------------------------
//	QamMatrix1x6 : vecteur colonne x6
// ---------------------------------------------------------------------------

QamMatrix1x6::QamMatrix1x6()
    : QGenericMatrix<1, 6, float>()
{
}

void QamMatrix1x6::set(float v0, float v1, float v2, float v3, float v4, float v5 )
{
    m_(0) = v0 ;
    m_(1) = v1 ;
    m_(2) = v2 ;
    m_(3) = v3 ;
    m_(4) = v4 ;
    m_(5) = v5 ;
}

void QamMatrix1x6::set(float v[] )
{
    for ( int r = 0 ; r < 6 ; ++r )	m_(r) = v[r] ;
}

float QamMatrix1x6::norm() const
{
    float res = 0.0 ;
    for ( int r = 0 ; r < 6 ; ++r )	res += qPow(m_(r), 2 ) ;
    return qSqrt( res ) ;
}

QamMatrix6x1 QamMatrix1x6::toRow() const
{
    QamMatrix6x1 res ;
    res.fill(0.0) ;
    for ( int i = 0 ; i < 6 ; ++i )	res(i) = this->m_(i) ;
    return res ;
}

// ---------------------------------------------------------------------------
//	QamMatrix6x1 : vecteur ligne x6
// ---------------------------------------------------------------------------

QamMatrix6x1::QamMatrix6x1()
    : QGenericMatrix<6, 1, float>()
{
}

void QamMatrix6x1::set(float v0, float v1, float v2, float v3, float v4, float v5 )
{
    m_(0) = v0 ;
    m_(1) = v1 ;
    m_(2) = v2 ;
    m_(3) = v3 ;
    m_(4) = v4 ;
    m_(5) = v5 ;
}

void QamMatrix6x1::set(float v[] )
{
    for ( int c = 0 ; c < 6 ; ++c )	m_(c) = v[c] ;
}

float QamMatrix6x1::norm() const
{
    float res = 0.0 ;
    for ( int c = 0 ; c < 6 ; ++c )	res += qPow(m_(c), 2 ) ;
    return qSqrt( res ) ;
}

QamMatrix1x6 QamMatrix6x1::toColumn() const
{
    QamMatrix1x6 res ;
    res.fill(0.0) ;
    for ( int i = 0 ; i < 6 ; ++i )	res(i) = this->m_(i) ;
    return res ;
}

// [Tx,Ty,Tz,Rx,Ry,Rz] degrees -> radians
QamMatrix6x1 QamMatrix6x1::toRadians() const
{
    QamMatrix6x1 res = *this ;
    res(3) = qDegreesToRadians( this->m_(3) ) ;
    res(4) = qDegreesToRadians( this->m_(4) ) ;
    res(5) = qDegreesToRadians( this->m_(5) ) ;
    return res ;
}

// [Tx,Ty,Tz,Rx,Ry,Rz] radians -> degrees
QamMatrix6x1 QamMatrix6x1::toDegrees() const
{
    QamMatrix6x1 res = *this ;
    res(3) = qRadiansToDegrees( this->m_(3) ) ;
    res(4) = qRadiansToDegrees( this->m_(4) ) ;
    res(5) = qRadiansToDegrees( this->m_(5) ) ;
    return res ;
}
