# Hexapode
Projet BTS SNIR 2nd année.  
__Un simulateur doit posséder 3 aspects :__
* Donner  une  vue  de  l’extérieur  qui  est  le  résultat  de  la  
simulation  de  vol.  
* La  sonorisation  reproduit  exactement  les  bruits  perçus  dans  le  cockpit  (bruit  de  moteur,  
bruit de l’air...).  
* Faire subir au pilote les effets de l’accélération, le tangage, le roulis et le lacet.  

__Problématique :__  
*Mettre en place le 3ème aspect sur un simulateur d'hélicoptère, la sensation de mouvement, à l'aide d'un hexapode (6 vérins sous le siège).*

# Etudiants
Killian\
Flavian (Chef d'équipe)\
Aurélien \
Yohann 

# Tâches
*Killian*  
__FlightSimMotionControl - assurer la récupération des informations
de la cinématique de l’aéronef :__  
* étudier le protocole de dialogue UDP utilisé par X-Plane ;  
* rechercher les datarefs nécessaires ;  
* définir les périodes d’échantillonnage ;  
* proposer une IHM minimale pour l’application ;  
* intégration globale, tests et documentation technique.  

*Flavian*  
__FlightSimMotionControl - mettre un place un serveur Modbus/IP :__  
* étudier le protocole d’échange à implanter (Modbus/IP) ;  
* définir la cartographie Modbus requise ;  
* développer un programme serveur de test (CLI) ;  
* réaliser l’intégration du serveur (FlightSimMotionControl) ;  
* prendre en compte la géométrie de l’hexapode ;  
* implémenter le MGI ;  
* tests et documentation technique.  

*Aurélien*  
__Hexapode - compléter le programme Matlab/Simulink afin de
récupérer les informations instantanées de longueur des vérins
maintenues par le serveur Modbus :__  
* caractériser le produit existant ;  
* étudier les possibilités d’adjonction de code dans les
schémas-blocs Simulink ;  
* développer une solution viable (pour au moins un vérin) ;  
* intégration globale, tests et documentation technique.  

*Yohann*  
__FlightSimMockup : Compléter le modèle 3D de la maquette afin de
visualiser les éléments du poste de pilotage (à minima siège et
console d’instruments) et éventuellement le pilote :__  
* étudier le moteur de scène 3D ;  
* s‘approprier les ressources de la bibliothèque GLam ;  
* ajouter et animer un simple cube ;  
* modéliser les éléments à ajouter par combinaisons de
volumes simples ;  
* développer le code correspondant pour les éléments ;  
* intégration globale, tests et documentation technique.  
