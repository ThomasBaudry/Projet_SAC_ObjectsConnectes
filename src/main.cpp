/* Copyright (C) 2022
 * All rights reserved.
 *
 * Projet SAC
 * Ecole du Web
 * Cours Objets connectés (c)2022
 *  
    @file     main.cpp
    @author   BAUDRY Thomas
    @version  1.2 08/12/22 

    Historique des versions
           Version    Date       Auteur       Description
           1.1        21/08/15  Alain       Première version du logiciel
           1.2        03/11/22  Thomas      Seconde version du logiciel

    platform = espressif32
    board = esp32doit-devkit-v1
    framework = arduino
    lib_deps =  
            ESPAsyncWebServer-esphome                   (Pour accéder au Wifi)
            AsyncTCP-esphome                            (Pour utiliser les focntionnalités TCP)
            bblanchon/ArduinoJson@^6.17.2               (Pour accéder au fonctionnalités Json)
            adafruit/Adafruit GFX Library @ ^1.10.1
            adafruit/Adafruit SSD1306 @ ^2.4.0
            adafruit/Adafruit NeoPixel @ ^1.7.0
            adafruit/Adafruit VEML6075 Library@^2.2.0
            adafruit/Adafruit BusIO@^1.14.1

    Autres librairies (à copier dans le répertoire lib)
        ° WifiManager
            //Remarques
            //Pour trouver le WifiManager (dans la branche development)
            //   https://github.com/tzapu/WiFiManager/tree/development
            //   Ne pas oublier d'appuyez sur l'ampoule et choisir : ajouter Lib
        ° Classes Oled :
            - MyOled
            - MyOledView
            - MyOledViewErrorWifiConnexion
            - MyOledViewInitialisation
            - MyOledViewWifiAp
            - MyOledViewWorking
            - MyOledViewWorkingCOLD
            - MyOledViewWorkingHEAT
            - MyOledViewWorkingOFF
        ° MyButton
            //Fonctionnement des boutons Action et Reset
        ° MYLIB
            //Fonctions utiles (voir plus bas)
        ° DHT
            //Fonctionnement du DHT22

    
    Fonctions utiles (utilitaires)
        /lib/MYLIB/myFunctions.cpp
            //Pour vérifier plus simplement que deux chaines sont identiques
            bool isEqualString(std::string line1, std::string line2)
            //Pour extraire une partie d'une chaine de caractères avec l'aide d'un index
            std::string getValue(std::string data, char separator, int index)
            //Pour remplacer plus facilement une sous chaine
            bool replaceAll(std::string& source, const std::string& from, const std::string& to)
            //Pour obtenir un chaine aléatoire d'une certaine longeur
            std::string get_random_string(unsigned int len)
    
    Configuration du système

            GPIO12 : pin 12  LED_Rouge 
            GPIO14 : pin 14  LED_Vert                              
            GPIO27 : pin 27  LED_Jaune  
            GPIO15 : pin 15  DHT 22  (Sensor de température)

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
WiFiClient client;
HTTPClient http;
#define WEBSERVER_H

//Pour la gestion du serveur ESP32
#include "MyServer.h"
MyServer *myServer = NULL;

//Variable pour la connection Wifi
const char *SSID = "SAC_";
const char *PASSWORD = "sac_";
String ssIDRandom;

//Définition de variable général
#define SERIAL_BEGIN_CONFIG   9600  
#define SERVER_PORT 80         // Port du serveur.
#define GPIO_PIN_DHT_22   15   //GPIO15
#define NOM_FOUR   "Four9394"  //Nom du Four utilisé par le système 
#define API_ADRESS_GETALLWOODS   "http://149.56.141.62:3000/api/woods/getAllWoods"

//Définition des trois leds de statut
#define GPIO_PIN_LED_LOCK_RED           12     //GPIO12
#define GPIO_PIN_LED_OK_GREEN           14     //GPIO14
#define GPIO_PIN_LED_HEAT_YELLOW        27     //GPIO27

//Definition des éléments de l'ecran OLED
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET 4 // Reset pin # (or -1 if sharing Arduino reset pin)
#define OLED_I2C_ADDRESS 0x3C // Adresse I2C de l'écran Oled
#define OLD_VEILLE_SECONDES 30  // Temps avant mise en veille de l'ecran Oled

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

// Initialisation des vues Oled
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

// Variable pour la gestion du four.
int etatFour = 0; // Etat du four / Différents etat possible: OFF = 0, COLD = 1, HEAT = 2
float tempDemander = 23; // La température de chauffage attendu
int tempsSechage = 20; // Le temps de séchage du bois.

// Variable Utilitaire
String JsonListeBois;
String JsonLeBois;
int nbSecondes = 20;        // Nombre de secondes restante pour le fonctionnement du four.
float temp = 0;             // La température actuel du Four.
char laTemperature[100];    // Température actuel mais en char.
char lesSecondes[100];      // Secondes restante mais en char.             
bool demarrer = false;      // Savoir si le four est démmarrer ou non.

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


    // Recupère la Température
    if (string(actionToDo.c_str()).compare(string("askTempFour")) == 0) {
      temp = myTemp->getTemperature();
      sprintf(laTemperature, "%4.1f °C;%is", temp, nbSecondes);
      return(laTemperature); }

    // Démarre le Four.
    if (string(actionToDo.c_str()).compare(string("startAction")) == 0) {
        demarrer = true;
        return(""); }

    // Recupère la liste des bois.
    if (string(actionToDo.c_str()).compare(string("askListeWood")) == 0) {
        http.begin(client, API_ADRESS_GETALLWOODS);
        http.GET();
        JsonListeBois = http.getString();
        http.end();
        return(JsonListeBois.c_str()); }

    // Recupère les informations d'un bois.
    if (string(actionToDo.c_str()).compare(string("afficherBois")) == 0) {
        char buffer[100];
        sprintf(buffer, "http://149.56.141.62:3000/api/woods/getWood/%S", arg1.c_str());
        http.begin(client, buffer);
        http.addHeader("Authorization","eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpYXQiOjE2Njg3ODUyMjF9.jhT6LpcaUyx5w0gXGldjC9TZxymvArrzvPt6GG2WukM");
        http.GET();
        JsonLeBois = http.getString();
        http.end();
        DynamicJsonDocument doc(2048);
        deserializeJson(doc,JsonLeBois);
        for(JsonObject elem : doc.as<JsonArray>()){
            tempDemander = elem["temperature"];
            tempsSechage = elem["dryingTime"];
            nbSecondes = tempsSechage;
        }
        return(JsonLeBois.c_str()); }
   
    std::string result = "";
    return result;
}

//fonction qui permet d'affiché et rafraichir la vue d'etat du four.
// Four prêt à démmarer     : Etat OFF
// Four en attente          : Etat COLD
// Four en fonctionnement   : Etat HEAT
void RefreshMyOledParams(){
switch(etatFour){
    case 0: // Etat OFF
        myOledViewWorkingOFF->setParams("nomDuSysteme", nomSysteme.c_str());
        myOledViewWorkingOFF->setParams("idDuSysteme", idSysteme.c_str());
        myOledViewWorkingOFF->setParams("temperature", laTemperature);
        myOledViewWorkingOFF->setParams("ipDuSysteme", WiFi.localIP().toString().c_str());
        myOled->displayView(myOledViewWorkingOFF);
        break;

    case 1: // Etat COLD
        myOledViewWorkingCOLD->setParams("nomDuSysteme", nomSysteme.c_str());
        myOledViewWorkingCOLD->setParams("idDuSysteme", idSysteme.c_str());
        myOledViewWorkingCOLD->setParams("temperature", laTemperature);
        myOledViewWorkingCOLD->setParams("ipDuSysteme", WiFi.localIP().toString().c_str());
        myOled->displayView(myOledViewWorkingCOLD);
        break;
            
    case 2: // Etat HEAT
        myOledViewWorkingHEAT->setParams("nomDuSysteme", nomSysteme.c_str());
        myOledViewWorkingHEAT->setParams("idDuSysteme", idSysteme.c_str());
        myOledViewWorkingHEAT->setParams("temperature", laTemperature);
        myOledViewWorkingHEAT->setParams("ipDuSysteme", WiFi.localIP().toString().c_str());
        myOled->displayView(myOledViewWorkingHEAT);
        break;
}
}

void setup() { 
    Serial.begin(SERIAL_BEGIN_CONFIG);
    delay(100);

    // Initialisation OLED et Affichage Oled Initialisation
    myOled->init(OLED_I2C_ADDRESS);
    myOled->veilleDelay(OLD_VEILLE_SECONDES);
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

    //Actualisation de la vue Oled Initialisation
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
    myServer = new MyServer(SERVER_PORT);
    myServer->initAllRoutes();
    myServer->initCallback(&CallBackMessageListener);


    // Clignotement 2 fois des LEDs pour signifié que le système est opérationnel.
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

    myOledViewWorkingOFF = new MyOledViewWorkingOFF();
    myOledViewWorkingCOLD = new MyOledViewWorkingCOLD();
    myOledViewWorkingHEAT = new MyOledViewWorkingHEAT();

    digitalWrite(GPIO_PIN_LED_OK_GREEN,HIGH);

 }

void loop() {
    temp = myTemp->getTemperature();
    sprintf(laTemperature, "%4.1f C", temp);
    RefreshMyOledParams(); 
    if(demarrer){
        if(etatFour == 0){
            etatFour = 1;
            digitalWrite(GPIO_PIN_LED_OK_GREEN,LOW);
            digitalWrite(GPIO_PIN_LED_HEAT_YELLOW,HIGH);
        }

        if(temp > (tempDemander * 0.90) && temp < (tempDemander * 1.10)){
            if(etatFour == 1){
                etatFour = 2;
                digitalWrite(GPIO_PIN_LED_HEAT_YELLOW,LOW);
                digitalWrite(GPIO_PIN_LED_LOCK_RED,HIGH);
            }
            sprintf(lesSecondes, "%i secondes.", nbSecondes);
            nbSecondes--;
            if(nbSecondes == 0){
                demarrer = false;
                nbSecondes = tempsSechage;
                etatFour = 0;
                digitalWrite(GPIO_PIN_LED_LOCK_RED,LOW);
                digitalWrite(GPIO_PIN_LED_OK_GREEN,HIGH);
            }
        }
        else{
            if(temp < (tempDemander * 0.90) ){
                demarrer = false;
                nbSecondes = tempsSechage;
                etatFour = 0;
                digitalWrite(GPIO_PIN_LED_LOCK_RED,LOW);
                digitalWrite(GPIO_PIN_LED_OK_GREEN,HIGH);
            }
            else{
                if(etatFour == 2){
                    etatFour = 1;
                    digitalWrite(GPIO_PIN_LED_LOCK_RED,LOW);
                    digitalWrite(GPIO_PIN_LED_HEAT_YELLOW,HIGH);
                }
            }
        }
    }
    delay(1000);
  }