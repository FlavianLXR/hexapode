/*  ---------------------------------------------------------------------------
 *  filename    :   mainwindow.cpp
 *  description :   IMPLEMENTATION de la classe MainWindow
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

#include "mainwindow.h"
#include "_ABOUT"

MainWindow::MainWindow(QWidget* parent )
	: QMainWindow(parent)
	, m_hexapod( 0 )
	, m_hexapodConfigurator( 0 )
    , m_tcpClient( 0 )
    , m_mode( JOG )
{
    // scène 3D et son inteface de pilotage

    m_glamWidget = new FlightSimMockup ;
    m_glamWidget->setMinimumSize(300, 300 ) ;
    //
    resetScene() ;
    //
    m_glamWidgetUi = new GLamWidgetUi( this ) ;
    m_glamWidgetUi->bindTo( m_glamWidget ) ;

#ifdef Q_OS_ANDROID
    m_glamWidgetUi->setCommentVisible(false) ;
#endif
#ifdef Q_OS_IOS
    m_glamWidgetUi->setCommentVisible(false) ;
#endif

    // interface utilisateur de l'application

    createLeftPanel() ;     // m_leftBox
    createRightPanel() ;    // m_rightBox

    setupUi( this ) ;

    QWidget* centralWidget = new QWidget( this ) ;

	m_layout = new QBoxLayout(QBoxLayout::LeftToRight, centralWidget ) ;
	m_layout->setSpacing(2) ;

	m_layout->addLayout( m_leftBox ) ;
	m_layout->addWidget( m_glamWidget ) ;
	m_layout->addLayout( m_rightBox ) ;
	m_layout->setStretchFactor( m_glamWidget, 6 ) ;
	m_layout->setStretchFactor( m_rightBox, 1 ) ;
	m_layout->setSpacing(1) ;

	this->setCentralWidget( centralWidget ) ;

	// connexions signaux-slots

	connect(m_glamWidget, SIGNAL(initialized()), this, SLOT(glamInitialized()) ) ;
	connect(m_glamWidget, SIGNAL( message(QString) ), m_glamWidgetUi, SLOT( addComment(QString) ) ) ;

	for ( int i = 0 ; i < 6 ; ++i ) {
		connect(m_slider[i], SIGNAL(valueChanged(int)), this, SLOT(sliderValueChanged(int)) ) ;
	}

	connect(m_glamWidget, SIGNAL(actuatorLenChanged()), this, SLOT(setSliderValue()) ) ;
    connect(m_glamWidget, SIGNAL(autoRunning(int)), this, SLOT(setActivityLed(int)) ) ;

    connect(m_glamWidget, SIGNAL(getRemoteMgi()), this, SLOT(getRemoteMgi() ) ) ;

    // Qt5 new connection syntax...
	connect(m_btnInit, &QPushButton::clicked, this, &MainWindow::init_clicked ) ;
	connect(m_btnDown, &QPushButton::clicked, this, &MainWindow::down_clicked ) ;

    connect(m_btnUp, &QPushButton::clicked, this, &MainWindow::up_clicked ) ;
    connect(m_btnExport, &QPushButton::clicked, this, &MainWindow::export_clicked ) ;

    connect(m_btnModbusCsv, &QPushButton::clicked, this, &MainWindow::loadCsv_clicked ) ;
    connect(m_btnModbusConnect, &QPushButton::clicked, this, &MainWindow::modbusConnect_clicked ) ;
    connect(m_btnModbusRun, &QPushButton::clicked, this, &MainWindow::modbusRun_clicked ) ;

    connect(m_hexapodConfigurator, SIGNAL(loadTemplate()), this, SLOT(loadTemplateFromFile()) ) ;
    connect(m_hexapodConfigurator, SIGNAL(saveTemplate()), this, SLOT(saveTemplateToFile()) ) ;

    connect(m_glamWidgetUi, SIGNAL(resetPressed()), this, SLOT(resetScene()) ) ;
	// let's go...

	QString s ;
	s += "<b>" ;
	s += QString(FLIGHTSIMMOCKUP_TITLE) ;
	s += " - v" + QString(FLIGHTSIMMOCKUP_VERSION) ;
	s += "</b>" ;
	s += m_glamWidget->help() ;
	m_glamWidgetUi->setComment( s ) ;

	m_glamWidget->setFocus() ;
}

// vue de profil par défaut...

void MainWindow::resetScene()
{
    m_glamWidget->scene().setRotationX(-90) ;
}

// slot invoqué APRÈS GLamWidget::createObjects()

void MainWindow::glamInitialized()
{
    m_hexapod = m_glamWidget->hexapod() ;

    // Register HexapodData and its two streams operator to qt meta-object system
    qRegisterMetaType<HexapodData>() ;
//    qRegisterMetaTypeStreamOperators<HexapodData>("HexapodData") ;
    readSettings() ;

    m_hexapodConfigurator->bindTo( m_hexapod ) ;
    setSliderStroke() ;
}

// choix d'animation Rx, Ry et Rz

void MainWindow::on_chkRoll_toggled(bool checked )
{
	Q_UNUSED( checked ) ;
	m_glamWidget->setAnimation(m_chkRoll->isChecked(), m_chkPitch->isChecked(), m_chkYaw->isChecked() ) ;
}

void MainWindow::on_chkPitch_toggled(bool checked )
{
	Q_UNUSED( checked ) ;
	m_glamWidget->setAnimation(m_chkRoll->isChecked(), m_chkPitch->isChecked(), m_chkYaw->isChecked() ) ;
}

void MainWindow::on_chkYaw_toggled(bool checked )
{
	Q_UNUSED( checked ) ;
	m_glamWidget->setAnimation(m_chkRoll->isChecked(), m_chkPitch->isChecked(), m_chkYaw->isChecked() ) ;
}

// positions départ (à mi-hauteur) / repos (basse)

void MainWindow::init_clicked()
{
    m_glamWidget->setPosition( FlightSimMockup::ToMiddle ) ;
}

void MainWindow::down_clicked()
{
    m_glamWidget->setPosition( FlightSimMockup::ToBottom ) ;
}

void MainWindow::up_clicked()
{
    m_glamWidget->setPosition( FlightSimMockup::ToTop ) ;
}

// new v0.6 : export current data to clipboard
// CSV format = roll;pitch;yaw;l1;l2;l3;l4;l5;l6

void MainWindow::export_clicked()
{
    if ( !m_hexapod )	return ;

    QamMatrix6x1 len = m_hexapod->mgd() ;
    QamMatrix6x1 kin = m_hexapod->mgi() ;

    if ( m_chkPercentLen->isChecked() ) {   // longueur des vérins en %
        float minL = m_hexapod->data().minLen ;
        float maxL = m_hexapod->data().maxLen ;
        for ( int i = 0 ; i < 6 ; ++i ) len(i) = qRound( ( len(i) - minL ) * ( 100 / (maxL - minL) ) ) ;
    }

    QString	mess = QString("%1;%2;%3;%4;%5;%6;%7;%8;%9\n")
            .arg(kin(3)).arg(kin(4)).arg(kin(5))
            .arg(len(0)).arg(len(1)).arg(len(2))
            .arg(len(3)).arg(len(4)).arg(len(5)) ;

    QGuiApplication::clipboard()->setText( mess ) ;
}

// mise à jour hexapode --> sliders

void MainWindow::setSliderStroke()
{
	if ( !m_hexapod )	return ;

	HexapodData d = m_hexapod->data() ;
	for ( int i = 0 ; i < 6 ; ++i ) {
		disconnect(m_slider[i], SIGNAL(valueChanged(int)), this, SLOT(sliderValueChanged(int)) ) ;
		m_slider[i]->setRange(d.minLen, d.maxLen ) ;
		connect(m_slider[i], SIGNAL(valueChanged(int)), this, SLOT(sliderValueChanged(int)) ) ;
	}
	setSliderValue() ;
}

void MainWindow::setSliderValue()
{
	if ( !m_hexapod )	return ;

	QamMatrix6x1	len = m_hexapod->mgd() ;
	for ( int i = 0 ; i < 6 ; ++i ) {
		disconnect(m_slider[i], SIGNAL(valueChanged(int)), this, SLOT(sliderValueChanged(int)) ) ;
		m_slider[i]->setValue( len(i) ) ;
		connect(m_slider[i], SIGNAL(valueChanged(int)), this, SLOT(sliderValueChanged(int)) ) ;
	}
}

// mise à jour sliders --> hexapode

void MainWindow::sliderValueChanged(int v )
{
	Q_UNUSED( v ) ;
	if ( m_hexapod ) {
		QamMatrix6x1	len ;
		for ( int i = 0 ; i < 6 ; ++i )	len(i) = m_slider[i]->value() ;
		m_hexapod->setMgd( len ) ;
	}
}

// indicateurs de mouvement automatique en cours
// mode = 0:stop, 1:localMGD, 2:localMGI, 3:remote

void MainWindow::setActivityLed(int mode )
{
    m_led[0]->setBlink( mode == 1 ? QamWidget::Fast : QamWidget::Fixed ) ;
    m_led[0]->setState( mode % 3 ?  QamWidget::Yellow : QamWidget::Off ) ;

    m_led[2]->setBlink( mode == 3 ? QamWidget::Slow : QamWidget::Fixed ) ;
    m_led[2]->setState( mode == 3 ? QamWidget::Red : QamWidget::Off ) ;
}

// fermeture -> sauvegarde de la configuration courante

void MainWindow::closeEvent(QCloseEvent* event )
{
    writeSettings() ;
    event->accept() ;
}

// sauvegarde inter-sessions

void MainWindow::readSettings()
{
    QSettings settings( QSettings::IniFormat, QSettings::UserScope, "qam.fr", "flightsimmockup" ) ;

    QPoint	pos = settings.value("MainWindow/pos", QPoint(10,10) ).toPoint() ;
    QSize	size = settings.value("MainWindow/size", QSize(640,480) ).toSize() ;
    resize( size ) ;
    move( pos ) ;

    QString ld = QDir::homePath() ;
#ifdef Q_OS_MAC
    ld = ld + QDir::separator() + "*" ;
#endif
    m_configDir = settings.value("Files/configDir", ld ).toString() ;

    HexapodData data = settings.value("Data/hexapod", QVariant() ).value<HexapodData>() ;
//    HexapodData data = settings.value("Data/hexapod", HexapodData() ).value<HexapodData>() ;
    // test
    if ( data.baseGap == 0 ) {
        m_hexapod->setData( HexapodData() ) ;
    }
    //
    else m_hexapod->setData(data) ;
}

void MainWindow::writeSettings()
{
    QSettings settings( QSettings::IniFormat, QSettings::UserScope, "qam.fr", "flightsimmockup" ) ;

    settings.setValue("MainWindow/pos", pos() ) ;
    settings.setValue("MainWindow/size", size() ) ;
    settings.setValue("Files/configDir", m_configDir ) ;

    QVariant qVar ;
    qVar.setValue( m_hexapod->data() ) ;
    settings.setValue("Data/hexapod", qVar ) ;
}

// Modbus client    // new v0.7

void MainWindow::loadCsv_clicked()
{
    QString	fileName = QFileDialog::getOpenFileName(
        this,
        tr("Ouvrir..."),
        m_configDir,
        tr("Configuration Modbus (*.csv)") ) ;

    if ( !fileName.isEmpty() ) {
        // mémorisation du répertoire source utilisé
        m_configDir = QFileInfo( fileName ).absolutePath() ;
#ifdef Q_OS_MAC
        m_configDir = m_configDir + QDir::separator() + "*" ;
#endif

        // configuration Modbus client
        m_modbusMap = new QamModbusMap(QamModbusMap::ClientMode, this ) ;

        m_tcpClient = new QamTcpClient(m_modbusMap, this ) ;

        m_modbusMap->loadMap( fileName ) ;

        m_edtServer->setText( QString("%1:%2").arg(m_modbusMap->host()).arg(m_modbusMap->port()) ) ;

        m_btnModbusConnect->setEnabled( true ) ;
    }
}

// connexion :      JOG --> CONNECT
// déconnexion :    AUTO | CONNECT --> JOG

void MainWindow::modbusConnect_clicked()
{
    if ( m_tcpClient == nullptr )   return ;

    bool connected = m_tcpClient->state() == QAbstractSocket::ConnectedState ;

    if ( connected ) {
        //
        if ( m_mode == AUTO )   modbusRun_clicked() ;
        //
        m_tcpClient->sockClose() ;
        m_btnModbusRun->setText("Run") ;
        connected = false ;
    }
    else {
        QString host = m_modbusMap->host() ;
        quint16 port = m_modbusMap->port() ;

        m_tcpClient->sockConnect(host, port ) ;
        if ( m_tcpClient->waitForConnected(3000) ) {
            connected = true ;
        }
        else {
            //info("tcp/ip", "Connection failed!" ) ;
        }
    }

    m_mode = connected ? CONNECT : JOG ;
    m_hexapodConfigurator->rwEnabled( connected ) ;
    m_btnModbusConnect->setText(connected ? "Close" : "Connect" ) ;
    m_btnModbusRun->setEnabled( connected ) ;
    m_led[1]->setState( connected ? QamWidget::Green : QamWidget::Off ) ;
}

// marche : CONNECT --> AUTO
// arrêt :  AUTO --> CONNECT

void MainWindow::modbusRun_clicked()
{
    if ( m_mode == AUTO ) {
        m_mode = CONNECT ;
        m_btnModbusRun->setText("Run") ;
        enableLocalMGI( true ) ;
        m_glamWidget->setRemoteAnimation( false ) ;
    }
    else if ( m_mode == CONNECT ) {
        m_mode = AUTO ;
        m_btnModbusRun->setText("Stop") ;
        enableLocalMGI( false ) ;
        m_glamWidget->setRemoteAnimation( true ) ;
    }
}

void MainWindow::on_sliSampling_valueChanged(int val )
{
    m_labSamplingPeriod->setText( QString("%1 ms").arg(val*20) ) ;
}

// fixe val dans l'intervalle min/max

void MainWindow::setInRange(float& val, float min, float max )
{
    if ( val < min )    val = min ;
    if ( val > max )    val = max ;
}

// slot de lecture Ti/Ri sur serveur Modbus

void MainWindow::getRemoteMgi()
{
    static unsigned autoCount = 0 ;

    QamMatrix6x1    kin ;
    QamModbusMap::PrimaryTable  table = QamModbusMap::HoldingRegister ;

    // lecture des registres
    kin(0) =  m_modbusMap->value(table, "Tx" ).toFloat();
    kin(1) =  m_modbusMap->value(table, "Ty" ).toFloat();
    kin(2) =  m_modbusMap->value(table, "Tz" ).toFloat();
    kin(3) =  m_modbusMap->value(table, "Rx" ).toFloat()/100 ;
    kin(4) =  m_modbusMap->value(table, "Ry" ).toFloat()/100 ;
    kin(5) =  m_modbusMap->value(table, "Rz" ).toFloat()/100 ;
    /*
    kin(0) =  m_modbusMap->value(table, "Tx" ).toFloat() ;
    kin(1) =  m_modbusMap->value(table, "Ty" ).toFloat() ;
    kin(2) =  m_modbusMap->value(table, "Tz" ).toFloat() ;
    kin(3) =  m_modbusMap->value(table, "Rx" ).toFloat() ;
    kin(4) =  m_modbusMap->value(table, "Ry" ).toFloat() ;
    kin(5) =  m_modbusMap->value(table, "Rz" ).toFloat() ;*/

    // tests d'intervalle
    float mt = m_hexapod->data().maxTrans ;
    for ( int i = 0 ; i <= 1 ; ++i )    setInRange( kin(i), -mt, mt ) ;

    //setInRange( kin(2), m_hexapod->minAltitude(), m_hexapod->minAltitude() ) ;

    float mr = m_hexapod->data().maxAngle ;
    for ( int i = 3 ; i <= 5 ; ++i )    setInRange( kin(i), -mr, mr ) ;

    // move it!
    if ( (autoCount % m_sliSampling->value() ) == 0 ) m_hexapod->setMgi(kin) ;
    autoCount++ ;

    // ------------------------------------------------------------------
    // pour test : modification arbitraire de la table modbus distante...
    {
   /*     static int cpt = 0 ;
        if ( cpt == 3000 )  cpt = 0 ;    pas d'accoup...

        HexapodData	data = m_hexapod->data() ;
        float v = data.maxAngle * SIN( 2 * M_PI * ( cpt % 100 ) / 100.0 ) ;
        m_modbusMap->setValue(table, "Rx", QString("%1").arg( v ) ) ;
        v = data.maxAngle * SIN( 2 * M_PI * ( cpt % 150 ) / 150.0 ) ;
        m_modbusMap->setValue(table, "Rz", QString("%1").arg( v ) ) ;
        v = data.maxTrans * SIN( 2 * M_PI * ( cpt % 50 ) / 50.0 ) ;
        m_modbusMap->setValue(table, "Ty", QString("%1").arg( v ) ) ;
        v = data.maxTrans * SIN( 2 * M_PI * ( cpt % 75 ) / 75.0 ) ;
        m_modbusMap->setValue(table, "Tx", QString("%1").arg( v ) ) ;
        cpt++ ;*/
    }
    // ------------------------------------------------------------------
}

// csv --> hexapod template...

void MainWindow::loadTemplateFromFile()
{
    HexapodData	data ;
    QamModbusMap::PrimaryTable  table = QamModbusMap::HoldingRegister ;

    data.baseRadius = m_modbusMap->value(table, "baseRadius" ).toFloat() ;
    data.baseGap = m_modbusMap->value(table, "baseGap" ).toFloat() ;
    data.topRadius = m_modbusMap->value(table, "topRadius" ).toFloat() ;
    data.topGap = m_modbusMap->value(table, "topGap" ).toFloat() ;
    data.minLen = m_modbusMap->value(table, "minLen" ).toFloat() ;
    data.maxLen = m_modbusMap->value(table, "maxLen" ).toFloat() ;
    data.maxAngle = m_modbusMap->value(table, "maxAngle" ).toFloat() ;
    data.maxTrans = m_modbusMap->value(table, "maxTrans" ).toFloat() ;
    data.maxSpeed = m_modbusMap->value(table, "maxSpeed" ).toFloat() ;

    m_hexapod->setData( data ) ;
    m_hexapodConfigurator->set( data ) ;

    down_clicked() ;    // force position initiale...
}

// hexapod template --> csv...

void MainWindow::saveTemplateToFile()
{
    HexapodData	data = m_hexapod->data() ;
    QamModbusMap::PrimaryTable  table = QamModbusMap::HoldingRegister ;

    m_modbusMap->setValue(table, "baseRadius", QString("%1").arg(data.baseRadius) ) ;
    m_modbusMap->setValue(table, "baseGap", QString("%1").arg(data.baseGap) ) ;
    m_modbusMap->setValue(table, "topRadius", QString("%1").arg(data.topRadius) ) ;
    m_modbusMap->setValue(table, "topGap", QString("%1").arg(data.topGap) ) ;
    m_modbusMap->setValue(table, "minLen", QString("%1").arg(data.minLen) ) ;
    m_modbusMap->setValue(table, "maxLen", QString("%1").arg(data.maxLen) ) ;
    m_modbusMap->setValue(table, "maxAngle", QString("%1").arg(data.maxAngle) ) ;
    m_modbusMap->setValue(table, "maxTrans", QString("%1").arg(data.maxTrans) ) ;
    m_modbusMap->setValue(table, "maxSpeed", QString("%1").arg(data.maxSpeed) ) ;
}

void MainWindow::enableLocalMGI(bool enabled )
{
    if ( !enabled ) {
        m_chkRoll->setChecked( false ) ;
        m_chkPitch->setChecked( false ) ;
        m_chkYaw->setChecked( false ) ;
    }
    m_chkRoll->setEnabled( enabled ) ;
    m_chkPitch->setEnabled( enabled ) ;
    m_chkYaw->setEnabled( enabled ) ;

    m_btnInit->setEnabled( enabled ) ;
    m_btnDown->setEnabled( enabled ) ;
    m_btnUp->setEnabled( enabled ) ;
}

// UI : construction des panneaux de commandes gauche et droit

void MainWindow::createLeftPanel()
{
    m_leftBox = new QVBoxLayout ;

    // hexapode : groupe 'configurateur'
    // --------------------------------------------------------------

    m_hexapodConfigurator = new HexapodConfigurator( this ) ;
    m_hexapodConfigurator->rwEnabled( false ) ;
    m_hexapodConfigurator->setStyleSheet("QGroupBox { font-weight: bold; color: darkBlue; } " ) ;

    m_leftBox->addWidget(m_hexapodConfigurator) ;

    // hexapode : groupe 'animation MGI'
    // --------------------------------------------------------------

    m_animGroup = new QGroupBox(tr("Hexapod Animation (MGI)"), this ) ;
    m_animGroup->setStyleSheet("QGroupBox { font-weight: bold; color: darkBlue; } " ) ;

    QVBoxLayout* vbox = new QVBoxLayout ;
    vbox->setSpacing(8) ;

    // rotations roulis, tangage, lacet

    m_chkRoll = new QCheckBox("Roll", this ) ;
    m_chkRoll->setObjectName( QStringLiteral("chkRoll") ) ;
    m_chkPitch = new QCheckBox("Pitch", this ) ;
    m_chkPitch->setObjectName( QStringLiteral("chkPitch") ) ;
    m_chkYaw = new QCheckBox("Yaw", this ) ;
    m_chkYaw->setObjectName( QStringLiteral("chkYaw") ) ;

    QHBoxLayout* hbox = new QHBoxLayout ;
    hbox->addWidget( m_chkRoll ) ;
    hbox->addWidget( m_chkPitch ) ;
    hbox->addWidget( m_chkYaw ) ;
    vbox->addLayout(hbox) ;

    // modbus client

    m_btnModbusCsv = new QPushButton("Modbus map...", this ) ;                  // new v0.7
    m_btnModbusCsv->setObjectName( QStringLiteral("btnModbusCsv") ) ;
    m_edtServer = new QLineEdit(this ) ;
    m_edtServer->setReadOnly( true ) ;

    m_sliSampling = new QSlider(this) ;
    m_sliSampling->setObjectName( QStringLiteral("sliSampling") ) ;
    m_sliSampling->setRange(1,10) ;
    m_sliSampling->setOrientation(Qt::Horizontal) ;
    m_sliSampling->setTickInterval(1) ;
    m_sliSampling->setTickPosition(QSlider::TicksAbove) ;
    m_labSamplingPeriod = new QLabel(this) ;
    m_labSamplingPeriod->setFixedWidth(50) ;
    on_sliSampling_valueChanged(1) ;

    m_btnModbusConnect = new QPushButton("Connect", this ) ;
    m_btnModbusConnect->setObjectName( QStringLiteral("btnModbusConnect") ) ;
    m_btnModbusConnect->setMinimumWidth(100) ;
    m_btnModbusConnect->setEnabled( false ) ;

    m_btnModbusRun = new QPushButton("Run", this ) ;
    m_btnModbusRun->setObjectName( QStringLiteral("btnModbusRun") ) ;
    m_btnModbusRun->setMinimumWidth(100) ;
    m_btnModbusRun->setEnabled( false ) ;

    QFrame* line = new QFrame ;
    line->setFrameShape( QFrame::HLine ) ;
    vbox->addWidget( line ) ;
    vbox->addWidget( m_btnModbusCsv ) ;

    QHBoxLayout* hbox3 = new QHBoxLayout ;
    QLabel* labServer = new QLabel("server: ", this ) ;
    hbox3->addWidget( labServer ) ;
    hbox3->addWidget( m_edtServer ) ;
    vbox->addLayout(hbox3) ;

    QHBoxLayout* hbox6 = new QHBoxLayout ;
    QLabel* labSampling = new QLabel("sampling: ", this ) ;
    hbox6->addWidget( labSampling ) ;
    hbox6->addWidget( m_sliSampling ) ;
    hbox6->addWidget( m_labSamplingPeriod ) ;
    vbox->addLayout(hbox6) ;

    QHBoxLayout* hbox4 = new QHBoxLayout ;
    hbox4->addWidget( m_btnModbusConnect ) ;
    hbox4->addWidget( m_btnModbusRun ) ;
    vbox->addLayout(hbox4) ;

    m_animGroup->setLayout( vbox ) ;
    m_leftBox->addWidget( m_animGroup ) ;

    // indicateurs de mode
    // --------------------------------------------------------------

    for ( int i = 0 ; i < 3 ; ++i ) {
        m_led[i] = new QamLed( this ) ;
        m_led[i]->setOrientation( QamWidget::Right ) ;
        m_led[i]->setShape(QamWidget::Square ) ;
        m_led[i]->setState( QamWidget::Off ) ;
    }
    m_led[0]->setText("Local animation") ;
    m_led[1]->setText("Connected to server") ;
    m_led[2]->setText("Remote control") ;

    QFrame* line1 = new QFrame ;
    line1->setFrameShape( QFrame::HLine ) ;
    m_leftBox->addWidget( line1 ) ;
    for ( int i = 0 ; i < 3 ; ++i ) {
        m_leftBox->addSpacing( 6 ) ;
        m_leftBox->addWidget( m_led[i] ) ;
    }

    // export vers clipboard
    // --------------------------------------------------------------

    m_btnExport = new QPushButton("Current position to clipboard", this ) ;     // new v0.6
    m_btnExport->setObjectName( QStringLiteral("btnExport") ) ;
    m_btnExport->setToolTip("CSV format = roll;pitch;yaw;L1;L2;L3;L4;L5;L6") ;
    m_chkPercentLen = new QCheckBox("Percentage lengths", this ) ;
    m_chkPercentLen->setObjectName( QStringLiteral("chkPercentLen") ) ;

    QFrame* line2 = new QFrame ;
    line2->setFrameShape( QFrame::HLine ) ;
    m_leftBox->addStretch() ;
    m_leftBox->addWidget( line2 ) ;
    m_leftBox->addSpacing( 6 ) ;
    m_leftBox->addWidget( m_chkPercentLen ) ;
    m_leftBox->addSpacing( 6 ) ;
    m_leftBox->addWidget( m_btnExport ) ;
    m_leftBox->addSpacing( 6 ) ;
}

void MainWindow::createRightPanel()
{
    m_rightBox = new QVBoxLayout ;

    // interface de pilotage scène 3D
    // --------------------------------------------------------------

    m_rightBox->addWidget( m_glamWidgetUi ) ;

    // hexapode : groupe 'animation MGD'
    // --------------------------------------------------------------

    QGroupBox* mgdGroup = new QGroupBox("Actuator Lengths (MGD)") ;
    mgdGroup->setStyleSheet("QGroupBox { font-weight: bold; color: darkBlue; } " ) ;

    QVBoxLayout* vbox = new QVBoxLayout ;

    // positionnement bas / milieu / haut

    m_btnDown = new QPushButton("Dn", this ) ;
    m_btnDown->setObjectName( QStringLiteral("btnDown") ) ;
    m_btnInit = new QPushButton("Mid", this ) ;
    m_btnInit->setObjectName( QStringLiteral("btnInit") ) ;
    m_btnUp = new QPushButton("Up", this ) ;
    m_btnUp->setObjectName( QStringLiteral("btnUp") ) ;

    QHBoxLayout* btnbox = new QHBoxLayout ;
    btnbox->addWidget( m_btnDown ) ;
    btnbox->addWidget( m_btnInit ) ;
    btnbox->addWidget( m_btnUp ) ;
    vbox->addLayout(btnbox) ;

    // contrôles unitaires des vérins

    QHBoxLayout* sliderbox = new QHBoxLayout ;
    for ( int i = 0 ; i < 6 ; ++i ) {
        m_slider[i] = new QSlider(Qt::Vertical, this ) ;
        m_slider[i]->setMinimumHeight(200) ;
        sliderbox->addWidget( m_slider[i] ) ;
    }
    vbox->addLayout(sliderbox) ;

    mgdGroup->setLayout( vbox ) ;
    m_rightBox->addWidget(mgdGroup) ;
    m_rightBox->setStretchFactor( m_glamWidgetUi, 2 ) ;
    m_rightBox->setStretchFactor( mgdGroup, 1 ) ;
}
