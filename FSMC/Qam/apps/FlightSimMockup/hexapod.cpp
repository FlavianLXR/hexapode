/*  ---------------------------------------------------------------------------
 *  filename    :   hexapod.cpp
 *  description :   IMPLEMENTATION de la classe Hexapod (Stewart platform)
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

#include "hexapod.h"

#ifdef QAMDEBUG
	#include <QDebug>
#endif

Hexapod::Hexapod()
    : /*m_vMin( 280 )
	, m_vMax( 530 )
	, m_aMax( 15 )
    , m_tMax( 100 )
    , m_vSpeed( 30 )
	, m_bRay( 300 )
	, m_bDec(  60 )
	, m_pRay( 250 )
	, m_pDec(  40 )
    ,*/ m_base( 0 )
	, m_platine( 0 )
{
    setData(HexapodData());
	m_vKin.fill( 0.0 ) ;
	m_vLen.fill( m_vMin ) ;
	m_mKin.setToIdentity() ;
	m_mKinMockup.setToIdentity() ;
	setGeometry() ;
	createObjects() ;
}

Hexapod::~Hexapod()
{
	delete m_piv ;
	for ( int i = 0 ; i < 6 ; ++i ) {
		delete m_corps[i] ;
		delete m_tige[i] ;
		delete m_sp[i] ;
		delete m_sb[i] ;
	}
	delete m_platine ;
	delete m_base ;
}

// caractéristiques mécaniques

HexapodData	Hexapod::data() const
{
	HexapodData	data ;
	data.baseRadius = m_bRay ;
	data.baseGap = m_bDec ;
	data.topRadius = m_pRay ;
	data.topGap = m_pDec ;
	data.minLen = m_vMin ;
	data.maxLen = m_vMax ;
	data.maxAngle = m_aMax ;
    data.maxTrans = m_tMax ;
    data.maxSpeed = m_vSpeed ;
	return data ;
}

void Hexapod::setData(const HexapodData& data )
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

float Hexapod::minAltitude() const { return m_pMinAlt ; }
float Hexapod::maxAltitude() const { return m_pMaxAlt ; }

// création des objets 3D

void Hexapod::updateBaseGeometry()
{
	if ( m_base == 0 )	return ;

	QList<QPointF> shape ;
	for ( int i = 0 ; i < 6 ; ++i )	shape << m_bAnc[i].toPointF() ;
	qreal thickness = m_base->thickness() ;
	*m_base = GLamSolid(shape, thickness ) ;

	GLamHole* hole = new GLamHole(GLam2D::circularShape(m_bRay/2,90) ) ;
	m_base->addHole(hole) ;
	delete hole ;
}

void Hexapod::updatePlatineGeometry()
{
	if ( m_platine == 0 ) return ;

	QList<QPointF> shape ;
	for ( int i = 0 ; i < 6 ; ++i )	shape << m_pAnc[i].toPointF() ;
	qreal thickness = m_platine->thickness() ;
	*m_platine = GLamSolid(shape, thickness ) ;
	m_platine->setMaterial( GLamMaterial::CyanPlastic ) ;

	GLamHole* hole = new GLamHole(GLam2D::circularShape(m_pRay/3,90) ) ;
	m_platine->addHole(hole) ;
	delete hole ;
}

void Hexapod::createObjects()
{
	// plaques d'ancrage des vérins (géométrie modifiable)

	QList<QPointF> shape ;
	for ( int i = 0 ; i < 6 ; ++i )	shape << m_bAnc[i].toPointF() ;
	m_base = new GLamSolid(shape, 10 ) ;
	updateBaseGeometry() ;

	shape.clear() ;
	for ( int i = 0 ; i < 6 ; ++i )	shape << m_pAnc[i].toPointF() ;
	m_platine = new GLamSolid(shape, 1 ) ;
	updatePlatineGeometry() ;

	// actuateurs linéaires (vérins)

	for ( int i = 0 ; i < 6 ; ++i ) {
		m_sb[i] = new GLamSphere( 40 ) ;
		m_sb[i]->setMaterial( GLamMaterial::GreenPlastic ) ;
		m_sp[i] = new GLamSphere( 40 ) ;
		m_sp[i]->setMaterial( GLamMaterial::RedPlastic ) ;
		m_corps[i] = new GLamCylinder( 20, 0.8 * m_vMin ) ;
		m_corps[i]->setMaterial( GLamMaterial::YellowPlastic ) ;
		m_tige[i] = new GLamCylinder( 10, vlength(i) ) ;
		m_tige[i]->setMaterial( GLamMaterial::Bronze ) ;

        m_corps[i]->setShadingEffect(GLamObject::Flat ) ;

        m_num[i] = new GLamText( QString("%1").arg(i+1) ) ; // new v0.6
        m_num[i]->setFontSize(50) ;
        m_num[i]->setThickness(5) ;
	}

	// pivot virtuel

	m_piv = new GLamSphere( 40 ) ;
	m_piv->setMaterial( GLamMaterial::WhitePlastic ) ;
}

// projection du modèle sur la scène 3D

void Hexapod::draw(float globalScale )
{
    m_base->transformMatrix().setToIdentity() ;

	m_base->transformMatrix().scale( globalScale ) ;

	m_base->transformMatrix().translate( 0, 0, -m_pMinAlt - m_base->thickness() ) ;

    m_base->draw() ;

	m_platine->transformMatrix().setToIdentity() ;
	m_platine->transformMatrix().scale( globalScale ) ;
	m_platine->transformMatrix() *= m_mKinMockup ;
    m_platine->draw() ;


	for ( int i = 0 ; i < 6 ; ++i ) {
		m_sb[i]->transformMatrix().setToIdentity() ;
		m_sb[i]->transformMatrix().scale( globalScale ) ;
		m_sb[i]->transformMatrix().translate( m_bAnc[i] ) ;
		m_sb[i]->draw() ;
		m_sp[i]->transformMatrix().setToIdentity() ;
		m_sp[i]->transformMatrix().scale( globalScale ) ;
		m_sp[i]->transformMatrix().translate( m_mKin.map( m_pAncRef[i] ) ) ;
		m_sp[i]->draw() ;

		QVector3D v = m_mKin.map( m_pAncRef[i] ) - m_bAnc[i] ;
		QQuaternion q = QQuaternion::rotationTo( QVector3D(0,0,1),  v ) ;

		m_corps[i]->setLength( 0.8 * m_vMin ) ;
		m_corps[i]->transformMatrix().setToIdentity() ;
		m_corps[i]->transformMatrix().scale( globalScale ) ;
		m_corps[i]->transformMatrix().translate( m_bAnc[i] ) ;
		m_corps[i]->transformMatrix().rotate( q ) ;
		m_corps[i]->transformMatrix().translate( 0, 0, 0.1 * m_vMin ) ;
		m_corps[i]->draw() ;

		m_tige[i]->setLength( m_vLen(i) ) ;
		m_tige[i]->transformMatrix().setToIdentity() ;
		m_tige[i]->transformMatrix().scale( globalScale ) ;
		m_tige[i]->transformMatrix().translate( m_bAnc[i] ) ;
		m_tige[i]->transformMatrix().rotate( q ) ;
		m_tige[i]->draw() ;

        m_num[i]->transformMatrix().setToIdentity() ;       // new v0.6
        m_num[i]->transformMatrix().scale( globalScale ) ;
        m_num[i]->transformMatrix().translate( m_mKin.map( m_pAncRef[i] + QVector3D(0,0,50) ) ) ;
        m_num[i]->transformMatrix().rotate(90, 0, 0, -1 ) ;
        m_num[i]->draw() ;
	}

	m_piv->transformMatrix().setToIdentity() ;
	m_piv->transformMatrix().scale( globalScale ) ;
	m_piv->transformMatrix().translate( m_mKin.map( m_pPiv ) ) ;
	m_piv->draw() ;
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

void Hexapod::setGeometry()
{
    // 1/2 écart angulaire entre 2 points adjacents ..
	float al = qAsin( m_bDec / m_bRay ) ;	// .. sur la base fixe
	float be = qAsin( m_pDec / m_pRay ) ;	// et sur la platine mobile

	//	i  |	0		1			2			3			4			5
	// ----+----------------------------------------------------------------
	// _aA |  +al	2PI/3-al	2PI/3+al	4PI/3-al	4PI/3+al	     -al
	// _aB |  -be	     +be	2PI/3-be	2PI/3+be	4PI/3-be	4PI/3+be

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

	// pivot virtuel
	m_pPiv = QVector3D( 0.0,  0.0, 0.0 ) ;

	// plans d'ancrage triangulaires
	updateBaseGeometry() ;
	updatePlatineGeometry() ;
}

// longueur courante du vérin d'indice i
// affecte m_vLen en fonction de m_mKin

float Hexapod::vlength(int i )
{
    QVector3D v = m_mKin.map( m_pAncRef[i] ) - m_bAnc[i] ;

	float eps = (m_pMaxAlt - m_pMinAlt) * 0.01 ;
	bool err = ( v.length() > m_vMax + eps )||( v.length() < m_vMin - eps ) ;
	m_corps[i]->setMaterial( err ? GLamMaterial::RedPlastic :GLamMaterial::YellowPlastic ) ;
	m_vLen(i) = v.length() ;

	return v.length() ;
}

// -----------------------------------------------------------------------
// MGI : raz des translations et rotations

void Hexapod::resetPosition()
{
	m_vKin.fill( 0.0 ) ;
	setMatrix_MGI() ;
}

// MGI : modification des translations / rotations

void Hexapod::setMgi(const QamMatrix6x1& kin )
{
	m_vKin = kin ;
	setMatrix_MGI() ;
}

// MGD : modification des longueurs des vérins

void Hexapod::setMgd(const QamMatrix6x1& len )
{
	bool err = false ;
	QamMatrix6x1 kin = newtonRaphson(m_vKin.toRadians(), len, err ).toDegrees() ;

	if ( err )	return ;

	m_vKin = kin ;
	setMatrix_MGD() ;
}


// -----------------------------------------------------------------------
// fonction objectif de l'algorithme de Newton-Raphson
// affecte les membres _tf et _jac
// kin :	vecteur des inconnues Tx,Ty,Tz,Rx,Ry,Rz		(mm/radians)
// len :	vecteur des longueurs des vérins			(mm)

void Hexapod::targetFunct(const QamMatrix6x1& kin, const QamMatrix6x1& len )
{
	float c1 = COS( kin(3) ) ;	float s1 = SIN( kin(3) ) ;
	float c2 = COS( kin(4) ) ;	float s2 = SIN( kin(4) ) ;
	float c3 = COS( kin(5) ) ;	float s3 = SIN( kin(5) ) ;

	for ( int i = 0 ; i < 6 ; ++i ) {
		float x = -m_bRay * _cA(i) + kin(0) + m_pRay * ( _cB(i) * (c3*c1-s3*s2*s1) + _sB(i) * (-c2*s1) )  ;
		float y = -m_bRay * _sA(i) + kin(1) + m_pRay * ( _cB(i) * (c3*s1+s3*s2*c1) + _sB(i) * ( c2*c1) )  ;
		float z =  m_pMinAlt       + kin(2) + m_pRay * ( _cB(i) * (-s3*c2) + _sB(i) * s2 ) ;
		_tf(i) = qPow(x, 2 ) + qPow(y, 2 ) + qPow(z, 2 ) - qPow(len(i), 2 ) ;
		_jac(i,0) =  2 * x ;
		_jac(i,1) =  2 * y ;
		_jac(i,2) =  2 * z ;
		_jac(i,3) =  2 * m_pRay * (( _cB(i)*(-c3*s1-s3*s2*c1) - _sB(i)*c2*c1) * x + (_cB(i)*(c3*c1-s3*s2*s1) - _sB(i)*c2*s1) * y ) ;
		_jac(i,4) =  2 * m_pRay * ((-_cB(i)*s3*c2*s1 + _sB(i)*s2*s1) * x + (_cB(i) * s3*c2*s1 - _sB(i)*s2*c1) * y + (_cB(i)*s3*s2 + _sB(i)*c2) * z ) ;
		_jac(i,5) = -2 * m_pRay *    _cB(i)*(c3*c2) * z ;
	}

	_tf  /= qPow(m_pMinAlt, 2 ) ;
	_jac /= qPow(m_pMinAlt, 2 ) ;
}

// -----------------------------------------------------------------------
// schéma de Newton-Raphson
// utilise les membres _tf et _jac affectés par targetFunct()
//
// kin :	vecteur conditions initiales Tx,Ty,Tz,Rx,Ry,Rz	(mm/radians)
// len :	vecteur des longueurs des vérins				(mm)

QamMatrix6x1 Hexapod::newtonRaphson(const QamMatrix6x1& kin, const QamMatrix6x1& len, bool& err )
{
    int				imax = 50 ;		// nombre max. d'itérations
	qreal			eps = 10e-6 ;	// seuil de convergence
	QamMatrix6x1	res = kin ;

	for ( int i = 0 ; i <= imax ; ++i ) {

		targetFunct(res, len ) ;

		if ( _tf.norm() <= eps ) {	// issue attendue...
			err = false ;
//			qDebug() << "NR ok, i=" << i ;
			break ;
		}
		else if ( i >= imax ) {
			err = true ;
			qDebug() << "pb de convergence" ;
			break ;
		}
		else {						// itération...
			QamMatrix6x6 m = _jac ;
			m.inverted() ;
			QamMatrix1x6 nx = res.toColumn() ;
			nx -= m.mult( _tf ) ;
			res = nx.toRow() ;
		}
	}

	// modulo 2PI sur les angles de rotation
	for ( int i = 3 ; i < 6 ; ++i ) {
		int sign = ( res(i) > 0 ? 1 : -1 ) ;
		while ( qAbs( res(i) ) > 2 * M_PI )	res(i) -= sign * 2 * M_PI ;
	}
	// Tz négatif interdit !
	if ( res(2) < 0 )	res(2) = 0 ;

	return res ;
}

// détermination des matrices 3D après modif. m_vKin par Newton-Raphson

void Hexapod::setMatrix_MGD()
{
    float eps = 1e-3 ;
	for ( int i = 0 ; i < 6 ; ++i ) {
		if ( qAbs( m_vKin(i) ) < eps )	m_vKin(i) = 0.0 ;
	}

	float c1 = COS( qDegreesToRadians( m_vKin(3) ) ) ;
	float s1 = SIN( qDegreesToRadians( m_vKin(3) ) ) ;
	float c2 = COS( qDegreesToRadians( m_vKin(4) ) ) ;
	float s2 = SIN( qDegreesToRadians( m_vKin(4) ) ) ;
	float c3 = COS( qDegreesToRadians( m_vKin(5) ) ) ;
	float s3 = SIN( qDegreesToRadians( m_vKin(5) ) ) ;

	m_mKin.setToIdentity() ;
	m_mKin.translate( QVector3D(m_vKin(0), m_vKin(1), m_vKin(2) ) ) ;
	m_mKin(0,0) = c3 * c1 - s3 * s2 * s1 ;
	m_mKin(0,1) = -c2 * s1 ;
	m_mKin(0,2) = 0 ;
	m_mKin(1,0) = c3 * s1 + s3 * s2 * c1 ;
	m_mKin(1,1) = c2 * c1 ;
	m_mKin(1,2) = 0 ;
	m_mKin(2,0) = -s3 * c2 ;
	m_mKin(2,1) = s2 ;
	m_mKin(2,2) = 1 ;

	m_mKinMockup.setToIdentity() ;
	m_mKinMockup.translate( QVector3D(m_vKin(0), m_vKin(1), m_vKin(2) ) ) ;
	m_mKinMockup.rotate(m_vKin(3), 0, 0, 1 ) ;
	m_mKinMockup.rotate(m_vKin(4), 1, 0, 0 ) ;
	m_mKinMockup.rotate(m_vKin(5), 0, 1, 0 ) ;

	m_vKin.set(m_vKin(0), m_vKin(1), m_vKin(2), m_vKin(4), m_vKin(5), m_vKin(3) ) ;

	for ( int i = 0 ; i < 6 ; ++i )	vlength(i) ;	// màj m_vLen
}

// détermination des matrices 3D après modif. directe de m_vKin

void Hexapod::setMatrix_MGI()
{
    m_mKin.setToIdentity() ;
	m_mKin.translate( QVector3D(m_vKin(0), m_vKin(1), m_vKin(2) ) ) ;
	m_mKin.rotate(m_vKin(3), 1, 0, 0 ) ;
	m_mKin.rotate(m_vKin(4), 0, 1, 0 ) ;
	m_mKin.rotate(m_vKin(5), 0, 0, 1 ) ;

	m_mKinMockup = m_mKin ;

	for ( int i = 0 ; i < 6 ; ++i )	vlength(i) ;	// màj m_vLen
}
