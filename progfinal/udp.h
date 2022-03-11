#ifndef UDP_H
#define UDP_H

#include <QWidget>
#include <QUdpSocket>
#include "modipslave.h"
QT_BEGIN_NAMESPACE
namespace Ui { class UDP; }
QT_END_NAMESPACE

class UDP : public QWidget
{
    Q_OBJECT

public:
    explicit UDP(QWidget *parent = nullptr);
    ~UDP();
    ModipSlave* slave;

private slots:
    void traitementDatagram();
    void envoieDatagram();

signals:

private:
    Ui::UDP *ui;
     QUdpSocket *socket = nullptr;
     void change(float);
};
#endif // UDP_H




