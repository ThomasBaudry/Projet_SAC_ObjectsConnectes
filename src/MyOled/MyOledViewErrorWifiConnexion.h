/**
    Class MyOledViewErrorWifiConnexion : Gestion d'une VUE de gestion d'erreur wifi pour le OLed
    @file MyOledViewErrorWifiConnexion.h 
    @author Thomas Baudry
    @version 1.1 10/11/2022
    
    Historique des versions   
                            Versions  Date      Auteur      Description
                            1.1      10/11/22   Moi         Première version de la classe
                            
                            
    platform = espressif32
    board = esp32doit-devkit-v1
    framework = arduino
    lib_deps = 
        Aucune
    Autres librairies (à copier dans le répertoire lib)
        Aucune
    
    Exemple d'utilisation 
        //Définition 
            #include "MyOledViewErrorWifiConnexion.h"
        ...
        //Utilisation
        MyOledViewErrorWifiConnexion = new MyOledViewErrorWifiConnexion();
        myOled->displayView(MyOledViewErrorWifiConnexion);
**/
#ifndef MYOLEDVIEWERRORWIFICONNEXION_H
#define MYOLEDVIEWERRORWIFICONNEXION_H

#include "MyOledView.h"

class MyOledViewErrorWifiConnexion: public MyOledView {
    
    public:
        void setNomDuSysteme(std::string val);

    private:
        void display( Adafruit_SSD1306 *adafruit);
        void update(Adafruit_SSD1306 *adafruit);
        
        std::string nomDuSysteme = "???";
      
};
#endif