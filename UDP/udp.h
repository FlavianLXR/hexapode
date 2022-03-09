#ifndef UDP_H
#define UDP_H

#include <QWidget>
#include <QUdpSocket>

QT_BEGIN_NAMESPACE
namespace Ui { class UDP; }
QT_END_NAMESPACE

class UDP : public QWidget
{
    Q_OBJECT

public:
    UDP(QWidget *parent = nullptr);
    ~UDP();

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




