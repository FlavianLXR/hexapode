/*  ---------------------------------------------------------------------------
 *  filename    :   glamgear.cpp
 *  description :   IMPLEMENTATION de la classe GLamGear
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

#include "glamgear.h"

GLamGear::GLamGear()
	: GLamObject()
	, m_thickness( 1.0f )
	, m_meshAngle( 0.0f )
	, m_innerRadius( 0.2f )
	, m_module( 0.2f )
	, m_toothCount( 8 )
	, m_ratio( 1.0f )
{
	setClassName("GLamGear") ;
}

GLamGear::GLamGear(qreal innerRadius, qreal module, uint toothCount, qreal thickness )
	: GLamObject()
	, m_thickness( thickness )
	, m_meshAngle( 0.0f )
	, m_innerRadius( innerRadius )
	, m_module( module )
	, m_toothCount( toothCount )
	, m_ratio( 1.0f )
{
	setClassName("GLamGear") ;
}

GLamGear::GLamGear(const GLamGear& obj, uint toothCount )
	: GLamObject()
{
	*this = obj ;
	if ( toothCount )	m_toothCount = toothCount ;
}

GLamGear::~GLamGear()
{
}

GLamGear& GLamGear::operator =(const GLamGear& obj )
{
	*(GLamObject*)this = obj ;

	m_innerRadius = obj.m_innerRadius ;
	m_module = obj.m_module ;
	m_toothCount = obj.m_toothCount ;
	m_thickness = obj.m_thickness ;
	m_meshAngle = obj.m_meshAngle ;
	m_ratio = obj.m_ratio ;

	updateNeeded() ;

	return *this ;
}

void GLamGear::setInnerRadius(qreal radius )
{
	m_innerRadius = qAbs( radius ) ;
	updateNeeded() ;
}

void GLamGear::setThickness(qreal thickness )
{
	m_thickness = qAbs( thickness ) ;
	updateNeeded() ;
}

// accouplement avec une autre roue dentée 'gear'
// le vecteur 'direction' donne la direction de l'axe de l'objet courant
// par rapport à celui de 'gear'
// 'direction' est corrigé pour fournir la position relative de l'axe de l'objet courant
// mise à jour de l'angle de calage 'm_meshAngle'
// retourne le rapport de transmission 'm_ratio' mis à jour

qreal GLamGear::meshTo(const GLamGear& gear, QVector2D& direction )
{
	qreal dirAngle = qAcos( direction.x() / direction.length() ) ;
	if ( direction.y() < 0.0f )	dirAngle *= -1.0f ;

	qreal interAxis = this->pitchRadius() + gear.pitchRadius() ;
	direction.setX( interAxis * qCos(dirAngle) ) ;
	direction.setY( interAxis * qSin(dirAngle) ) ;

	// angles inter-dents
	qreal delta1 = 2.0f * M_PI / gear.toothCount() ;
	qreal delta2 = 2.0f * M_PI / this->toothCount() ;
	// écart angulaire entre le sommet d'une dent et le vecteur directeur
	qreal corr1 = fmod( dirAngle - qDegreesToRadians( gear.meshAngle() ), delta1 ) ;
	// calage de l'objet courante
	m_meshAngle = dirAngle + M_PI + ( corr1 / delta1 + 0.5f ) * delta2 ;
	// correction du calage dans l'intervalle [0;delta2]
	qreal corr2 = fmod( m_meshAngle, delta2 ) ;
	m_meshAngle = corr2 ;
	// rapport de transmission
	m_ratio = -gear.ratio() * (qreal)gear.toothCount() / this->toothCount() ;

	updateNeeded() ;

	return m_ratio ;
}

void GLamGear::create()
{
	qreal r0 = innerRadius() ;
	qreal r1 = pitchRadius() - 1.25 * module() ;
	qreal r2 = outerRadius() ;
	qreal r3 = pitchRadius() ;
	qreal delta = ( 2.0f * M_PI / toothCount() ) / 4.0f ;
	qreal efp = 1.1 / 2.25 * delta ;	// écart d'angle fond de dent / diam. primitif
	qreal eps = 0.1 * delta ;			// écart d'angle diam. primitif / sommet de dent

	qreal z = m_thickness ;

//	qDebug() << "GLamGear::create()" << meshAngle() << module() << toothCount() << r0 << r1 << r3 << r2 ;

	for ( uint t = 0 ; t < m_toothCount ; ++t ) {
		qreal a = 2.0 * M_PI * t / m_toothCount ;
		a -= 1.5 * delta ;	// première dent centrée sur axe 0X
		a += m_meshAngle ;	// correction pour engrenage

		// flasques : r0 --> r1
		quad(
			QVector3D(r0 * qCos(a), r0 * qSin(a), z ),
			QVector3D(r1 * qCos(a), r1 * qSin(a), z ),
			QVector3D(r1 * qCos(a + 3 * delta ), r1 * qSin(a + 3 * delta), z ),
			QVector3D(r0 * qCos(a + 2 * delta ), r0 * qSin(a + 2 * delta), z ),
			-z
		) ;
		quad(
			QVector3D(r0 * qCos(a + 2 * delta ), r0 * qSin(a + 2 * delta), z ),
			QVector3D(r1 * qCos(a + 3 * delta ), r1 * qSin(a + 3 * delta), z ),
			QVector3D(r1 * qCos(a + 4 * delta ), r1 * qSin(a + 4 * delta), z ),
			QVector3D(r0 * qCos(a + 4 * delta ), r0 * qSin(a + 4 * delta), z ),
			-z
		) ;
		// flasques : r1 -> r3
		quad(
			QVector3D(r1 * qCos(a), r1 * qSin(a), z ),
			QVector3D(r3 * qCos(a + delta - efp ), r3 * qSin(a + delta - efp), z ),
			QVector3D(r3 * qCos(a + 2 * delta + efp ), r3 * qSin(a + 2 * delta + efp), z ),
			QVector3D(r1 * qCos(a + 3 * delta ), r1 * qSin(a + 3 * delta), z ),
			-z
		) ;
		// flasques : r3 -> r2
		quad(
			QVector3D(r3 * qCos(a + delta - efp ), r3 * qSin(a + delta - efp), z ),
			QVector3D(r2 * qCos(a + delta + eps ), r2 * qSin(a + delta + eps), z ),
			QVector3D(r2 * qCos(a + 2 * delta - eps ), r2 * qSin(a + 2 * delta - eps), z ),
			QVector3D(r3 * qCos(a + 2 * delta + efp ), r3 * qSin(a + 2 * delta + efp), z ),
			-z
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
		zExtrude(
			QVector3D(r3 * qCos(a + delta - efp ), r3 * qSin(a + delta - efp), 0.0f ),
			QVector3D(r1 * qCos(a), r1 * qSin(a), z )
		) ;
		zExtrude(
			QVector3D(r1 * qCos(a + 3 * delta ), r1 * qSin(a + 3 * delta), 0.0f ),
			QVector3D(r3 * qCos(a + 2 * delta + efp ), r3 * qSin(a + 2 * delta + efp), z )
		) ;
		// épaisseur : r3 -> r2
		zExtrude(
			QVector3D(r2 * qCos(a + delta + eps ), r2 * qSin(a + delta + eps), 0.0f ),
			QVector3D(r3 * qCos(a + delta - efp ), r3 * qSin(a + delta - efp), z )
		) ;
		zExtrude(
			QVector3D(r3 * qCos(a + 2 * delta + efp ), r3 * qSin(a + 2 * delta + efp), 0.0f ),
			QVector3D(r2 * qCos(a + 2 * delta - eps ), r2 * qSin(a + 2 * delta - eps), z )
		) ;
		// épaisseur : r2 (diamètre extérieur)
		zExtrude(
			QVector3D(r2 * qCos(a + 2 * delta - eps ), r2 * qSin(a + 2 * delta - eps), 0.0f ),
			QVector3D(r2 * qCos(a + delta + eps ), r2 * qSin(a + delta + eps), z )
		) ;
		// épaisseur : r1 (fond de dent)
		zExtrude(
			QVector3D(r1 * qCos(a + 4 * delta ), r1 * qSin(a + 4 * delta), 0.0f ),
			QVector3D(r1 * qCos(a + 3 * delta ), r1 * qSin(a + 3 * delta), z )
		) ;
	}
}
