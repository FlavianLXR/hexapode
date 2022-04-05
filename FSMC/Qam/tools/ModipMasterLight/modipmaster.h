#ifndef MODIPMASTER_H
#define MODIPMASTER_H

#include <QObject>
#include <QSocketNotifier>
#include <qammodbusmap.h>
#include <qamtcpclient.h>

class ModipMaster : public QObject
{
	Q_OBJECT

  public:
	explicit ModipMaster(const QString& configFile, QObject* parent = 0 ) ;

  signals:
	void quit() ;	// signal à émettre pour terminer l'application...

  public slots:
	void readConsole() ;
	void info(const QString& src, const QString& msg ) ;
//	void valueChanged(int table, const QString& name ) ;

  private:
	QamModbusMap*				m_map ;
	QamModbusMap::PrimaryTable	m_table ;

	QamTcpClient*		m_client ;
	QSocketNotifier*	m_notifier ;
} ;

#endif // MODIPMASTER_H
