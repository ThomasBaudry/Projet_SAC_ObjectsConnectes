/**
    Gestion d'une VUE pour le OLed : Fonctionnement en cour
    @file MyOledViewWorkingHEAT.cpp
    @author Thomas Baudry
    @version 1.1 10/11/22
*/
#include <Arduino.h>
#include "MyOledViewWorkingHEAT.h"

using namespace std;

void  MyOledViewWorkingHEAT::update(Adafruit_SSD1306 *adafruit){
    Serial.println("Update my view MyOledViewWorkingHEAT");
    }

void  MyOledViewWorkingHEAT::display( Adafruit_SSD1306 *adafruit) {

    adafruit->clearDisplay();
    adafruit->setTextSize(2);
    adafruit->setCursor(0, 0);
    adafruit->print(getTag("nomDuSysteme").c_str());

    adafruit->setTextSize(1);
    adafruit->setCursor(0, 20);
    adafruit->print("Id: ");
    adafruit->print(getTag("idDuSysteme").c_str());

    adafruit->setCursor(80, 20);
    adafruit->print("Heating");

    adafruit->setTextSize(2);
    adafruit->setCursor(50, 30);
    adafruit->print(getTag("temperature").c_str());
    
    adafruit->setTextSize(1);
    adafruit->setCursor(40, 50);
    adafruit->print(getTag("ipDuSysteme").c_str());

    adafruit->display();

    }