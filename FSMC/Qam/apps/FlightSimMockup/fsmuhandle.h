#ifndef FSMUHANDLE_H
#define FSMUHANDLE_H

#include <GLamObjects>
#include <GLamGroup>
#include <glamwire.h>

class FSmuHandle : public GLamGroup
{
    public:
    FSmuHandle();


    private:
    GLamStL*		m_stlImport ;
    GLamStL*		m_stlImport2 ;
    GLamStL*		m_stlImport3 ;
    GLamStL*		m_stlImport4 ;
    GLamWire*		m_wire ;
    GLamCylinder*	m_primCyl ;
    qreal           m_diameter ;

};

#endif // FSMUHANDLE_H
