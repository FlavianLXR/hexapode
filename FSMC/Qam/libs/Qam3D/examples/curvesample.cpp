/*  ---------------------------------------------------------------------------
 *  filename    :   curvesample.cpp
 *  description :   IMPLEMENTATION de la classe CurveSample
 *
 *	project     :	Qam3D/GLam: GLamCurve examples
 *  start date  :   décembre 2009
 *  ---------------------------------------------------------------------------
 *  Copyright 2009-2016 by Alain Menu   <alain.menu@ac-creteil.fr>
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

#include "curvesample.h"

CurveSample::CurveSample(Type type )
	: GLamCurve()
{
	QList<QVector3D> curve ;

	if ( type == Trefoil ) {

		// hypotrochoïde (Möbius Trefoil Knot)
		// équation paramétrique avec t dans l'intervalle [ 0 ; 2PI ]

		int n = 400 ;
		for (int i = 0 ; i < n ; i++ ) {
			double t = 2 * M_PI * (double)i / ( n - 1 ) ;
			double x = qCos( t ) + 2 * qCos( 2 * t ) ;
			double y = qSin( t ) - 2 * qSin( 2 * t ) ;
			double z = 2 * qSin( 3 * t ) ;
			curve << QVector3D(x, y, z ) ;
		}
		setShape( GLam2D::circularShape(0.2, 30 ) ) ;
//		setMaterial( GLamMaterial( QColor(250, 240, 12 ), true ) ) ;
		material().setColor( QColor(250, 240, 12 ) ) ;
		material().setShiny() ;
	}

	else if ( type == Cycloid ) {

		// trisectrice de Ceva (courbe 2D)
		// équation polaire avec theta dans l'intervalle [ 0 ; 2PI ]

		float a = 1.0 ;
		int   n = 500 ;
		for (int i = 0 ; i < n ; i++ ) {
			float theta = 2 * M_PI * (float)i / ( n - 1 ) ;
			float ro = a * ( 1 + 2 * qCos( 2 * theta ) ) ;
			float x = ro * qSin( theta ) ;
			float y = ro * qCos( theta ) ;
			curve << QVector3D(x, y, 0 ) ;
		}
//		setShape( GLam2D::circularShape(0.05, 4 ) ) ;
		setShape( GLam2D::oblongShape(QSizeF(0.1, 0.05), 20 ) ) ;
		setMaterial( GLamMaterial( QColor(150, 40, 110 ), true ) ) ;
//		material().setShiny() ;
	}

	else if ( type == CubicCircle ) {

		// horoptère (Cubic Circle)
		// équation cartésienne avec y dans l'intervalle [ ymin ; ymax ]

		float ymin = -6.0 ;
		float ymax = +6.0 ;
		float r = 1.2 ;
		int   n = 150 ;
//		int   n = 15 ;
		for (int i = 0 ; i < n ; i++ ) {
			float y = ymin + ( ymax - ymin ) * (float)i / ( n - 1 ) ;
			float x = 2 * r / ( 1 + y * y ) ;
			float z = x * y ;
			curve << QVector3D(x, y, z ) ;
		}
/*
		// section "tube creux"
		QList<QPointF> shape ;
		shape << GLam2D::arcShape(0.5,    0, 360, 90 ) ;
		shape << GLam2D::arcShape(0.35, 360,   0, 90 ) ;
		setShape( shape ) ;
*/
		setShape( GLam2D::circularShape(r/5/*0.2*/, 90 ) ) ;
	}

	setCurve( curve ) ;
}
