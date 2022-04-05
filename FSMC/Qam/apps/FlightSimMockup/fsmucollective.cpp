#include "fsmucollective.h"

FSmuCollective::FSmuCollective()
        : GLamGroup()
{

    m_primCyl[1] = new GLamCylinder(13.5, 160 ) ;
    m_primCyl[1]->defaultMatrix().rotate(-90, 0, 1, 0 ) ;
    m_primCyl[1]->transformMatrix().translate( 190, 0, 0) ;

    //m_primCyl[1]->setMaterial( GLamMaterial::BlackPlastic ) ;

    this->addObject( m_primCyl[1] ) ;

    m_primCyl[2] = new GLamCylinder(13.5, 300 ) ;
    m_primCyl[2]->defaultMatrix().rotate(-90, 0, 1, 0 ) ;
    m_primCyl[2]->defaultMatrix().rotate(-40, 0, 1, 0 ) ;
    m_primCyl[2]->transformMatrix().translate( 245, 0, -8) ;

    //m_primCyl[2]->setMaterial( GLamMaterial::BlackPlastic ) ;

    this->addObject( m_primCyl[2] ) ;


}
