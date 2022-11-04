/**
    Class MyOledViewInitialisation : Gestion d'une VUE d'initialisation pour le OLed
    @file MyOledViewInitialisation.h 
    @author Thomas Baudry
    @version 1.1 03/11/2022
    
    Historique des versions   
                            Versions  Date      Auteur      Description
                            1.1      03/11/22   Moi         Première version de la classe
                            
                            
    platform = espressif32
    board = esp32doit-devkit-v1
    framework = arduino
    lib_deps = 
        Aucune
    Autres librairies (à copier dans le répertoire lib)
        Aucune
    
    Exemple d'utilisation 
        //Définition 
            #include "MyOledViewInitialisation.h"
        ...
        //Utilisation
        myOledViewInitialisation = new MyOledViewInitialisation();
        myOled->displayView(myOledViewInitialisation);
**/
