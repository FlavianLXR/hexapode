/*  ---------------------------------------------------------------------------
 *  filename    :   flightsimmockup.h
 *  description :   INTERFACE de la classe FlightSimMockup
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

#ifndef FLIGHTSIMMOCKUP_H
#define FLIGHTSIMMOCKUP_H

#include <GLamWidget>
#include <GLamExamples>

#include <hexapod.h>
#include <fsmuframe.h>
#include <fsmuconsole.h>
#include <fsmuseat.h>
#include <fsmuhandle.h>
#include <fsmurudder.h>
#include <fsmucollective.h>
#
class FlightSimMockup : public GLamWidget
{
	Q_OBJECT

  public:
	FlightSimMockup(QWidget* parent = Q_NULLPTR ) ;
	~FlightSimMockup() ;

	QString help() ;

	Hexapod* hexapod() { return m_hexapod ; }

	// expérimental 24/04/2020
//	void setAngles(float roll, float pitch, float yaw ) ;
    // new v0.6
    enum ZPos { Stop, ToBottom, ToMiddle, ToTop } ;

  protected:
	void createObjects() ;
	void drawObjects() ;
	void animateObjects() ;
	void paintOver(QPainter* painter ) ;
	void keyPressEvent(QKeyEvent* event ) ;

  signals:
	void actuatorLenChanged() ;
    void getRemoteMgi() ;
    void autoRunning(int) ; // 0:stop, 1:localMGD, 2:localMGI, 3:remote

  public slots:
    void setRemoteAnimation(bool remote ) ;
	void setAnimation(bool roll, bool pitch, bool yaw ) ;
    void setPosition(ZPos position ) ;

  private:
    // animations
    bool            m_remote ;      // contrôle distant
    bool			m_roll ;        // animation roulis
    bool			m_pitch ;       // animation tangage
    bool			m_yaw ;         // animation lacet
    ZPos            m_zMode ;       // positions down/middle/up
    bool            m_isRunning ;   // indicateur de mvt en cours

	// composants 3D
    Axis3D*			m_xyzAxis ;		// repère initial
    Hexapod*		m_hexapod ;		// plateforme de Stewart
    FSmuFrame*		m_support ;		// support mécano-soudé
    FSmuConsole*    m_console ;
    FSmuSeat*       m_seat    ;
    FSmuHandle*     m_handle  ;
    FSmuRudder*     m_rubber  ;
    FSmuCollective* m_collective ;
} ;

#endif // FLIGHTSIMMOCKUP_H
