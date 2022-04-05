#include "fsmuseat.h"

FSmuSeat::FSmuSeat()
    : GLamGroup()
{
    QList<QPointF> shape;
//  Base

    shape <<QPoint( 100,0 ) <<QPoint (445,0) <<QPoint (490,220)
         <<QPoint(100,220) ;

    m_base = new GLamSolid (shape,290 ) ;
    m_base->defaultMatrix().rotate(90, 1, 0, 0 ) ;
    m_base->transformMatrix().translate( 0, 0, 90) ;

    m_base->setMaterial( GLamMaterial::BlackRubber ) ;

    this->addObject( m_base ) ;

// Squelette

    //Gauche horizontal
    m_primCyl[1] = new GLamCylinder(10, 720 ) ;
    m_primCyl[1]->defaultMatrix().rotate(-10, 0, 1, 0 ) ;
    m_primCyl[1]->transformMatrix().translate( 50, -60, 0) ;

    m_primCyl[1]->setMaterial( GLamMaterial::BlackPlastic ) ;

    addObject( m_primCyl[1] ) ;

    //droite horizontal
    m_primCyl[2] = new GLamCylinder(10, 720 ) ;
    m_primCyl[2]->defaultMatrix().rotate(-10, 0, 1, 0 ) ;
    m_primCyl[2]->transformMatrix().translate( 50, -410, 0) ;

    m_primCyl[2]->setMaterial( GLamMaterial::BlackPlastic ) ;

    addObject( m_primCyl[2] ) ;

    m_primCyl[3] = new GLamCylinder(10, 370 ) ;
    m_primCyl[3]->defaultMatrix().rotate(90, 1, 0, 0 ) ;
    m_primCyl[3]->transformMatrix().translate( 48, 10, 50) ;

    m_primCyl[3]->setMaterial( GLamMaterial::BlackPlastic ) ;

    addObject( m_primCyl[3] ) ;

    m_primCyl[4] = new GLamCylinder(10, 370 ) ;
    m_primCyl[4]->defaultMatrix().rotate(90,1, 0, 0 ) ;
    m_primCyl[4]->transformMatrix().translate( -75, 720, 50) ;

    m_primCyl[4]->setMaterial( GLamMaterial::BlackPlastic ) ;

    addObject( m_primCyl[4] ) ;

    m_primCyl[5] = new GLamCylinder(10, 105 ) ;
    m_primCyl[5]->defaultMatrix().rotate(90,1, 0, 0 ) ;
    m_primCyl[5]->defaultMatrix().rotate(-242,0, 1, 0 ) ;
    m_primCyl[5]->transformMatrix().translate( -57, 235, -20) ;

    m_primCyl[5]->setMaterial( GLamMaterial::BlackPlastic ) ;

    addObject( m_primCyl[5] ) ;

    m_primCyl[6] = new GLamCylinder(10, 105 ) ;
    m_primCyl[6]->defaultMatrix().rotate(90,1, 0, 0 ) ;
    m_primCyl[6]->defaultMatrix().rotate(62,0, 1, 0 ) ;
    m_primCyl[6]->transformMatrix().translate( -358, 235, 200) ;

    m_primCyl[6]->setMaterial( GLamMaterial::BlackPlastic ) ;

    addObject( m_primCyl[6] ) ;

    m_primCyl[7] = new GLamCylinder(10, 370 ) ;
    m_primCyl[7]->defaultMatrix().rotate(90, 1, 0, 0 ) ;
    m_primCyl[7]->transformMatrix().translate( 15, 235, 50) ;

    m_primCyl[7]->setMaterial( GLamMaterial::BlackPlastic ) ;

    addObject( m_primCyl[7] ) ;

    m_primCyl[8] = new GLamCylinder(10, 410 ) ;
    m_primCyl[8]->defaultMatrix().rotate(90,1, 0, 0 ) ;
    m_primCyl[8]->defaultMatrix().rotate(90,0, 1, 0 ) ;
    m_primCyl[8]->transformMatrix().translate( -58, 10, 50) ;

    m_primCyl[8]->setMaterial( GLamMaterial::BlackPlastic ) ;

    addObject( m_primCyl[8] ) ;

    m_primCyl[9] = new GLamCylinder(10, 410 ) ;
    m_primCyl[9]->defaultMatrix().rotate(90,1, 0, 0 ) ;
    m_primCyl[9]->defaultMatrix().rotate(90,0, 1, 0 ) ;
    m_primCyl[9]->transformMatrix().translate( -412, 10, 50) ;

    m_primCyl[9]->setMaterial( GLamMaterial::BlackPlastic ) ;

    addObject( m_primCyl[9] ) ;

    m_primCyl[10] = new GLamCylinder(10, 230 ) ;
    m_primCyl[10]->defaultMatrix().rotate(10, 0, 1, 0 ) ;
    m_primCyl[10]->transformMatrix().translate( 440, -58, 75) ;

    m_primCyl[10]->setMaterial( GLamMaterial::BlackPlastic ) ;

    addObject( m_primCyl[10] ) ;

    m_primCyl[11] = new GLamCylinder(10, 230 ) ;
    m_primCyl[11]->defaultMatrix().rotate(10, 0, 1, 0 ) ;
    m_primCyl[11]->transformMatrix().translate( 440, -412, 75) ;

    m_primCyl[11]->setMaterial( GLamMaterial::BlackPlastic ) ;

    addObject( m_primCyl[11] ) ;

    m_primCyl[12] = new GLamCylinder(10, 370 ) ;
    m_primCyl[12]->defaultMatrix().rotate(90, 1, 0, 0 ) ;
    m_primCyl[12]->transformMatrix().translate( 450, 10, 50) ;

    m_primCyl[12]->setMaterial( GLamMaterial::BlackPlastic ) ;

    addObject( m_primCyl[12] ) ;



//  Back
    shape.clear();
    shape <<QPoint( 100,220 ) <<QPoint (490,220) <<QPoint (500,230) <<QPoint (480,300) <<QPoint(460,320)
          <<QPoint(170,260) <<QPoint(140,270) <<QPoint(-50,720) <<QPoint(-70,740) ;

    m_back = new GLamSolid (shape,470 ) ;
    m_back->defaultMatrix().rotate(90, 1, 0, 0 ) ;



    m_back->setMaterial( GLamMaterial::CyanPlastic );

    this->addObject( m_back ) ;

}
