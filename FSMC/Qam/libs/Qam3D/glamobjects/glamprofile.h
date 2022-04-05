/*  ---------------------------------------------------------------------------
 *  filename    :   glamprofile.h
 *  description :   INTERFACE de la classe GLamProfile
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

#ifndef GLAMPROFILE_H
#define GLAMPROFILE_H

#include "glamobject.h"

class GLamSolid ;
class GLamHole ;

class GLamProfile : public GLamObject
{
  public:
	GLamProfile() ;
	GLamProfile(qreal thickness ) ;
	GLamProfile(const QList<QPointF>& points, uint numSlices = 0 ) ;
	GLamProfile(QVector4D v0, QVector4D v1, uint numSlices = 0 ) ;
	GLamProfile(const GLamProfile& obj ) ;
	~GLamProfile() ;

	GLamProfile& operator =(const GLamProfile& obj ) ;

	void extrudeAlong(qreal length ) ;
	void extrudeAlong(QVector4D begin, QVector4D end ) ;
	void extrudeAlong(const QList<QPointF>& points ) ;
	void extrudeAlong(const QList<QPointF>& points, QVector2D begin, QVector2D end ) ;
	void extrudeOutside(const QList<QPointF>& points ) ;
	void extrudeInside(const QList<QPointF>& points ) ;

    /*! Propriété @ref profile_closed "closed". */
	bool isClosed() const ;
    /*! Propriété @ref profile_closed "closed". */
    void setClosed(bool closed = true ) ;

  protected:
	virtual void create() ;

  private:
	QList<QPointF>		m_pts ;
	uint				m_numSlices ;
	bool				m_closed ;
	QList<QVector4D>	m_path ;

	friend class GLamSolid ;
	friend class GLamHole ;
} ;

#endif // GLAMPROFILE_H

/*!
  @class GLamProfile
  @brief Profilé périphérique.

La classe GLamProfile permet la construction d'un profilé à partir de la
description de sa section transversale dans le plan 0XY.
Un objet GLamProfile est destiné principalement à habiller les contours des
objets GLamSolid dont les bords sont par défaut droits verticaux.
@n Le profil peut aussi être utilisé seul en tant que section extrudée le long
d'un chemin, dans ce cas la section peut être fermée (propriété @c closed) aux
extrémités.

Un profil est assimilable à une pièce de tôle laminée, il peut être construit
de plusieurs manières :
@n - par 4 points ;
@n - par une enveloppe segmentée ;
@n - par une courbe de Bézier entre deux points ;
@n - ou par une courbe de bSpline d'ordre 2.

<hr><h2>Propriétés</h2>

<p>@anchor profile_closed @c closed : Fermeture aux extrémités.
- bool @b isClosed () const ;
- void @b setClosed(bool @a closed = true ) ;

<hr><h2>Exemple</h2>
<table border="0" width="100%" cellspacing=20px ><tr>
<td>@image html "GLamProfile_example.png" width=400px </td>
<td>
@code
// <<< SUITE DE L'EXEMPLE DE LA CLASSE GLamSolid >>>

// attributs
    GLamSolid*  m_profile[3] ;

// construction
    m_profile[0] = new GLamSolid( *m_solid[0] ) ;   // profil segmenté
    qreal e = m_profile[0]->thickness() ;
    pts.clear() ;
    pts << QPointF() << QPointF(0.05, 0.05 ) ;
    pts << QPointF(0.05, e - 0.05 ) << QPointF(0.0, e ) ;
    GLamProfile edge( pts ) ;
    m_profile[0]->setProfile( &edge ) ;

    m_profile[1] = new GLamSolid( *m_solid[1] ) ;   // profil type Bézier
    e = m_profile[1]->thickness() ;
    pts.clear() ;
    pts << QPointF(0, -e / 2 ) ;
    pts << QPointF(0.1, -e / 2  ) ;
    pts << QPointF(0.1, -e / 2  + 0.1 ) ;
    pts << QPointF(0.01, -e / 2  + 0.1 ) ;
    pts << GLam2D::xMirrored(pts) ;
    pts = GLam2D::translate(pts, 0, e / 2 ) ;
    edge = GLamProfile(pts, 20 ) ;
    m_profile[1]->setProfile( &edge ) ;

    m_profile[2] = new GLamSolid( *m_solid[2] ) ;   // profil type bSpline
    e = m_profile[2]->thickness() ;
    edge = GLamProfile( QVector4D(0, 0, 0.25, 0 ), QVector4D(0, e, 0.25, 0 ), 12 ) ;
    m_profile[2]->setProfile( &edge ) ;

// projection
    for( int i = 0 ; i < 3 ; ++i )  m_profile[i]->draw() ;

@endcode
</td></tr>
</table>

@n
*/
