/*  ---------------------------------------------------------------------------
 *  filename    :   dialog.h
 *  description :   INTERFACE de la classe d'usage UI
 *
 *	project     :	ModipButler
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

#include <QCloseEvent>

#include <QamTcpSocket>
#include <QamModbusMap>

class Dialog : public QDialog, private Ui::Dialog
{
	Q_OBJECT

  public:
	explicit Dialog(QWidget* parent = 0 ) ;

  protected:
	void closeEvent(QCloseEvent* event ) ;

  public slots:
	void on_pbuLoad_clicked() ;
	void on_pbuStart_clicked() ;
	void on_pbuAbout_clicked() ;
	void info(const QString& source, const QString& message ) ;
	void valueChanged(int table,const QString& name ) ;

  private:
	void readSettings() ;
	void writeSettings() ;
	void load() ;
	void showPrompt(const QString& prompt ) ;
	void showMessage(const QString& msg, bool bold = false, QColor color = Qt::black ) ;

  private:
	QString				m_configDir ;
	bool				m_isRunning ;
	QamModbusMap*		m_modbusMap ;
	QamTcpServer*		m_tcpServer ;
} ;

#endif // DIALOG_H
