#include "fsmurudder.h"

FSmuRudder::FSmuRudder()
        : GLamGroup()
{

 // pedale droite
    QList<QPointF> shape;

    shape <<QPoint( 0,0 ) <<QPoint (30,0) <<QPoint (30,188)
         <<QPoint(0,188) ;

    m_solid = new GLamSolid (shape,30 ) ;
    m_solid->defaultMatrix().rotate(90, 1, 0, 0 ) ;
    m_solid->defaultMatrix().rotate(180, 0, 1, 0 ) ;
    m_solid->defaultMatrix().rotate(25, 0, 0, 1 ) ;

    m_solid->setMaterial( GLamMaterial::BlackRubber ) ;

    this->addObject( m_solid ) ;

    m_primCyl[1] = new GLamCylinder(8.5, 100 ) ;
    m_primCyl[1]->defaultMatrix().rotate(90, 1, 0, 0 ) ;
    m_primCyl[1]->transformMatrix().translate( 60, 160, 0) ;

    m_primCyl[1]->setMaterial( GLamMaterial::BlackPlastic ) ;

    addObject( m_primCyl[1] ) ;

    m_primCyl[2] = new GLamCylinder(8.5, 100 ) ;
    m_primCyl[2]->defaultMatrix().rotate(90, 1, 0, 0 ) ;
    m_primCyl[2]->transformMatrix().translate( 35, 110, 0) ;

    m_primCyl[2]->setMaterial( GLamMaterial::BlackPlastic ) ;

    addObject( m_primCyl[2] ) ;


 // pedale gauche

    shape.clear();
    shape <<QPoint( 0,0 ) <<QPoint (30,0) <<QPoint (30,188)
         <<QPoint(0,188) ;

    m_solid2 = new GLamSolid (shape,30 ) ;
    m_solid2->defaultMatrix().rotate(90, 1, 0, 0 ) ;
    m_solid2->defaultMatrix().rotate(180, 0, 1, 0 ) ;
    m_solid2->defaultMatrix().rotate(25, 0, 0, 1 ) ;
    m_solid2->transformMatrix().translate( 0, 0, 85 ) ;

    m_solid2->setMaterial( GLamMaterial::BlackRubber ) ;

    this->addObject( m_solid2 ) ;

    m_primCyl[3] = new GLamCylinder(8.5, 100 ) ;
    m_primCyl[3]->defaultMatrix().rotate(90, 1, 0, 0 ) ;
    m_primCyl[3]->transformMatrix().translate( 35, 110, -210) ;

    m_primCyl[3]->setMaterial( GLamMaterial::BlackPlastic ) ;

    addObject( m_primCyl[3] ) ;

    m_primCyl[4] = new GLamCylinder(8.5, 100 ) ;
    m_primCyl[4]->defaultMatrix().rotate(90, 1, 0, 0 ) ;
    m_primCyl[4]->transformMatrix().translate( 60, 160, -210) ;

    m_primCyl[4]->setMaterial( GLamMaterial::BlackPlastic ) ;

    addObject( m_primCyl[4] ) ;


}
