#include "fsmuhandle.h"

FSmuHandle::FSmuHandle()
    : GLamGroup()
    , m_diameter( 27 )
{

    qreal	E = 370 ;
    qreal	e = 230 ;
    qreal	a = -20 ;

    QVector3D	p0( 0, 0, 0 ) ;
    QVector3D	v0( a, 0, 0 ) ;
    QVector3D	p1( 0, 0, E ) ;
    QVector3D	v1( e, 0, 0 ) ;
    m_wire = new GLamWire(BezierCurve(p0,v0,p1,v1), m_diameter ) ;
    m_wire->defaultMatrix().translate(0,0,100) ;
   // m_wire->material().setColor(Qt::red) ;
    this->addObject( m_wire ) ;

    m_primCyl = new GLamCylinder(15, 50 ) ;
    m_primCyl->transformMatrix().translate( -5, 0, 455) ;

    //m_primCyl->setMaterial( GLamMaterial::BlackPlastic ) ;

    this->addObject( m_primCyl ) ;


    m_stlImport4 = new GLamStL(":/Manche-stl.stl") ;
    m_stlImport4->defaultMatrix().scale(1) ;
    m_stlImport4->defaultMatrix().translate(-30,40,500) ;
    m_stlImport4->defaultMatrix().rotate(-90, 0, 0, 1 ) ;
    m_stlImport4->material().set(GLamMaterial::Obsidian ) ;

    GLamMesh mesh4( m_stlImport4->toTrimesh() ) ;
    this->addObject( &mesh4 ) ;


  //base manche

    m_stlImport = new GLamStL(":/CY600-stl.stl") ;
    m_stlImport->defaultMatrix().scale(1) ;
    m_stlImport->defaultMatrix().rotate(90, 1, 0, 0 ) ;
    m_stlImport->material().set(GLamMaterial::BlackRubber ) ;
    //m_stlImport->material().setTransparency( 0.8 ) ;

    GLamMesh mesh( m_stlImport->toTrimesh() ) ;
    this->addObject( &mesh ) ;


    m_stlImport2 = new GLamStL(":/CY610-stl.stl") ;
    m_stlImport2->defaultMatrix().scale(1) ;
    //m_stlImport2->defaultMatrix().rotate(90, 1, 0, 0 ) ;
    m_stlImport2->defaultMatrix().translate(0,0,-5) ;
    m_stlImport2->material().set(GLamMaterial::BlackPlastic ) ;
    //m_stlImport->material().setTransparency( 0.6 ) ;

    GLamMesh mesh2( m_stlImport2->toTrimesh() ) ;
    this->addObject( &mesh2 ) ;


    m_stlImport3 = new GLamStL(":/CY620-stl.stl") ;
    m_stlImport3->defaultMatrix().scale(1) ;
    //m_stlImport2->defaultMatrix().rotate(90, 1, 0, 0 ) ;
    m_stlImport3->defaultMatrix().translate(0,0,-5) ;
    m_stlImport3->material().set(GLamMaterial::BlackPlastic ) ;
    //m_stlImport->material().setTransparency( 0.6 ) ;

    GLamMesh mesh3( m_stlImport3->toTrimesh() ) ;
    this->addObject( &mesh3 ) ;
}
