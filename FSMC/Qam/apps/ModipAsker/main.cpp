/*  ---------------------------------------------------------------------------
 *  filename    :   main.cpp
 *  description :   point d'entrée
 *
 *	project     :	ModipAsker
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

#include "dialog.h"
#include <QApplication>

int main(int argc, char *argv[] )
{
	QApplication a(argc, argv) ;

#ifndef Q_OS_MAC
	a.setWindowIcon( QIcon(":/_res/icons/qam47.xpm") ) ;
#endif

	Dialog w ;
	w.setWindowTitle( MODIPASKER_TITLE ) ;
	
	w.show() ;

	return a.exec() ;
}
