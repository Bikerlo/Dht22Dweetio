#include <Arduino.h>
#include <WiFi.h>
#include <Adafruit_Sensor.h>
//#include <Adafruit_TSL2591.h>
#include <Wire.h>
#include <Adafruit_BME280.h>

#define BME_SCK 22
#define BME_MISO 19
#define BME_MOSI 23
#define BME_CS 21

#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BME280 bme; // I2C
float temp=0;
float humidity=0;
const char* ssid = "JCTORRESO";
const char* password =  "3167732449";
const char* host = "dweet.io";

void connectToNetwork() {
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Estableciendo conexion WiFi..");
  }
 
  Serial.println("Conectado a la red");
 
}
 
void setup() {
 
  Serial.begin(9600);
 
  connectToNetwork();  //Conectarse a la red que especificamos arriba
 
  Serial.println(WiFi.macAddress());
  Serial.println(WiFi.localIP());
 
//  WiFi.disconnect(true);
Serial.println(WiFi.localIP());
// Inicializa el sensor interno de la esp
/*if (!bme.begin(0x76, &Wire)) {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    while (1);
  }
  */
 
}
 
void loop() {
  
  temp=temperatureRead();


  Serial.println();
  
  //inicia conexion al servicio dweet
   Serial.println("Conectandose a:");
   Serial.println(host);

   WiFiClient client;

   const int httpPort = 80;
   
   if(!client.connect(host, httpPort)){
     Serial.println("Conexion Fallida");
     delay(1000);
     return;
   }
//String url = "/dweet/for/uceva08?temperatura=23.5&humedad=80&indice=20";
  String url = "/dweet/for/peaceful-pleasure?" +String("temperatura=")+ String(temp) + String("&humdedad=56");
  


  Serial.print("Solicitando el recurso: ");

  client.print(String("GET ") + url + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" 
                    + "Connection: close\r\n\r\n");
  unsigned long timeout = millis(); //Leo la hora
  while(client.available() == 0){   //Mientras hayan cero letras disponibles...
      if(millis() - timeout > 5000){ //Resto la hora actual a la hora inicial, si es mas de 5s
        Serial.println("Expiro el tiempo de espera");
        client.stop();
        return;
      }
  }

  while(client.available()){
    String line = client.readStringUntil('\r');
    Serial.println(line);
  }
  
  Serial.println("Fin de conexion");
  client.stop();
  delay(20000);
}
