#include <Wire.h>         // incluir libreria para I2C
#include <SPI.h>           // incluir libreria para SPI

#include <RTClib.h>      // incluir libreria para RTC
RTC_DS1307 rtc;     // llama al RTC_DS1307 como rtc

#include <SD.h>         // incluir libreria para SD
const int chipSelect = D8; // // chip esclavo SD puerto SPI - en el uno pin 4 - en el esp8266 nodemcu 1.0 v3 pin D8 gpio15
File myFile;// buscar encontrar o crear archivo en raiz sd

#include <DHT.h> // incluir libreria DHT sensor
#define DHTPIN D4 // sensor en pin D4
#define DHTTYPE DHT11   // DHT11 DHT21 DHT22
DHT dht(DHTPIN, DHTTYPE);

#include <Adafruit_Sensor.h>  // incluir libreria sensor BMP280
#include <Adafruit_BMP280.h>
#define BMP280_I2C_ADDRESS  0x76 // o direccion i2c  0x77
Adafruit_BMP280 bmp;

#define LDR A0

////////////////////////////////////////////////////////////////////////////////////////////////////////////

void setup () {
  Serial.begin(9600); {}   // inicializa comunicacion serial a 9600 bps
  Serial.println(F("Iniciando:"));

  if (!rtc.begin()) {       // sino inicia el modulo
    Serial.println(F("Modulo RTC fallo"));  // muestra mensaje de error
    return;         // se sale regresando el programa
  }
  Serial.print(F("Modulo RTC iniciado correctamente en:")); // muestra mensaje de confirmacion y tiempo trascurrido menos espera inicial

  if (!SD.begin(chipSelect)) {       // sino inicia el modulo
    Serial.println(F("Modulo SD fallo"));  // muestra mensaje de error
    return;         // se sale regresando el programa
  }
  Serial.print(F("Modulo SD iniciado correctamente en:")); // muestra mensaje confirmacion y tiempo trascurrido menos espera inicial
  myFile = SD.open("DATA.txt", FILE_WRITE); //abrir sd, buscar crear archivo y abrir modo escritura
  if (myFile) { //para tratar el archivo
    Serial.println(F("Archivo abierto correctamente"));
                   myFile.println("Fecha, Hora, TemperaturaDHT ºC, Humedad %, Presion hPa, Altitud, TemperaturaBMP, Luminosidad"); //imprimir en el archivo los siguientes encabezados.
                   myFile.close(); //cerrmos el archivo.
  }


//  if (!dht.begin()) {       // sino inicia el modulo
//    Serial.println(F("Modulo DHT fallo"));  // muestra mensaje de error
//    return;         // se sale regresando el programa
 // }
  dht.begin();
  Serial.print(F("Modulo DHT iniciado correctamente en:")); // muestra mensaje de confirmacion y tiempo trascurrido menos espera inicial
  
  if (!bmp.begin()) {       // sino inicia el modulo
    Serial.println(F("Modulo BMP fallo"));  // muestra mensaje de error
    return;         // se sale regresando el programa
  }
  Serial.print(F("Modulo BMP iniciado correctamente en:")); // muestra mensaje de confirmacion y tiempo trascurrido menos espera inicial
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////

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
  myFile.close();
  delay(1000);
}


void loggingTemperaturaDHT() {
  // ¡La lectura de la temperatura o la humedad tarda unos 250 milisegundos!
  // Las lecturas del sensor también pueden tener hasta 2 segundos de antigüedad (es un sensor muy lento)
  // Leer la temperatura en grados Celsius
  float t = dht.readTemperature();
  //Leer la temperatura como Fahrenheit
  //float f = dht.readTemperature(true);

  //Compruebe si alguna lectura falló y salga antes (para volver a intentarlo).
  if  (isnan(t) /*|| isnan(f)*/) {
    Serial.println(F("Error al leer temperatura del sensor DHT!"));
    return;
  }

  //debugging purposes
  Serial.print(F("TemperaturaDHT: "));
  Serial.print(t);
  Serial.println(" °C");
  //Serial.print(f);
  //Serial.println(" *F\t");

  myFile = SD.open("DATA.txt", FILE_WRITE);
  if (myFile) {
    Serial.println("abrir con éxito");
    myFile.print(t);
    myFile.println(",");
  }
  myFile.close();
}


void loggingHumedad() {

  float h = dht.readHumidity();

  //Compruebe si alguna lectura falló y salga antes (para volver a intentarlo).
  if  (isnan(h) /*|| isnan(f)*/) {
    Serial.println(F("Error al leer humedad del sensor DHT!"));
    return;
  }

  //debugging purposes
  Serial.print(F("Humedad: "));
  Serial.print(h);
  Serial.println(F("%"));


  myFile = SD.open("DATA.txt", FILE_WRITE);
  if (myFile) {
    Serial.println(F("abrir con éxito"));
                   myFile.print(h);
                   myFile.println(",");
  }
  myFile.close();
}


void logginPresion() {

  float p = bmp.readPressure() / 100;

  //Compruebe si alguna lectura falló y salga antes (para volver a intentarlo).
  if  (isnan(p)) {
    Serial.println(F("Error al leer Presion del sensor BMP!"));
    return;
  }

  //debugging purposes
  Serial.print(F("Presion: "));
  Serial.print(p);
  Serial.println(F("hPa"));


  myFile = SD.open("DATA.txt", FILE_WRITE);
  if (myFile) {
    Serial.println(F("abrir con éxito"));
    myFile.print(p);
    myFile.println(",");
  }
  myFile.close();
}


void logginAltitud() {

  float p = bmp.readPressure() / 100;

  //Compruebe si alguna lectura falló y salga antes (para volver a intentarlo).
  if  (isnan(p)) {
    Serial.println(F("Error al leer Presion del sensor BMP!"));
    return;
  }

  //debugging purposes
  Serial.print(F("Altitud: "));
  Serial.print(bmp.readAltitude(p));
  Serial.println(F("M."));


  myFile = SD.open("DATA.txt", FILE_WRITE);
  if (myFile) {
    Serial.println(F("abrir con éxito"));
    myFile.print(bmp.readAltitude(p));
    myFile.println(",");
  }
  myFile.close();
}


void logginTemperaturaBMP() {

  float t2 = bmp.readTemperature() / 100;

  //Compruebe si alguna lectura falló y salga antes (para volver a intentarlo).
  if  (isnan(t2)) {
    Serial.println(F("Error al leer Temperatura del sensor BMP!"));
    return;
  }

  //debugging purposes
  Serial.print(F("TemperaturaBMP: "));
  Serial.print(t2);
  Serial.println(F(" °C"));


  myFile = SD.open("DATA.txt", FILE_WRITE);
  if (myFile) {
    Serial.println(F("abrir con éxito"));
    myFile.print(t2);
    myFile.println(",");
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

  //debugging purposes
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
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////

void loop() {
  loggingTiempo();
    delay(1000);
  loggingTemperaturaDHT();
    delay(1000);
  loggingHumedad();
    delay(1000);
  logginPresion();
    delay(1000);
  logginAltitud();
    delay(1000);
  logginTemperaturaBMP();
    delay(1000);
  logginLuz();
    delay(1000);
  delay(5000);
}
