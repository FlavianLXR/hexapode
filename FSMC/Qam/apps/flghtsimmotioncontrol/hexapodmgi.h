// -----------------------------------------------------------------------
// hexapodmgi.h
// Copyright 2021 by Alain Menu   <alain.menu@ac-creteil.fr>
// -----------------------------------------------------------------------

#include <qam6dof.h>
#include <QVector3D>
#include <QMatrix4x4>

#define	EPSILON	1e-6
#define	COS(A)	( qAbs( qCos(A) ) < EPSILON ? 0.0 : qCos(A) )
#define SIN(A)	( qAbs( qSin(A) ) < EPSILON ? 0.0 : qSin(A) )

// Hexapode (plateforme de Stewart)
// -----------------------------------------------------------------------
// base :    plan d'ancrage des actuateurs linéaires au sol
// platine : plan d'ancrage support de la partie mobile
// repère :  0XYZ avec origine (pivot virtuel) au centre de la platine
// -----------------------------------------------------------------------

class HexapodData
{
  public :
    float	baseRadius ;	// rayon ancrages base (mm)
    float	baseGap ;		// 1/2 écart ancrages base (mm)
    float	topRadius ;		// rayon ancrages platine (mm)
    float	topGap ;		// 1/2 écart ancrages platine (mm)
    float	minLen ;		// longueur min. (vérin rentré) (mm)
    float	maxLen ;		// longueur max. (vérin sorti) (mm)
    float	maxAngle ;		// angle absolu max. pour Rx,Ry,Rz (degrés)
    float	maxTrans ;		// déplacement absolu max. pour Tx,Ty (mm)
    float	maxSpeed ;		// vitesse linéaire max. (mm/s)
} ;

class HexapodMGI
{
  public:
    HexapodMGI() ;
    void setData(const HexapodData& data ) ;
    void setMGI(QamMatrix6x1 vKin ) ;
    void resetMGI() ;
    float minAltitude() const { return m_pMinAlt ; }
    float maxAltitude() const { return m_pMaxAlt ; }
    QamMatrix6x1 actuatorLen() const ;

  private:

    // cinématique	(unités: mm et degrés)

    QamMatrix6x1	m_vKin ;		// 6-DOF [ Tx,Ty,Tz, Rx,Ry,Rz ]

    // géométrie	(unités: mm et degrés)

    float			m_bRay ;		// rayon ancrages base
    float			m_bDec ;		// 1/2 écart ancrages base
    float			m_pRay ;		// rayon ancrages platine
    float			m_pDec ;		// 1/2 écart ancrages platine

    float			m_vMin ;		// longueur min. (vérin rentré)
    float			m_vMax ;		// longueur max. (vérin sorti)

    float			m_aMax ;		// angle abs. max. (degrés)
    float			m_tMax ;		// Tx/Ty abs. max. (mm)
    float			m_vSpeed ;		// vitesse max. (mm/s)

    // données calculées (système statique)

    QVector3D		m_bAnc[6] ;		// coord. points d'ancrage base fixe
    QVector3D		m_pAnc[6] ;		// coord. points d'ancrage platine
    QVector3D		m_pAncRef[6] ;	// points d'ancrage platine au repos

    float			m_pMinAlt ;		// altitude minimale (vérins rentrés)
    float			m_pMaxAlt ;		// altitude maximale (vérins sortis)

    // données calculées (système dynamique)

    QMatrix4x4		m_mKin ;		// matrice résultante 6-DOF
    QamMatrix6x1	m_vLen ;		// longueurs courantes des vérins

  private:
    void setGeometry() ;			// calculs système statique
    float vlength(int i ) ;			// m_mKin --> m_vLen (vérin n°i)
} ;
