#ifndef FSMURUDDER_H
#define FSMURUDDER_H

#include <GLamObjects>
#include <GLamGroup>

class FSmuRudder : public GLamGroup
{
    public:
    FSmuRudder();

    private:
    GLamSolid*      m_solid ;
    GLamSolid*      m_solid2 ;
    GLamCylinder*	m_primCyl[5] ;

};

#endif // FSMURUDDER_H
