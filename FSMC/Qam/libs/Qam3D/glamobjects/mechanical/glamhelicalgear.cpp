/*  ---------------------------------------------------------------------------
 *  filename    :   glamhelicalgear.cpp
 *  description :   IMPLEMENTATION de la classe GLamHelicalGear
 *
 *	project     :	Qam3D/GLam: 3D object collection
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

#include "glamhelicalgear.h"

GLamHelicalGear::GLamHelicalGear()
	: GLamGear()
	, m_helixAngle( 0.0f )
{
	setClassName("GLamHelicalGear") ;
}

GLamHelicalGear::GLamHelicalGear(qreal innerRadius, qreal module, uint toothCount, qreal helixAngle, qreal thickness )
	: GLamGear(innerRadius, module, toothCount, thickness )
	, m_helixAngle( helixAngle )
{
	setClassName("GLamHelicalGear") ;
}

GLamHelicalGear::GLamHelicalGear(const GLamHelicalGear& obj, uint toothCount )
	: GLamGear( obj, toothCount )
{
	m_helixAngle = obj.m_helixAngle ;
	updateNeeded() ;
}

GLamHelicalGear::~GLamHelicalGear()
{
}

GLamHelicalGear& GLamHelicalGear::operator =(const GLamHelicalGear& obj )
{
	*(GLamGear*)this = obj ;
	m_helixAngle = obj.m_helixAngle ;
	updateNeeded() ;
	return *this ;
}

qreal GLamHelicalGear::helixAngle() const { return m_helixAngle ; }

qreal GLamHelicalGear::meshTo(const GLamHelicalGear& gear, QVector2D& direction )
{
	m_helixAngle = -gear.m_helixAngle ;
	return GLamGear::meshTo(gear, direction ) ;
}

void GLamHelicalGear::create()
{
	qreal r0 = innerRadius() ;
	qreal r1 = pitchRadius() - 1.25 * module() ;
	qreal r2 = outerRadius() ;
	qreal r3 = pitchRadius() ;
	qreal delta = ( 2.0f * M_PI / toothCount() ) / 4.0f ;
	qreal efp = 1.1 / 2.25 * delta ;	// écart d'angle fond de dent / diam. primitif
	qreal eps = 0.1 * delta ;			// écart d'angle diam. primitif / sommet de dent

	qreal z = m_thickness ;

	for ( uint t = 0 ; t < toothCount() ; ++t ) {
		qreal a = 2.0 * M_PI * t / toothCount() ;
		a -= 1.5 * delta ;	// première dent centrée sur axe 0X
		a += m_meshAngle ;	// correction pour engrenage

		qreal b = a + qTan( qDegreesToRadians(m_helixAngle) ) * z / pitchRadius() ;

		// flasques : r0 --> r1
		quad(
			QVector3D(r0 * qCos(a), r0 * qSin(a), z ),
			QVector3D(r1 * qCos(a), r1 * qSin(a), z ),
			QVector3D(r1 * qCos(a + 3 * delta ), r1 * qSin(a + 3 * delta), z ),
			QVector3D(r0 * qCos(a + 2 * delta ), r0 * qSin(a + 2 * delta), z )
		) ;
		quad(
			QVector3D(r0 * qCos(a + 2 * delta ), r0 * qSin(a + 2 * delta), z ),
			QVector3D(r1 * qCos(a + 3 * delta ), r1 * qSin(a + 3 * delta), z ),
			QVector3D(r1 * qCos(a + 4 * delta ), r1 * qSin(a + 4 * delta), z ),
			QVector3D(r0 * qCos(a + 4 * delta ), r0 * qSin(a + 4 * delta), z )
		) ;

		quad(
			QVector3D(r0 * qCos(b), r0 * qSin(b), 0.0f ),
			QVector3D(r1 * qCos(b), r1 * qSin(b), 0.0f ),
			QVector3D(r1 * qCos(b + 3 * delta ), r1 * qSin(b + 3 * delta), 0.0f ),
			QVector3D(r0 * qCos(b + 2 * delta ), r0 * qSin(b + 2 * delta), 0.0f )
		) ;
		quad(
			QVector3D(r0 * qCos(b + 2 * delta ), r0 * qSin(b + 2 * delta), 0.0f ),
			QVector3D(r1 * qCos(b + 3 * delta ), r1 * qSin(b + 3 * delta), 0.0f ),
			QVector3D(r1 * qCos(b + 4 * delta ), r1 * qSin(b + 4 * delta), 0.0f ),
			QVector3D(r0 * qCos(b + 4 * delta ), r0 * qSin(b + 4 * delta), 0.0f )
		) ;

		// flasques : r1 -> r3
		quad(
			QVector3D(r1 * qCos(a), r1 * qSin(a), z ),
			QVector3D(r3 * qCos(a + delta - efp ), r3 * qSin(a + delta - efp), z ),
			QVector3D(r3 * qCos(a + 2 * delta + efp ), r3 * qSin(a + 2 * delta + efp), z ),
			QVector3D(r1 * qCos(a + 3 * delta ), r1 * qSin(a + 3 * delta), z )
		) ;
		quad(
			QVector3D(r1 * qCos(b), r1 * qSin(b), 0.0f ),
			QVector3D(r3 * qCos(b + delta - efp ), r3 * qSin(b + delta - efp), 0.0f ),
			QVector3D(r3 * qCos(b + 2 * delta + efp ), r3 * qSin(b + 2 * delta + efp), 0.0f ),
			QVector3D(r1 * qCos(b + 3 * delta ), r1 * qSin(b + 3 * delta), 0.0f )
		) ;
		// flasques : r3 -> r2
		quad(
			QVector3D(r3 * qCos(a + delta - efp ), r3 * qSin(a + delta - efp), z ),
			QVector3D(r2 * qCos(a + delta + eps ), r2 * qSin(a + delta + eps), z ),
			QVector3D(r2 * qCos(a + 2 * delta - eps ), r2 * qSin(a + 2 * delta - eps), z ),
			QVector3D(r3 * qCos(a + 2 * delta + efp ), r3 * qSin(a + 2 * delta + efp), z )
		) ;
		quad(
			QVector3D(r3 * qCos(b + delta - efp ), r3 * qSin(b + delta - efp), 0.0f ),
			QVector3D(r2 * qCos(b + delta + eps ), r2 * qSin(b + delta + eps), 0.0f ),
			QVector3D(r2 * qCos(b + 2 * delta - eps ), r2 * qSin(b + 2 * delta - eps), 0.0f ),
			QVector3D(r3 * qCos(b + 2 * delta + efp ), r3 * qSin(b + 2 * delta + efp), 0.0f )
		) ;

		// épaisseur : r0 (diamètre intérieur)
		zExtrude(
			QVector3D(r0 * qCos(a), r0 * qSin(a), 0.0f ),
			QVector3D(r0 * qCos(a + 2 * delta ), r0 * qSin(a + 2 * delta), z )
		) ;
		zExtrude(
			QVector3D(r0 * qCos(a + 2 * delta ), r0 * qSin(a + 2 * delta), 0.0f ),
			QVector3D(r0 * qCos(a + 4 * delta ), r0 * qSin(a + 4 * delta), z )
		) ;

		// épaisseur : r1 -> r3
		quad(
			QVector3D(r3 * qCos(a + delta - efp ), r3 * qSin(a + delta - efp), z ),
			QVector3D(r1 * qCos(a), r1 * qSin(a), z ),
			QVector3D(r1 * qCos(b), r1 * qSin(b), 0.0f ),
			QVector3D(r3 * qCos(b + delta - efp ), r3 * qSin(b + delta - efp), 0.0f )
		) ;
		quad(
			QVector3D(r1 * qCos(a + 3 * delta ), r1 * qSin(a + 3 * delta), z ),
			QVector3D(r3 * qCos(a + 2 * delta + efp ), r3 * qSin(a + 2 * delta + efp), z ),
			QVector3D(r3 * qCos(b + 2 * delta + efp ), r3 * qSin(b + 2 * delta + efp), 0.0f ),
			QVector3D(r1 * qCos(b + 3 * delta ), r1 * qSin(b + 3 * delta), 0.0f )
		) ;

		// épaisseur : r3 -> r2
		quad(
			QVector3D(r2 * qCos(a + delta + eps ), r2 * qSin(a + delta + eps), z ),
			QVector3D(r3 * qCos(a + delta - efp ), r3 * qSin(a + delta - efp), z ),
			QVector3D(r3 * qCos(b + delta - efp ), r3 * qSin(b + delta - efp), 0.0f ),
			QVector3D(r2 * qCos(b + delta + eps ), r2 * qSin(b + delta + eps), 0.0f )
		) ;
		quad(
			QVector3D(r3 * qCos(a + 2 * delta + efp ), r3 * qSin(a + 2 * delta + efp), z ),
			QVector3D(r2 * qCos(a + 2 * delta - eps ), r2 * qSin(a + 2 * delta - eps), z ),
			QVector3D(r2 * qCos(b + 2 * delta - eps ), r2 * qSin(b + 2 * delta - eps), 0.0f ),
			QVector3D(r3 * qCos(b + 2 * delta + efp ), r3 * qSin(b + 2 * delta + efp), 0.0f )
		) ;

		// épaisseur : r2 (diamètre extérieur)
		quad(
			QVector3D(r2 * qCos(a + delta + eps ), r2 * qSin(a + delta + eps), z ),
			QVector3D(r2 * qCos(b + delta + eps ), r2 * qSin(b + delta + eps), 0.0f ),
			QVector3D(r2 * qCos(b + 2 * delta - eps ), r2 * qSin(b + 2 * delta - eps), 0.0f ),
			QVector3D(r2 * qCos(a + 2 * delta - eps ), r2 * qSin(a + 2 * delta - eps), z )
		) ;
		// épaisseur : r1 (fond de dent)
		quad(
			QVector3D(r1 * qCos(a + 3 * delta ), r1 * qSin(a + 3 * delta), z ),
			QVector3D(r1 * qCos(b + 3 * delta ), r1 * qSin(b + 3 * delta), 0.0f ),
			QVector3D(r1 * qCos(b + 4 * delta ), r1 * qSin(b + 4 * delta), 0.0f ),
			QVector3D(r1 * qCos(a + 4 * delta ), r1 * qSin(a + 4 * delta), z )
		) ;
	}
}
