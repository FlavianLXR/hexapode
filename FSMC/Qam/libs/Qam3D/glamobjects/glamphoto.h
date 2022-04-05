#ifndef GLAMPHOTO_H
#define GLAMPHOTO_H

//#include "glamobject.h"
#include <GLamObjects>

//class GLamPhoto : public GLamObject
class GLamPhoto : public GLamCube
{
    public:
    GLamPhoto() ;
    GLamPhoto(int w, int h ) ;
//    GLamPrism(const QList<QPointF>& baseShape);

    void setTexture(const QString& filename ) ;

    protected:
//    virtual void create() ;

    private:
//    QList<QPointF>		m_baseShape ;
//    QList<QPointF>		m_topShape ;
    qreal				m_w ;
    qreal				m_h ;

};

#endif // GLAMPHOTO_H
