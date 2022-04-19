#ifndef FSMUCOLLECTIVE_H
#define FSMUCOLLECTIVE_H

#include <GLamObjects>
#include <GLamGroup>
#include <glamwire.h>



class FSmuCollective : public GLamGroup
{

    public:
    FSmuCollective();

    private:
    GLamCylinder*	m_primCyl[3] ;

};

#endif // FSMUCOLLECTIVE_H
