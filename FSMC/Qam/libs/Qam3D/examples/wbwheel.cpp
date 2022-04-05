/*  ---------------------------------------------------------------------------
 *  filename    :   wbwheel.cpp
 *  description :   IMPLEMENTATION de la classe WbWheel
 *
 *	project     :	Qam3D/GLam: WifiBot wheel example
 *  start date  :   décembre 2009
 *  ---------------------------------------------------------------------------
 *  Copyright 2009-2016 by Alain Menu   <alain.menu@ac-creteil.fr>
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

#include "wbwheel.h"
//#include <GLamObjects>

// dimensions (cm) du pneu

#define	P_W1	 6.0			// largeur hors tout

// dimensions (cm) d'un cran

#define C_W1	( P_W1 / 2.0 )	// longueur
#define C_W2	( C_W1 / 3.0 )	// point de cassure
#define	C_H1	 1.0			// largeur au centre
#define	C_H2	 0.75			// largeur au bord
#define	C_H3	-1.0			// inclinaison 0XY
#define	C_T1	 1.0			// épaisseur
#define	C_RAD	 0.15			// rayon d'arrondi des sommets


WbWheel::WbWheel(Side side )
	: GLamGroup()
	, m_side( side )
{
	createTyre() ;
	createRim() ;
	addObject( m_tyre ) ;
	addObject( m_rim ) ;
//	defaultMatrix().rotate(60, 0, 1, 0 ) ;
	defaultMatrix().rotate(90, 0, 1, 0 ) ; // pour animation Rz par défaut..
	defaultMatrix().scale( 0.1 ) ;
}

WbWheel::~WbWheel()
{
}

void WbWheel::createTyre()
{
	qreal pdef1[][2] = {
		{ R_Jante, -L_Pneu / 2 + L_PneuInt }, { 0.70 * R_Env, -L_Pneu / 2 },
		{ 0.98 * R_Env, -L_Pneu / 2 }, { R_Env, 0 },  { 0.98 * R_Env, L_Pneu / 2 },
		{ 0.70 * R_Env, L_Pneu / 2 }, { R_Jante, L_Pneu / 2 - L_PneuExt } } ;

	QList<QPointF>	pts = GLam2D::tabToList((qreal*)pdef1, 14 ) ;
	pts.push_front( pts.first() ) ;
	pts.push_back( pts.last() ) ;
	pts = GLam2D::bSplineShape(pts, 100 ) ;

	GLamRevolution tyre( pts ) ;	// enveloppe du pneu
	tyre.setClosed( false ) ;
	tyre.defaultMatrix().rotate( m_side == Right ? 90 : -90, 0, 1, 0 ) ;

	m_tyre = new GLamGroup ;
	m_tyre->addObject( &tyre ) ;

//	qreal pdef2[][2] = {
//		{ 0, 0 }, { L_Cran, H3_Cran }, { L_Cran, H3_Cran + H2_Cran },
//		{ L2_Cran, H3_Cran * L2_Cran / L_Cran + H2_Cran }, { 0, H1_Cran } } ;

	qreal pdef2[][2] = {
		{ 0, 0 }, { C_W1, C_H3 }, { C_W1, C_H3 + C_H2  },
		{ C_W2, C_H3 * C_W2 / C_W1 + C_H2 }, { 0, C_H1 } } ;

	pts = GLam2D::tabToList((qreal*)pdef2, 10 ) ;
	pts = GLam2D::roundedShape(pts, 12, C_RAD ) ;

	GLamSolid rightTread(pts, C_T1 ) ;	// cran droit

	qreal tz =  R_Pneu - C_T1 ;
	qreal zw = -0.1 ;
	QMatrix4x4 m(	 1,  0,  0,  0,
					 0,  1,  0,  0,
					zw,  0,  1, tz,
					 0,  0,  0,  1 ) ;

	for ( int i = 0 ; i < Nb_Crans ; ++i ) {
		qreal a = i * 360.0 / Nb_Crans ;
		rightTread.defaultMatrix().setToIdentity() ;
		rightTread.defaultMatrix().rotate(a, 1, 0, 0 ) ;
		rightTread.defaultMatrix() *= m ;
		m_tyre->addObject( &rightTread ) ;
	}

	pts = GLam2D::yMirrored( pts ) ;
	GLamSolid leftTread(pts, C_T1 ) ;	// cran gauche

	m(2, 0 ) = -m(2, 0 ) ;

	for ( int i = 0 ; i < Nb_Crans ; ++i ) {
		qreal a = ( i + 0.5 ) * 360.0 / Nb_Crans ;
		leftTread.defaultMatrix().setToIdentity() ;
		leftTread.defaultMatrix().rotate(a, 1, 0, 0 ) ;
		leftTread.defaultMatrix() *= m ;
		m_tyre->addObject( &leftTread ) ;
	}

	m_tyre->material().set(GLamMaterial::Pewter ) ;
}

void WbWheel::createRim()
{
	qreal  pdef[][2] = {
		{ R_Jante, L_Jante }, { R_Fix, L2_Axe + L_Fix }, { R_Fix, L2_Axe },
		{ R_Axe,  L2_Axe }, { R_Axe, 0 }, { R_Jante, 0 } } ;

	QList<QPointF>	pts = GLam2D::tabToList((qreal*)pdef, 12 ) ;
	pts = GLam2D::translate(pts, 0, -L_Pneu / 2 + L_PneuInt ) ;
	
	m_rim = new GLamRevolution( pts ) ;		// jante

	m_rim->defaultMatrix().rotate( m_side == Right ? 90 : -90, 0, 1, 0 ) ;

	m_rim->material().set(GLamMaterial::Pearl ) ;
}
