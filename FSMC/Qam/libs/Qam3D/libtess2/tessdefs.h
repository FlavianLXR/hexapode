/*  ---------------------------------------------------------------------------
 *  filename    :   tessdefs.h
 *  description :   DEFINITIONS GLOBALES
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

#ifndef TESSDEFS_H
#define TESSDEFS_H

#include <cmath>
#include <map>
#include <list>
#include <queue>
#include <set>
#include <stack>

using namespace std ;

class TessPoint ;
class TessLine ;

template <class T, class KeyType>      class    SplayTree ;
typedef map<unsigned int, TessPoint*>           PointMap ;
typedef map<unsigned int, TessLine*>            LineMap ;
typedef priority_queue<TessPoint>               PQueue ;
typedef SplayTree<TessLine*, double>            EdgeBST ;
typedef list<unsigned int>                      Monopoly ;
typedef list<Monopoly>                          Monopolys ;
typedef vector<unsigned int>                    Triangle ;
typedef list<Triangle>                          Triangles ;
typedef map<unsigned int, set<unsigned int> >   AdjEdgeMap ;

#endif	// TESSDEFS_H
