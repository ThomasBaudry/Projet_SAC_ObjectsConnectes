/**
    Class MyOledViewWifiAp : Gestion d'une VUE de gestion du wifi pour le OLed
    @file MyOledViewWifiAp.h 
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
            #include "MyOledViewWifiAp.h"
        ...
        //Utilisation
        myOledViewWifiAp = new MyOledViewWifiAp();
        myOled->displayView(myOledViewWifiAp);
**/
#ifndef MYOLEDVIEWWIFIAP_H
#define MYOLEDVIEWWIFIAP_H

#include "MyOledView.h"

class MyOledViewWifiAp: public MyOledView {
    
    public:
        void setNomDuSysteme(std::string val);
        void setSsIDDuSysteme(std::string val);
        void setPassDuSysteme(std::string val);

    private:
        void display( Adafruit_SSD1306 *adafruit);
        void update(Adafruit_SSD1306 *adafruit);
        
        std::string nomDuSysteme = "???";
        std::string ssIDDuSyteme = "???";
        std::string passDuSysteme = "???";
      
};
#endif