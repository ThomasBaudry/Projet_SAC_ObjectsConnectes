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
#ifndef MYOLEDVIEWINITIALISATION_H
#define MYOLEDVIEWINITIALISATION_H

#include "MyOledView.h"

class MyOledViewInitialisation: public MyOledView {
    
    public:
        void setNomDuSysteme(std::string val);
        void setIdDuSysteme(std::string val);
        void setSensibiliteBoutonAction(std::string val);
        void setSensibiliteBoutonReset(std::string val);

    private:
        void display( Adafruit_SSD1306 *adafruit);
        void update(Adafruit_SSD1306 *adafruit);
        
        std::string nomDuSysteme = "???";
        std::string idDuSyteme = "???";
        std::string SensibiliteBoutonAction = "???";
        std::string SensibiliteBoutonReset = "???";
      
};
#endif