/*  ---------------------------------------------------------------------------
 *  filename    :   dialog.h
 *  description :   INTERFACE de la classe d'usage UI
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

#ifndef DIALOG_H
#define DIALOG_H

#include "ui_dialog.h"
#include "_ABOUT"

#include <QamTcpSocket>
#include <QamUdpSocket>

#include <QHostInfo>
#include <QCloseEvent>
#include <QMessageBox>
#include <QDir>

class Dialog : public QDialog, private Ui::Dialog
{
	Q_OBJECT

  public:
	explicit Dialog(QWidget *parent = 0 ) ;
	~Dialog() ;

  protected :
	void closeEvent(QCloseEvent* event ) ;

  public slots:
	void sockReceive(const QByteArray& frame ) ;
	void sockInfo(const QString& msg ) ;

  private slots:
	void on_rbuTcp_toggled(bool checked ) ;
	void on_pbuConnect_clicked() ;
	void on_chkMbapTiAuto_toggled(bool checked ) ;
	void on_chkMbapLenAuto_toggled(bool checked ) ;
	void on_cbxPduFunct_currentIndexChanged(int index ) ;
	void on_rbuPduAddrDec_toggled(bool checked ) ;
	void on_rbuPduNumberDec_toggled(bool checked ) ;
	void on_rbuPduValueDec_toggled(bool checked ) ;
	void on_pbuSend_clicked() ;
	void on_pbuAbout_clicked() ;

  private:
	void enableConnectionFields(bool enabled = true ) ;
	void enableNumberFields(bool enabled = true ) ;
	void enableValueFields(bool enabled = true ) ;
	void enableDataFields(bool enabled = true ) ;

	QByteArray toHexArray(const QString& s, bool* ok = 0 ) ;
	QString	toHexString(const QByteArray& frame, char separator = ' ' ) ;

	void showPrompt(const QString& prompt ) ;
	void showMessage(const QString& msg, bool bold = true, QColor color = Qt::black ) ;
	void showFrame(const QString& msg, bool pduBold = true, QColor color = Qt::black ) ;
	void showWarning(const QString& msg ) ;

  private:
	bool	m_isProtoTcp ;
	bool	m_isConnected ;

	QamTcpClient*	m_tcpClient ;
	QamUdpSocket*	m_udpClient ;

	bool	m_mbapTiIsAuto ;
	bool	m_mbapLenIsAuto ;

	quint16		m_mbapTi ;
	quint16		m_mbapPi ;
	quint16		m_mbapLen ;
	quint8		m_mbapUi ;

	quint8		m_funct ;
	quint16		m_addr ;
	quint16		m_number ;
	quint16		m_value ;
	QByteArray	m_data ;
} ;

#endif // DIALOG_H
