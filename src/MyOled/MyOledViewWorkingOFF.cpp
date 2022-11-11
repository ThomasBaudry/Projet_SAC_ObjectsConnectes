/**
    Gestion d'une VUE pour le OLed : Fonctionnement à l'arret
    @file MyOledViewWorkingOFF.cpp
    @author Thomas Baudry
    @version 1.1 10/11/22
*/
#include <Arduino.h>
#include "MyOledViewWorkingOFF.h"

using namespace std;

void  MyOledViewWorkingOFF::display( Adafruit_SSD1306 *adafruit) {
    Serial.println("MyOledViewWorkingOFF");
    
    adafruit->clearDisplay();
    adafruit->setTextSize(2);
    adafruit->setCursor(0, 0);
    adafruit->print(getTag("nomDuSysteme").c_str());

    adafruit->setTextSize(1);
    adafruit->setCursor(0, 20);
    adafruit->print("Id: ");
    adafruit->print(getTag("idDuSysteme").c_str());

    adafruit->setCursor(80, 20);
    adafruit->print("Ready");
    adafruit->setCursor(80, 30);
    adafruit->print(getTag("temperature").c_str());
    
    adafruit->setCursor(40, 50);
    adafruit->print(getTag("ipDuSysteme").c_str());

    adafruit->display();
    }