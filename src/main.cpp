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
#define GPIO_PIN_LED_LOCK_ROUGE         12 //GPIO12
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
#include "MyOled.h"
MyOled *myOled = new MyOled(&Wire, OLED_RESET, SCREEN_HEIGHT, SCREEN_WIDTH);

#include "MyOledViewInitialisation.h"
MyOledViewInitialisation *myOledViewInit = NULL;

char buffer[100];
bool demarrer = false;
int nbSecondes = 20;

const string nomSysteme = "SAC System";
const string idSysteme = "Id0000";
const string sensibiliteBoutonStart = "47";
const string sensibiliteBoutonReset  = "104";


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

    float temp = myTemp->getTemperature();
    sprintf(buffer, "%4.1f °C", temp);
    if (string(actionToDo.c_str()).compare(string("askTempFour")) == 0) {
      return(buffer); }

    if (string(actionToDo.c_str()).compare(string("startAction")) == 0) {
        Serial.println("Demarrage du four!");
        demarrer = true;
        return(""); }
   
    std::string result = "";
    return result;
    }

void setup() { 
    Serial.begin(9600);
    delay(100);

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

    //Initialisation des LED statuts
    pinMode(GPIO_PIN_LED_LOCK_ROUGE  ,OUTPUT);
    pinMode(GPIO_PIN_LED_OK_GREEN,      OUTPUT);
    pinMode(GPIO_PIN_LED_HEAT_YELLOW,OUTPUT);

    //Initialisation des boutons
    myButtonT8->init(T8);
    int SensibiliteT8 = myButtonT8->autoSensibilisation(); //Trouve la sensibilité automatiquement
    sprintf(buffer, "%i", SensibiliteT8);
    sensibiliteBoutonStart << buffer;

    myButtonT9->init(T9);
    myButtonT9->autoSensibilisation(); //Trouve la sensibilité automatiquement

    //Initialisation sensor température
    myTemp = new TemperatureStub();
    myTemp->init(15, DHT22); 

    // ----------- Routes du serveur ----------------
    myServer = new MyServer(80);
    myServer->initAllRoutes();
    myServer->initCallback(&CallBackMessageListener);

    // Initialisation et Affichage Oled Initialisation
    myOled->init(OLED_I2C_ADDRESS);
    myOled->veilleDelay(30); //En secondes
    myOledViewInit = new MyOledViewInitialisation();
    myOledViewInit->setNomDuSysteme(nomSysteme.c_str());
    myOledViewInit->setIdDuSysteme(idSysteme.c_str());
    myOledViewInit->setSensibiliteBoutonAction(sensibiliteBoutonStart.c_str());
    myOledViewInit->setSensibiliteBoutonReset(sensibiliteBoutonReset.c_str());
    myOled->displayView(myOledViewInit);

 }

void loop() {
    float temp = myTemp->getTemperature();
    sprintf(buffer, "%4.1f °C", temp);
    //Serial.println(buffer);

    if(demarrer){
        sprintf(buffer, "%i secondes.", nbSecondes);
        Serial.println(buffer);
        nbSecondes--;
        if(nbSecondes == 0){
            Serial.println("Cuisson terminé!");
            demarrer = false;
            nbSecondes = 20;
        }
    }
    delay(1000);
  }