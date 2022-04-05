/*  ---------------------------------------------------------------------------
 *  filename    :   dialog.cpp
 *  description :   IMPLEMENTATION de la classe d'usage UI
 *
 *	project     :	ModipAsker
 *  start date  :   janvier 2014
 *  ---------------------------------------------------------------------------
 *  Copyright 2014-2016 by Alain Menu   <alain.menu@ac-creteil.fr>
 *
 *  This file is part of "Qam Modbus over IP Project"
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

#include "dialog.h"
#include <QDebug>

Dialog::Dialog(QWidget* parent )
	: QDialog(parent)
	, m_isProtoTcp( true )
	, m_isConnected( false )
	, m_tcpClient( 0 )
	, m_mbapTiIsAuto( true )
	, m_mbapLenIsAuto( true )
{
	setupUi(this) ;
	
	pbuAbout->setIcon(QIcon(":/_res/icons/qam47.png") ) ;

	edtMbapTi->setEnabled( !m_mbapTiIsAuto ) ;
	edtMbapLen->setEnabled( !m_mbapLenIsAuto ) ;

#ifdef Q_OS_MAC
	edtDialog->setFont( QFont("Monaco", 12 ) ) ;
#else
	edtDialog->setFont( QFont("Courier", 10 ) ) ;
#endif

	on_cbxPduFunct_currentIndexChanged( cbxPduFunct->currentIndex() ) ;

	m_tcpClient = new QamTcpClient( 0, this ) ;

	connect( m_tcpClient, SIGNAL(sockReceived(QByteArray)), this, SLOT(sockReceive(QByteArray)) ) ,
	connect( m_tcpClient, SIGNAL(sockInfo(QString)), 		this, SLOT(sockInfo(QString)) ) ;

	m_udpClient = new QamUdpSocket( this ) ;

	connect( m_udpClient, SIGNAL(sockReceived(QByteArray)), this, SLOT(sockReceive(QByteArray)) ) ,
	connect( m_udpClient, SIGNAL(sockInfo(QString)),		this, SLOT(sockInfo(QString)) ) ;
}

Dialog::~Dialog()
{
}

void Dialog::closeEvent(QCloseEvent* event )
{
	if ( m_isConnected ) {
		m_tcpClient->sockClose() ;
		m_udpClient->close() ;
	}
	event->accept() ;
}

// transport layer

void Dialog::on_rbuTcp_toggled(bool checked )
{
	m_isProtoTcp = checked ;
}

// open / close connection

void Dialog::on_pbuConnect_clicked()
{
	if ( m_isConnected ) {
		if ( m_isProtoTcp )	m_tcpClient->sockClose() ;
		else				m_udpClient->sockClose() ;
		m_isConnected = false ;
		pbuConnect->setText("Connect") ;
	}
	else {
		if ( m_isProtoTcp ) {
			m_tcpClient->sockConnect(edtAddress->text(), edtPort->text().toInt() ) ;
		}
		else {
			m_udpClient->sockBind() ;
			m_udpClient->setRemoteHost(edtAddress->text(), edtPort->text().toInt() ) ;
			m_isConnected = true ;
			pbuConnect->setText("Close") ;
		}
	}

	enableConnectionFields( !m_isConnected ) ;
}

void Dialog::enableConnectionFields(bool enabled  )
{
	labServer->setEnabled( enabled ) ;
	edtAddress->setEnabled( enabled ) ;
	labPort->setEnabled( enabled ) ;
	edtPort->setEnabled( enabled ) ;
	rbuTcp->setEnabled( enabled ) ;
	rbuUdp->setEnabled( enabled ) ;
}

// MBAP Transaction Id auto inc

void Dialog::on_chkMbapTiAuto_toggled(bool checked )
{
	m_mbapTiIsAuto = checked ;
	edtMbapTi->setEnabled( !m_mbapTiIsAuto ) ;
}

// MBAP Len auto calculate

void Dialog::on_chkMbapLenAuto_toggled(bool checked )
{
	m_mbapLenIsAuto = checked ;
	edtMbapLen->setEnabled( !m_mbapLenIsAuto ) ;
}

// Modbus function

void Dialog::on_cbxPduFunct_currentIndexChanged(int index )
{
	m_funct = ( index < 6 ? index + 1 : index + 9 ) ;

	enableNumberFields( ( m_funct != 5 )&&( m_funct != 6 ) ) ;
	enableValueFields( ( m_funct == 5 )||( m_funct == 6 ) ) ;
	enableDataFields( ( m_funct == 15 )||( m_funct == 16 ) ) ;
}

void Dialog::enableNumberFields(bool enabled )
{
	labPduNumber->setEnabled( enabled ) ;
	edtPduNumber->setEnabled( enabled ) ;
	rbuPduNumberDec->setEnabled( enabled ) ;
	rbuPduNumberHex->setEnabled( enabled ) ;
	switch ( m_funct ) {
	case 1 :
	case 2 :
	case 15 :	edtPduNumber->setToolTip("1..2000 (07D0)") ;
				break ;
	case 3 :
	case 4 :	edtPduNumber->setToolTip("1..125 (007D)") ;
				break ;
	case 16 :	edtPduNumber->setToolTip("1..120 (0078)") ;
				break ;
	default :	edtPduNumber->setToolTip("") ;
	}
}

void Dialog::enableValueFields(bool enabled )
{
	labPduValue->setEnabled( enabled ) ;
	edtPduValue->setEnabled( enabled ) ;
	rbuPduValueDec->setEnabled( enabled ) ;
	rbuPduValueHex->setEnabled( enabled ) ;
	switch ( m_funct ) {
	case 5 :	edtPduValue->setToolTip("0000 or FF00") ;
				break ;
	case 6 :	edtPduValue->setToolTip("0..65535 (FFFF)") ;
				break ;
	default :	edtPduValue->setToolTip("") ;
	}
}

void Dialog::enableDataFields(bool enabled )
{
	labPduData->setEnabled( enabled ) ;
	edtPduData->setEnabled( enabled ) ;
	switch ( m_funct ) {
	case 15 :
	case 16 :	edtPduData->setToolTip("format HH HH HH HH ...") ;
				break ;
	default :	edtPduData->setToolTip("") ;
	}
}

// PDU fields unit

void Dialog::on_rbuPduAddrDec_toggled(bool checked )
{
	bool ok ;
	quint16	v = edtPduAddr->text().toUShort(&ok, checked ? 16 : 10 ) ;
	QString s ;
	s = s.setNum(v, checked ? 10 : 16 ).toUpper() ;
	if ( !checked )	while ( s.length() < 4 ) s = "0" + s ;
	edtPduAddr->setText( s ) ;
}

void Dialog::on_rbuPduNumberDec_toggled(bool checked )
{
	bool ok ;
	quint16	v = edtPduNumber->text().toUShort(&ok, checked ? 16 : 10 ) ;
	QString s ;
	s = s.setNum(v, checked ? 10 : 16 ).toUpper() ;
	if ( !checked )	while ( s.length() < 4 ) s = "0" + s ;
	edtPduNumber->setText( s ) ;
}

void Dialog::on_rbuPduValueDec_toggled(bool checked )
{
	bool ok ;
	quint16	v = edtPduValue->text().toUShort(&ok, checked ? 16 : 10 ) ;
	QString s ;
	s = s.setNum(v, checked ? 10 : 16 ).toUpper() ;
	if ( !checked )	while ( s.length() < 4 ) s = "0" + s ;
	edtPduValue->setText( s ) ;
}

// client socket

void Dialog::sockReceive(const QByteArray& frame )
{
	showPrompt("recv: ") ;
	showFrame(toHexString( frame ) + "\n", true, Qt::darkBlue ) ;
}

void Dialog::sockInfo(const QString& msg )
{
	// connection ack ?
	if (( !m_isConnected )&&( msg.startsWith("connected") )) {
		pbuConnect->setText("Close") ;
		m_isConnected = true ;
	}
	showPrompt("info: ") ;
	showMessage(msg + "\n", false, Qt::darkGray ) ;
}

// sending request

void Dialog::on_pbuSend_clicked()
{
	// getting UI values...

	bool ok ;
	m_mbapTi = edtMbapTi->text().toUShort(&ok, 10 ) ;
	if ( !ok )	showWarning("syntax error in MBAP Transaction Identifier field\n") ;
	m_mbapPi = edtMbapPi->text().toUShort(&ok, 16 ) ;
	if ( !ok )	showWarning("syntax error in MBAP Protocol Identifier field\n") ;
	m_mbapLen = edtMbapLen->text().toUShort(&ok, 10 ) ;
	if ( !ok )	showWarning("syntax error in MBAP Length field\n") ;
	m_mbapUi = edtMbapUi->text().toUShort(&ok, 16 ) ;
	if ( !ok )	showWarning("syntax error in MBAP Unit Identifierfield\n") ;

	m_addr   = edtPduAddr->text().toUShort(&ok, rbuPduAddrDec->isChecked() ? 10 : 16 ) ;
	if ( !ok )	showWarning("syntax error in PDU Address field\n") ;

	bool numberOk ;
	m_number = edtPduNumber->text().toUShort(&numberOk, rbuPduNumberDec->isChecked() ? 10 : 16 ) ;
	bool valueOk ;
	m_value  = edtPduValue->text().toUShort(&valueOk, rbuPduValueDec->isChecked() ? 10 : 16 ) ;

	bool dataOk ;
	m_data   = toHexArray( edtPduData->text(), &dataOk ) ;

	// checking values...

	if ( m_addr > 9998 )	showWarning("invalid PDU Address\n" ) ;

	quint16	len = 0 ;
	quint8	n = 0 ;

	switch ( m_funct ) {
	case 1 :
	case 2 :	len = 6 ;
				if ( !numberOk )	showWarning("syntax error in PDU Number field\n") ;
				if (( m_number < 1 )||( m_number > 2000 ))	showWarning("invalid PDU Number\n" ) ;
				break ;
	case 3 :
	case 4 :	len = 6 ;
				if ( !numberOk )	showWarning("syntax error in PDU Number field\n") ;
				if (( m_number < 1 )||( m_number > 125 ))	showWarning("invalid PDU Number\n" ) ;
				break ;
	case 5 :	len = 6 ;
				if ( !valueOk )	showWarning("syntax error in PDU Value field\n") ;
				if ( m_value )	m_value = 0xFF00 ;
				break ;
	case 6 :	len = 6 ;
				break ;
	case 15 :	if ( !numberOk )	showWarning("syntax error in PDU Number field\n") ;
				n = m_number / 8 + ( m_number % 8 ? 1 : 0 ) ;
				len = 7 + n ;
				if (( m_number < 1 )||( m_number > 2000 ))	showWarning("invalid PDU Number\n" ) ;
				if ( !dataOk )	showWarning("syntax error in PDU Data field\n") ;
				if ( m_data.count() != n )	showWarning("bad PDU Data size\n" ) ;
				break ;
	case 16 :	if ( !numberOk )	showWarning("syntax error in PDU Number field\n") ;
				n = m_number * 2 ;
				len = 7 + n ;
				if (( m_number < 1 )||( m_number > 120 ))	showWarning("invalid PDU Number\n" ) ;
				if ( !dataOk )	showWarning("syntax error in PDU Data field\n") ;
				if ( m_data.count() != n )	showWarning("bad PDU Data size\n" ) ;
				break ;
	}

	if ( m_mbapLenIsAuto ) {
		m_mbapLen = len ;
		QString s ;
		s.setNum(m_mbapLen, 10 ) ;
		edtMbapLen->setText( s ) ;
	}
	else {
		if ( m_mbapLen != len )	showWarning("bad MBAP length\n" ) ;
	}

	// building request frame...

	QByteArray	frame ;

	frame.append( ( m_mbapTi >> 8 ) & 0xFF ) ;
	frame.append( m_mbapTi & 0xFF ) ;
	frame.append( ( m_mbapPi >> 8 ) & 0xFF ) ;
	frame.append( m_mbapPi & 0xFF ) ;
	frame.append( ( m_mbapLen >> 8 ) & 0xFF ) ;
	frame.append( m_mbapLen & 0xFF ) ;
	frame.append( m_mbapUi ) ;

	frame.append( m_funct ) ;
	frame.append( ( m_addr >> 8 ) & 0xFF ) ;
	frame.append( m_addr & 0xFF ) ;

	switch ( m_funct ) {
	case 1 :
	case 2 :
	case 3 :
	case 4 :	frame.append( ( m_number >> 8 ) & 0xFF ) ;
				frame.append( m_number & 0xFF ) ;
				break ;
	case 5 :
	case 6 :	frame.append( ( m_value >> 8 ) & 0xFF ) ;
				frame.append( m_value & 0xFF ) ;
				break ;
	case 15 :
	case 16 :	frame.append( ( m_number >> 8 ) & 0xFF ) ;
				frame.append( m_number & 0xFF ) ;
				frame.append( n ) ;
				frame.append( m_data ) ;
				break ;
	}

	// sending...

	if ( m_isConnected ) {
		if ( m_isProtoTcp )	m_tcpClient->sockWrite( frame ) ;
		else				m_udpClient->sockWrite( frame ) ;
	}

	showPrompt("send: ") ;
	showFrame(toHexString( frame ) + "\n", true, Qt::darkGreen ) ;

	// automatic Transaction Unit number ?

	if ( m_mbapTiIsAuto ) {
		quint16 tu = edtMbapTi->text().toUShort(&ok, 10 ) ;
		tu += 1 ;
		edtMbapTi->setText( QString().setNum( tu ) ) ;
	}
}

// hex string (format HH HH HH...) to byte array...

QByteArray Dialog::toHexArray(const QString& s , bool* ok )
{
	QByteArray	res ;

	QString tmp = s.simplified() ;
	tmp.remove(' ') ;
	if ( tmp.size() % 2 )	tmp = "0" + tmp ;
	bool b ;
	for (int i = 0 ; i < tmp.size() / 2 ; ++i )	res.append(tmp.mid( i * 2, 2 ).toUShort(&b, 16 ) ) ;
	if ( ok != 0 ) *ok = b ;
	return res ;
}

// frame as hex string (format HH HH HH HH ...)

QString	Dialog::toHexString(const QByteArray& frame, char separator )
{
	QString s ;
	for ( int i = 0 ; i < frame.count() ; ++i ) {
		s += QString("%1").arg( quint8( frame.at(i) ), 2, 16, QLatin1Char('0') ).toUpper() ;
		s += separator ;
		if ( i == 6 ) s += separator ;	// séparation MBAP/PDU
	}
	return s ;
}

// log messages

void Dialog::showPrompt(const QString& prompt )
{
	edtDialog->moveCursor(QTextCursor::End ) ;
	edtDialog->setTextColor( Qt::darkGray ) ;
	edtDialog->setFontWeight( QFont::Normal ) ;
	edtDialog->insertPlainText( prompt ) ;
	edtDialog->moveCursor(QTextCursor::End ) ;
}

void Dialog::showMessage(const QString& msg, bool bold, QColor color )
{
	edtDialog->moveCursor(QTextCursor::End ) ;
	edtDialog->setTextColor( color ) ;
	edtDialog->setFontWeight( bold ? QFont::Bold : QFont::Normal ) ;
	edtDialog->insertPlainText( msg ) ;
	edtDialog->moveCursor(QTextCursor::End ) ;
}

void Dialog::showFrame(const QString& msg, bool pduBold, QColor color )
{
	edtDialog->moveCursor(QTextCursor::End ) ;
	edtDialog->setTextColor( color ) ;
	edtDialog->setFontWeight( QFont::Normal ) ;
	edtDialog->insertPlainText( msg.left(21) ) ;
	edtDialog->setFontWeight( pduBold ? QFont::Bold : QFont::Normal ) ;
	edtDialog->insertPlainText( msg.right( msg.size() - 21 ) ) ;
	edtDialog->moveCursor(QTextCursor::End ) ;
}

void Dialog::showWarning(const QString& msg )
{
	showPrompt("warn: ") ;
	showMessage(msg, false, Qt::darkRed ) ;
}

void Dialog::on_pbuAbout_clicked()
{
	QMessageBox::information(this,
	   tr("About..."),
	   QString(MODIPASKER_ABOUTMESSAGE_HTML) ) ;
}
