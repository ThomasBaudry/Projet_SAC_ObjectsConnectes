/**
    Class MyOledViewWorkingOFF : Gestion d'une VUE de fonctionnement à l'arret pour le OLed
    @file MyOledViewWorkingOFF.h 
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
            #include "MyOledViewWorkingOFF.h"
        ...
        //Utilisation
        MyOledViewWorkingOFF = new MyOledViewWorkingOFF();
        myOled->displayView(MyOledViewWorkingOFF);
**/
#ifndef MYOLEDVIEWWORKINGOFF_H
#define MYOLEDVIEWWORKINGOFF_H

#include "MyOledViewWorking.h"

class MyOledViewWorkingOFF: public MyOledViewWorking {
    
    public:

    private:
        void display( Adafruit_SSD1306 *adafruit);
      
};
#endif