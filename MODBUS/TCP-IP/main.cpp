#include <QCoreApplication>
#include "modipslave.h"

#include <iostream>
using namespace std ;

int main(int argc, char *argv[] )
{
    QCoreApplication app(argc, argv ) ;

    QStringList	args = QCoreApplication::arguments() ;
//	if ( args.size() < 2 ) {
//		cerr << "usage: " << qPrintable( args.at(0) ) << " <configFile>" << endl ;
//		return -1 ;
//	}

    //ModipSlave*	slave = new ModipSlave( args.at(1),  &app ) ;
    ModipSlave*	slave = new ModipSlave( ":/fsmockup.csv",  &app ) ;

    QObject::connect(slave, SIGNAL(quit()), &app, SLOT(quit()) ) ;

    return app.exec() ;
}
