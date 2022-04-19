/*  ---------------------------------------------------------------------------
 *  filename    :   hexapod.h
 *  description :   INTERFACE de la classe Hexapod (Stewart platform)
 *
 *	project     :	Scène 3D LLF Maquette éch. 1:1 FlightSim
 *  start date  :   novembre 2017
 *  ---------------------------------------------------------------------------
 *  Copyright 2017 by Alain Menu   <alain.menu@ac-creteil.fr>
 *
 *  This file is part of "FlightSim Mock-up"
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

#ifndef HEXAPOD_H
#define HEXAPOD_H

#include <GLamObjects>
#include <glamtext.h>
#include <qam6dof.h>

// Hexapode (plateforme de Stewart)
// ----------------------------------------------------------------------------
// base :    plan d'ancrage des actuateurs linéaires au sol
// platine : plan d'ancrage support de la partie mobile
// repère :  0XYZ avec origine (pivot virtuel) au centre de la platine
// ----------------------------------------------------------------------------

// structure d'échange des caractéristiques mécaniques

class HexapodData
{
  public :

    HexapodData()
    {
            baseRadius = 500;
            baseGap = 86;
            topRadius = 300;
            topGap = 35;
            minLen = 560;
            maxLen = 860;
            maxAngle = 15;
            maxTrans = 40;
            maxSpeed = 30;
    }
    float	baseRadius ;	// rayon ancrages base (mm)
    float	baseGap ;		// 1/2 écart ancrages base (mm)
    float	topRadius ;		// rayon ancrages platine (mm)
    float	topGap ;		// 1/2 écart ancrages platine (mm)
    float	minLen ;		// longueur min. (vérin rentré) (mm)
    float	maxLen ;		// longueur max. (vérin sorti) (mm)
	float	maxAngle ;		// angle absolu max. pour Rx,Ry,Rz (degrés)
    float	maxTrans ;		// déplacement absolu max. pour Tx,Ty (mm)
    float	maxSpeed ;		// vitesse linéaire max. (mm/s)

    // HexapodData streaming for QSettings read/write   new v0.7
    friend QDataStream& operator <<(QDataStream& out, const HexapodData& data ) {
            out << data.baseRadius ;
            out << data.baseGap ;
            out << data.topRadius ;
            out << data.topGap ;
            out << data.minLen ;
            out << data.maxLen ;
            out << data.maxAngle ;
            out << data.maxTrans ;
            out << data.maxSpeed ;
            return out ;
    }

    friend QDataStream& operator >>(QDataStream& in, HexapodData& data ) {
            in >> data.baseRadius ;
            in >> data.baseGap ;
            in >> data.topRadius ;
            in >> data.topGap ;
            in >> data.minLen ;
            in >> data.maxLen ;
            in >> data.maxAngle ;
            in >> data.maxTrans ;
            in >> data.maxSpeed ;
            return in ;
    }
} ;

Q_DECLARE_METATYPE(HexapodData)     // new v0.7

// modèle 3D du système

class Hexapod
{
  public:
	Hexapod() ;
	~Hexapod() ;
	void draw(float globalScale = 1.0 ) ;

	// caractéristiques mécaniques
	HexapodData	data() const ;
	void setData(const HexapodData& data ) ;
	float minAltitude() const ;
	float maxAltitude() const ;

	// translations / rotations	[Tx, Ty, Tz, Rx, Ry, Rz]
	void resetPosition() ;
	QamMatrix6x1 mgi() const { return m_vKin ; }
	void setMgi(const QamMatrix6x1& kin ) ;

	// longueurs des actuateurs [L0, L1, L2, L3, L4, L5]
	QamMatrix6x1 mgd() const { return m_vLen ; }
	void setMgd(const QamMatrix6x1& len ) ;

	// matrice de transformation de la platine
	QMatrix4x4 transformMatrix() const { return m_mKinMockup ; }

  private:

	// actuateurs linéaires (unité: mm)

	float			m_vMin ;		// longueur min. (vérin rentré)
	float			m_vMax ;		// longueur max. (vérin sorti)
    float			m_aMax ;		// angle abs. max. (degrés)
    float			m_tMax ;		// Tx/Ty abs. max. (mm)
    float			m_vSpeed ;		// vitesse max. (mm/s)
    QamMatrix6x1	m_vLen ;		// longueurs courantes des vérins

	float			m_pMinAlt ;		// altitude minimale (vérins rentrés)
	float			m_pMaxAlt ;		// altitude maximale (vérins sortis)

	// géométrie (unité: mm et radians)

	float			m_bRay ;		// rayon ancrages base
	float			m_bDec ;		// 1/2 écart ancrages base
	float			m_pRay ;		// rayon ancrages platine
	float			m_pDec ;		// 1/2 écart ancrages platine

	QVector3D		m_bAnc[6] ;		// coord. points d'ancrage base fixe
	QVector3D		m_pAnc[6] ;		// coord. points d'ancrage platine
	QVector3D		m_pAncRef[6] ;	// points d'ancrage platine au repos

	// Alpha(i) = _aA(i) = angle point d'ancrage base n°i p/r Ox
	// Beta(i)  = _aB(i) = angle point d'ancrage platine n°i p/r Ox

	QamMatrix1x6	_aA ;			// angles Alpha
	QamMatrix1x6	_cA ;			// cosinus de _aA
	QamMatrix1x6	_sA ;			// sinus de _aA
	QamMatrix1x6	_aB ;			// angles Beta
	QamMatrix1x6	_cB ;			// cosinus de _aB
	QamMatrix1x6	_sB ;			// sinus de _aB

	void setGeometry() ;
	void updateBaseGeometry() ;
	void updatePlatineGeometry() ;

	// cinématique (unités: mm et degrés)

	QamMatrix6x1	m_vKin ;		// 6-DOF [ Tx,Ty,Tz, Rx,Ry,Rz ]
	QMatrix4x4		m_mKin ;		// matrices résultantes
	QMatrix4x4		m_mKinMockup ;
	QVector3D		m_pPiv ;		// pivot virtuel platine

	// Newton-Raphson

	QamMatrix1x6	_tf ;
	QamMatrix6x6	_jac ;

	void targetFunct(const QamMatrix6x1& kin, const QamMatrix6x1& len ) ;
	QamMatrix6x1 newtonRaphson(const QamMatrix6x1& kin, const QamMatrix6x1& len, bool& err ) ;

	void setMatrix_MGD() ;		// ui --> longueur des vérin(s)
	void setMatrix_MGI() ;		// ui --> translations/rotations
	float vlength(int i ) ;		// m_mKin --> longueur courante actuateur

	// composants 3D

	GLamSolid*		m_base ;		// plans d'ancrage des vérins
	GLamSolid*		m_platine ;
	GLamSphere*		m_sb[6] ;		// points d'ancrage des vérins
	GLamSphere*		m_sp[6] ;
	GLamCylinder*	m_corps[6] ;	// vérins
	GLamCylinder*	m_tige[6] ;
	GLamSphere*		m_piv ;			// matérialisation pivot virtuel

    GLamText*       m_num[6] ;      // numéros des vérins

	void createObjects() ;
};

#endif // HEXAPOD_H
