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

void  MyOledViewErrorWifiConnexion::update(Adafruit_SSD1306 *adafruit){
    Serial.println("Update my view MyOledViewErrorWifiConnexion");
    }

void  MyOledViewErrorWifiConnexion::display( Adafruit_SSD1306 *adafruit) {
    Serial.println("MyOledViewErrorWifiConnexion");

    // 
    // Display à faire !!!
    //

    }