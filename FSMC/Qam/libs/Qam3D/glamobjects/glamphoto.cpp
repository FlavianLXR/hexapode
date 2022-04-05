#include "glamphoto.h"

GLamPhoto::GLamPhoto()
//    : GLamObject()
    : GLamCube(100,100,10)
    , m_w(100)
    , m_h(100)
{

}

GLamPhoto::GLamPhoto(int w, int h )
//    : GLamObject()
    : GLamCube(w,h,10)
    , m_w(w)
    , m_h(h)
{

}
/*
void GLamPhoto::create()
{
    QVector2D v1(0,0) ;
    QVector2D v2(m_w,0) ;
    QVector2D v3(m_w,m_h) ;
    QVector2D v4(0,m_h) ;
    quad(v1, v2, v3, v4 ) ;
}
*/
void GLamPhoto::setTexture(const QString& filename )
{
    this->material().setTexture( filename ) ;

    QVector2D t1(0,0) ;
    QVector2D t2(1,0) ;
    QVector2D t3(1,1) ;
    QVector2D t4(0,1) ;
//    quadTex(t1, t2, t3, t4 ) ;
    triTex(t1,t2,t3);
    triTex(t1,t3,t4) ;
    triTex(t1,t2,t3);
    triTex(t1,t3,t4) ;
}
