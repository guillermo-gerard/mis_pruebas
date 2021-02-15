//#include <Arduino_FreeRTOS.h>//5mentariosXahora
#include <Wire.h>         // incluir libreria para I2C
#include <SPI.h>           // incluir libreria para SPI
#include <RTClib.h>      // incluir libreria para RTC
#include <SD.h>         // incluir libreria para SD
const int chipSelect = 4; // // chip esclavo 1 SD puerto SPI en pin 4
#include <U8g2lib.h>   // incluir libreria Oled
#include <DHT.h> // incluir libreria DHT sensor
#define DHTPIN 2 // sensor en pin 2
#define DHTTYPE DHT11   // DHT 11 
//#define DHTTYPE DHT22   // DHT 22  (AM2302)
//#define DHTTYPE DHT21   // DHT 21 (AM2301)
#include <Adafruit_Sensor.h>  // incluir libreria sensor BMP280
#include <Adafruit_BMP280.h>
Adafruit_BMP280 bmp;
DHT dht(DHTPIN, DHTTYPE);
RTC_DS1307 rtc;     // llama al RTC_DS1307 como rtc

U8G2_SSD1306_128X32_UNIVISION_1_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);  //driver especifico para tipo de pantalla segun libreria en uso
unsigned long tiempo; //variable donde almacenar tiempo desde encendido
byte contador = 0; //por si quiero contar algo...
byte variable = 0; //por si quiero llevar algo a 0...

File myFile;// buscar encontrar o crear archivo en raiz sd

void setup () {
  tiempo = micros;
  delayMicroseconds(1000000);   //esperar tiempo micro-segundos = 1 segundo
  Serial.begin(9600); {}   // inicializa comunicacion serial a 9600 bps
  Serial.println("Iniciando:");
  
  if (!rtc.begin()) {       // sino inicia el modulo
    Serial.println(F("Modulo RTC fallo"));  // muestra mensaje de error
    return;         // se sale regresando el programa
  }
  Serial.print(F("Modulo RTC iniciado correctamente en:")); // muestra mensaje de confirmacion y tiempo trascurrido menos espera inicial
  Serial.println(tiempo - 1000000);
  
  if (!SD.begin(chipSelect)) {       // sino inicia el modulo
    Serial.println(F("Modulo SD fallo"));  // muestra mensaje de error
    return;         // se sale regresando el programa
  }
  Serial.print(F("Modulo SD iniciado correctamente en:")); // muestra mensaje confirmacion y tiempo trascurrido menos espera inicial
  Serial.println(tiempo - 1000000);
  myFile=SD.open("DATA.txt", FILE_WRITE); //abrir sd, buscar crear archivo y abrir modo escritura
    if (myFile) { //para tratar el archivo
    Serial.println("Archivo abierto correctamente");
    myFile.println("Fecha, Hora, TemperaturaDHT ºC, Humedad %, Presion hPa, Altitud, TemperaturaBMP"); //imprimir en el archivo los siguientes encabezados.
  myFile.close(); //cerrmos el archivo.
}

if (!u8g2.begin()) {       // sino inicia el modulo
    Serial.println(F("Modulo Display fallo"));  // muestra mensaje de error
    return;         // se sale regresando el programa
  }
  Serial.print(F("Modulo Display iniciado correctamente en:")); // muestra mensaje confirmacion y tiempo trascurrido menos espera inicial
  Serial.println(tiempo - 1000000);
  Serial.print(tiempo);
}

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
    Serial.println("Error al leer temperatura del sensor DHT!");
    return;
  }
  
  //debugging purposes
  Serial.print("TemperaturaDHT: "); 
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
  
  byte h = dht.readHumidity();
  
  //Compruebe si alguna lectura falló y salga antes (para volver a intentarlo).
  if  (isnan(h) /*|| isnan(f)*/) {
    Serial.println("Error al leer humedad del sensor DHT!");
    return;
  }
  
  //debugging purposes
  Serial.print("Humedad: "); 
  Serial.print(h);
  Serial.println("%");

  
  myFile = SD.open("DATA.txt", FILE_WRITE);
  if (myFile) {
    Serial.println("abrir con éxito");
    myFile.print(h);
    myFile.println(",");
  }
  myFile.close();
}

void logginPresion() {
  
 float p = bmp.readPressure()/100;
  
  //Compruebe si alguna lectura falló y salga antes (para volver a intentarlo).
  if  (isnan(p)){
    Serial.println("Error al leer Presion del sensor BMP!");
    return;
  }
  
  //debugging purposes
  Serial.print("Presion: "); 
  Serial.print(p);
  Serial.println("hPa");

  
  myFile = SD.open("DATA.txt", FILE_WRITE);
  if (myFile) {
    Serial.println("abrir con éxito");
    myFile.print(p);
    myFile.println(",");
  }
  myFile.close();
}

void logginAltitud() {
    
 float p = bmp.readPressure()/100;
  
  //Compruebe si alguna lectura falló y salga antes (para volver a intentarlo).
  if  (isnan(p)){
    Serial.println("Error al leer Presion del sensor BMP!");
    return;
  }
  
  //debugging purposes
  Serial.print("Altitud: "); 
  Serial.print(bmp.readAltitude(p));
  Serial.println("M.");

  
  myFile = SD.open("DATA.txt", FILE_WRITE);
  if (myFile) {
    Serial.println("abrir con éxito");
    myFile.print(bmp.readAltitude(p));
    myFile.println(",");
  }
  myFile.close();
}

void logginTemperaturaBMP() {
  
 float t2 = bmp.readTemperature()/100;
  
  //Compruebe si alguna lectura falló y salga antes (para volver a intentarlo).
  if  (isnan(t2)){
    Serial.println("Error al leer Temperatura del sensor BMP!");
    return;
  }
  
  //debugging purposes
  Serial.print("TemperaturaBMP: "); 
  Serial.print(t2);
  Serial.println(" °C");

  
  myFile = SD.open("DATA.txt", FILE_WRITE);
  if (myFile) {
    Serial.println("abrir con éxito");
    myFile.print(t2);
    myFile.println(",");
  }
  myFile.close();
}

void loop() {
  loggingTiempo();
  loggingTemperaturaDHT();
  loggingHumedad();
  logginPresion();
  logginAltitud();
  logginTemperaturaBMP();
  
  delay(5000);
}
