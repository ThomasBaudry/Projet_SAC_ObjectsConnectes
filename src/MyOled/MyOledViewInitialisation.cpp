/**
    Gestion d'une VUE pour le OLed : Initialisation du four
    @file MyOledViewInitialisation.cpp
    @author Thomas Baudry
    @version 1.1 04/11/22
*/
#include <Arduino.h>
#include "MyOledViewInitialisation.h"

using namespace std;

void MyOledViewInitialisation::setNomDuSysteme(std::string val){
    nomDuSysteme = val;
}
void MyOledViewInitialisation::setIdDuSysteme(std::string val){
    idDuSysteme = val;
}
void MyOledViewInitialisation::setSensibiliteBoutonAction(std::string val){
    SensibiliteBoutonAction = val;
}
void MyOledViewInitialisation::setSensibiliteBoutonReset(std::string val){
    SensibiliteBoutonReset = val;
}


void MyOledViewInitialisation::update(Adafruit_SSD1306 *adafruit){
    Serial.println("Update my view MyOledViewInitialisation");
    }

void MyOledViewInitialisation::display( Adafruit_SSD1306 *adafruit) {
    Serial.println("MyOledViewInitialisation");

    char strId[120];
    char strAction[120];
    char strReset[120];
    sprintf(strId, "Id:  %s", idDuSysteme.c_str());
    sprintf(strAction, "Bouton ACTION : %s", SensibiliteBoutonAction.c_str());
    sprintf(strReset, "Bouton RESET : %s", SensibiliteBoutonReset.c_str());

    adafruit->clearDisplay();

    adafruit->setCursor(0, 0);
    adafruit->setTextSize(2);
    adafruit->println(nomDuSysteme.c_str());
    adafruit->setTextSize(1);
    adafruit->println(strId);
    adafruit->println("INITIALISATION");
    adafruit->println(strAction);
    adafruit->println(strReset);
    adafruit->display();
    }