#include <QApplication>
#include <QtGui>
#include <QWidget>
#include "modipslave.h"
#include "udp.h"
#include <iostream>
using namespace std ;
///*
int main(int argc, char *argv[] )
{
    QApplication app(argc, argv ) ;

    UDP w;
    w.show();
    //if ( args.size() < 2 ) {
    //    cerr << "usage: " << qPrintable( args.at(0) ) << " <configFile>" << endl ;
    //    return -1 ;
    //}
    ModipSlave* slave = new ModipSlave( ":/fsmockup.csv",  &app ) ;
    w.slave = slave;

    QObject::connect(slave, SIGNAL(quit()), &app, SLOT(quit()) ) ;
    QObject::connect(w.slave, SIGNAL(quit()), &app, SLOT(quit()) ) ;
    return app.exec() ;
}





//*/

/*
int main(int argc, char *argv[] )
{
    QApplication app(argc, argv ) ;
    app.setWindowIcon(QIcon("hexa.png"));
    UDP w;
    w.show();
    //QStringList	args = QApplication::arguments() ;
//	if ( args.size() < 2 ) {
//		cerr << "usage: " << qPrintable( args.at(0) ) << " <configFile>" << endl ;
//		return -1 ;
//	}

    //ModipSlave*	slave = new ModipSlave( args.at(1),  &app ) ;

    w.slave = new ModipSlave( "fsmockup.csv",  &app ) ;

    QObject::connect(w.slave, SIGNAL(quit()), &app, SLOT(quit()) ) ;

    return app.exec() ;
}
*/

/*
int main(int argc, char *argv[] )
{
    QApplication app(argc, argv ) ;
    QStringList args = QCoreApplication::arguments() ;

    //if ( args.size() < 2 ) {
    //    cerr << "usage: " << qPrintable( args.at(0) ) << " <configFile>" << endl ;
    //    return -1 ;
    //}
    ModipSlave* slave = new ModipSlave( ":/fsmockup.csv",  &app ) ;

    QObject::connect(slave, SIGNAL(quit()), &app, SLOT(quit()) ) ;

    return app.exec() ;
}
*/
