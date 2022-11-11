/* 
 *
 * Projet Sac
 * Ecole du Web
 * Cours Objets connectés (c)2021
 *  
    @file     main.cpp
    @author   Thomas Baudry
    @version  1.0 30/09/2022 


Configuration du système

            GPIO12 : pin 12   Rouge 
            GPIO14 : pin 14  Vert                              
            GPIO27 : pin 27  Jaune                              


 * */
#include <iostream>
#include <string>
#include <Arduino.h>
#include <ArduinoJson.h>

#include "myFunctions.cpp" //fonctions utilitaires

using namespace std;

// Gestion Wifi
#include <HTTPClient.h>
#include <WiFiManager.h>
WiFiManager wm;
#define WEBSERVER_H

//Pour la gestion du serveur ESP32
#include "MyServer.h"
MyServer *myServer = NULL;

//Variable pour la connection Wifi
const char *SSID = "SAC_";
const char *PASSWORD = "sac_";
String ssIDRandom;


//Définition des trois leds de statut
#define GPIO_PIN_LED_LOCK_RED           12 //GPIO12
#define GPIO_PIN_LED_OK_GREEN             14 //GPIO14
#define GPIO_PIN_LED_HEAT_YELLOW        27 //GPIO27

//Definition des éléments de l'ecran OLED
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET 4 // Reset pin # (or -1 if sharing Arduino reset pin)
#define OLED_I2C_ADDRESS 0x3C // Adresse I2C de l'écran Oled

//Gestion Button
#include "MyButton.h"
MyButton *myButtonT8 = new MyButton();
MyButton *myButtonT9 = new MyButton();

//Gestion capteur température
#include <Adafruit_Sensor.h>
#include "TemperatureStub.h"
TemperatureStub *myTemp;

//Gestion Ecran Oled
#include "MyOled/MyOled.h"
#include "MyOled/MyOledView.h"
#include "MyOled/MyOledViewInitialisation.h"
#include "MyOled/MyOledViewWorking.h"
#include "MyOled/MyOledViewWifiAp.h"
#include "MyOled/MyOledViewErrorWifiConnexion.h"
#include "MyOled/MyOledViewWorkingOFF.h"
#include "MyOled/MyOledViewWorkingCOLD.h"
#include "MyOled/MyOledViewWorkingHEAT.h"

MyOled *myOled = new MyOled(&Wire, OLED_RESET, SCREEN_HEIGHT, SCREEN_WIDTH);
MyOledViewInitialisation *myOledViewInit = NULL;
MyOledViewWifiAp *myOledViewWifi = NULL;
MyOledViewWorkingOFF *myOledViewWorkingOFF = NULL;
MyOledViewWorkingCOLD *myOledViewWorkingCOLD = NULL;
MyOledViewWorkingHEAT *myOledViewWorkingHEAT = NULL;

// Variable pour l'affichage Oled
const string nomSysteme = "SAC System";
const string idSysteme = "Id0000";
char sensibiliteBoutonStart[20] = "???";
char sensibiliteBoutonReset[20] = "???";

int etatFour = 0; // Etat du four / Différents etat possible: OFF = 0, COLD = 1, HEAT = 2

// Variable Utilitaire
char buffer[100];
bool demarrer = false;
int nbSecondes = 20;
float temp = 0;

//fonction statique qui permet aux objets d'envoyer des messages (callBack) 
//  arg0 : Action 
// arg1 ... : Parametres
std::string CallBackMessageListener(string message) {
    while(replaceAll(message, std::string("  "), std::string(" ")));
    //Décortiquer le message
    string actionToDo = getValue(message, ' ', 0);
    string arg1 = getValue(message, ' ', 1);
    string arg2 = getValue(message, ' ', 2);
    string arg3 = getValue(message, ' ', 3);
    string arg4 = getValue(message, ' ', 4);
    string arg5 = getValue(message, ' ', 5);
    string arg6 = getValue(message, ' ', 6);
    string arg7 = getValue(message, ' ', 7);
    string arg8 = getValue(message, ' ', 8);
    string arg9 = getValue(message, ' ', 9);
    string arg10 = getValue(message, ' ', 10);


    std::string nomDuFour = "Four9394";
    if (string(actionToDo.c_str()).compare(string("askNomFour")) == 0) {
     return(nomDuFour.c_str()); }

    
    if (string(actionToDo.c_str()).compare(string("askTempFour")) == 0) {
      temp = myTemp->getTemperature();
      sprintf(buffer, "%4.1f °C", temp);
      return(buffer); }

    if (string(actionToDo.c_str()).compare(string("startAction")) == 0) {
        Serial.println("Demarrage du four!");
        demarrer = true;
        return(""); }
   
    std::string result = "";
    return result;
}

void RefreshMyOledParams(){
switch(etatFour){
    case 0: // Etat OFF
        myOledViewWorkingOFF->setParams("nomDuSysteme", nomSysteme.c_str());
        myOledViewWorkingOFF->setParams("idDuSysteme", idSysteme.c_str());
        myOledViewWorkingOFF->setParams("temperature", buffer);
        myOledViewWorkingOFF->setParams("ipDuSysteme", WiFi.localIP().toString().c_str());
        break;

    case 1: // Etat COLD
        myOledViewWorkingCOLD->setParams("nomDuSysteme", nomSysteme.c_str());
        myOledViewWorkingCOLD->setParams("idDuSysteme", idSysteme.c_str());
        myOledViewWorkingCOLD->setParams("temperature", buffer);
        myOledViewWorkingCOLD->setParams("ipDuSysteme", WiFi.localIP().toString().c_str());
        break;
            
    case 2: // Etat HEAT
        myOledViewWorkingHEAT->setParams("nomDuSysteme", nomSysteme.c_str());
        myOledViewWorkingHEAT->setParams("idDuSysteme", idSysteme.c_str());
        myOledViewWorkingHEAT->setParams("temperature", buffer);
        myOledViewWorkingHEAT->setParams("ipDuSysteme", WiFi.localIP().toString().c_str());
        break;
}
}

void setup() { 
    Serial.begin(9600);
    delay(100);

    // Initialisation OLED et Affichage Oled Initialisation
    myOled->init(OLED_I2C_ADDRESS);
    myOled->veilleDelay(30); //En secondes
    myOledViewInit = new MyOledViewInitialisation();
    myOledViewInit->setNomDuSysteme(nomSysteme.c_str());
    myOledViewInit->setIdDuSysteme(idSysteme.c_str());
    myOledViewInit->setSensibiliteBoutonAction(sensibiliteBoutonStart);
    myOledViewInit->setSensibiliteBoutonReset(sensibiliteBoutonReset);
    myOled->displayView(myOledViewInit);

    //Initialisation des LED statuts
    pinMode(GPIO_PIN_LED_LOCK_RED  ,OUTPUT);
    pinMode(GPIO_PIN_LED_OK_GREEN,      OUTPUT);
    pinMode(GPIO_PIN_LED_HEAT_YELLOW,OUTPUT);

    //Initialisation des boutons
    myButtonT8->init(T8);
    int buttonSensiT8 = myButtonT8->autoSensibilisation(); //Trouve la sensibilité automatiquement
    sprintf(sensibiliteBoutonStart, "%i", buttonSensiT8);
    
    myButtonT9->init(T9);
    int buttonSensiT9 = myButtonT9->autoSensibilisation(); //Trouve la sensibilité automatiquement
    sprintf(sensibiliteBoutonReset, "%i", buttonSensiT9);

    myOledViewInit->setSensibiliteBoutonAction(sensibiliteBoutonStart);
    myOledViewInit->setSensibiliteBoutonReset(sensibiliteBoutonReset);
    myOled->displayView(myOledViewInit);

    //Initialisation sensor température
    myTemp = new TemperatureStub();
    myTemp->init(15, DHT22); 

 //Connection au WifiManager
    String ssIDRandom, PASSRandom;
    String stringRandom;
    stringRandom = get_random_string(4).c_str();
    ssIDRandom = SSID;
    ssIDRandom = ssIDRandom + stringRandom;
    stringRandom = get_random_string(4).c_str();
    PASSRandom = PASSWORD;
    PASSRandom = PASSRandom + stringRandom;

char strToPrint[128];
    sprintf(strToPrint, "Identification : %s   MotDePasse: %s", ssIDRandom, PASSRandom);
    Serial.println(strToPrint);


 if (!wm.autoConnect(ssIDRandom.c_str(), PASSRandom.c_str())){
        Serial.println("Erreur de connexion.");
      
        }
    else {
        Serial.println("Connexion Établie.");
        }

    // Affichage OLED Connection WIFI
    myOledViewWifi = new MyOledViewWifiAp();
    myOledViewWifi->setNomDuSysteme(nomSysteme.c_str());
    myOledViewWifi->setSsIDDuSysteme(ssIDRandom.c_str());
    myOledViewWifi->setPassDuSysteme(PASSRandom.c_str());
    myOled->displayView(myOledViewWifi);

    // ----------- Routes du serveur ----------------
    myServer = new MyServer(80);
    myServer->initAllRoutes();
    myServer->initCallback(&CallBackMessageListener);

    for (int i=0;i<2;i++) 
    {
        digitalWrite(GPIO_PIN_LED_OK_GREEN,HIGH);
        digitalWrite(GPIO_PIN_LED_HEAT_YELLOW,HIGH);
        digitalWrite(GPIO_PIN_LED_LOCK_RED,HIGH);
        delay(500);
        digitalWrite(GPIO_PIN_LED_OK_GREEN,LOW);
        digitalWrite(GPIO_PIN_LED_HEAT_YELLOW,LOW);
        digitalWrite(GPIO_PIN_LED_LOCK_RED,LOW);
        delay(500); 
    }
 }

void loop() {
    temp = myTemp->getTemperature();
    sprintf(buffer, "%4.1f °C", temp);
    RefreshMyOledParams();

    if(demarrer){
        if(etatFour == 0){
            etatFour = 1;
        }
        if(etatFour == 1){
            myOled->displayView(myOledViewWorkingCOLD);
        }
        sprintf(buffer, "%i secondes.", nbSecondes);
        Serial.println(buffer);
        nbSecondes--;
        if(nbSecondes == 0){
            Serial.println("Cuisson terminé!");
            demarrer = false;
            nbSecondes = 20;
            etatFour = 2;
        }
    }else{
        if(etatFour == 0){
            myOled->displayView(myOledViewWorkingOFF);
        }
        if(etatFour == 2){
            myOled->displayView(myOledViewWorkingHEAT);
        }
    }
    delay(1000);
  }