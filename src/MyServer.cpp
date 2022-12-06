/**
    Gestion d'un serveur WEB
    @file MyServer.cpp
    @author Alain Dubé
    @version 1.1 20/11/20 
*/
#include <Arduino.h>
#include "MyServer.h"
#include <ArduinoJson.h>
using namespace std;


typedef std::string (*CallbackType)(std::string);
CallbackType MyServer::ptrToCallBackFunction = NULL;

//Exemple pour appeler une fonction CallBack
//if (ptrToCallBackFunction) (*ptrToCallBackFunction)(stringToSend); 
void MyServer::initCallback(CallbackType callback) {
    ptrToCallBackFunction = callback;
    }

void MyServer::initAllRoutes() { 
    currentTemperature = 3.3f;

    //Initialisation du SPIFF.
    if (!SPIFFS.begin(true)) {
        Serial.println("An Error has occurred while mounting SPIFFS");
        return;
        }

    //Route initiale (page html)
    this->on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/index.html", "text/html");
        });

    //Route du script JavaScript
    this->on("/script.js", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/script.js", "text/javascript");
        });

    //Route du style CSS
    this->on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/style.css", "text/css");
        });

    //Route image
    this->on("/logo_SAC.png", HTTP_GET, [](AsyncWebServerRequest *request) {
        request->send(SPIFFS, "/logo_SAC.png", "image/png");
        });


    // Recupère la liste des bois
    this->on("/getListeWood", HTTP_GET, [](AsyncWebServerRequest *request) {
        std::string repString = "";
        if (ptrToCallBackFunction) repString = (*ptrToCallBackFunction)("askListeWood");
        DynamicJsonDocument doc(2048);
        deserializeJson(doc,repString);
        String lesBois;
        for(JsonObject elem : doc.as<JsonArray>()){
            String woodName = elem["name"];
            lesBois += woodName + String(";");
        }
        request->send(200, "text/plain", lesBois);
        });
    
    // Recupère les information d'un Bois
    this->on("/afficherBois", HTTP_POST, [](AsyncWebServerRequest *request) {
        std::string repString = "";
        String nomBois = request->getParam("nomBois", true)->value();
        char buffer[100];
        sprintf(buffer, "afficherBois %S", nomBois);
        if (ptrToCallBackFunction) repString = (*ptrToCallBackFunction)(buffer);
        DynamicJsonDocument doc(2048);
        deserializeJson(doc,repString);
        String leBois;
        for(JsonObject elem : doc.as<JsonArray>()){
            String woodName = elem["name"];
            String woodType = elem["type"];
            String woodOrigine = elem["origin"];
            String woodDryingTime = elem["dryingTime"];
            String woodTemperature = elem["temperature"];
            leBois += woodName + String(";") +  woodType + String(";") + woodOrigine + String(";") + woodDryingTime + String(";") + woodTemperature + String(";");
            Serial.println(leBois);
        }
        request->send(200, "text/plain", leBois);
        });

    // Recupère la température du four
    this->on("/lireTemp", HTTP_GET, [](AsyncWebServerRequest *request) {
        std::string repString = "";
        if (ptrToCallBackFunction) repString = (*ptrToCallBackFunction)("askTempFour");
        String lireTempDuFour =String(repString.c_str());
        request->send(200, "text/plain", lireTempDuFour );
        });

    // Demarrage du four
    this->on("/ActionToDo", HTTP_POST, [](AsyncWebServerRequest *request) {
        if (request->hasParam("actionToDo", true)) {
        String actionToDo = request->getParam("actionToDo", true)->value();
        if (string(actionToDo.c_str()).compare(string("startAction")) == 0) {
            if (ptrToCallBackFunction)(*ptrToCallBackFunction)("startAction");
            }
        }
        request->send(204);
        });

   
    this->onNotFound([](AsyncWebServerRequest *request){
        request->send(404, "text/plain", "Page Not Found");
        });
    
    this->begin();
};