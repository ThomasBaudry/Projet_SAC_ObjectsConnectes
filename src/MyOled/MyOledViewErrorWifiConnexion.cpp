/**
    Gestion d'une VUE pour le OLed : Gestion Erreur Connexion Wifi
    @file MyOledViewErrorWifiConnexion.cpp
    @author Thomas Baudry
    @version 1.1 10/11/22
*/
#include <Arduino.h>
#include "MyOledViewErrorWifiConnexion.h"

using namespace std;

void MyOledViewErrorWifiConnexion::setNomDuSysteme(std::string val){
    nomDuSysteme = val;
}

// Actualisation de la vue du Oled
void  MyOledViewErrorWifiConnexion::update(Adafruit_SSD1306 *adafruit){
    Serial.println("Update my view MyOledViewErrorWifiConnexion");
    }

// Affichage de la vue du Oled
void  MyOledViewErrorWifiConnexion::display( Adafruit_SSD1306 *adafruit) {
    Serial.println("MyOledViewErrorWifiConnexion");

    adafruit->setCursor(0, 0);
    adafruit->setTextSize(2);
    adafruit->println(nomDuSysteme.c_str());
    adafruit->setTextSize(1);
    adafruit->println("Erreur Connexion Wifi");

    }