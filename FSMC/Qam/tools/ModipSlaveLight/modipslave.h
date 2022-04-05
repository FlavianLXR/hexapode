#ifndef MODIPSLAVE_H
#define MODIPSLAVE_H

#include <QObject>
#include <qammodbusmap.h>
#include <qamtcpserver.h>

class ModipSlave : public QObject
{
	Q_OBJECT

  public:
	explicit ModipSlave(const QString& configFile, QObject* parent = 0 ) ;

  signals:
	void quit() ;	// signal à émettre pour terminer l'application...

  public slots:
	void info(const QString& src, const QString& msg ) ;
	void valueChanged(int table, const QString& name ) ;

  private:
	QamModbusMap*	m_map ;
	QamTcpServer*	m_server ;
} ;

#endif // MODIPSLAVE_H
