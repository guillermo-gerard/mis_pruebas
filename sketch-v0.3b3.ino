/*
 * a mica NodeMCU 1.0 v3ESP8266
 * A0 ---> Salida módulo LDR
 * D1 // gpio5 ---> SCL ---> RTC DS1307 + oled ssd1306 132x28 + BMP2280
 * D2 // gpio4 ---> SDA ---> RTC DS1307 + oled ssd1306 132x28 + BMP2280
 * D3 // gpio ---> res4.7k a vcc +
 * D4 // gpio2 ---> res4.7k a vcc + ---> DHT out
 * D5 // gpio14 ---> SCLK ---> Modulo // adaptador MicroSD Tarjeta
 * D6 // gpio12 ---> MISO ---> Modulo // Adaptador Tarjeta MicroSD
 * D7 // gpio13 ---> MOSI ---> Modulo // Adaptador Tarjeta MicroSD
 * D8 // gpio15 ---> res10k a gnd ---> Módulo CC / SS // Tarjeta adaptadora MicroSD
*/

#include <Wire.h>         // incluir libreria para I2C
#include <SPI.h>           // incluir libreria para SPI

#include <RTClib.h>      // incluir libreria para RTC
RTC_DS1307 rtc;     // llama al RTC_DS1307 como rtc
//#define RTC_I2C_ADDRESS 0x68 // 
//#define RTC_EEPROM_I2C_ADDRESS 0x50 //

#include <SD.h>         // incluir libreria para SD
const int chipSelect = D8; // // chip esclavo SD puerto SPI - en el uno pin 4 - en el esp8266 nodemcu 1.0 v3 pin D8 gpio15
File myFile;// buscar encontrar o crear archivo en raiz sd

#include <Adafruit_GFX.h>   // libreria para pantallas graficas
#include <Adafruit_SSD1306.h>   // libreria para controlador SSD1306
#define OLED_I2C_ADDRESS 0x3C  //
#define OLED_RESET -1      // necesario por la libreria pero no usado
#define ANCHO 128     // reemplaza ocurrencia de ANCHO por 128
#define ALTO 32       // reemplaza ocurrencia de ALTO por 32
Adafruit_SSD1306 oled(ANCHO, ALTO, &Wire, OLED_RESET);  // crea objeto

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
  Serial.begin(9600); {}   // inicializa comunicacion serial a 9600 bps
  Serial.println(F("Iniciando:"));
  Wire.begin(D2 , D1);   // configurar serial i2c en pines(sda, scl)
  delay (1000);
  
  if (!rtc.begin()) {       // sino inicializa modulo con direccion I2C definida previamente
    Serial.println(F("Modulo RTC fallo"));  // muestra mensaje de error
    return;         // se sale regresando el programa
  }
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  Serial.print(F("Modulo RTC iniciado correctamente")); // muestra mensaje de confirmacion

  if (!SD.begin(chipSelect)) {       // sino inicia el modulo
    Serial.println(F("Modulo SD fallo"));  // muestra mensaje de error
    return;         // se sale regresando el programa
  }
  Serial.print(F("Modulo SD iniciado correctamente")); // muestra mensaje confirmacion
  myFile = SD.open("DATA.txt", FILE_WRITE); //abrir sd, buscar crear archivo y abrir modo escritura
  if (myFile) { //para tratar el archivo
    Serial.println(F("Archivo abierto correctamente"));
    myFile.println("Fecha, Hora, Temperatura DHT ºC, Temperatura BMP ºC, Humedad %, Presion hPa, Altitud M, Luminosidad"); //imprimir en el archivo los siguientes encabezados.
    myFile.close(); //cerrmos el archivo.
  }

if (!oled.begin(SSD1306_SWITCHCAPVCC, OLED_I2C_ADDRESS)); { // sino inicializa modulo con direccion I2C definida previamente
    Serial.println(F("Modulo Display fallo"));  // muestra mensaje de error
    return;         // se sale regresando el programa
   }
   Serial.print(F("Modulo Display iniciado correctamente"));// muestra mensaje confirmacion
   
  if (!bmp.begin(BMP280_I2C_ADDRESS)) {       // sino inicializa modulo con direccion I2C definida previamente
    Serial.println(F("Modulo BMP fallo"));  // muestra mensaje de error
    return;         // se sale regresando el programa
  }
  Serial.print(F("Modulo BMP iniciado correctamente")); // muestra mensaje de confirmacion

  /* if (!dht.begin()) {       // sino inicializa modulo con direccion I2C definida previamente
      Serial.println(F("Modulo DHT fallo"));  // muestra mensaje de error
      return;         // se sale regresando el programa
    }
  */
  dht.begin();
  Serial.println(F("Modulo DHT iniciado correctamente")); // muestra mensaje de confirmacion y tiempo trascurrido menos espera inicial
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////

void formatoPantalla() {
  oled.clearDisplay();      // limpia pantalla 
  oled.drawRect(0, 0, 64, 11, WHITE); // dibuja rectangulo
  oled.drawRect(64, 0, 64, 11, WHITE); // dibuja rectangulo
  oled.drawRect(0, 10, 64, 11, WHITE); // dibuja rectangulo
  oled.drawRect(64, 10, 64, 11, WHITE); // dibuja rectangulo
  oled.drawRect(0, 20, 64, 11, WHITE); // dibuja rectangulo
  oled.drawRect(64, 20, 64, 11, WHITE); // dibuja rectangulo
/*  
  oled.setCursor(5, 3);   // ubica cursor en coordenadas 28,34
  oled.setTextSize(2);      // establece tamano de texto en 2
  oled.setTextColor(WHITE);   // establece color al unico disponible (pantalla monocromo)
  oled.print("--->FC<---");     // escribe texto
  oled.setCursor(12, 15);   // ubica cursor en coordenadas 28,34
  oled.setTextSize(2);      // establece tamano de texto en 2
  oled.setTextColor(WHITE);   // establece color al unico disponible (pantalla monocromo)
  oled.print("servicios");     // escribe texto
*/ 
  oled.display();     // muestra en pantalla todo lo establecido anteriormente
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
  formatoPantalla();
  delay(1000);
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
