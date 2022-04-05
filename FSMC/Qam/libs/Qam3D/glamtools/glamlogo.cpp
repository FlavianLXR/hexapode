/*  ---------------------------------------------------------------------------
 *  filename    :   glamlogo.cpp
 *  description :   IMPLEMENTATION de la classe GLamLogo
 *
 *	project     :	Qam3D/GLam: 3D object collection
 *  start date  :   février 2010
 *  ---------------------------------------------------------------------------
 *  Copyright 2010-2020 by Alain Menu   <alain.menu@ac-creteil.fr>
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

#include "glamlogo.h"

GLamLogo::GLamLogo(const QString& text )
    : GLamGroup()
{
    // points 2D de construction du logo

  #define PTS_BASE  7
  #define PTS_G     ( PTS_BASE + 14 )
  #define PTS_L     ( PTS_G + 6 )
  #define PTS_A     ( PTS_L + 19 )
  #define PTS_M     ( PTS_A + 17 )

    double pts[][2] = {
        // base = 7
        { 4.50,0.00}, { 5.00,0.50}, { 5.00,0.75}, { 0.00,7.50}, {-5.00,0.75},
        {-5.00,0.50}, {-4.50,0.00},
        // lettre G = 14
        {-2.75,0.00}, {-2.00,2.00}, {-2.75,2.00}, {-3.00,1.50}, {-2.65,1.50},
        {-3.10,0.50}, {-4.15,0.50}, {-2.50,3.25}, { 3.20,3.25}, { 2.75,3.75},
        {-2.75,3.75}, {-5.00,0.75}, {-5.00,0.50}, {-4.50,0.00},
        // lettre L = 6
        { 4.50,0.00}, { 5.00,0.50}, {-1.50,0.50}, {-1.00,3.00}, {-1.35,3.00},
        {-2.25,0.00},
        // lettre A = 19
        { 1.25,0.75}, { 1.00,1.25}, { 0.75,2.50}, { 0.50,2.85}, { 0.25,3.00},
        {-0.25,3.00}, {-0.55,2.75}, {-0.25,2.40}, { 0.00,2.50}, { 0.25,2.40},
        { 0.35,2.25}, { 0.25,2.00}, {-0.35,2.00}, {-0.65,1.75}, {-0.75,1.10},
        {-0.45,0.75}, { 0.25,0.75}, { 0.50,0.90}, { 0.60,0.75},
        // lettre M = 17
        { 5.00,0.75}, { 3.70,2.50}, { 3.30,2.85}, { 2.70,3.00}, { 2.35,2.75},
        { 2.00,3.00}, { 1.15,3.00}, { 1.75,0.75}, { 2.50,0.75}, { 1.85,2.25},
        { 2.25,2.25}, { 3.10,0.75}, { 3.60,0.75}, { 2.65,2.25}, { 3.00,2.25},
        { 3.40,2.00}, { 4.25,0.75}
    } ;

    int n = 0 ;
    QList<QPointF> points ;
    for ( ; n < PTS_BASE ; ++n )  points << QPointF(pts[n][0], pts[n][1] ) ;

    QList<QPointF> shape = GLam2D::roundedShape(points, 30, 0.05 ) ;
    shape = GLam2D::expand(shape, 0.05 ) ;
    GLamSolid* base = new GLamSolid(shape, 0.5 ) ;
    base->defaultMatrix().translate(0, 0, -0.01 ) ;
    base->material().set(GLamMaterial::Jade ) ;

    points.clear() ; shape.clear() ; n = PTS_BASE ;
    for ( ; n < PTS_G ; ++n )  points << QPointF(pts[n][0], pts[n][1] ) ;
    shape = GLam2D::roundedShape(points, 30, 0.1 ) ;
    GLamSolid* letter_g = new GLamSolid(shape, 0.7 ) ;
    letter_g->material().set(GLamMaterial::Gold ) ;

    points.clear() ; shape.clear() ; n = PTS_G ;
    for ( ; n < PTS_L ; ++n )  points << QPointF(pts[n][0], pts[n][1] ) ;
    shape = GLam2D::roundedShape(points, 30, 0.1 ) ;
    GLamSolid* letter_l = new GLamSolid(shape, 0.7 ) ;
    letter_l->material().set(GLamMaterial::Gold ) ;

    points.clear() ; shape.clear() ; n = PTS_L ;
    for ( ; n < PTS_A ; ++n )  points << QPointF(pts[n][0], pts[n][1] ) ;
    shape = GLam2D::roundedShape(points, 50, 0.1 ) ;
    GLamSolid* letter_a = new GLamSolid(shape, 0.7 ) ;

//  version avec addHole() sans décalage... -> ok
//    points.clear() ;
//    points << GLam2D::translate( GLam2D::circularShape(0.3,30), 0, 1.35 ) ;
//    GLamHole* m_hole = new GLamHole( points, 0.7 )  ;
//    letter_a->addHole(m_hole ) ;
    GLamHole* m_hole = new GLamHole( GLam2D::circularShape(0.3,30), 0.7 )  ;
    letter_a->addHole(m_hole, QPointF(0,1.35) ) ;

    letter_a->material().set(GLamMaterial::Ruby ) ;

    points.clear() ; shape.clear() ; n = PTS_A ;
    for ( ; n < PTS_M ; ++n )  points << QPointF(pts[n][0], pts[n][1] ) ;
    shape = GLam2D::roundedShape(points, 50, 0.1 ) ;
    GLamSolid* letter_m = new GLamSolid(shape, 0.7 ) ;
    letter_m->material().set(GLamMaterial::Ruby ) ;

    this->addObject( base ) ;
    this->addObject( letter_g ) ;
    this->addObject( letter_l ) ;
    this->addObject( letter_a ) ;
    this->addObject( letter_m ) ;

    if ( !text.isEmpty() ) {
        GLamText*   m_text = new GLamText( QString("v %1").arg(text) ) ;
        m_text->setFontSize( 1.2 ) ;
        m_text->setThickness( 0.5 ) ;
        float decalx = -m_text->width() / 2.0 ;
        m_text->defaultMatrix().translate( decalx, 4.2, 0.1 ) ;
        this ->addObject( m_text ) ;
    }

    defaultMatrix().scale(0.4) ;
    defaultMatrix().translate(0, -4, 0 ) ;
}
