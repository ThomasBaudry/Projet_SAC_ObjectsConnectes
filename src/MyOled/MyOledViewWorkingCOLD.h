/**
    Class MyOledViewWorkingCOLD : Gestion d'une VUE de fonctionnement en pause pour le OLed
    @file MyOledViewWorkingCOLD.h 
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
            #include "MyOledViewWorkingCOLD.h"
        ...
        //Utilisation
        MyOledViewWorkingCOLD = new MyOledViewWorkingCOLD();
        myOled->displayView(MyOledViewWorkingCOLD);
**/
#ifndef MYOLEDVIEWWORKINGCOLD_H
#define MYOLEDVIEWWORKINGCOLD_H

#include "MyOledViewWorking.h"

class MyOledViewWorkingCOLD: public MyOledViewWorking {
    
    public:

    private:
        void display( Adafruit_SSD1306 *adafruit);
        void update(Adafruit_SSD1306 *adafruit);
      
};
#endif