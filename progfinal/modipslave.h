#ifndef MODIPSLAVE_H
#define MODIPSLAVE_H

#include <QObject>
#include <qammodbusmap.h>
#include <qamtcpserver.h>
#include <hexapodmgi.h>
class ModipSlave : public QObject
{
    Q_OBJECT

  public:

    explicit ModipSlave(const QString& configFile, QObject* parent = 0 ) ;
     void SetValue(float ,QString);
         QamMatrix6x1 MGI(float Rx, float Ry, float Rz);
QamModbusMap*	m_map ;
  signals:
    void quit() ;	// signal à émettre pour terminer l'application...

  public slots:
    void info(const QString& src, const QString& msg ) ;
    void valueChanged(int table, const QString& name ) ;

  private:

    QamTcpServer*	m_server ;
    HexapodMGI*     m_hexapodmgi;
} ;

#endif // MODIPSLAVE_H
