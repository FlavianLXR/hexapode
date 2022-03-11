// -----------------------------------------------------------------------
// hexapodmgi.cpp
// Copyright 2021 by Alain Menu   <alain.menu@ac-creteil.fr>
// -----------------------------------------------------------------------

#include <hexapodmgi.h>

// -----------------------------------------------------------------------
// constructeur par défaut

HexapodMGI::HexapodMGI()
    : m_bRay( 300 )
    , m_bDec(  60 )
    , m_pRay( 250 )
    , m_pDec(  40 )
    , m_vMin( 280 )
    , m_vMax( 530 )
    , m_aMax( 15 )
    , m_tMax( 100 )
    , m_vSpeed( 30 )
{
    m_vKin.fill( 0.0 ) ;
    m_vLen.fill( m_vMin ) ;
    m_mKin.setToIdentity() ;
    setGeometry() ;
}

void HexapodMGI::setData(const HexapodData& data )
{
    m_bRay = data.baseRadius ;
    m_bDec = data.baseGap ;
    m_pRay = data.topRadius ;
    m_pDec = data.topGap ;
    m_vMin = data.minLen ;
    m_vMax = data.maxLen ;
    m_aMax = data.maxAngle ;
    m_tMax = data.maxTrans ;
    m_vSpeed = data.maxSpeed ;
    setGeometry() ;
}

// -----------------------------------------------------------------------
// système statique (au repos)
// calcul des coord. et angles des points d'ancrage (sens anti-horaire)
//
// système équilatéral pour base et platine = 2 * M_PI / 3 (120°)
// décalage angulaire entre base et platine =     M_PI / 3 ( 60°)
//
// repère de référence : centre des triangles d'ancrage
// axe Ox orienté vers l'avant de la maquette, entre _aB(0) et -aB(1)

void HexapodMGI::setGeometry()
{
    // 1/2 écart angulaire entre 2 points adjacents ..
    float al = qAsin( m_bDec / m_bRay ) ;	// .. sur la base fixe
    float be = qAsin( m_pDec / m_pRay ) ;	// et sur la platine mobile

    // Alpha(i) = _aA(i) = angle point d'ancrage base n°i p/r Ox
    // Beta(i)  = _aB(i) = angle point d'ancrage platine n°i p/r Ox

    //	i  |	0		1			2			3			4			5
    // ----+----------------------------------------------------------------
    // _aA |  +al	2PI/3-al	2PI/3+al	4PI/3-al	4PI/3+al	     -al
    // _aB |  -be	     +be	2PI/3-be	2PI/3+be	4PI/3-be	4PI/3+be

    QamMatrix1x6	_aA ;			// angles Alpha (radians)
    QamMatrix1x6	_cA ;			// cosinus de _aA
    QamMatrix1x6	_sA ;			// sinus de _aA
    QamMatrix1x6	_aB ;			// angles Beta (radians)
    QamMatrix1x6	_cB ;			// cosinus de _aB
    QamMatrix1x6	_sB ;			// sinus de _aB

    for ( int i = 0 ; i < 6 ; ++i ) {
        // base fixe
        int ka = ( ( i + 1 ) / 2 ) % 3 ;					// 0 1 1 2 2 0
        _aA(i) = ka * 2 * M_PI / 3 + qPow(-1,i) * al ;

        _aA(i) -= M_PI / 3 ;		// déphasage p/r axe Ox platine

        _cA(i) = COS( _aA(i) ) ;
        _sA(i) = SIN( _aA(i) ) ;

        // platine mobile
        int kb = ( i / 2 ) % 3 ;							// 0 0 1 1 2 2
        _aB(i) = kb * 2 * M_PI / 3 - qPow(-1,i) * be ;
        _cB(i) = COS( _aB(i) ) ;
        _sB(i) = SIN( _aB(i) ) ;
    }

    // altitude minimale de la platine (vérins rentrés)
    m_pMinAlt = qSqrt( qPow(m_vMin,2) + 2 * m_bRay * m_pRay * COS(_aB(0) - _aA(0)) - qPow(m_bRay,2) - qPow(m_pRay,2) ) ;
    // altitude maximale de la platine (vérins sortis)
    m_pMaxAlt = qSqrt( qPow(m_vMax,2) + 2 * m_bRay * m_pRay * COS(_aB(0) - _aA(0)) - qPow(m_bRay,2) - qPow(m_pRay,2) ) ;

    // coordonnées initiales des points d'ancrage
    for ( int i = 0 ; i < 6 ; ++i ) {
        m_bAnc[i] = QVector3D(m_bRay * _cA(i), m_bRay * _sA(i), -m_pMinAlt ) ;
        m_pAnc[i] = QVector3D(m_pRay * _cB(i), m_pRay * _sB(i), 0 ) ;
        m_pAncRef[i] = m_pAnc[i] ;
    }
}

// -----------------------------------------------------------------------
// calcul MGI (longueurs respectives des 6 vérins)
// param = 6-DOF [ Tx,Ty,Tz, Rx,Ry,Rz ] unités mm et degrés

void HexapodMGI::setMGI(QamMatrix6x1 vKin )
{
    m_vKin = vKin ;

    m_mKin.setToIdentity() ;						// calcul matrice 4x4

    m_mKin.translate( QVector3D(m_vKin(0), m_vKin(1), m_vKin(2) ) ) ;
    m_mKin.rotate(m_vKin(3), 1, 0, 0 ) ;
    m_mKin.rotate(m_vKin(4), 0, 1, 0 ) ;
    m_mKin.rotate(m_vKin(5), 0, 0, 1 ) ;

    for ( int i = 0 ; i < 6 ; ++i )	vlength(i) ;	// màj m_vLen
}

// longueur courante du vérin d'indice i (affecte m_vLen en fonction de m_mKin)

float HexapodMGI::vlength(int i )
{
    QVector3D v = m_mKin.map( m_pAncRef[i] ) - m_bAnc[i] ;

    float eps = (m_pMaxAlt - m_pMinAlt) * 0.01 ;
//	bool err = ( v.length() > m_vMax + eps )||( v.length() < m_vMin - eps ) ;
    m_vLen(i) = v.length() ;

    return v.length() ;
}

void HexapodMGI::resetMGI()
{
    QamMatrix6x1 m ;
    m.fill( 0.0 ) ;
    setMGI(m) ;
}

// -----------------------------------------------------------------------
// sélecteur des longueurs des vérins

QamMatrix6x1 HexapodMGI::actuatorLen() const
{
    return m_vLen ;
}
