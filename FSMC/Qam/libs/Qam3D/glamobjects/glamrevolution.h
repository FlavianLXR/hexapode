/*  ---------------------------------------------------------------------------
 *  filename    :   glamrevolution.h
 *  description :   INTERFACE de la classe GLamRevolution
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

#ifndef GLAMREVOLUTION_H
#define GLAMREVOLUTION_H

#include "glamobject.h"
#include "../glamtools/glampolygon.h"

// classe de base des objets de section radiale constante
// définie dans 0XZ (demi-plan X+), centrage sur l'origine 0XY
// centrage vertical p/r 0Z si m_zCentered

class GLamRevolution : public GLamObject
{
  public:
	GLamRevolution() ;
	GLamRevolution(const QList<QPointF>& points, uint numSlices = 0 ) ;
	GLamRevolution(QVector4D v0, QVector4D v1, uint numSlices = 0 ) ;
	GLamRevolution(const GLamRevolution& obj ) ;
	~GLamRevolution() ;

	GLamRevolution& operator =(const GLamRevolution& obj ) ;

    qreal angle() const ;
    void setAngle(qreal angle ) ;

    qreal eastAngle() const ;
	qreal westAngle() const ;
    void setEastAngle(qreal eastAngle ) ;
	void setWestAngle(qreal westAngle ) ;
    void setLongitudes(qreal eastAngle, qreal westAngle ) ;

    /*! Propriété @ref revolution_closed "closed". */
	bool isClosed() const ;
    /*! Propriété @ref revolution_closed "closed". */
	void setClosed(bool closed = true ) ;

	qreal innerDiameter() const ;
	qreal outerDiameter() const ;

    /*! Propriété @ref revolution_numSectors "numSectors". */
	uint numSectors() const ;
    /*! Propriété @ref revolution_numSlices "numSlices". */
	uint numSlices() const ;
    /*! Propriété @ref revolution_numSectors "numSectors". */
	void setNumSectors(uint numSectors ) ;

    virtual void setNumSlices(uint numSlices ) ;
    virtual void setNumFaces(uint numFaces ) ;

    void setZCentered(bool centered = true ) ;

	QList<QPointF> points() const ;

	QList<GLamPolygon> polygons(QMatrix4x4 transform = QMatrix4x4() ) const ;

	void setTexture(const QString& filename ) ;

  protected:
    void setPoints(const QList<QPointF>& points ) ;
	virtual void create() ;

  private:
	void calculateRadius() ;
	void calculateZMin() ;

  private:
    QList<QPointF>	m_pts ;            // section radiale dans 0XZ (demi-plan X+)
	uint			m_numSectors ;
	uint			m_numSlices ;
    qreal			m_eastAngle ;
    qreal			m_westAngle ;
	qreal			m_innerRadius ;
	qreal			m_outerRadius ;
	bool			m_closed ;
	bool			m_zCentered ;
	qreal			m_zMin ;
} ;

#endif // GLAMREVOLUTION_H

/*!
@class GLamRevolution
@brief Classe de base des objets de section radiale constante.

GLamRevolution est une classe générique de définition d'objets 3D ayant
une section radiale constante. Un objet est défini par un profil radial
dans 0XZ sous forme d'une suite de points 2D,
l'axe vertical de la pièce est à l'origine 0XY, la révolution est par
défaut complète (360°).
@n Si l'amplitude de révolution est incomplète,
la classe peut générer ou non les surfaces radiales de fermeture de l'objet
en fonction de la propriété @c closed (@a true par défaut).

<hr><h2>Propriétés</h2>

<p>@anchor revolution_numSectors @c numSectors : Nombre de facettes (secteurs)
pour une révolution complète. La valeur par défaut est fixée à 30, elle ne peut être
inférieure à 3.
- uint @b numSectors () const ;
- void @b setNumSectors (uint @a numSectors ) ;

<p>@anchor revolution_numSlices @c numSlices : Nombre de tranches suivant 0Z.
- uint @b numSlices () const ;
- void @b setNumSlices (uint @a numSlices ) ;

<p>@anchor revolution_angle @c angle : Amplitude de révolution en degrés (défaut 360).
Elle peut être spécifiée par l'angle signé (sens trigo) par rapport à l'axe 0X ou
par des valeurs 0..180° de part et d'autre de ce même axe (longitudes Est et Ouest).
- qreal @b angle () const ;
- void @b setAngle (qreal @a angle ) ;
- qreal @b eastAngle () const ;
- qreal @b westAngle () const ;
- void @b setEastAngle (qreal @a eastAngle ) ;
- void @b setWestAngle (qreal @a westAngle ) ;
- void @b setLongitudes (qreal @a eastAngle, qreal @a westAngle ) ;

<p>@anchor revolution_closed @c closed : Fermeture radiale du solide (@a true par défaut).
Cette propriété n'a d'effet que lorsque la révolution est incomplète.
- bool @b isClosed () const ;
- void @b setClosed (bool @a closed = true ) ;

<hr><h2>Exemple</h2>
<table border="0" width="100%" cellspacing=20px ><tr>
<td>@image html "glamrevolution_example.png" width=400px </td>
<td>
@code
// attributs
    GLamRevolution* m_revolution[4] ;

// construction
    QList<QPointF> points ;
    points << QPointF(0.5,0) << QPointF(1.5,0) << QPointF(1.5,1) ;
    points << QPointF(1,1) << QPointF(1,0.5) << QPointF(0.6,0.5) << QPointF(0.5,0.4) ;

    // pièce mécanique
    m_revolution[0] = new GLamRevolution(points ) ;
    m_revolution[0]->defaultMatrix().translate(-0.5,0.5,0 ) ;
    m_revolution[0]->defaultMatrix().rotate(45, 0, 0, 1 ) ;
    m_revolution[0]->setLongitudes(10,130) ;
    m_revolution[0]->material().setColor(0.3, 0.6, 0.9 ) ;
    m_revolution[0]->material().setShiny() ;

    // galet
    m_revolution[1] = new GLamRevolution( QVector4D(0,0,1.5,0), QVector4D(0,0.75,1.5,0), 30 ) ;
    m_revolution[1]->setAngle(300) ;
    m_revolution[1]->defaultMatrix().translate( -1, -1.5, 0 ) ;
    m_revolution[1]->defaultMatrix().rotate(30, 0, 0, 1 ) ;
    m_revolution[1]->material().setColor(0.7, 0.4, 0.3 ) ;

    // pomme
    m_revolution[2] = new GLamRevolution( QVector4D(0,0,1,0), QVector4D(0,1,1,0.75), 90 ) ;
    m_revolution[2]->defaultMatrix().translate( 1.5, -1, 0 ) ;
    m_revolution[2]->material().setColor(0.7, 0.1, 0.2 ) ;

    // queue de la pomme
    points.clear() ;
    points << QPointF(0,1) << QPointF(0.02,1) << QPointF(0.05,1.5) << QPointF(0,1.5) ;
    m_revolution[3] = new GLamRevolution(points ) ;
    m_revolution[3]->defaultMatrix().translate( 1.5, -1, 0 ) ;
    m_revolution[3]->material().setColor(0.1, 0.3, 0.2 ) ;

// projection
    for (int i = 0 ; i < 4 ; ++i )  m_revolution[i]->draw() ;

@endcode
</td></tr>
</table>

@n
*/
