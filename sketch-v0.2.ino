


#include <Wire.h> // incluye libreria de bus I2C
#include <SPI.h> //for the SD card module
#include <SD.h> // for the SD card
#include <DHT.h> // for the DHT sensor
#include <RTClib.h> // for the RTC
#include <Adafruit_Sensor.h>  // incluye librerias para sensor BMP280
#include <Adafruit_BMP280.h>

Adafruit_BMP280 bmp;

//define DHT pin
#define DHTPIN 2     // what pin we're connected to

// uncomment whatever type you're using
#define DHTTYPE DHT11   // DHT 11 
//#define DHTTYPE DHT22   // DHT 22  (AM2302)
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

// initialize DHT sensor for normal 16mhz Arduino
DHT dht(DHTPIN, DHTTYPE);

// change this to match your SD shield or module;
// Arduino Ethernet shield and modules: pin 4
// Data loggin SD shields and modules: pin 10
// Sparkfun SD shield: pin 8
const int chipSelect = 4; 

// Create a file to store the data
File myFile;

// RTC
RTC_DS1307 rtc;

void setup() {
  Serial.println("Iniciando:");
  
  //initializing the DHT sensor
  dht.begin();
  bmp.begin();
    //if (!bmp.begin()) { // si falla la comunicacion con el sensor mostrar
    //Serial.println("BMP no encontrado!"); // texto y detener flujo del programa
    //while (1); // mediante bucle infinito
  //}  

  //initializing Serial monitor
  Serial.begin(9600);
  
  // setup for the RTC
  while(!Serial); // for Leonardo/Micro/Zero
    if(! rtc.begin()) {
      Serial.println("No se pudo encontrar RTC");
      while (1);
    }
    else {
      // la siguiente línea establece el RTC en la fecha y hora en que se compiló este boceto
      rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    }
    if(! rtc.isrunning()) {
      Serial.println("RTC NO se está ejecutando!");
    }
    
  // setup for the SD card
  Serial.print("Inicializando tarjeta SD...");

  if(!SD.begin(chipSelect)) {
    Serial.println("inicialización fallida!");
    return;
  }
  Serial.println("inicialización realizada");
    
  //open file
  myFile=SD.open("DATA.txt", FILE_WRITE);

  // if the file opened ok, write to it:
  if (myFile) {
    Serial.println("Archivo abierto correctamente");
    // print the headings for our data
    myFile.println("Fecha, Hora, TemperaturaDHT ºC, Humedad %, Presion hPa, Altitud, TemperaturaBMP");
  }
  myFile.close();
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
