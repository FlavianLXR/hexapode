#include "fsmuconsole.h"


FSmuConsole::FSmuConsole()
        : GLamGroup()
{
    QList<QPointF> shape;
//  Socle
    shape <<QPoint( 0,0 ) <<QPoint (570,0) <<QPoint (570,90)
         <<QPoint(340,200) <<QPoint( 240,550) <<QPoint(0,550) ;

    m_socle = new GLamSolid (shape,220 ) ;
    m_socle->defaultMatrix().rotate(90, 1, 0, 0 ) ;
    m_socle->defaultMatrix().rotate(180, 0, 1, 0 ) ;

    m_socle->setMaterial( GLamMaterial::BlackPlastic ) ;

    this->addObject( m_socle ) ;

//  Instruments
    shape.clear();
    shape <<QPoint( 0,0 ) <<QPoint (456,0) <<QPoint (456,290)
     <<QPoint(0,290) ;

    m_instru = new GLamSolid (shape,230 ) ;
    m_instru->defaultMatrix().rotate(90, 1, 0, 0 ) ;
    m_instru->defaultMatrix().rotate(90, 0, 1, 0 ) ;
    m_instru->transformMatrix().translate( -115, 550, -230) ;

    m_instru->setMaterial( GLamMaterial::BlackPlastic );

    this->addObject( m_instru ) ;


// Texture Insturmentation 1

    m_texture = new GLamPhoto (456,290 ) ;
    m_texture->defaultMatrix().rotate(90, 0, 0, 1 ) ;
    m_texture->defaultMatrix().rotate(-90, 1, 0, 0 ) ;
    //m_texture->defaultMatrix().rotate(90, 1, 0, 0 ) ;
    m_texture->transformMatrix().translate( 112, -695, 230) ;


    m_texture->setTexture(":/photo-img.jpg") ;
    //m_texture->defaultMatrix().scale(1) ;


    this->addObject( m_texture ) ;


    // Texture Insturmentation 2

        m_texture2 = new GLamPhoto (220,150 ) ;
        m_texture2->defaultMatrix().rotate(90, 0, 0, 1 ) ;
        m_texture2->defaultMatrix().rotate(-75, 1, 0, 0 ) ;
        //m_texture->defaultMatrix().rotate(90, 1, 0, 0 ) ;
        m_texture2->transformMatrix().translate( 110,-340, 370) ;


        m_texture2->setTexture(":/photo2-img.jpg") ;
        //m_texture->defaultMatrix().scale(1) ;


        this->addObject( m_texture2 ) ;

}
