/*  ---------------------------------------------------------------------------
 *  filename    :   wbwheel.h
 *  description :   INTERFACE de la classe WbWheel
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

// modélisation d'une roue (gauche ou droite) du robot WifiBot
// mesures exprimées en centimètres

// ressources GLam utilisées --------------------------------------------------

// 	GLamGroup					roue complète, classe de base de WbWheel
// 		GLamRevolution			jante
// 		GLamGroup				pneu
//			GLamRevolution		enveloppe extérieure du pneu
//			GLamSolid			crans gauche et droit du pneu


#ifndef WBWHEEL_H
#define WBWHEEL_H

//#include "GLam/glamgroup.h"
#include <GLamGroup>
//#include <GLamObjects>

// dimensions (cm) ------------------------------------------------------------

// CF NOUVELLES DEF. DANS .CPP

#define R_Jante     3.10        // rayon extérieur de la jante
#define R_Pneu      6.20        // rayon hors tout du pneu (avec crans)
#define R_Env       5.90        // rayon de l'enveloppe du pneu (hors crans)
#define R_Axe       0.40        // rayon de l'axe
#define R_Fix       0.70        // rayon du logement de la vis axiale
#define L_Pneu      6.00        // largeur hors tout du pneu
#define L_Jante     5.40        // largeur hors tout de la jante
#define L_PneuInt   0.20        // débord intérieur du pneu p/r flasque jante
#define L_PneuExt   ( L_Pneu - L_Jante - L_PneuInt )
#define L2_Axe      3.20        // profondeur du logement de l'axe
#define L_Fix       1.60        // profondeur du lamage pour la vis axiale
#define L_Cran      ( L_Pneu / 2.0 )    // largeur 0X d'un cran
#define L2_Cran     ( 0.3 * L_Cran )    // point de cassure avant du cran
#define H1_Cran     1.00        // largeur 0Y d'un cran au centre du pneu
#define H2_Cran     0.75        // largeur 0Y d'un cran en périphérie du pneu
#define H3_Cran     ( -L_Cran / 3.0 )   // inclinaison 0XY d'un cran
#define Nb_Crans    20          // nb. de crans à gauche / à droite


class WbWheel : public GLamGroup
{
  public:
	enum Side { Left , Right } ;
	WbWheel(Side side = Left ) ;
	~WbWheel() ;

  private :
	void createTyre() ;
	void createRim() ;

  private :
	Side				m_side ;	// roue gauche (défaut) ou droite
	GLamGroup*			m_tyre ;	// pneu
	GLamRevolution*		m_rim ;		// jante
} ;

#endif // WBWHEEL_H
