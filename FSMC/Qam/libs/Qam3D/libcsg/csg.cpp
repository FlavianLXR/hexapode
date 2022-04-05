// csg.cpp

#include "csg.h"

// CSGVertex implementation
// ---------------------------------------------------------------------------

CSGVertex::CSGVertex()
{}

CSGVertex::CSGVertex(QVector3D p, QVector3D n, int id )
	: pos( p ), normal( n ), solidId( id )
{}

// nouveau vertex par interpolation linéaire suivant 0 <= t <= 1

inline static CSGVertex interpolate(const CSGVertex& a, const CSGVertex& b, float t )
{
	CSGVertex ret ;
	ret.pos = a.pos + ( b.pos - a.pos ) * t ;
	ret.normal = a.normal + ( b.normal - a.normal ) * t ;
	ret.solidId = a.solidId ;
	return ret ;
}

// CSGPlane implementation - space separator
// ---------------------------------------------------------------------------

CSGPlane::CSGPlane()
	: normal()
	, w( 0.0f )
{}

// plan défini par 3 points a,b,c

CSGPlane::CSGPlane(const QVector3D& a, const QVector3D& b, const QVector3D& c )
{
	// vecteur normal au plan = produit vectoriel de Vab et Vac
	this->normal = QVector3D::crossProduct(b - a, c - a).normalized() ;
	this->w = QVector3D::dotProduct(this->normal, a ) ;
}

bool CSGPlane::isOk() const
{
	return ( this->normal.length() > 0.0f ) ;
}

// inversion du plan

void CSGPlane::flip()
{
	this->normal = -this->normal ;
	this->w *= -1.0f ;
}

// traitement de la position spatiale d'un polygone p/r au plan courant

void CSGPlane::splitPolygon(
		const CSGPolygon& polygon,
		QVector<CSGPolygon>& coplanarFront,
		QVector<CSGPolygon>& coplanarBack,
		QVector<CSGPolygon>& front,
		QVector<CSGPolygon>& back ) const
{
	int polygonType = 0 ;	// type du polygone à traiter
	QVector<int> types ;	// type de chacun de ces vertices

	// détermination des types, vertex par vertex

	for ( int i = 0 ; i < polygon.vertices.size() ; i++ ) {
		float t = QVector3D::dotProduct(this->normal, polygon.vertices[i].pos ) - this->w ;
		int type = ( t < -CSG_EPSILON ) ? CSG_BACK : ( ( t > CSG_EPSILON ) ? CSG_FRONT : CSG_COPLANAR ) ;
		polygonType |= type ;
		types.push_back(type) ;
	}

	// placement du polygone dans la liste adéquate, avec découpage si nécessaire

	QVector<CSGVertex>	f ;
	QVector<CSGVertex>	b ;

	switch ( polygonType ) {

	case CSG_COPLANAR :	// le polygone est coplanaire avec le plan courant

			if ( QVector3D::dotProduct(this->normal, polygon.plane.normal ) > 0 )
					coplanarFront.push_back(polygon) ;
			else	coplanarBack.push_back(polygon) ;
			break ;

	case CSG_FRONT :	// le polygone est entièrement devant le plan courant

			front.push_back(polygon) ;
			break ;

	case CSG_BACK :		// le polygone est entièrement derrière le plan courant

			back.push_back(polygon) ;
			break ;

	case CSG_SPANNING :	// le polygone est coupé par le plan courant, il doit être scindé

			// pour tous les vertices du polygone
			for ( int i = 0 ; i < polygon.vertices.size() ; i++ ) {
				// séparation de ceux 'devant' et de ceux 'derrière'
				// (un vertex 'COPLANAR' est placé dans les 2 listes)
				CSGVertex vi = polygon.vertices[i] ;
				if (types[i] != CSG_BACK)	f.push_back(vi) ;
				if (types[i] != CSG_FRONT)	b.push_back(vi) ;
				// indice du vertex suivant
				int j = ( i + 1 ) % polygon.vertices.size() ;
				CSGVertex vj = polygon.vertices[j] ;
				// si le vertex suivant n'est pas du même coté que celui en cours
				// (cas où un est 'BACK' et l'autre 'FRONT')
				if ( ( types[i] | types[j] ) == CSG_SPANNING ) {
					// calcul du vertex de séparation par interpolation linéaire
					float t = this->w - QVector3D::dotProduct(this->normal, vi.pos ) ;
					t /= QVector3D::dotProduct(this->normal, vj.pos - vi.pos ) ;
					CSGVertex v = interpolate(vi, vj, t ) ;
					// et rangement de ce vertex dans les 2 listes
					f.push_back(v) ;
					b.push_back(v) ;
				}
			}
			// il faut au moins 3 vertices pour ajouter un nouveau polygone...
			if ( f.size() >= 3 ) front.push_back(CSGPolygon(f, polygon.solidId ) ) ;
			if ( b.size() >= 3 ) back.push_back(CSGPolygon(b, polygon.solidId ) ) ;
			break ;
	}
}

// CSGPolygon implementation - coplanar vectors
// ---------------------------------------------------------------------------

CSGPolygon::CSGPolygon()
{}

CSGPolygon::CSGPolygon(const QVector<CSGVertex> & list, int id )
	: vertices(list)
	, plane(vertices[0].pos, vertices[1].pos, vertices[2].pos)
	, solidId( id )
{}

void CSGPolygon::flip()
{
	QVector<CSGVertex> vv ;
	for ( int i = vertices.size() - 1 ; i >= 0 ; --i ) {
		vv << CSGVertex(vertices.at(i).pos, -vertices.at(i).normal, vertices.at(i).solidId ) ;
	}
	vertices = vv ;
	plane.flip() ;
}

// CSGNode implementation - Binary Space Partitioning (BSP) tree node
// ---------------------------------------------------------------------------

//	Algorithme de construction récursif :

//	1. Choisir un polygone P dans la liste (par exemple le premier...) ;
//
//	2. Créer un noeud N dans l'arbre BSP,
//	   et mettre P dans la liste des polygones de ce noeud ;
//
//	3. Pour chaque polygone de la liste :
//		- Si ce polygone est entièrement devant le plan contenant P,
//		  placer ce polygone dans la liste des polygones devant P
//		- Si ce polygone est entièrement derrière le plan contenant P,
//		  placer ce polygone dans la liste des polygones derrière P
//		- Si le plan contenant P est sécant avec le polygone,
//		  diviser le polygone en deux par le plan de P,
//		  et placer chacun des demi-polygones dans la liste adéquate (devant P, derrière P)
//		- Si le polygone est coplanaire avec P,
//		  l'ajouter à la liste des polygones du noeud N ;
//
//	4. Appliquer cet algorithme à la liste des polygones devant P ;
//
//	5. Appliquer cet algorithme à la liste des polygones derrière P.

CSGNode::CSGNode()
	: front(0)
	, back(0)
{
}

CSGNode::CSGNode(const QVector<CSGPolygon>& list )
	: front(0)
	, back(0)
{
	build(list) ;
}

CSGNode::~CSGNode()
{
	delete front ;
	delete back ;
}

// inversion du BSP tree courant : 'espace solide' <-> 'espace vide'
// (récursif)

void CSGNode::invert()
{
	for (int i = 0 ; i < this->polygons.size() ; i++ ) this->polygons[i].flip() ;
	this->plane.flip() ;
	if ( this->front )	this->front->invert() ;
	if ( this->back )	this->back->invert() ;

	CSGNode*  p = this->front ;
	this->front = this->back ;
	this->back  = p ;
}

// liste d'entrée après retrait des polygones contenus dans le BSP courant
// (récursif)

QVector<CSGPolygon> CSGNode::clipPolygons(const QVector<CSGPolygon>& list ) const
{
	if ( !this->plane.isOk() ) return list ;

	QVector<CSGPolygon> list_front, list_back ;

	for (int i = 0 ; i < list.size() ; i++ ) {
		this->plane.splitPolygon(list[i], list_front, list_back, list_front, list_back);
	}
	if ( this->front ) list_front = this->front->clipPolygons(list_front) ;
	if ( this->back )  list_back = this->back->clipPolygons(list_back) ;
	else list_back.clear() ;

	list_front << list_back ;
	return list_front ;
}

// retire du BSP tree courant les polygones situés à l'intérieur du BSP tree 'other'
// (récursif)

void CSGNode::clipTo(const CSGNode* other )
{
	this->polygons = other->clipPolygons(this->polygons) ;
	if ( this->front ) this->front->clipTo(other) ;
	if ( this->back )  this->back->clipTo(other) ;
}

// liste de tous les polygones de l'arbre
// (récursif)

QVector<CSGPolygon> CSGNode::allPolygons() const
{
	QVector<CSGPolygon> list = this->polygons ;
	QVector<CSGPolygon> list_front, list_back ;

	if ( this->front ) list_front = this->front->allPolygons() ;
	if ( this->back )  list_back = this->back->allPolygons() ;
	list << list_front ;
	list << list_back ;

	return list ;
}

/*
// copie conforme du BSP tree courant
// (récursif)

CSGNode* CSGNode::clone() const
{
	CSGNode* ret = new CSGNode() ;
	ret->polygons = this->polygons ;
	ret->plane = this->plane ;
	if ( this->front ) ret->front = this->front->clone() ;
	if ( this->back )  ret->back = this->back->clone() ;
	return ret ;
}
*/

// construction de l'arbre binaire
// (récursif)

void CSGNode::build(const QVector<CSGPolygon>& list )
{
	if ( !list.size() ) return ;

	if ( !this->plane.isOk() ) this->plane = list[0].plane ;

	QVector<CSGPolygon> list_front, list_back ;

	for ( int i = 0 ; i < list.size() ; i++ ) {
		this->plane.splitPolygon(list[i], this->polygons, this->polygons, list_front, list_back ) ;
	}
	if ( list_front.size() ) {
		if ( !this->front ) this->front = new CSGNode ;
		this->front->build(list_front) ;
	}
	if ( list_back.size() ) {
		if ( !this->back ) this->back = new CSGNode ;
		this->back->build(list_back) ;
	}
}

// Public interface implementation
// ---------------------------------------------------------------------------

QVector<CSGPolygon> csgModelToPolygons(const CSGModel& model )
{
	QVector<CSGPolygon> list ;
	for ( int i = 0 ; i < model.vertices.size() ; i += 3 ) {
		QVector<CSGVertex> triangle ;
		int id = model.vertices[i].solidId ;
		for ( int j = 0 ; j < 3 ; j++ ) {
			CSGVertex v = model.vertices[ i + j ] ;
			triangle.push_back(v) ;
		}
		list.push_back( CSGPolygon(triangle, id ) ) ;
	}
	return list ;
}

CSGModel csgModelFromPolygons(const QVector<CSGPolygon>& polygons )
{
	CSGModel model ;
	for ( int i = 0 ; i < polygons.size() ; i++ ) {
		const CSGPolygon& poly = polygons[i] ;
		for ( int j = 2 ; j < poly.vertices.size() ; j++ ) {
			model.vertices.push_back( poly.vertices[0] ) ;
			model.vertices.push_back( poly.vertices[j - 1] ) ;
			model.vertices.push_back( poly.vertices[j] ) ;
		}
	}
	return model ;
}

CSGModel csgUnion(const CSGModel& model1, const CSGModel& model2 )
{
	CSGNode* a = new CSGNode( csgModelToPolygons(model1) ) ;
	CSGNode* b = new CSGNode( csgModelToPolygons(model2) ) ;

	a->clipTo(b) ;
	b->clipTo(a) ;
	b->invert() ;
	b->clipTo(a) ;
	b->invert() ;
	a->build( b->allPolygons() ) ;

	CSGNode* node = new CSGNode( a->allPolygons() ) ;
	QVector<CSGPolygon> polygons = node->allPolygons() ;

	delete a ; delete b ; delete node ;

	return csgModelFromPolygons( polygons ) ;
}

CSGModel csgIntersection(const CSGModel& model1, const CSGModel& model2 )
{
	CSGNode* a = new CSGNode( csgModelToPolygons(model1) ) ;
	CSGNode* b = new CSGNode( csgModelToPolygons(model2) ) ;

	a->invert() ;
	b->clipTo(a) ;
	b->invert() ;
	a->clipTo(b) ;
	b->clipTo(a) ;
	a->build( b->allPolygons() ) ;
	a->invert() ;

	CSGNode* node = new CSGNode( a->allPolygons() ) ;
	QVector<CSGPolygon> polygons = node->allPolygons() ;

	delete a ; delete b ; delete node ;

	return csgModelFromPolygons( polygons ) ;
}

CSGModel csgDifference(const CSGModel& model1, const CSGModel& model2 )
{
	CSGNode* a = new CSGNode( csgModelToPolygons(model1) ) ;
	CSGNode* b = new CSGNode( csgModelToPolygons(model2) ) ;

	a->invert() ;
	a->clipTo(b) ;
	b->clipTo(a) ;
	b->invert() ;
	b->clipTo(a) ;
	b->invert() ;
	a->build( b->allPolygons() ) ;
	a->invert() ;

	CSGNode* node = new CSGNode( a->allPolygons() ) ;
	QVector<CSGPolygon> polygons = node->allPolygons() ;

	delete a ; delete b ; delete node ;

	return csgModelFromPolygons( polygons ) ;
}
