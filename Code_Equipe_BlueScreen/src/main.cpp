//Inclusion générale
#include <Arduino.h>
#ifdef ArduCAM_ESP32S_UNO_PSRAM
#include <WiFi.h>
#include <AsyncTCP.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#endif
#include <string>
#include <ESPAsyncWebServer.h>


// Ajouter des reponses en Json
#include <ArduinoJson.h>
#include <AsyncJson.h>
#include <SPIFFS.h>


//inclusion des différente librairie utilisé
#include "PWM/pwm.h"
#include "HCSR04/HCSR04.H"
#include "HCSR04_2/HCSR04_2.h"
#include "Led/led.h"
#include "L298N/L298N.h"


/*          Note d'utilisation
    -Roues gauche
        Pin A18 (D6) In1
        Pin A13 (D5) In2
    -Roues droite
        Pin A16 (D4) In3
        Pin A15 (D3) In4
    -Roue Vitesse 
        -Pin A14 (D2) Ena et Enb 
    -Distance Arriere 
        Pin A4 (S2) Trigger
        Pin A5 (S3) Echo
    -Distance Avant
        Pin A19 (D12) Trigger
        A17 (D11) Echo
    -DEL rouge STOP
        Pin A3 (S1) STOP
    -DEL Distance
        Pin A10 (D10) Rouge
        Pin A11 (D9)
        Pin A12 (D8)
 
    *1 seul module L298N est utilisé. il y a deux servo moteur branché en parallèle à chaque entrée*
*/


//Broche Servo moteur
const int in1 = A18;
const int in2 = A13;
const int in3 = A16;
const int in4 = A15;


// Set LED_BUILTIN if it is not defined by Arduino framework
#ifndef LED_BUILTIN
#define LED_BUILTIN 2
#endif

//Broche HCSR04 AVANT
const int trig_av = A19;
const int echo_av = A17;

//Broche HCSR04 ARRIERE
const int trig_arr = A4;
const int echo_arr = A5;

//Def LED
//#define arretLedPin A18
#define RedPin A10
#define GreenPin A11
#define BluePin A12



// PWM parametres
#define PWM_Motor  A14 // D2
#define PWM_RED_FREQUENCY 5000
#define PWM_RED_CHANNEL 0
#define PWM_RED_RESOLUTION 8

//Variable Globale
float consigneGlobale;


//instance PMW
PWM PMW_Control(PWM_Motor, PWM_RED_FREQUENCY, PWM_RED_CHANNEL, PWM_RED_RESOLUTION);



//Instance capteur de distance AVANT
HCSR04 distance_av(trig_av, echo_av);


//Instance capteur de distance ARRIERE
HCSR04_2 distance_arr(trig_arr, echo_arr);



//instance Led
//Led ControlArretLedPin(arretLedPin);

Led ControlLedStop(GPIO_NUM_5);
Led ControlLedRouge(RedPin);
Led ControlLedVerte(GreenPin);
Led ControlLedBleu(BluePin);


//instance L2998N MotorControl
MotorControl ControlMoteur(in1, in2, in3, in4, PWM_Motor);





//Connection a l'internet
AsyncWebServer server(80);

//const char *ssid = "BELL268";
//const char *password = "eric202b";

const char *ssid = "TGE-IOT";
const char *password = "iOTN3t$$"; 

void notFound(AsyncWebServerRequest *request)
{
    request->send(404, "text/plain", "Not found");
}



//Initialisation
void setup()
{
    Serial.begin(115200);

    // Initialize SPIFFS
    if (!SPIFFS.begin(true))
    {
        Serial.println("An Error has occurred while mounting SPIFFS");
        return;
    }

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    if (WiFi.waitForConnectResult() != WL_CONNECTED)
    {
        Serial.printf("WiFi Failed!\n");
        return;
    }

    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());

   
    // Route to load style file
    server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request)
        { request->send(SPIFFS, "/style.css", "text/css"); });

    // Route to load script file
    server.on("/script.js", HTTP_GET, [](AsyncWebServerRequest *request)
        { request->send(SPIFFS, "/script.js", "text/javascript"); });

    // Route to load script file
    server.on("/logo.png", HTTP_GET, [](AsyncWebServerRequest *request)
        { request->send(SPIFFS, "/logo.png"); });

    // Route to HTML page
    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
        {request->send(SPIFFS, "/index.html", String(), false); });



    


    // Route BOUTON ARRET et Redemarer 
    /*
    server.on("/stopLed", HTTP_POST, [](AsyncWebServerRequest *request){
    String state = request->arg("state")
    if (state == "on") {
      ControlArretLedPin.allumer();
    } else if (state == "off") {
      ControlArretLedPin.Eteindre();
    }
    request->send(200, "text/plain", "OK"); });
    */



    //Route pour /statut : Distance mesuree, etat du ROver, puissance et dirrection
    server.on("/statut", HTTP_GET, [](AsyncWebServerRequest *request)
              {
        StaticJsonDocument<100> data;
        String response;

        float distance_avant = distance_av.getDistance();

        
        if(distance_avant > 30){
            ControlLedVerte.allumer();
            ControlLedBleu.Eteindre();
            ControlLedRouge.Eteindre();
        }
        else if(distance_avant > 10){
            ControlLedVerte.Eteindre();
            ControlLedBleu.allumer();
            ControlLedRouge.Eteindre();
        }
        else if(distance_avant < 10) {
            ControlLedVerte.Eteindre();
            ControlLedBleu.Eteindre();
            ControlLedRouge.allumer();
            ControlMoteur.stop();
            ControlLedStop.allumer();

        }
        else {
            ControlLedVerte.Eteindre();
            ControlLedBleu.Eteindre();
            ControlLedRouge.Eteindre();
        }

        if(distance_arr.getDistance() < 10) {
            ControlMoteur.stop();
            ControlLedStop.allumer();
        }
        
        //Route de la distance vers JS
        data["distance_av"]= distance_avant;
        data["distance_arr"]= distance_arr.getDistance();


            serializeJson(data, response);
            request->send(200, "application/json",  response); });




      //Envoyer la valeur de la forme sur le moteur et retourner des parametres
    //Parametre (Tension consigne) (Tension mesuree) 
    //0 a 3.3V
    server.on("/status", HTTP_POST, [](AsyncWebServerRequest *request)
              {
         StaticJsonDocument<100> data;
         String response;

        //Variable et constante
        float tension_affichage;
        float Consigne;
    


                if (request->hasArg("consigne")) {
                    sscanf(request->arg("consigne").c_str(),"%f", &Consigne);
                    
                    PMW_Control.applyValue(Consigne*255/100);

                    tension_affichage = (Consigne);
                    data["tension_affichage"] = tension_affichage;
                    data["consigne"] = Consigne;
                    
                    


                    //Serial.println("Valeur envoyer");
                    Serial.print(Consigne);
                    Serial.print("      ");
                }
            
                

            serializeJson(data, response);
            request->send(200, "application/json",  response); });




    server.on("/control", HTTP_POST, [](AsyncWebServerRequest *request) {
    String direction = request->arg("direction");
    String cote = request->arg("cote");
    String consigneStr = request->arg("consigne");
    float consigne = consigneStr.toFloat();

    consigneGlobale = consigne *2;


    if (direction == "avancer") {
        if (cote == "centre") {
            ControlMoteur.forward_centre(consigneGlobale);
            ControlLedStop.Eteindre();
        } else if (cote == "gauche") {
            ControlMoteur.forward_gauche(consigneGlobale);
            ControlLedStop.Eteindre();
        } else if (cote == "droit") {
            ControlMoteur.forward_droit(consigneGlobale);
            ControlLedStop.Eteindre();
        }
    } else if (direction == "reculer") {
        if (cote == "centre") {
            ControlMoteur.backward_centre(consigneGlobale);
            ControlLedStop.Eteindre();
        } else if (cote == "gauche") {
            ControlMoteur.backward_gauche(consigneGlobale);
            ControlLedStop.Eteindre();
        } else if (cote == "droit") {
            ControlMoteur.backward_droit(consigneGlobale);
            ControlLedStop.Eteindre();
        }
    } else if (direction == "arret") {
        ControlMoteur.stop();
        ControlLedStop.allumer();
        request->send(200, "text/plain", "OK");
        return;
    }

 
    request->send(200, "text/plain", "OK");
});
    


    server.onNotFound(notFound);

    server.begin();


}

  





void loop()
{

/*
  TEST Distance
float distanceARR = distance_arr.getDistance();
float distanceAV = distance_av.getDistance();


Serial.print("Distance Arriere:");
Serial.print(distanceARR);
Serial.print("        ");

Serial.print("Distance Avant:");
Serial.print(distanceAV);
Serial.print("        ");

delay(5000);
*/



/*TEST Servo-Moteur
ControlMoteur.forward_centre(100);
delay(2000);
ControlMoteur.stop();
delay(2000);

ControlMoteur.forward_gauche(100);
delay(2000);
ControlMoteur.stop();
delay(2000);

ControlMoteur.forward_droit(100);
delay(2000);
ControlMoteur.stop();
delay(2000);

ControlMoteur.backward_centre(100);
delay(2000);
ControlMoteur.stop();
delay(2000);

ControlMoteur.backward_gauche(100);
delay(2000);
ControlMoteur.stop();
delay(2000);

ControlMoteur.backward_droit(100);
delay(2000);
ControlMoteur.stop();
delay(2000);
*/

}

