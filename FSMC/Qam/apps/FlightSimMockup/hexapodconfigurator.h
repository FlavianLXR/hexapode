/*  ---------------------------------------------------------------------------
 *  filename    :   hexapodconfigurator.h
 *  description :   INTERFACE de la classe HexapodConfigurator
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

#ifndef HEXAPODCONFIGURATOR_H
#define HEXAPODCONFIGURATOR_H

#include <QtGui>
#include <QtWidgets>

#include <hexapod.h>

class HexapodConfigurator : public QGroupBox
{
	Q_OBJECT

  public:
	explicit HexapodConfigurator(QWidget* parent = nullptr ) ;
	void bindTo(Hexapod* hexapod ) ;
	void set(const HexapodData& data ) ;

    void rwEnabled(bool enabled = true ) {  // new v0.7
        m_btnLoad->setEnabled( enabled ) ;
        m_btnSave->setEnabled( enabled ) ;
    }

  signals:
    void loadTemplate() ;                   // new v0.7
    void saveTemplate() ;

  public slots:
	void on_spinLowRadius_valueChanged(int i ) ;
	void on_spinLowGap_valueChanged(int i ) ;
	void on_spinHighRadius_valueChanged(int i ) ;
	void on_spinHighGap_valueChanged(int i ) ;
	void on_spinMinLen_valueChanged(int i ) ;
	void on_spinMaxLen_valueChanged(int i ) ;
	void on_spinAngle_valueChanged(int i ) ;
    void on_spinTrans_valueChanged(int i ) ;
    void on_spinSpeed_valueChanged(int i ) ;

    void on_btnLoad_clicked() { emit loadTemplate() ; }
    void on_btnSave_clicked() { emit saveTemplate() ; }

private:
	Hexapod*		m_hexapod ;
	bool			m_sendToHexapod ;

	QVBoxLayout*	m_vbox ;
	QSpinBox*		m_spinLowRadius ;
	QSpinBox*		m_spinLowGap ;
	QSpinBox*		m_spinHighRadius ;
	QSpinBox*		m_spinHighGap ;
	QSpinBox*		m_spinMinLen ;
	QSpinBox*		m_spinMaxLen ;
	QLineEdit*		m_edtStroke ;
	QSpinBox*		m_spinAngle ;
    QSpinBox*		m_spinTrans ;
    QSpinBox*		m_spinSpeed ;
	QLineEdit*		m_edtRefAltitude ;

    QPushButton*    m_btnLoad ;             // new v0.7
    QPushButton*    m_btnSave ;

	bool			m_monoColumn ;

	QSpinBox* createSpinBox(int min, int max, QString name ) ;
	void addLine(QVBoxLayout* vbox ) ;
	void addSpinBox(QVBoxLayout* vbox, QSpinBox* spin, QString label ) ;
	void addEditBox(QVBoxLayout* vbox, QLineEdit* edt, QString label, bool ro = true ) ;
	void updateAltitude() ;
} ;

#endif // HEXAPODCONFIGURATOR_H
