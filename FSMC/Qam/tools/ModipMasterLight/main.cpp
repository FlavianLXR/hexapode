#include <QCoreApplication>
#include <modipmaster.h>

#include <iostream>
using namespace std ;

int main(int argc, char *argv[] )
{
	QCoreApplication app(argc, argv ) ;

	QStringList	args = QCoreApplication::arguments() ;
	if ( args.size() < 2 ) {
		cerr << "usage: " << qPrintable( args.at(0) ) << " <configFile>" << endl ;
		return -1 ;
	}

	ModipMaster*	master = new ModipMaster( args.at(1),  &app ) ;

	QObject::connect(master, SIGNAL(quit()), &app, SLOT(quit()) ) ;

	return app.exec() ;
}
