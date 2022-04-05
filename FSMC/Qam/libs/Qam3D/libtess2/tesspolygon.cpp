/*  ---------------------------------------------------------------------------
 *  filename    :   tesspolygon.cpp
 *  description :   IMPLEMENTATION de la classe TessPolygon
 *
 *	project     :	Qam3D/GLam tesselation useful resources
 *  start date  :   décembre 2009
 *  ---------------------------------------------------------------------------
 *  Copyright 2009-2017 by Alain Menu   <alain.menu@ac-creteil.fr>
 *
 *  This file is part of "Qam3D/GLam Qt Library"
 *
 *  This program is free software ;  you can  redistribute it and/or  modify it
 *  under the terms of the  GNU General Public License as published by the Free
 *  Software Foundation ; either version 3 of the License, or  (at your option)
 *  any later version.
 *
 *  This program is distributed in the hope that it will be useful, but WITHOUT
 *  ANY WARRANTY ; without even the  implied  warranty  of  MERCHANTABILITY  or
 *  FITNESS FOR  A PARTICULAR PURPOSE. See the  GNU General Public License  for
 *  more details.
 *
 *	You should have  received  a copy of the  GNU General Public License  along
 *	with this program. If not, see <http://www.gnu.org/licenses/>.
 *  ---------------------------------------------------------------------------
 */

#include "tesspolygon.h"
#include <QDebug>

// ----------------------------------------------------------------------------

extern long int l_id ;
extern long int p_id ;

extern double orient2d(double* pa, double* pb, double* pc) ;

void UpdateKey(BTreeNode<TessLine*,double>* node, double y )
{
	node->data()->setKeyValue(y) ;
}

// ----------------------------------------------------------------------------

TessPolygon::TessPolygon(vector<double> contour )
	: m_ncontours( 0 )
{
	l_id = 0 ;
	p_id = 0 ;

//	m_nVertices.clear() ;
//	m_points.clear() ;
//	m_edges.clear() ;
//	m_edgebst.MakeEmpty() ;

	const double infinity = 1.0e38 ;
	m_xmin = m_ymin =  infinity ;
	m_xmax = m_ymax = -infinity ;

//	m_ncontours = 0 ;
	unsigned int nv = contour.size() / 2 ;
	m_nVertices.push_back( nv ) ;

	unsigned int i = 1 ;
//	unsigned int first, last ;
//	double x, y ;
//	TessType type ;

//	first = i ;
//	last = first + m_nVertices[ m_ncontours ] - 1 ;
	for ( unsigned int j = 0 ; j < m_nVertices[ m_ncontours ] ; j++, i++ ) {
		double x = contour.at( j * 2 ) ;
		double y = contour.at( j * 2 + 1 ) ;
//		type = INPUT ;
		if ( isExist(x,y) ) {
			// TODO
		}
		TessPoint* point = new TessPoint(i, x, y, INPUT ) ;
		if( x > m_xmax ) m_xmax = x ;
		if( x < m_xmin ) m_xmin = x ;
		if( y > m_ymax ) m_ymax = y ;
		if (y < m_ymin ) m_ymin = y ;
		m_points[i] = point ;
	}
	m_ncontours++ ;

//	this->initializate() ;
}

TessPolygon::~TessPolygon()
{
	// clear all dynamic allocated memory

	PointMap::iterator itp = m_points.begin() ;
	for ( ; itp != m_points.end() ; itp++ ) delete itp->second ;

	LineMap::iterator itl = m_edges.begin() ;
	for ( ; itl != m_edges.end() ; itl++ )	delete itl->second ;
}

void TessPolygon::addHole(vector<double> contour )
{
//	qDebug() << "addHole()" << contour.size() / 2 << "points" ;
//	for ( int i = 0 ; i < m_nVertices.size() ; ++i ) qDebug() << i << m_nVertices.at(i) ;

	unsigned int i = m_nVertices[ m_ncontours - 1 ] + 1 ;

	i = 1 ;
	for ( unsigned int c = 0 ; c < m_nVertices.size() ; ++c ) {
		i += m_nVertices.at(c) ;
	}
//	i += 1 ;
	unsigned int nv = contour.size() / 2 ;
	m_nVertices.push_back( nv ) ;
//	qDebug() << "before" << m_points.size() << m_ncontours << i ;

	for ( unsigned int j = 0 ; j < m_nVertices[ m_ncontours ] ; j++, i++ ) {
		double x = contour.at( j * 2 ) ;
		double y = contour.at( j * 2 + 1 ) ;
		if ( isExist(x,y) ) {
			// TODO
		}
		TessPoint* point = new TessPoint(i, x, y, INPUT ) ;
		if( x > m_xmax ) m_xmax = x ;
		if( x < m_xmin ) m_xmin = x ;
		if( y > m_ymax ) m_ymax = y ;
		if (y < m_ymin ) m_ymin = y ;
		m_points[i] = point ;
	}
	m_ncontours++ ;
//	qDebug() << "after " << m_points.size() << m_ncontours ;
}

// main triangulation function

void TessPolygon::triangulation()
{
//	qDebug() << "triangulation() step 0" ;
	initializate() ;
//	qDebug() << "triangulation() step 1" ;
	partition2Monotone() ;
//	qDebug() << "triangulation() step 2" ;
	searchMonotones() ;
//	qDebug() << "triangulation() step 3" ;
	Monopolys::iterator it = m_mpolys.begin() ;
	for ( ; it != m_mpolys.end() ; it++ ) triangulateMonotone( *it ) ;
//	qDebug() << "triangulation() step 4" ;
}

// check duplicated points

bool TessPolygon::isExist(double x, double y )
{
	bool result = false ;

	PointMap::iterator it = m_points.begin() ;
	for ( ; it != m_points.end() ; it++ ) {
		if ( it->second->m_x == x && it->second->m_y == y ) {
			result = true ;
			break ;
		}
	}
	return result ;
}

// polygon initialization
// to find types of all polygon vertices,
// create a priority queue for all vertices
// construct an edge set for each vertex (the set holds all edges starting from
// the vertex, only for loop searching purpose)

void TessPolygon::initializate()
{
//	qDebug() << "initializate() step 0" ;
	// end of read_BDM()
	int sid, eid ;
	int num = 0 ;

	for ( unsigned int j = 0 ; j < m_ncontours ; j++ ) {
		for ( unsigned int i = 1 ; i <= m_nVertices[j] ; i++ ) {
			sid = num + i ;
			eid = ( i == m_nVertices[j] ? num + 1 : num + i + 1 ) ;
			TessType type = INPUT ;
			TessLine* line = new TessLine( m_points[sid], m_points[eid], type ) ;
			m_edges[l_id] = line ;
		}
		num += m_nVertices[j] ;
	}

//	qDebug() << "initializate() step 1" ;

	int sum = 0 ;
	for ( unsigned int i = 0 ; i < m_ncontours ; i++ ) {
		sum += m_nVertices[i] ;
		m_nVertices[i] = sum ;
	}
	p_id = num ;
	//
//	qDebug() << "initializate() step 2" ;

	PointMap::iterator it = m_points.begin() ;
//
//qDebug() << m_points.size() << "points" ;
//for ( ; it != m_points.end() ; it++ ) {
//	int id = it->first ;
//	qDebug() << it->first << ":" << m_points[id]->m_x << "," << m_points[id]->m_y ;
//}
//it = m_points.begin() ;
//
	for ( ; it != m_points.end() ; it++ ) {
		int id = it->first ;
		int idp = prev(id) ;
		int idn = next(id) ;
		TessPoint p = *m_points[id] ;
		TessPoint pnext = *m_points[idn] ;
		TessPoint pprev = *m_points[idp] ;

		// intérieur du polygone à droite ?
		if (( p > pnext )&&( pprev > p ))
				m_points[id]->m_type = REGULAR_DOWN ;
		// intérieur du polygone à gauche ?
		else if (( p > pprev )&&( pnext > p ))
				m_points[id]->m_type = REGULAR_UP ;
		else {
			double pa[2], pb[2], pc[2] ;

			pa[0] = m_points[idp]->m_x ;
			pa[1] = m_points[idp]->m_y ;

			pb[0] = m_points[id]->m_x ;
			pb[1] = m_points[id]->m_y ;

			pc[0] = m_points[idn]->m_x ;
			pc[1] = m_points[idn]->m_y ;

			// angle horaire entre ba et bc < pi ==> ordre CCW pour abc, area > 0
			// angle horaire entre ba et bc > pi ==> ordre CW pour abc, area < 0
			// a, b et c colinéaires ==> area = 0
			double area = orient2d(pa, pb, pc ) ;	// predicates.cc

			// deux voisins à gauche (suivant 0Y+) ?
			if (( pprev > p )&&( pnext > p ))
				m_points[id]->m_type = ( area > 0 ? END : MERGE ) ;

			// deux voisins à droite (suivant 0Y+) ?
			if (( pprev < p )&&( pnext < p ))
				m_points[id]->m_type = ( area > 0 ? START : SPLIT ) ;
		}
		m_qpoints.push( *(it->second) ) ;
		m_startAdjEdgeMap[id].insert(id) ;
	}
//	qDebug() << "initializate() step 4" ;
}

// return the previous point (or edge) id for a given point (or edge)

unsigned int TessPolygon::prev(unsigned int i )
{
	unsigned int j(0), prevLoop(0), currentLoop(0) ;

	while ( i > m_nVertices[currentLoop] ) {
		prevLoop = currentLoop ;
		currentLoop++ ;
	}

	if (( i == 1 )||( i == m_nVertices[prevLoop] + 1 ))
			j = m_nVertices[currentLoop] ;
	else	if ( i <= m_nVertices[currentLoop] ) j = i - 1 ;

	return j ;
}

// return the next point (or edge) id for a given point (or edge)

unsigned int TessPolygon::next(unsigned int i )
{
	unsigned int j(0), prevLoop(0), currentLoop(0) ;

	while ( i > m_nVertices[currentLoop] ) {
		prevLoop = currentLoop ;
		currentLoop++ ;
	}

	if ( i < m_nVertices[currentLoop] )
			j = i + 1 ;
	else	if ( i == m_nVertices[currentLoop] )  {
				if ( currentLoop == 0 )
						j = 1 ;
				else	j = m_nVertices[prevLoop] + 1 ;
			}

	return j ;
}

// handle start vertex

void TessPolygon::handleStartVertex(unsigned int i )
{
//qDebug() << "in handleStartVertex()" << i ;
	double y = m_points[i]->m_y ;
//qDebug() << "step1" ;
	m_edgebst.InOrder(UpdateKey, y ) ;
//qDebug() << "step2" ;
//qDebug() << m_edges.size() << i ;
	m_edges[i]->setHelper(i) ;
//qDebug() << "step3" ;
	m_edges[i]->setKeyValue(y) ;
//qDebug() << "step4" ;
	m_edgebst.Insert( m_edges[i] ) ;
//qDebug() << "out handleStartVertex()" ;
}

// handle end vertex

void TessPolygon::handleEndVertex(unsigned int i )
{
	double y = m_points[i]->m_y ;
	m_edgebst.InOrder(UpdateKey, y ) ;

	unsigned int previ = prev(i) ;
	TessLine* edge = m_edges[ previ ] ;

	unsigned int helper = m_edges[ previ ]->helper() ;
	if( m_points[helper]->m_type == MERGE ) addDiagonal(i, helper ) ;

	m_edgebst.Delete( edge->keyValue() ) ;
}

// handle split vertex

void TessPolygon::handleSplitVertex(unsigned int i )
{
	double x = m_points[i]->m_x ;
	double y = m_points[i]->m_y ;
	m_edgebst.InOrder(UpdateKey, y ) ;

	BTreeNode<TessLine*, double>*  leftnode ;
	m_edgebst.FindMaxSmallerThan(x, leftnode ) ;
	TessLine* leftedge = leftnode->data() ;

	unsigned int helper=leftedge->helper() ;
	addDiagonal(i, helper ) ;
	leftedge->setHelper(i) ;
	m_edges[i]->setHelper(i) ;
	m_edges[i]->setKeyValue(y) ;
	m_edgebst.Insert( m_edges[i] ) ;
}

// handle merge vertex

void TessPolygon::handleMergeVertex(unsigned int i )
{
	double x = m_points[i]->m_x ;
	double y = m_points[i]->m_y ;
	m_edgebst.InOrder(UpdateKey, y ) ;

	unsigned int previ = prev(i) ;
	unsigned int helper = m_edges[previ]->helper() ;
	if ( m_points[helper]->m_type == MERGE ) addDiagonal(i, helper ) ;
	m_edgebst.Delete( m_edges[previ]->keyValue() ) ;

	BTreeNode<TessLine*, double>*  leftnode ;
	m_edgebst.FindMaxSmallerThan(x, leftnode ) ;

	TessLine* leftedge = leftnode->data() ;
	helper = leftedge->helper() ;
	if( m_points[helper]->m_type == MERGE ) addDiagonal(i, helper ) ;
	leftedge->setHelper(i) ;
}

// handle regular up vertex

void TessPolygon::handleRegularVertexUp(unsigned int i )
{
	double x = m_points[i]->m_x ;
	double y = m_points[i]->m_y ;
	m_edgebst.InOrder(UpdateKey, y ) ;

	BTreeNode<TessLine*, double>*  leftnode ;
	m_edgebst.FindMaxSmallerThan(x, leftnode ) ;

	TessLine* leftedge = leftnode->data() ;
	unsigned int helper = leftedge->helper() ;
	if( m_points[helper]->m_type == MERGE ) addDiagonal(i, helper ) ;
	leftedge->setHelper(i) ;
}

// handle regular down vertex

void TessPolygon::handleRegularVertexDown(unsigned int i )
{
	double y = m_points[i]->m_y ;
	m_edgebst.InOrder(UpdateKey, y ) ;

	unsigned int previ = prev(i) ;
	unsigned int helper = m_edges[previ]->helper() ;
	if ( m_points[helper]->m_type == MERGE ) addDiagonal(i, helper ) ;
	m_edgebst.Delete( m_edges[previ]->keyValue() ) ;
	m_edges[i]->setHelper(i) ;
	m_edges[i]->setKeyValue(y) ;
	m_edgebst.Insert( m_edges[i] ) ;
}

// add a diagonal from point id i to j

void TessPolygon::addDiagonal(unsigned int i, unsigned int j )
{
	TessLine* diag = new TessLine(m_points[i], m_points[j], INSERT ) ;
	m_edges[ diag->id() ] = diag ;
	m_startAdjEdgeMap[i].insert( diag->id() ) ;
	m_startAdjEdgeMap[j].insert( diag->id() ) ;
	m_diagonals[ diag->id() ] = diag ;
}

// auxiliary function to find monotone polygon pieces:
// calculate angle B for A, B, C three given points

double TessPolygon::angleCosb(double* pa, double* pb, double* pc )
{
	double dxab = pa[0] - pb[0] ;
	double dyab = pa[1] - pb[1] ;
	double dxcb = pc[0] - pb[0] ;
	double dycb = pc[1] - pb[1] ;
	double dxab2 = dxab * dxab ;
	double dyab2 = dyab * dyab ;
	double dxcb2 = dxcb * dxcb ;
	double dycb2 = dycb * dycb ;
	double ab = dxab2 + dyab2 ;
	double cb = dxcb2 + dycb2 ;
	double cosb = dxab * dxcb + dyab * dycb ;
	double denom = sqrt( ab * cb ) ;
	cosb /= denom ;
	return cosb ;
}

// auxiliary function to find monotone polygon pieces:
// for any given edge, find the next edge we should choose when searching for
// monotone polygon pieces

unsigned int TessPolygon::selectNextEdge(TessLine* edge )
{
	unsigned int eid = edge->endPoint(1)->m_id ;
	set<unsigned int> edges = m_startAdjEdgeMap[eid] ;
//    assert( !edges.empty() ) ;

	unsigned int nexte = 0 ;
	if( edges.size() == 1 )  nexte = *(edges.begin()) ;
	else if ( edges.size() > 1 ) {
		unsigned int nexte_ccw(0), nexte_cw(0) ;
		double max = -2.0, min = 2.0 ;
		set<unsigned int>::iterator it = edges.begin() ;
		for ( ; it != edges.end() ; it++ ) {
			if ( *it == edge->id() ) continue ;
			double A[2], B[2], C[2] ;
			A[0] = edge->endPoint(0)->m_x ;
			A[1] = edge->endPoint(0)->m_y ;
			B[0] = edge->endPoint(1)->m_x ;
			B[1] = edge->endPoint(1)->m_y ;
			if ( edge->endPoint(1) != m_edges[*it]->endPoint(0) ) m_edges[*it]->reverse() ;
			C[0] = m_edges[*it]->endPoint(1)->m_x ;
			C[1] = m_edges[*it]->endPoint(1)->m_y ;
			double area = orient2d(A, B, C ) ;	// predicates.cc
			double cosb = angleCosb(A, B, C ) ;

			if(( area > 0 )&&( max < cosb )) {
				nexte_ccw = *it ;
				max = cosb ;
			}
			else if ( min > cosb ) {
				nexte_cw = *it ;
				min = cosb ;
			}
		}
		nexte = ( nexte_ccw != 0 ? nexte_ccw : nexte_cw ) ;
	}
	return nexte ;
}

// partition polygon to monotone polygon pieces

void TessPolygon::partition2Monotone()
{
	if ( m_qpoints.top().m_type != START ) {
		qDebug() << "Please check your input (1)" ;
//		cout<<"Please check your input polygon:\n1)orientations?\n2)duplicated points?\n";
//		cout<<"poly2tri stopped.\n";
//		exit(1) ;
	}

	while ( !m_qpoints.empty() ) {
		TessPoint vertex = m_qpoints.top() ;
		m_qpoints.pop() ;
		unsigned int id = vertex.m_id ;

		switch( vertex.m_type ) {
			case START:        handleStartVertex(id) ;       break ;
			case END:          handleEndVertex(id) ;         break ;
			case MERGE:        handleMergeVertex(id) ;       break ;
			case SPLIT:        handleSplitVertex(id) ;       break ;
			case REGULAR_UP:   handleRegularVertexUp(id) ;   break ;
			case REGULAR_DOWN: handleRegularVertexDown(id) ; break ;
			default:
				qDebug() << "No duplicated points please!" ;
//				cout<<"No duplicated points please! poly2tri stopped\n";
//				exit(1);
				break;
		}
	}
}

// searching all monotone pieces

void TessPolygon::searchMonotones()
{
	int loop = 0 ;
	LineMap edges = m_edges ;

	while( edges.size() > m_diagonals.size() ) {
		loop++ ;
		Monopoly poly ;
		LineMap::iterator it = edges.begin() ;
		TessPoint* startp /*= startp*/ = it->second->endPoint(0) ;
		TessPoint* endp = 0 ;
		TessLine*  next = it->second ;

		poly.push_back( startp->m_id ) ;

		for (;;) {
			endp = next->endPoint(1) ;
			if( next->type() != INSERT ) {
				edges.erase( next->id() ) ;
				m_startAdjEdgeMap[ next->endPoint(0)->m_id ].erase( next->id() ) ;
			}
			if ( endp == startp ) break ;
			poly.push_back( endp->m_id ) ;

			unsigned int nexte = selectNextEdge(next) ;
			if ( nexte == 0 ) {
				qDebug() << "Please check your input (2)" ;
//				cout<<"Please check your input polygon:\n";
//				cout<<"1)orientations?\n2)with duplicated points?\n3)is a simple one?\n";
//				cout<<"poly2tri stopped.\n";
//				exit(1);
			}
//			assert( nexte > 0 ) ;
			next = edges[nexte] ;
			if ( next->endPoint(0) != endp ) next->reverse() ;
		}
		m_mpolys.push_back( poly ) ;
	}
}

// triangulate a monotone polygon

void TessPolygon::triangulateMonotone(Monopoly& mpoly )
{
	PQueue qvertex ;
	Monopoly::iterator it = mpoly.begin(), itnext ;
	for ( ; itnext = it, it != mpoly.end() ; it++ ) {
		itnext++ ;
		if ( itnext == mpoly.end() ) itnext = mpoly.begin() ;
		TessPoint point = *m_points[*it] ;
		TessPoint pointnext = *m_points[*itnext] ;
		point.m_left = ( point > pointnext ? true : false ) ;
		qvertex.push(point) ;
	}

	stack<TessPoint> spoint ;
	for( int i = 0; i < 2; i++ ) { spoint.push( qvertex.top() ) ; qvertex.pop() ; }

	while ( qvertex.size() > 1 ) {
		TessPoint topQueuePoint = qvertex.top() ;
		TessPoint topStackPoint = spoint.top() ;

		if ( topQueuePoint.m_left != topStackPoint.m_left ) {
			while ( spoint.size()  > 1 ) {
				TessPoint p1 = spoint.top() ;
				spoint.pop() ;
				TessPoint p2 = spoint.top() ;
				Triangle v(3) ;
				v[0] = topQueuePoint.m_id ;
				v[1] = p1.m_id ;
				v[2] = p2.m_id ;
				m_triangles.push_back(v) ;
//				if(_debug) _logfile<<"Add triangle:"<<v[0]<<" "<<v[1]<<" "<<v[2]<<'\n';
			}
			spoint.pop() ;
			spoint.push( topStackPoint ) ;
			spoint.push( topQueuePoint ) ;
		}
		else {
			while( spoint.size() > 1 ) {
				TessPoint stack1Point = spoint.top() ;
				spoint.pop() ;
				TessPoint stack2Point = spoint.top() ;
				spoint.push(stack1Point) ;
				double pa[2], pb[2], pc[2] ;
				pa[0] = topQueuePoint.m_x ; pa[1] = topQueuePoint.m_y ;
				pb[0] = stack2Point.m_x ;   pb[1] = stack2Point.m_y ;
				pc[0] = stack1Point.m_x ;   pc[1] = stack1Point.m_y ;

				double area=orient2d(pa, pb, pc ) ;	// predicates.cc

				bool left = stack1Point.m_left ;
				if (( area > 0 && left )||( area < 0 && !left )) {
					Triangle v(3) ;
					v[0] = topQueuePoint.m_id ;
					v[1] = stack2Point.m_id ;
					v[2] = stack1Point.m_id ;
					m_triangles.push_back(v) ;
//					if(_debug) _logfile<<"Add triangle:"<<v[0]<<" "<<v[1]<<" "<<v[2]<<'\n';
					spoint.pop() ;
				}
				else break ;
			}
			spoint.push(topQueuePoint) ;
		}
		qvertex.pop() ;
	}

	TessPoint lastQueuePoint = qvertex.top() ;
	while ( spoint.size() !=1 ) {
		TessPoint topPoint = spoint.top() ;
		spoint.pop() ;
		TessPoint top2Point = spoint.top() ;
		Triangle v(3) ;
		v[0] = lastQueuePoint.m_id ;
		v[1] = topPoint.m_id ;
		v[2] = top2Point.m_id ;
		m_triangles.push_back(v) ;
//		if(_debug) _logfile<<"Add triangle:"<<v[0]<<" "<<v[1]<<" "<<v[2]<<'\n';
	}
}
