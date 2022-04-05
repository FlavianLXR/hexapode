/*  ---------------------------------------------------------------------------
 *  filename    :   qammodbusmap.h
 *  description :   INTERFACE de la classe QamModbusMap
 *					Modélisation des tables primaires Modbus
 *
 *	project     :	Qam Modbus over TCP/IP
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

#ifndef QAMMODBUSMAP_H
#define QAMMODBUSMAP_H

/*!
  @file
  @brief Modélisation de la cartographie d'un équipement Modbus
 */

/*!
 @class QamModbusMap
 @brief Modélisation de la cartographie d'un équipement Modbus.

La classe QamModbusMap est conçue de manière à permettre à deux applications Qt
jouant respectivement les rôles d'esclave et de maître sur un réseau Modbus TCP/IP
de partager un seul et unique fichier de description de cartographie. Elle permet
aussi d'établir une communication entre une application cliente et un serveur de type
automate programmable. La prise en charge des couches réseau TCP/IP est assurée par
les classes QamTcpServer et QamTcpClient.
@n QamModbusMap modélise la vue métier d'un serveur Modbus TCP/IP. Les
quatre tables standards sont représentées : Coils (R/W, number 00001..09999),
Discrete Inputs (R, number 10001..19999), Input Registers (R, number 30001..39999)
et Holding Registers (R/W, number 40001..49999). En pratique, QamModbusMap accepte
pour la table Holding Registers des offsets allant jusqu'à FFFF (number 40001..105536).
@n Chaque table peut contenir des données primaires 1 bit pour les deux premières
et 16 bits pour les deux autres. Les tables 16 bits peuvent aussi contenir des
données composées de plusieurs registres adjacents.
@n Chaque donnée est caractérisée par un nom, un commentaire sous forme de texte
libre associé, un format d'affichage et une valeur exprimée en conformité avec le
format d'affichage. Se reporter à la classe QamModbusData pour voir les différents
formats autorisés.
@n La configuration du serveur ou du client Modbus est assurée par chargement d'un
fichier de description (format CSV avec séparateur point-virgule).
Le fichier doit respecter les contraintes suivantes :
- lignes de commentaire introduites par un symbole # en première colonne ;
- lignes vides acceptées ;
- adresse IP du serveur (étiquette HOST) : nécessaire en mode Client. En cas d'absence
de cette information, le serveur est considéré local à la machine hôte du client ;
- port de service (étiquette PORT) : nécessaire en modes Client et Serveur. En cas
d'absence de cette information, le service est supposé sur le port 502 ;
- description du serveur sous forme de texte libre (étiquette INFO) ;
- entrées de table spécifiées par @b exactement 7 champs :
	- champ 1 : numéro dans la table (décimal) ;
	- champ 2 : taille en nombre de mots (décimal) en cohérence avec le format
				d'affichage, peut être laissé à 0 pour les données de taille 1 bit ;
	- champ 3 : masque (hexadécimal), FFFF pour les données primaires,
				peut être laissé à 0 pour les données de taille 1 bit ;
	- champ 4 : nom (texte libre) ;
	- champ 5 : commentaire (texte libre) ;
	- champ 6 : format d'affichage, à choisir parmi Hex, Bool, Uint, Int, Ascii, Bcd
				(champ 2 = 1) ; Float, Long (champ 2 = 2) ; Str8 (champ 2 = 4) ou
				Str16 (champ 2 = 8) ;
	- champ 7 : valeur, exprimée conformément au format d'affichage.

@n Exemple de fichier CSV :
@verbatim
# configuration file for QamModbusMap

# 2 fields for server address, port and short description

HOST;192.168.2.3
PORT;502
INFO;my Modbus server short explanation...

# 7 fields for entries: num; size; mask; name; comment; display; value

# 0xxxx - coils
00001;0;0;		Moteur_X;		arrêt/marche moteur axe X;				Bool;	0
00002;0;0;		Sens_X;			sens moteur axe X (X+ si 1);			Bool;	0

# 1xxxx - discrete inputs
10101;0;0;		Fdc_X-;			capteur FDC axe X-;						Bool;	0
10102;0;0;		Fdc_X+;			capteur FDC axe X+;						Bool;	0
10103;0;0;		Pom_X;			capteur POM axe X;						Bool;	0

# 3xxxx - input registers
30001;1;FFFF;	Cons_X;			position axe X (mm);					Uint;	1664
30011;1;FFFF;	Etat_Arm;		état armoire de puissance;				Hex;	0200
30011;1;0FFF;	Temp;			température en 1/10 de degrés Celsius;	Bcd;	200
30011;1;1000;	Alim;			présence tension;						Bool;	0

# 4xxxx - holding registers
40001;1;FFFF;	Cons_X;			consigne de position axe X (mm);		Uint;	1664
40108;1;FFFF;	C_Loc;			poste Contrôle (local);					Hex;	01FE
40108;1;0001;	C_Mon;			demande de traitement (montage);		Bool;	0
40108;1;01FE;	C_Tab_Num;		numéro de pièce 0..19;					Uint;	255
40108;1;0600;	C_Tab_Ori;		orientation pièce 0..3;					Uint;	0
40108;1;0800;	C_Def;			défaut reconnaissance pièce;			Bool;	0
40203;1;FFFF;	MyAscii;		;										Ascii;	AM
40204;1;FFFF;	MyBcd;			;										Bcd;	1492
40206;2;FFFF;	MyFloat;		;										Float;	-3.14159
40208;2;FFFF;	MyLong;			;										Long;	-987654321
40701;4;FFFF;	Hello;			message de bienvenue;					Str8;	Bonjour
@endverbatim
@n Les entrées 00001, 00002, 10101, 10102 et 10103 sont des données de taille 1 bit, leur
format est obligatoirement Bool.
@n Les données primaires 30011 et 40108 sont décomposées en données secondaires représentant
chacune un certain nombre de bits de leur donnée primaire.
@n Les données MyFloat et MyLong occupent respectivement les emplacements 40206-40207 et
40208-40209 (formats 32 bits).
@n La donnée composée nommée "Hello" occupe en réalité les emplacements 40701, 40702, 40703 et
40704 de la table Holding Registers.
 */

//#include "qamabstractserver.h"
#include "../QamSockets/qamabstractserver.h"

#include "qammodbusdata.h"
#include <QMutex>
#include <QEventLoop>
#include <QStringList>
#include <QList>
#include <QFile>
#include <QFileInfo>
#include <QTextStream>

//#define MODBUSMAP_VERSION_MAJOR	1
//#define MODBUSMAP_VERSION_MINOR	4
#include "_ABOUT"

#define	MODBUSMAP_ENTRY_SIZE	7

class QamModbusMap : public QamAbstractServer
{
	Q_OBJECT

  public:
	/*! Mode de fonctionnement. */
	typedef enum {
		/*! Mode serveur TCP (mode par défaut) */
		ServerMode,
		/*! Mode client TCP */
		ClientMode
	} Mode ;

	/*! Tables primaires.*/
	typedef enum {
		/*! Read/Write, number 00001..09999 */
		Coil = 0,
		/*! Read only, number 10001..19999 */
		DiscretInput = 1,
		/*! Read only, number 30001..39999 */
		InputRegister = 3,
		/*! Read/Write, number 40001..49999 */
		HoldingRegister = 4
	} PrimaryTable ;

	explicit QamModbusMap(Mode mode = ServerMode, QObject* parent = 0 ) ;

	static QString version() ;
	static QString tableAsString(PrimaryTable table ) ;

	friend class QamModbusMapViewer ;	// accès aux méthodes data()

	/*! En mode 'verbose' (actif par défaut), le PDU des trames Modbus échangées
	 * est remonté par le signal info() sous forme d'un préfixe 'recv' ou 'send'
	 * suivi d'une suite d'octets en hexadécimal.
	 */
	inline void setVerbose(bool verbose = true ) { m_verbose = verbose ; }

  public:
	/*! Mode défini lors de la construction, serveur (défaut) ou client. */
	Mode mode() const { return m_mode ; }
	/*! Nom d'hôte (issu du fichier de configuration). */
	QString host() const { return m_host ; }
	/*! Port de service (issu du fichier de configuration). */
	quint16 port() const { return m_port ; }
	/*! Description du serveur (issue du fichier de configuration). */
	QString description() const { return m_desc ; }

  signals:
	// signal de synchro. requête client / réponse serveur
	void responseDone() ;
	/*! Signal de confirmation de changement de valeur d'une entrée de table. */
	void valueChanged(int table, const QString& name ) ;

	// dialogue réseau mode serveur
	// ---------------------------------------------------------------------------

  public:
//	virtual QByteArray responseToRequest(QByteArray& request ) ;
	virtual QByteArray responseToClientRequest(QByteArray& request ) ;
  private:
	bool isRangeAvailable(PrimaryTable table, quint16 addrBegin, quint16 addrEnd ) ;
	QByteArray	exceptionResponse(QByteArray& request, quint8 exceptionCode, const QString& message ) ;

	// dialogue réseau mode client
	// ---------------------------------------------------------------------------

  public slots:
//	virtual void response(QByteArray response ) ;
	virtual void responseFromServer(QByteArray response ) ;

  private:
	virtual void setServerAvailable(bool serverAvailable ) ;


	// accesseurs tables/données/formats
	// ---------------------------------------------------------------------------

  public:
	QStringList nameList(PrimaryTable table, bool withComposite = false ) ;
	QStringList itemList(const QString& name ) ;
	QStringList itemList(PrimaryTable table, const QString& name ) ;
	QString display(PrimaryTable table, const QString& name ) ;
	bool exists(PrimaryTable table, const QString& name ) ;
	bool exists(PrimaryTable table, quint16 address ) ;

  private:
	QamModbusData& data(PrimaryTable table, const QString& name ) ;
	QamModbusData& data(PrimaryTable table, quint16 address ) ;
	bool checkPrimaryName(PrimaryTable table, const QString& name ) ;
	bool checkFormattedValue(const QamModbusData& data, const QString& value ) ;

	// modificateurs de données Modbus
	// ---------------------------------------------------------------------------

  public slots:
	void setValue(PrimaryTable table, const QString& name, const QString& value ) ;
	void setLocalValue(PrimaryTable table, const QString& name, const QString& value ) ;
	void setRemoteValue(PrimaryTable table, const QString& name, const QString& value ) ;

  private:
	void buildAndSendWriteFrame() ;

	// sélecteurs de données Modbus
	// ---------------------------------------------------------------------------

  public:
	QString value(PrimaryTable table, const QString& name ) ;
	QString localValue(PrimaryTable table, const QString& name ) ;
	QString remoteValue(PrimaryTable table, const QString& name ) ;

  private:
	void buildAndSendReadFrame() ;

	// création de la cartographie locale
	// ---------------------------------------------------------------------------

  public:
	bool loadMap(const QString& filename ) ;
	bool addData(const QStringList& entry, int line = 0 ) ;

  private:
	bool addBitData(PrimaryTable table, quint16 address, const QString& name, const QString& comment, quint16 value = 0 ) ;
	bool addWordData(PrimaryTable table, quint16 address, quint16 mask, const QString& name, const QString& comment, const QString& display, quint16 value = 0 ) ;

	// remontée d'informations
	// ---------------------------------------------------------------------------

  signals:
	/*! Signal de remontée d'information. La @a source peut être "tcp/ip"
	 * dans le cas d'une information en provenance des couches réseau TCP/IP,
	 * "config" pour une erreur de remplissage des tables,
	 * ou "modbus" pour un message lié à l'analyse d'une trame Modbus
	 * (couche applicative).
	 */
	void info(const QString& source, const QString& message ) ;

  private:
	virtual void networkInfo(const QString& message ) ;
	void configInfo(const QString& message ) ;
	void modbusInfo(const QString& message ) ;
	void addDataInfo(const QString& message, int line = 0 ) ;


	// attributs
	// ---------------------------------------------------------------------------

  private:
	bool		m_verbose ;
	Mode		m_mode ;
	QString		m_host ;
	quint16		m_port ;
	QString		m_desc ;
	bool		m_isServerAvailable ;

	QamModbusData*	m_nullData ;

	QMutex			m_mutex ;

	QList<QamModbusData>*	m_coils ;
	QList<QamModbusData>*	m_discreteInputs ;
	QList<QamModbusData>*	m_inputRegisters ;
	QList<QamModbusData>*	m_holdingRegisters ;

	PrimaryTable	m_table ;
	QString			m_name ;
	int				m_item ;

	quint16		m_mbapTi ;
	quint16		m_mbapPi ;
	quint16		m_mbapLen ;
	quint8		m_mbapUi ;

	quint8		m_funct ;
	quint16		m_addr ;
	quint16		m_number ;
	quint16		m_value ;
	QByteArray	m_data ;
	quint8		m_exception ;
} ;

#endif // QAMMODBUSMAP_H
