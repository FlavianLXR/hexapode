/*  ---------------------------------------------------------------------------
 *  filename    :   mainwindow.h
 *  description :   INTERFACE de la classe MainWindow
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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui>
#include <QtWidgets>

#include "ui_mainwindow.h"
#include "flightsimmockup.h"
#include <GLamWidgetUi>
#include <QamLed>

#include <hexapodconfigurator.h>

#include <QSettings>        // new v0.7
#include <QamTcpSocket>
#include <QamModbusMap>

class MainWindow : public QMainWindow , private Ui::MainWindow
{
	Q_OBJECT

  public:
	explicit MainWindow(QWidget* parent = 0 ) ;
	//test
//	~MainWindow() { delete m_glamWidget ; }
	//
    enum Mode { JOG, CONNECT, DEMO, AUTO } ;
    // JOG : configuration, MGD
    // CONNECT : JOG + connexion serveur modbus (accès R/W configuration)
    // DEMO : animation locale rotation(s) (MGI)
    // AUTO : pilotage via serveur Modbus

  protected:
    void closeEvent(QCloseEvent* event ) ;

  private slots:
    void resetScene() ;
	void on_chkRoll_toggled(bool checked ) ;
	void on_chkPitch_toggled(bool checked ) ;
	void on_chkYaw_toggled(bool checked ) ;
    void on_sliSampling_valueChanged(int val ) ;

	void sliderValueChanged(int v ) ;
	void setSliderValue() ;

    void setActivityLed(int mode ) ;

	void glamInitialized() ;

    void getRemoteMgi() ;
    void loadTemplateFromFile() ;
    void saveTemplateToFile() ;

  private:
	void init_clicked() ;
	void down_clicked() ;
    void up_clicked() ;
    void export_clicked() ;

    void readSettings() ;
    void writeSettings() ;
    void loadCsv_clicked() ;
    void modbusConnect_clicked() ;
    void modbusRun_clicked() ;

  private:
    void enableLocalMGI(bool enabled = true ) ;
    void createLeftPanel() ;
    void createRightPanel() ;
    void setSliderStroke() ;
    void setInRange(float& val, float min, float max ) ;

	FlightSimMockup*	m_glamWidget ;
	GLamWidgetUi*		m_glamWidgetUi ;

	Hexapod*				m_hexapod ;	// lien fourni par m_glamWidget
	HexapodConfigurator*	m_hexapodConfigurator ;

	QVBoxLayout*	m_leftBox ;
	QVBoxLayout*	m_rightBox ;
	QBoxLayout*		m_layout ;

    QGroupBox*      m_animGroup ;
	QCheckBox*		m_chkRoll ;
	QCheckBox*		m_chkPitch ;
	QCheckBox*		m_chkYaw ;

	QPushButton*	m_btnInit ;
	QPushButton*	m_btnDown ;
    QPushButton*	m_btnUp ;

    QCheckBox*		m_chkPercentLen ;   // new v0.6
    QPushButton*    m_btnExport ;

    QPushButton*    m_btnModbusCsv ;    // new v0.7
    QLineEdit*      m_edtServer ;
    QSlider*        m_sliSampling ;
    QLabel*         m_labSamplingPeriod ;
    QPushButton*    m_btnModbusConnect ;
    QPushButton*    m_btnModbusRun ;

	QSlider*		m_slider[6] ;

    QamLed*			m_led[3] ;          // anim. locale / modbus connect / modbus run

    QString         m_configDir ;       // new v0.7
    QamModbusMap*	m_modbusMap ;
    QamTcpClient*	m_tcpClient ;

    Mode            m_mode ;
} ;

#endif // MAINWINDOW_H
