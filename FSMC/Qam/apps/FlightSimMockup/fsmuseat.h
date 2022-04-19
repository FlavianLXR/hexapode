#ifndef FSMUSEAT_H
#define FSMUSEAT_H

#include <QObject>
#include <GLamGroup>
#include <glamwire.h>

class FSmuSeat : public GLamGroup
{
    public:
    FSmuSeat();

    private:
    GLamSolid*		m_base ;
    GLamSolid*		m_back ;
    GLamCylinder*	m_primCyl[14] ;


};

#endif // FSMUSEAT_H
