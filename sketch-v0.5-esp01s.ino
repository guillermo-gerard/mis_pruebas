/*
   ESP8266-01s
   TX--->RX
   RX--->TX
   chpd/en-->pullUp4.7k/+3.3vcc
   GPIO 0--->pullDown4.7k/GND
   GPIO 2--->pullUp/+3.3vcc
   vcc--->vcc
   gnd--->gnd
   
(luego de la programacion solo son necesarios vcc & gnd)

*/


//#include <Wire.h>         // incluir libreria para I2C
//#include <SPI.h>           // incluir libreria para SPI

#include <ESP8266WiFi.h> // https://github.com/esp8266/Arduino
#include <DNSServer.h> // Servidor DNS local usado para redirigir todas las solicitudes al portal de configuración 
#include <ESP8266WebServer.h>// Servidor web local usado para servir el portal de configuración. 
//#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include "ArduinoOTA.h" // https://github.com/esp8266/Arduino/tree/master/libraries/ArduinoOTA
#include <WiFiManager.h> // incluir libreria para Portal-WiFi https://github.com/tzapu/WiFiManager#include <ESP8266WiFi.h> //https://github.com/esp8266/Arduino
#define AP_SSID "ESP8266-01s" // Nombre AccesPoint con la ssid "NodeMCU-ESP8266"
#define AP_PASS "password" // Contraseña AccesPoint con la "password"

//-----------------------------------
unsigned long lecturaMillis = 0;  //|
//-----------------------------------

void setup() {
  // put your setup code here, to run once:
  //  pinMode(pinWDU, INPUT);
  //  attachInterrupt(digitalPinToInterrupt(pinWDU), CHANGE);
  Serial.begin(9600); {}   // inicializa comunicacion serial a 9600 bps
  Serial.println(F("Iniciando:"));
  //  Wire.begin(D2 , D1);   // configura e inicializa serial i2c en pines(sda, scl)
  //  SPI.begin();           // Inicializa serial spi
  WiFi.mode(WIFI_AP_STA); // modo establecido explícitamente, esp por defecto es STA + AP
  Serial.println(F("Wifi Iniciado"));
  WiFiManager wifiManager; //declaración de objeto wifiManager
  // wifiManager.resetSettings(); // wifiManager.resetSettings(); //restablecer la configuración - borrar las credenciales para realizar pruebas
  // Conectarse automáticamente usando credenciales guardadas,
  // si falla la conexión, inicia un punto de acceso con el nombre especificado ("AutoConnectAP"),
  // si está vacío generará automáticamente SSID, si la contraseña está en blanco será AP anónimo (wm.autoConnect ())
  // luego entra en un bucle de bloqueo en espera de configuración y devolverá un resultado exitoso

  wifiManager.setConfigPortalTimeout(60); //si cliente no accede datos de configuracion se renicia en (i) segundos
  wifiManager.setConnectTimeout (30); // establecer el tiempo de espera de la conexión

  bool res;
  // res = wifiManager.autoConnect(); // nombre de AP generado automáticamente a partir de chipid
  // res = wifiManager.autoConnect("AutoConnectAP"); // si solo desea un punto de acceso no seguro
  res = wifiManager.autoConnect("AutoConnectAP", "password"); // ("nombre red SSID, contraseña");
  if (!res) {
    Serial.println("Fallo la Coneccion :(");
    ESP.restart();  // reiniciar esp
  }
  else {
    Serial.println("Coneccion Realizada :)");
  }

  ArduinoOTA.setPort(8266); // El puerto predeterminado es 8266
  ArduinoOTA.setHostname("OTA-ESP8266-01s"); // El nombre de host predeterminado es esp8266- [ChipID]
  // Sin autenticación por defecto
  // ArduinoOTA.setPassword("admin");
  // La contraseña también se puede configurar con su valor md5
  // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
  // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");

  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH)
      type = "sketch";
    else // U_SPIFFS
      type = "filesystem";

    // NOTA: si actualiza SPIFFS, este sería el lugar para desmontar SPIFFS usando SPIFFS.end()
    Serial.println("Iniciar actualización " + type);
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progreso: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Error de autenticación");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Inicio fallido");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Falló la conexión");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Error al recibir");
    else if (error == OTA_END_ERROR) Serial.println("Fallo al final");
  });
  ArduinoOTA.begin();
  WiFi.softAP(AP_SSID, AP_PASS); // Iniciamos AccesPoint con la ssid y password predefinidos
  Serial.println("Soporte Actualizaciones OTA");
  Serial.print("dirección IP de red: ");
  Serial.println(WiFi.localIP());
  Serial.print("dirección IP de AccesPoint: ");
  Serial.print(AP_SSID);
  Serial.println(WiFi.softAPIP());

}

void loop() {
  // put your main code here, to run repeatedly:

  ArduinoOTA.handle(); //llama constantemente a responder a una solicitud de actualización OTA

  /*  if (millis() >= lecturaMillis + 5000) {
      if (millis() >= lecturaMillis + 250) {

      }
      if (millis() >= lecturaMillis + 500) {

      }
      if (millis() >= lecturaMillis + 750) {

      }
      if (millis() >= lecturaMillis + 1000) {

      }
      if (millis() >= lecturaMillis + 1250) {

      }
      if (millis() >= lecturaMillis + 1500) {

      }
      if (millis() >= lecturaMillis + 1750) {

      }
      if (millis() >= lecturaMillis + 2000) {

          }
  */
  lecturaMillis = millis();
}
