/*  ---------------------------------------------------------------------------
 *  filename    :   glamcsg.h
 *  description :   INTERFACE de la classe GLamCSG
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

#ifndef GLAMCSG_H
#define GLAMCSG_H

#include "../libcsg/csg.h"
#include "glamgroup.h"
#include "../glamtools/glampolygon.h"

class GLamCSG : public GLamGroup
{
  public:
	GLamCSG() ;
	GLamCSG(const GLamObject* obj ) ;
	GLamCSG(const GLamCSG& obj ) ;

	GLamCSG& operator =(const GLamCSG& obj ) ;

	GLamCSG* merge(const GLamObject* obj ) ;
	GLamCSG* subtract(const GLamObject* obj ) ;
	GLamCSG* intersect(const GLamObject* obj ) ;

	/*! Surcharge de la méthode merge() sous forme d'opérateur. */
	GLamCSG& operator |=(const GLamObject& obj ) { return *merge(&obj) ; }
	/*! Surcharge de la méthode subtract() sous forme d'opérateur. */
	GLamCSG& operator -=(const GLamObject& obj ) { return *subtract(&obj) ; }
	/*! Surcharge de la méthode intersect() sous forme d'opérateur. */
	GLamCSG& operator &=(const GLamObject& obj ) { return *intersect(&obj) ; }

  protected:
	virtual void create() ;

  private:
	int addPrimitive(const GLamObject* obj ) ;

	CSGModel modelFromPolygons(const QList<GLamPolygon>& polygons, int id ) ;

  private:
	QList<CSGModel>	m_models ;
} ;

#endif

/*!	@class GLamCSG
	@brief Composition d'objet par ajout/enlèvement de matière.

La géométrie de construction de solides (CSG: @e Constructive @e Solid
@e Geometry) est une branche de la modélisation des solides.
Cette technique de modélisation géométrique concerne la représentation d'un
objet solide comme combinaison d'objets solides simples à l'aide d'opérateurs
géométriques booléens.
@n La classe GLamCSG matérialise cette technique.
C'est une spécialisation de GLamGroup permettant de construire
un objet complexe par combinaisons de plusieurs objets de base des branches
GLamPrism et GLamRevolution. Les combinaisons
possibles sont l'union, l'intersection et la soustraction.

<h2 class="groupheader">Exemple</h2>
<table border="0"><tr>
<td width="35%">
@code
// extrait de csgsample.h

class CsgSample
{
  public:
	CsgSample() ;
	void draw() ;

  private:
	GLamCube*		m_primBox ;
	GLamSphere*		m_primSph ;
	GLamCylinder*	m_primCyl ;
	GLamCSG*		m_csg[6] ;
} ;

// extrait de csgsample.cpp : projection

void CsgSample::draw()
{
	m_primBox->draw() ;
	m_primSph->draw() ;
	m_primCyl->draw() ;
	for ( int i = 0 ; i < 6 ; ++i )	m_csg[i]->draw() ;
}

@endcode
</td>
<td width="45%">
@code
// extrait de csgsample.cpp : construction

CsgSample::CsgSample()
{
	m_primBox = new GLamCube(1.8) ;
	m_primSph = new GLamSphere(2.4) ;
	m_primCyl = new GLamCylinder(0.6, 2.4 ) ;
	// ...

	for ( int i = 0 ; i < 6 ; ++i )	m_csg[i] = new GLamCSG( m_primBox ) ;

	m_csg[0]->intersect( m_primSph ) ;
	m_csg[1]->subtract( m_primSph ) ;
	m_csg[2]->subtract( m_primCyl ) ;

	GLamSphere* primSph = new GLamSphere( *m_primSph ) ;
	primSph->setDiameter(2.6) ;

	m_csg[3]->subtract( m_primSph )->intersect( primSph ) ;

	GLamCylinder* primCyl = new GLamCylinder( *m_primCyl ) ;

	m_csg[4]->subtract( primCyl ) ;
	primCyl->defaultMatrix().rotate(90, 0, 1, 0 ) ;

	( *m_csg[4] -= *primCyl ) &= *primSph;

	primSph->defaultMatrix().translate(1, 1, 1 ) ;
	m_csg[5]->subtract( primSph ) ;
	primCyl->defaultMatrix().setToIdentity() ;
	primCyl->defaultMatrix().translate(-1, -1, 0 ) ;
	primCyl->defaultMatrix().rotate(-15, 1, 0, 0 ) ;
	m_csg[5]->subtract( primCyl ) ;

	delete primSph ;
	delete primCyl ;
	// ...
}
@endcode
</td>
<td>@image html "GLamCSG_example.png" width=300px</td>
</tr>
</table>
*/
