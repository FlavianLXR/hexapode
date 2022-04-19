#ifndef FSMUCONSOLE_H
#define FSMUCONSOLE_H

#include <GLamObjects>
#include <GLamGroup>
#include <glamphoto.h>


// FlighSim Mock-up : console de commande

class FSmuConsole : public GLamGroup
{
    public:
    FSmuConsole();

    void paintOver(QPainter* painter ) ;

    private:
    GLamSolid*		m_socle ;
    GLamSolid*		m_instru ;
    GLamPhoto*      m_texture ;
    GLamPhoto*      m_texture2 ;

};

#endif // FSMUCONSOLE_H
