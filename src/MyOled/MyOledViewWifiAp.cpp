/**
    Gestion d'une VUE pour le OLed : Gestion Wifi
    @file MyOledViewWifiAp.cpp
    @author Thomas Baudry
    @version 1.1 10/11/22
*/
#include <Arduino.h>
#include "MyOledViewWifiAp.h"

using namespace std;

void MyOledViewWifiAp::setNomDuSysteme(std::string val){
    nomDuSysteme = val;
}
void  MyOledViewWifiAp::setSsIDDuSysteme(std::string val){
    ssIDDuSysteme = val;
}
void  MyOledViewWifiAp::setPassDuSysteme(std::string val){
    passDuSysteme = val;
}

// Actualisation de la vue du Oled
void  MyOledViewWifiAp::update(Adafruit_SSD1306 *adafruit){
    Serial.println("Update my view MyOledViewWifiAp");
    }

// Affichage de la vue du Oled
void  MyOledViewWifiAp::display( Adafruit_SSD1306 *adafruit) {
    Serial.println("MyOledViewWifiAp");
    char strNom[120];
    char strSsID[120];
    char strPass[120];
    sprintf(strNom, "Id:  %s", nomDuSysteme.c_str());
    sprintf(strSsID, "  SSID : %s", ssIDDuSysteme.c_str());
    sprintf(strPass, "  PASS : %s", passDuSysteme.c_str());

    adafruit->clearDisplay();
    adafruit->setCursor(0, 0);
    adafruit->setTextSize(2);
    adafruit->println(nomDuSysteme.c_str());
    adafruit->setTextSize(1);
    adafruit->println(strNom);
    adafruit->println("AP Configuration");
    adafruit->println(strSsID);
    adafruit->println(strPass);
    adafruit->display();

    }