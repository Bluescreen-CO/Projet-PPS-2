//====================================================================================================================================
//Nom: David Landry
//
//Description: Projet PPS, Dispositif de mesure de la qualité de l'air
//
//Cible: Maduino Zero LoroWAN
//
//Compilateur: Visual Studio
//
//Utilisation: Programme qui permet de prendre un position gps et la concentration de gaz. Ces données sont par la suite envoyer à ThnigsBord Cloud pour les afficher 
//sur une google map.
//
//Restriction: Utiliser la communication LoraWAN
//             
//
//Historique:
//1-R0 / 2024.10.17 / D.Landry / Création;
//1-R1 / 2024.10.18 / D.Landry / modification, Ajout Capteur DHT11;
//1-R2 / 2024.10.20 / D.Landry / modification, Ajout Module GPS;
//1-R3 / 2024.10.20 / D.Landry / modification, Ajout Fonction Affichage GPS;
//===============================réation====================================================================================================à

//Inclusion générale
#include <Arduino.h>
#include <DHT.h>
#include <TinyGPS++.h>
#include "wiring_private.h" 
#include <PubSubClient.h>
#include <lmic.h>
#include <hal/hal.h>




// Define pins for UART (SERCOM2)
#define GPS_TX_PIN 4  // TX pin 
#define GPS_RX_PIN 5  // RX pin 


//Définir broche capteur DHT11 et son type
#define DHTPIN 2
#define DHTTYPE DHT11

//ThingsBoard Cloud MQTT 
const char* mqtt_server = "?"; // L'adresse du serveur 
const char* DEVICE_TOKEN = "GdflYFINhOhdrAeebu16"; // Le device acces token


//Instance Seriel
Uart SerialGPS(&sercom2, GPS_RX_PIN, GPS_TX_PIN, SERCOM_RX_PAD_1, UART_TX_PAD_0);


//Instance TinyGPS++
TinyGPSPlus gps;


//Instance Capteur DHT11
DHT dht(DHTPIN, DHTTYPE);







void setup() 
{


  /*
  // Configure le GPS UART pins pour le SERCOM2
  pinPeripheral(GPS_RX_PIN, PIO_SERCOM);  // Set PA09 to SERCOM2 RX
  pinPeripheral(GPS_TX_PIN, PIO_SERCOM);  // Set PA08 to SERCOM2 TX
  */


  //démarage com Serial vitesse BUD (115200)
  SerialUSB.begin(115200);
  while (!SerialUSB);


  /*
  // Initialize GPS Serial (SERCOM2)
  SerialGPS.begin(9600);  // Set baud rate for GPS module communication

  //Message d<initialisation
  SerialUSB.println("Waiting for GPS data... INITIALISATION ...");
  */

  dht.begin();   


}




void loop()
{
  delay(3000);


  /*
  // Fait la lecture des donnee recus du module GPS
  while (SerialGPS.available() > 0) {
    gps.encode(SerialGPS.read());
  }

  // Verifie si la valeur recu est valide
  if (gps.location.isValid()) {
    SerialUSB.print("Latitude: ");
    SerialUSB.println(gps.location.lat(), 6);  // Latitude en degre

    SerialUSB.print("Longitude: ");
    SerialUSB.println(gps.location.lng(), 6);  // Longitude en degre

    SerialUSB.print("Satellites: ");
    SerialUSB.println(gps.satellites.value());  // Nombre de satellites

    SerialUSB.print("Altitude: ");
    SerialUSB.println(gps.altitude.meters());  // Altitude en metres
  } else {
    SerialUSB.println("Waiting for GPS fix...");
  }


delay(2000);
    // Read humidity and temperature from the sensor
    float humidity = dht.readHumidity();
    float temperature = dht.readTemperature(); // Default is Celsius

    // Print the results to the serial monitor
    SerialUSB.print(F("Humidity: "));
    SerialUSB.print(humidity);
    SerialUSB.print(F("%  Temperature: "));
    SerialUSB.print(temperature);
    SerialUSB.println(F("°C"));



*/
}











/*   TEST : Fonctionnement com Serial OK 
SerialUSB.print('a');
delay(1000);
*/




/*  TEST : Fonctionnemnt d'un capteur + envoie résultat Seriel
delay(2000);
    // Read humidity and temperature from the sensor
    float humidity = dht.readHumidity();
    float temperature = dht.readTemperature(); // Default is Celsius

    // Print the results to the serial monitor
    SerialUSB.print(F("Humidity: "));
    SerialUSB.print(humidity);
    SerialUSB.print(F("%  Temperature: "));
    SerialUSB.print(temperature);
    SerialUSB.println(F("°C"));
*/




  /*  TEST : Nombre de Satelite pour le module GPS
  SerialUSB.print("Satellites: ");
  SerialUSB.println(gps.satellites.value());
  */