/*  ---------------------------------------------------------------------------
 *  filename    :   tesspolygon.h
 *  description :   INTERFACE de la classe TessPolygon
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

#ifndef TESSPOLYGON_H
#define TESSPOLYGON_H

#include "tessline.h"

class TessPolygon
{
  public:
	TessPolygon(vector<double> contour ) ;	// CCW
	~TessPolygon() ;

	void addHole(vector<double> contour ) ;	// CW

	// from class BDMFile
	PointMap& points() { return m_points ; }
	LineMap&  edges() { return m_edges ; }

	// from class Polygon
	void triangulation() ;
	Triangles triangles() { return m_triangles ; }

  private:		// from class BDMFile
	bool isExist(double x, double y ) ;

  private:		// from class Polygon
	void initializate() ;
	unsigned int prev(unsigned int i ) ;
	unsigned int next(unsigned int i ) ;
	void handleStartVertex(unsigned int i ) ;
	void handleEndVertex(unsigned int i ) ;
	void handleSplitVertex(unsigned int i ) ;
	void handleMergeVertex(unsigned int i ) ;
	void handleRegularVertexUp(unsigned int i ) ;
	void handleRegularVertexDown(unsigned int i ) ;
	void addDiagonal(unsigned int i, unsigned int j ) ;
	double angleCosb(double* pa, double* pb, double* pc ) ;
	unsigned int selectNextEdge(TessLine* edge ) ;
	void partition2Monotone() ;
	void searchMonotones() ;
	void triangulateMonotone(Monopoly& mpoly ) ;

  protected:	// from class BDMFile
	unsigned int			m_ncontours ;   // number of contours
	vector<unsigned int>	m_nVertices ;   //
	PointMap				m_points ;      // all vertices
	LineMap					m_edges ;       // all edges
	double					m_xmin, m_xmax, m_ymin, m_ymax ; // boundary box for polygon

  private:		// from  class Polygon
	PQueue      m_qpoints ;				// priority queue for event points
	EdgeBST     m_edgebst ;				// edge binary searching tree (splaytree)
	Monopolys   m_mpolys ;				// all monotone polygon piece list
	Triangles   m_triangles ;			// all triangle list

										// data for monotone piece searching purpose:
	AdjEdgeMap  m_startAdjEdgeMap ;		// all edges starting from given points (map)
	LineMap     m_diagonals ;			// added diagonals to partition polygon to
										// monotone pieces, not all diagonals of
										// given polygon
} ;

#endif	// TESSPOLYGON_H
