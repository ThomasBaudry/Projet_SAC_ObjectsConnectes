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
    ssIDDuSyteme = val;
}
void  MyOledViewWifiAp::setPassDuSysteme(std::string val){
    passDuSysteme = val;
}


void  MyOledViewWifiAp::update(Adafruit_SSD1306 *adafruit){
    Serial.println("Update my view MyOledViewWifiAp");
    }

void  MyOledViewWifiAp::display( Adafruit_SSD1306 *adafruit) {
    Serial.println("MyOledViewWifiAp");

    // 
    // Display à faire !!!
    //

    }