## Projet C de session : "SAC" (Système Atmosphérique Connecté)
	# Matière : Objets Connectés
	# Professeur : Dubé Alain
	# Auteurs : Baudry Thomas - Guillou Quentin
	# Lien : https://github.com/Huo1ong/ObjetsConnectes_ProjetIODeSession
  	# API : http://149.56.141.62:3000/myApiDocs/

## Installation
Premièrement, vous devez créer un nouveau projet PlatformIO sous Visual Studio Code. 
	Nommez le comme vous le souhaiter, sélectionner le board : "DOIT ESP32 DEVKIT V1" correspondant à l'ESP32 que vous posséder.
En second, vous devez cloner le répertoire ou juste télécharger la version ZIP du projet présent sur le lien github. 
	Une fois ceci réalisé, il vouss suffit de remplacer les fichiers présent dans votre nouveau projet par ceux du dossier ZIP.
Pour terminer, il vous suffit de regarder l'entête de page du fichier main.cpp pour rajouter les librairies et dépendances à ajouter que vous ne posséder pas.


## Fonctionnalités
Ce projet a pour fonctionnalités de permettre le séchage de diverses types de bois. Une fois le programme lancé, un compte à rebours spécifique à chaque type de bois se lance.
Il faut savoir que si la température ambiante n'est pas égale à celle désirée, cela déclenchera une fonctionnalité du système :
	°si la température est trop froide, le programme de séchage s'arrêtera et recommencera à 0,
	°si elle est trop chaude, le compte à rebours du temps de séchage se mettra en pause jusqu'à ce que la température redescende à celle souhaitée.

## Utilité
Ce projet va aider l'entreprise de fabrication de meubles, Les Meubles Dubé inc. dans laquelle nous aidons.
