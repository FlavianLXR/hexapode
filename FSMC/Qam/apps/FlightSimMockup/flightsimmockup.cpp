/*  ---------------------------------------------------------------------------
 *  filename    :   flightsimmockup.cpp
 *  description :   IMPLEMENTATION de la classe FlightSimMockup
 *
 *	project     :	Scène 3D LLF Maquette éch. 1:1 FlightSim
 *  start date  :   septembre 2017
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

#include "flightsimmockup.h"
#include <QtMath>
#include <QDir>

#ifdef QAMDEBUG
	#include <QDebug>
#endif

FlightSimMockup::FlightSimMockup(QWidget* parent )
	: GLamWidget( parent )
    , m_remote( false )
	, m_roll( false )
	, m_pitch( false )
	, m_yaw( false )
//	, m_iMode( false )
//	, m_bMode( false )
    , m_zMode( Stop )
	, m_hexapod( 0 )
{
	// point de vue 3D initial...
	scene().setSize(1) ;			// taille 1U = 1 mêtre à 100%
	scene().setScale(0.7) ;			// zomm = 70%
	scene().setTranslationX(0.1) ;	// décalage scène 0.1*size() à gauche
}

FlightSimMockup::~FlightSimMockup()
{
    delete m_console ;
    delete m_seat    ;
    delete m_support ;
    delete m_hexapod ;
    delete m_xyzAxis ;
    delete m_handle  ;
    delete m_rubber  ;
    delete m_collective ;
}

// -----------------------------------------------------------------------
// mise en position de départ/repos (id. touches I/B)
/*
void FlightSimMockup::setPosition(bool initPosition )
{
	m_iMode = initPosition ; m_bMode = !initPosition ;
	emit autoRunning( true ) ;
}
*/
void FlightSimMockup::setPosition(ZPos position )
{
    m_zMode = position ;
//    emit autoRunning( true ) ;
    emit autoRunning( 1 ) ;     // 1:localMGD
}

// -----------------------------------------------------------------------
// pilotage de l'animation roulis, tangage, lacet

void FlightSimMockup::setRemoteAnimation(bool remote )
{
    m_remote = remote ;
    emit autoRunning( m_remote ? 3 : 0 ) ;
}

void FlightSimMockup::setAnimation(bool roll, bool pitch, bool yaw )
{
	m_roll = roll ;
	m_pitch = pitch ;
	m_yaw = yaw ;
    bool anim  = m_roll || m_pitch || m_yaw ;
    emit autoRunning( anim ? 2 : 0 ) ;      // 2:localMGI
}

// -----------------------------------------------------------------------

void FlightSimMockup::createObjects()
{
    m_xyzAxis = new Axis3D ;
    m_hexapod = new Hexapod ;
    m_support = new FSmuFrame ;
    m_console = new FSmuConsole ;
    m_seat    = new FSmuSeat ;
    m_handle  = new FSmuHandle ;
    m_rubber  = new FSmuRudder ;
    m_collective = new FSmuCollective ;

//	m_support->setName("support");
}

// projection des objets sur la scène 3D

void FlightSimMockup::drawObjects()
{
	float	globalScale = 0.001 ;	// toutes dim. en mm

	m_xyzAxis->transformMatrix().setToIdentity() ;
	m_xyzAxis->transformMatrix().scale( globalScale ) ;
	m_xyzAxis->draw() ;

	m_hexapod->draw(globalScale) ;

	m_support->transformMatrix().setToIdentity() ;
	m_support->transformMatrix().scale( globalScale ) ;
	m_support->transformMatrix() *= m_hexapod->transformMatrix() ;
	m_support->transformMatrix().translate( 150, 100, 90 ) ; // au centre de gravité...
    m_support->draw() ;

    m_seat->transformMatrix().setToIdentity() ;
    m_seat->transformMatrix().scale( globalScale ) ;
    m_seat->transformMatrix() *= m_hexapod->transformMatrix() ;
    m_seat->transformMatrix().translate( -420, 170, 194 ) ;
    m_seat->draw() ;

    m_collective->transformMatrix().setToIdentity() ;
    m_collective->transformMatrix().scale( globalScale ) ;
    m_collective->transformMatrix() *= m_hexapod->transformMatrix() ;
    m_collective->transformMatrix().translate( 150, 200, 185 ) ;
    m_collective->draw() ;

    m_console->transformMatrix().setToIdentity() ;
    m_console->transformMatrix().scale( globalScale ) ;
    m_console->transformMatrix() *= m_hexapod->transformMatrix() ;
    m_console->transformMatrix().translate( 675, 230, 194) ;
    m_console->draw() ;

    m_handle->transformMatrix().setToIdentity() ;
    m_handle->transformMatrix().scale( globalScale ) ;
    m_handle->transformMatrix() *= m_hexapod->transformMatrix() ;
    m_handle->transformMatrix().translate( 300, -55, 145 ) ;
    m_handle->draw() ;

    m_rubber->transformMatrix().setToIdentity() ;
    m_rubber->transformMatrix().scale( globalScale ) ;
    m_rubber->transformMatrix() *= m_hexapod->transformMatrix() ;
    m_rubber->transformMatrix().translate( 615, -110, 180 ) ;
    m_rubber->draw() ;
}

// animation périodique (période = 20 ms)

void FlightSimMockup::animateObjects()
{
    static unsigned mgiCount = 0 ;

    // remote MGI control ?

    if ( m_remote ) {
        emit getRemoteMgi() ;
    }

    HexapodData  d   = m_hexapod->data() ;

    // animation MGI Rx,Ry,Rz ?

    if ( m_roll || m_pitch || m_yaw ) {

        QamMatrix6x1 kin = m_hexapod->mgi() ;

        qreal hMax = d.topRadius * SIN( qDegreesToRadians( d.maxAngle ) ) ;
        qreal tc = 4 * hMax / d.maxSpeed ;	// temps pour un cycle (s)
        qreal te = mgiCount * 0.02 ;			// temps écoulé (s)

        if ( m_roll )	kin(3) = d.maxAngle * SIN( 2 * M_PI * te/tc ) ;
        if ( m_pitch )	kin(4) = d.maxAngle * SIN( 2 * M_PI * te/tc ) ;
        if ( m_yaw )	kin(5) = d.maxAngle * SIN( 2 * M_PI * te/tc ) ;

        m_hexapod->setMgi(kin) ;
        mgiCount++ ;
    }
    else    mgiCount = 0 ;

    // animation MGD vers position Tz min/moy/max ?

    if ( m_zMode != Stop ) {
        float v = ( d.maxLen + d.minLen ) / 2 ;
        if ( m_zMode == ToBottom )      v = d.minLen ;
        else if ( m_zMode == ToTop )    v = d.maxLen ;
		float step = 1.0 ;
		bool ok = true ;
		QamMatrix6x1 len = m_hexapod->mgd() ;
		for ( int i = 0 ; i < 6 ; ++i ) {
			float delta = len(i) - v ;
			if ( qAbs(delta) > 1.0 ) {
				len(i) += ( delta < 0 ? step : -step ) ;
				ok = false ;
			}
			else len(i) = v ;
		}
		m_hexapod->setMgd(len) ;
		emit actuatorLenChanged() ;
        if ( ok )  {
            m_zMode = Stop ;
            emit autoRunning( 0 ) ;
        }
	}
}

// sur-impression de texte sur scène 3D
// valeur courante DOF [Tx,Ty,Tz,Rx,Ry,Rz]
// valeur courante longueur des actuateurs

void FlightSimMockup::paintOver(QPainter* painter )
{
//	GLamWidget::paintOver(painter) ;
	QamMatrix6x1 len = m_hexapod->mgd() ;
	QamMatrix6x1 kin = m_hexapod->mgi() ;

    // correction chgt de cadran des angles     new v0.6
    for ( int i = 3 ; i <= 5 ; ++i ) {
        if ( kin(i) > +90 )  kin(i) -= 180 ;
        if ( kin(i) < -90 )  kin(i) += 180 ;
    }
    // limitation à 2 décimales
    for ( int i = 0 ; i <= 5 ; ++i ) {
        kin(i) = qRound( kin(i) * 100 ) / 100.0 ;
    }
    //

    QString	mess = QString("T ( %1 , %2 , %3 )  R ( %4 , %5 , %6 )")
					.arg(kin(0)).arg(kin(1)).arg(kin(2))
					.arg(kin(3)).arg(kin(4)).arg(kin(5)) ;
	painter->setPen( Qt::gray ) ;
	painter->drawText(10, height() - 10, mess ) ;

	mess = QString("L ( %1 , %2 , %3 , %4 , %5 , %6 )")
			.arg(len(0)).arg(len(1)).arg(len(2))
			.arg(len(3)).arg(len(4)).arg(len(5)) ;
	painter->drawText(10, height() - 30, mess ) ;
}

/*
// expérimental 24/04/2020 : non testé
void FlightSimMockup::setAngles(float roll, float pitch, float yaw )
{
	QamMatrix6x1 kin = m_hexapod->mgi() ;
	float mr = m_hexapod->data().maxAngle ;
	
	if ( roll >  mr )	roll =  mr ;
	if ( roll < -mr )	roll = -mr ;
	kin(3) = roll ;
	if ( pitch >  mr )	pitch =  mr ;
	if ( pitch < -mr )	pitch = -mr ;
	kin(4) = pitch ;
	if ( yaw >  mr )	yaw =  mr ;
	if ( yaw < -mr )	yaw = -mr ;
	kin(5) = yaw ;
	
	m_hexapod->setMgi(kin) ;
	emit actuatorLenChanged() ;
}
*/
//

// aide contextuelle

QString FlightSimMockup::help()
{
    QString s ;
    // new v0.6
    s += "<br>" ;
    s += "<table border cellpadding=\"3\" cellspacing=\"0\" >" ;
    s += "<tr><td>Zpos:</td><td align=\"center\">[R]eset</td><td align=\"center\">[M]iddle</td></tr>" ;
    s += "<tr><td>Zpos:</td><td align=\"center\">[B]ottom</td><td align=\"center\">[T]op</td></tr>" ;
    s += "</table>" ;
    s += "<br>" ;
    //
    s += "<table border cellpadding=\"3\" cellspacing=\"0\" >" ;
    s += "<tr><td>MGI:</td><td align=\"center\">Tx</td><td align=\"center\">Ty</td><td align=\"center\">Tz</td></tr>" ;
    s += "<tr><td>key:</td><td align=\"center\">x|X</td><td align=\"center\">y|Y</td><td align=\"center\">z|Z</td></tr>" ;
    s += "<tr><td>MGI:</td><td align=\"center\">Rx</td><td align=\"center\">Ry</td><td align=\"center\">Rz</td></tr>" ;
    s += "<tr><td>key:</td><td align=\"center\">^Le|^Ri</td><td align=\"center\">^Up|^Dn</td><td align=\"center\">w|W</td></tr>" ;
    s += "<tr><td>MGD:</td><td align=\"center\">len #0</td><td align=\"center\">len #1</td><td align=\"center\"></td></tr>" ;
    s += "<tr><td>key:</td><td align=\"center\">u|d</td><td align=\"center\">U|D</td><td align=\"center\"></td></tr>" ;
    s += "</table>" ;
    return s ;
}

// intercepteur clavier (cf. texte d'aide ci-dessus :-)

void FlightSimMockup::keyPressEvent(QKeyEvent* event )
{
	float	mr = m_hexapod->data().maxAngle ;
	float	r = 1 ;			// pas de rotation en degrés
	float	mt = 100 ;		// décalage absolu max. axes X Y
	float	mtz = 500 ;		// décalage absolu max. axe Z
	float	t = 10 ;		// pas de décalage en translation

	bool mgi = true ;		// action mode indirect (par défaut)
	bool mgd = false ;		// action mode direct

	bool chain = true ;		// chainage vers keyPressEvent() parent ?

		QamMatrix6x1 len = m_hexapod->mgd() ;
		QamMatrix6x1 kin = m_hexapod->mgi() ;

	if ( event->modifiers() & Qt::ShiftModifier ) switch( event->key() ) {

		case Qt::Key_X :		if ( kin(0) < mt )	kin(0) += t ; break ;
		case Qt::Key_Y :		if ( kin(1) < mt )	kin(1) += t ; break ;
		case Qt::Key_Z :		if ( kin(2) < mtz )	kin(2) += t ; break ;

		case Qt::Key_Left :		if ( kin(3) <  mr )	kin(3) += r ; chain = false ; break ;
		case Qt::Key_Right :	if ( kin(3) > -mr )	kin(3) -= r ; chain = false ; break ;
		case Qt::Key_Up :		if ( kin(4) <  mr )	kin(4) += r ; chain = false ; break ;
		case Qt::Key_Down :		if ( kin(4) > -mr )	kin(4) -= r ; chain = false ; break ;

		case Qt::Key_W :		if ( kin(5) <  mr )	kin(5) += r ; break ;

		case Qt::Key_U :	len(1) += 10 ;
							mgi = false ; mgd = true ;
							break ;
		case Qt::Key_D :	len(1) -= 10 ;
							mgi = false ; mgd = true ;
							break ;

		default :			mgi = false ;
	}
	else switch( event->key() ) {
		case Qt::Key_X :	if ( kin(0) > -mt )	kin(0) -= t ; break ;
		case Qt::Key_Y :	if ( kin(1) > -mt )	kin(1) -= t ; break ;
		case Qt::Key_Z :	if ( kin(2) > -mtz )	kin(2) -= t ; break ;

		case Qt::Key_W :	if ( kin(5) > -mr )	kin(5) -= r ; break ;

		case Qt::Key_R :	for ( int i = 0 ; i < 6 ; ++i ) kin(i) = 0 ;
							break ;
        case Qt::Key_M :	mgi = false ;
                            setPosition( ToMiddle ) ;
							break ;
        case Qt::Key_B :	mgi = false ;
                            setPosition( ToBottom ) ;
							break ;
        case Qt::Key_T :    mgi = false ;
                            setPosition( ToTop ) ;
                            break ;

		case Qt::Key_U :	len(0) += 10 ;
							mgi = false ; mgd = true ;
							break ;
		case Qt::Key_D :	len(0) -= 10 ;
							mgi = false ; mgd = true ;
							break ;

		case Qt::Key_P :	exportToPng( QDir::homePath() + "/Desktop/FSmockup.png" ) ;
							mgi = false ;
							break ;

		default :			mgi = false ;
	}
	if ( mgi ) {
		m_hexapod->setMgi(kin) ;
		emit actuatorLenChanged() ;
	}
	if ( mgd ) {
		m_hexapod->setMgd(len) ;
		emit actuatorLenChanged() ;
	}

	if ( chain )	GLamWidget::keyPressEvent( event ) ;
}
