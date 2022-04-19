#ifndef TRDATAGRAM_H
#define TRDATAGRAM_H

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
    void disconnected();

signals:

private:
    Ui::UDP *ui;
     QUdpSocket *socket = nullptr;
     QTimer *timer;
     void change(float);
};
#endif // TRDATAGRAM_H




