/*
   a mica NodeMCU 1.0 v3ESP8266
   A0 ---> Salida módulo LDR
   D1 // gpio5 ---> SCL ---> RTC DS1307 + oled ssd1306 132x28 + BMP2280
   D2 // gpio4 ---> SDA ---> RTC DS1307 + oled ssd1306 132x28 + BMP2280
   D3 // gpio ---> res4.7k a vcc +
   D4 // gpio2 ---> res4.7k a vcc + ---> DHT out
   D5 // gpio14 ---> SCLK ---> Modulo // adaptador MicroSD Tarjeta
   D6 // gpio12 ---> MISO ---> Modulo // Adaptador Tarjeta MicroSD
   D7 // gpio13 ---> MOSI ---> Modulo // Adaptador Tarjeta MicroSD
   D8 // gpio15 ---> res10k a gnd ---> Módulo CC / SS // Tarjeta adaptadora MicroSD
*/
//____________________________________
//|
unsigned long lecturaMillis = 0;    //|
byte segundosMillis = 0;            //|
byte minutosMillis = 0;             //|
byte horasMillis = 0;               //|
int diasMillis = 0;                 //|
//int pinWDU = D0;                  //|
//__________________________________//|

#include <Wire.h>         // incluir libreria para I2C
#include <SPI.h>           // incluir libreria para SPI



#include <ESP8266WiFi.h> // https://github.com/esp8266/Arduino
#include <DNSServer.h> // Servidor DNS local usado para redirigir todas las solicitudes al portal de configuración 
#include <ESP8266WebServer.h>// Servidor web local usado para servir el portal de configuración. 
//#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include "ArduinoOTA.h" // https://github.com/esp8266/Arduino/tree/master/libraries/ArduinoOTA
#include <WiFiManager.h> // incluir libreria para Portal-WiFi https://github.com/tzapu/WiFiManager#include <ESP8266WiFi.h> //https://github.com/esp8266/Arduino
#define AP_SSID "NodeMCU-ESP8266" // Nombre AccesPoint con la ssid "NodeMCU-ESP8266"
#define AP_PASS "password" // Contraseña AccesPoint con la "password"


#include <RTClib.h>      // incluir libreria para RTC
RTC_DS1307 rtc;     // llama al RTC_DS1307 como rtc
//#define RTC_I2C_ADDRESS 0x68 //
//#define RTC_EEPROM_I2C_ADDRESS 0x50 //

#include <SD.h>         // incluir libreria para SD
const int chipSelect = D8; // // chip esclavo SD puerto SPI - en el uno pin 4 - en el esp8266 nodemcu 1.0 v3 pin D8 gpio15
File myFile;// buscar encontrar o crear archivo en raiz sd
/*
#include <Adafruit_GFX.h>   // libreria para pantallas graficas
#include <Adafruit_SSD1306.h>   // libreria para controlador SSD1306
#define OLED_I2C_ADDRESS 0x3C  //
#define OLED_RESET -1      // necesario por la libreria pero no usado
#define ANCHO 128     // reemplaza ocurrencia de ANCHO por 128
#define ALTO 32       // reemplaza ocurrencia de ALTO por 32
Adafruit_SSD1306 oled(ANCHO, ALTO, &Wire, OLED_RESET);  // crea objeto
*/
#include <DHT.h> // incluir libreria DHT sensor
#define DHTPIN D4 // sensor en pin D4
#define DHTTYPE DHT11   // DHT11 DHT21 DHT22
DHT dht(DHTPIN, DHTTYPE);  // crea objeto

#include <Adafruit_Sensor.h>  // incluir libreria sensores unificados 
#include <Adafruit_BMP280.h>  // incluir libreria sensor BMP280
#define BMP280_I2C_ADDRESS  0x77  //  0x76
Adafruit_BMP280 bmp;  // crea objeto

#define LDR A0

////////////////////////////////////////////////////////////////////////////////////////////////////////////

void setup () {
  //  pinMode(pinWDU, INPUT);
  //  attachInterrupt(digitalPinToInterrupt(pinWDU), CHANGE);
  Serial.begin(9600); {}   // inicializa comunicacion serial a 9600 bps
  Serial.println(F("Iniciando:"));
  Wire.begin(D2 , D1);   // configura e inicializa serial i2c en pines(sda, scl)
  SPI.begin();           // Inicializa serial spi
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
  ArduinoOTA.setHostname("OTA-NodeMCU-ESP8266"); // El nombre de host predeterminado es esp8266- [ChipID]
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
  

if (!rtc.begin()) {       // sino inicializa modulo con direccion I2C definida previamente
  Serial.println(F("Modulo RTC fallo"));  // muestra mensaje de error
  return;         // se sale regresando el programa
}
rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
Serial.println(F("Modulo RTC iniciado correctamente")); // muestra mensaje de confirmacion

if (!SD.begin(chipSelect)) {       // sino inicia el modulo
  Serial.println(F("Modulo SD fallo"));  // muestra mensaje de error
  return;         // se sale regresando el programa
}
Serial.println(F("Modulo SD iniciado correctamente")); // muestra mensaje confirmacion
myFile = SD.open("DATA.txt", FILE_WRITE); //abrir sd, buscar crear archivo y abrir modo escritura
if (myFile) { //para tratar el archivo
  Serial.println(F("Archivo abierto correctamente"));
  myFile.println("Fecha, Hora, Temperatura DHT ºC, Temperatura BMP ºC, Humedad %, Presion hPa, Altitud M, Luminosidad"); //imprimir en el archivo los siguientes encabezados.
  myFile.close(); //cerrmos el archivo.
}

/*if (!oled.begin(SSD1306_SWITCHCAPVCC, OLED_I2C_ADDRESS)); { // sino inicializa modulo con direccion I2C definida previamente
  Serial.println(F("Modulo Display fallo"));  // muestra mensaje de error
  return;         // se sale regresando el programa
}
Serial.println(F("Modulo Display iniciado correctamente"));// muestra mensaje confirmacion
*/

if (!bmp.begin(BMP280_I2C_ADDRESS)) {       // sino inicializa modulo con direccion I2C definida previamente
  Serial.println(F("Modulo BMP fallo"));  // muestra mensaje de error
  return;         // se sale regresando el programa
}
Serial.println(F("Modulo BMP iniciado correctamente")); // muestra mensaje de confirmacion

/* if (!dht.begin()) {       // sino inicializa modulo con direccion I2C definida previamente
    Serial.println(F("Modulo DHT fallo"));  // muestra mensaje de error
    return;         // se sale regresando el programa
  }
*/
dht.begin();
Serial.println(F("Modulo DHT iniciado correctamente")); // muestra mensaje de confirmacion y tiempo trascurrido menos espera inicial
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////

void tiempoMillis() {
  if (millis() >= lecturaMillis + 1000) {
    lecturaMillis = millis();
    Serial.println (segundosMillis);
    segundosMillis++;

    if (segundosMillis == 60) {
      segundosMillis = 0;
      minutosMillis++;
    }
    if (minutosMillis == 60) {
      minutosMillis = 0;
      horasMillis++;
    }
    if (horasMillis  == 24) {
      horasMillis = 0;
      diasMillis++;
    }
    if (diasMillis == 365) {
      diasMillis = 0;
    }
    if (lecturaMillis < (lecturaMillis + 10000)) {
      lecturaMillis = 0;
    }
    /*
        Serial.print (diasMillis);
        Serial.print (F("-"));
        Serial.print (horasMillis);
        Serial.print (F(":"));
        Serial.print (minutosMillis);
        Serial.print (F(":"));
        Serial.println (segundosMillis);
    */
  }
}

/*void formatoPantalla() {
  oled.clearDisplay();      // limpia pantalla
  oled.drawRect(0, 0, 64, 11, WHITE); // dibuja rectangulo
  oled.drawRect(64, 0, 64, 11, WHITE); // dibuja rectangulo
  oled.drawRect(0, 10, 64, 11, WHITE); // dibuja rectangulo
  oled.drawRect(64, 10, 64, 11, WHITE); // dibuja rectangulo
  oled.drawRect(0, 20, 64, 11, WHITE); // dibuja rectangulo
  oled.drawRect(64, 20, 64, 11, WHITE); // dibuja rectangulo

    oled.setCursor(5, 3);   // ubica cursor en coordenadas 28,34
    oled.setTextSize(2);      // establece tamano de texto en 2
    oled.setTextColor(WHITE);   // establece color al unico disponible (pantalla monocromo)
    oled.print("--->FC<---");     // escribe texto
    oled.setCursor(12, 15);   // ubica cursor en coordenadas 28,34
    oled.setTextSize(2);      // establece tamano de texto en 2
    oled.setTextColor(WHITE);   // establece color al unico disponible (pantalla monocromo)
    oled.print("servicios");     // escribe texto
  oled.display();     // muestra en pantalla todo lo establecido anteriormente
}
  */
  
void loggingTiempo() {
  DateTime now = rtc.now();
  myFile = SD.open("DATA.txt", FILE_WRITE);
  if (myFile) {
    myFile.print(now.year(), DEC);
    myFile.print('/');
    myFile.print(now.month(), DEC);
    myFile.print('/');
    myFile.print(now.day(), DEC);
    myFile.print(',');
    myFile.print(now.hour(), DEC);
    myFile.print(':');
    myFile.print(now.minute(), DEC);
    myFile.print(':');
    myFile.print(now.second(), DEC);
    myFile.print(",");
  }

  //consola log
  Serial.println(F("Archivo abierto correctamente"));
  Serial.print(now.year(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.println(now.day(), DEC);
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.println(now.second(), DEC);
  Serial.println("guardado con éxito");
  myFile.close();
  delay(1000);
}

void loggingTemperaturaDHT() {

  float t = dht.readTemperature(); // Leer la temperatura en grados Celsius
  // float f = dht.readTemperature(true);  //Leer la temperatura como Fahrenheit

  //Compruebe si alguna lectura falló y salga antes (para volver a intentarlo).
  if  (isnan(t) /*|| isnan(f)*/) {
    Serial.println(F("Error al leer temperatura del sensor DHT!"));
    return;
  }

  //consola log
  Serial.print(F("Temperatura DHT: "));
  Serial.print(t);
  Serial.println(" °C");
  //Serial.print(f);
  //Serial.println(" *F\t");

  myFile = SD.open("DATA.txt", FILE_WRITE);
  if (myFile) {
    Serial.println("abrir con éxito");
    myFile.print(t);
    myFile.print(",");
  }
  myFile.close();
  Serial.println("guardado con éxito");
}

void logginTemperaturaBMP() {

  float tb = bmp.readTemperature();

  //Compruebe si alguna lectura falló y salga antes (para volver a intentarlo).
  if  (isnan(tb)) {
    Serial.println(F("Error al leer Temperatura del sensor BMP!"));
    return;
  }

  //consola log
  Serial.print(F("Temperatura BMP: "));
  Serial.print(tb);
  Serial.println(F(" °C"));


  myFile = SD.open("DATA.txt", FILE_WRITE);
  if (myFile) {
    Serial.println(F("abrir con éxito"));
    myFile.print(tb);
    myFile.print(",");
  }
  myFile.close();
  Serial.println("guardado con éxito");
}

void loggingHumedad() {

  float h = dht.readHumidity();

  //Compruebe si alguna lectura falló y salga antes (para volver a intentarlo).
  if  (isnan(h)) {
    Serial.println(F("Error al leer humedad del sensor!"));
    return;
  }

  //consola log
  Serial.print(F("Humedad: "));
  Serial.print(h);
  Serial.println(F("%"));


  myFile = SD.open("DATA.txt", FILE_WRITE);
  if (myFile) {
    Serial.println(F("abrir con éxito"));
    myFile.print(h);
    myFile.print(",");
  }
  myFile.close();
  Serial.println("guardado con éxito");
}

void logginPresion() {

  float p = bmp.readPressure() / 100;

  //Compruebe si alguna lectura falló y salga antes (para volver a intentarlo).
  if  (isnan(p)) {
    Serial.println(F("Error al leer Presion del sensor BMP!"));
    return;
  }

  //consola log
  Serial.print(F("Presion: "));
  Serial.print(p);
  Serial.println(F("mb/hPa"));


  myFile = SD.open("DATA.txt", FILE_WRITE);
  if (myFile) {
    Serial.println(F("abrir con éxito"));
    myFile.print(p);
    myFile.print(",");
  }
  myFile.close();
  Serial.println("guardado con éxito");
}

void logginAltitud() {

  float a = bmp.readAltitude(1016); // (1013.25) obtener por localizacion, o ir ajustando situandolo desde el nivel del suelo.

  //Compruebe si alguna lectura falló y salga antes (para volver a intentarlo).
  if  (isnan(a)) {
    Serial.println(F("Error al leer Presion del sensor BMP!"));
    return;
  }

  //consola log
  Serial.print(F("Altitud: "));
  Serial.print(a);
  Serial.println(F("M."));


  myFile = SD.open("DATA.txt", FILE_WRITE);
  if (myFile) {
    Serial.println(F("abrir con éxito"));
    myFile.print(a);
    myFile.print(",");
  }
  myFile.close();
}

void logginLuz() {

  pinMode (LDR, INPUT);
  int L = analogRead(LDR);

  //Compruebe si alguna lectura falló y salga antes (para volver a intentarlo).
  if  (isnan(L)) {
    Serial.println(F("Error al leer Luminosidad!"));
    return;
  }

  //consola log
  Serial.print(F("Luminosidad: "));
  Serial.print(L);
  Serial.println(F(" Luz"));


  myFile = SD.open("DATA.txt", FILE_WRITE);
  if (myFile) {
    Serial.println(F("abrir con éxito"));
    myFile.print(L);
    myFile.println(",");
  }
  myFile.close();
  Serial.println("guardado con éxito");
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////

void loop() {
  ArduinoOTA.handle(); //llama constantemente a responder a una solicitud de actualización OTA
//  formatoPantalla();
//  delay(1000);
  loggingTiempo();
  delay(1000);
  loggingTemperaturaDHT();
  delay(1000);
  logginTemperaturaBMP();
  delay(1000);
  loggingHumedad();
  delay(1000);
  logginPresion();
  delay(1000);
  logginAltitud();
  delay(1000);
  logginLuz();
  delay(1000);
  delay(5000);
}
