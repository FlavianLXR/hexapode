// csg.h

// Constructive Solid Geometry
// ---------------------------------------------------------------------------
// original CSG.JS library by Evan Wallace under MIT license.
// GitHub: https://github.com/evanw/csg.js/
// ---------------------------------------------------------------------------

#ifndef LIBCSG_H
#define LIBCSG_H

#include <QVector>
#include <QVector3D>

#define CSG_EPSILON		0.00001f

#define CSG_COPLANAR	0
#define CSG_FRONT		1
#define CSG_BACK		2
#define CSG_SPANNING	3

struct CSGVertex
{
	QVector3D	pos ;
	QVector3D	normal ;
	int			solidId ;

	CSGVertex() ;
	CSGVertex(QVector3D p, QVector3D n, int id = 0 ) ;
} ;

struct CSGModel
{
	QVector<CSGVertex> vertices ;
} ;

struct CSGPolygon ;

struct CSGPlane
{
	QVector3D	normal ;
	float		w ;

	CSGPlane() ;
	CSGPlane(const QVector3D& a, const QVector3D& b, const QVector3D& c) ;
	inline bool isOk() const ;
	void flip() ;
	void splitPolygon(const CSGPolygon& polygon, QVector<CSGPolygon>& coplanarFront, QVector<CSGPolygon>& coplanarBack, QVector<CSGPolygon>& front, QVector<CSGPolygon>& back ) const ;
} ;

struct CSGPolygon
{
	QVector<CSGVertex>	vertices ;
	CSGPlane			plane ;
	int					solidId ;

	CSGPolygon() ;
	CSGPolygon(const QVector<CSGVertex>& list, int id = 0 ) ;

	void flip() ;
};

struct CSGNode
{
	QVector<CSGPolygon> polygons ;
	CSGNode*			front ;
	CSGNode*			back ;
	CSGPlane			plane ;

	CSGNode() ;
	CSGNode(const QVector<CSGPolygon>& list ) ;
	~CSGNode() ;

//	CSGNode* clone() const ;
	void clipTo(const CSGNode* other ) ;
	void invert() ;
	void build(const QVector<CSGPolygon>& polygon ) ;
	QVector<CSGPolygon> clipPolygons(const QVector<CSGPolygon>& list ) const ;
	QVector<CSGPolygon> allPolygons() const ;
} ;

// ---------------------------------------------------------------------------

QVector<CSGPolygon> csgModelToPolygons(const CSGModel& model ) ;
CSGModel csgModelFromPolygons(const QVector<CSGPolygon>& polygons ) ;

CSGModel csgUnion(const CSGModel& model1, const CSGModel& model2 ) ;
CSGModel csgIntersection(const CSGModel& model1, const CSGModel& model2 ) ;
CSGModel csgDifference(const CSGModel& model1, const CSGModel& model2 ) ;

#endif	// LIBCSG_H
