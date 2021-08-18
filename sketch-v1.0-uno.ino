//  arduino
/*    UNO

       D0     ---> RX ---> mientras se usen impedira programacion
       D1     ---> TX ---> mientras se usen impedira programacion
       D2     ---> Salida modulo LDR
       D3     --->
       D4     ---> 
       D5     ---> Emulado RX Level Shifter ---> al TX del NodeMcu
       D6     ---> Emulado TX Level Shifter ---> al RX del NodeMcu 
       D7     ---> Modulo DHT
       D8     --->
       D9     ---> 
       D10    ---> SPI CC/SS 
       D11    ---> MOSI ---> 
       D12    ---> MISO ---> 
       D13    ---> SCLK ---> 
       D14 A0 ---> Reservado para el proyecto
       D15 A1 ---> Reservado para el proyecto
       D16 A2 ---> Reservado para el proyecto
       D17 A3 ---> Reservado para el proyecto
       D18 A4 ---> Reservado para el proyecto
       D19 A5 ---> Reservado para el proyecto
       vcc    ---> vcc
       gnd    ---> gnd
*/

//____________________________________
//                                  //|
unsigned long lecturaMillis = 0;    //|
const int8_t pinRX = 5;             //|
const int8_t pinTX = 6;             //|
//const int8_t pinSDA = 4;          //|
//const int8_t pinSCL = 5;          //|
//__________________________________//|

#include <Wire.h>
#include <SoftwareSerial.h>
SoftwareSerial mySerial (pinRX, pinTX); //rx, tx
#include <ArduinoJson.h>

#define pinLDR 4
uint8_t lumLDR;

#include <DHT.h> // incluir libreria DHT sensor
#define DHTPIN 7 // sensor en pin D7
#define DHTTYPE DHT11   // DHT11 DHT21 DHT22
DHT dht(DHTPIN, DHTTYPE);  // crea objeto
byte tempDHT, humDHT, sensDHT;

  void getLightLDR() {
    pinMode (pinLDR, INPUT);
    lumLDR = map (analogRead(pinLDR), 0, 1024, 100, 0);

    //Compruebe si alguna lectura fallo y salga antes (para volver a intentarlo).
    if  (isnan(lumLDR)) {
      Serial.println(F("Error al leer Luminosidad!"));
      return;
    }
    Serial.print(F("Luminosidad: %"));
    Serial.print(lumLDR);
    Serial.println(F(" Luz"));
  }
  void getTempDHT() {

    tempDHT = dht.readTemperature(); // Leer la temperatura en grados Celsius
    // float f = dht.readTemperature(true);  //Leer la temperatura como Fahrenheit

    //Compruebe si alguna lectura fallo y salga antes (para volver a intentarlo).
    if  (isnan(tempDHT) /*|| isnan(f)*/) {
      Serial.println(F("Error al leer temperatura del sensor DHT!"));
      return;
    }

    //consola log
    Serial.print(F("Temperatura DHT: "));
    Serial.print(tempDHT);
    Serial.println(" °C");
    //Serial.print(f);
    //Serial.println(" *F\tempDHT");
  }
  void getHumDHT() {

    humDHT = dht.readHumidity();

    //Compruebe si alguna lectura fallo y salga antes (para volver a intentarlo).
    if  (isnan(humDHT)) {
      Serial.println(F("Error al leer humedad del sensor DHT!"));
      return;
    }

    //consola log
    Serial.print(F("Humedad: "));
    Serial.print(humDHT);
    Serial.println(F("%"));
  }
  void getSensDHT() {

    sensDHT = dht.computeHeatIndex(); // Leer la temperatura en grados Celsius
    // float tempDHT = dht.computeHeatIndex(true);  //Leer la temperatura como Fahrenheit

    //Compruebe si alguna lectura fallo y salga antes (para volver a intentarlo).
    if  (isnan(sensDHT) /*|| isnan(f)*/) {
      Serial.println(F("Error al leer temperatura del sensor DHT!"));
      return;
    }

    //consola log
    Serial.print(F("Sensasion Termica DHT: "));
    Serial.print(sensDHT);
    Serial.println(" °C");
    //Serial.print(f);
    //Serial.println(" *F\tempDHT");
  }

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  mySerial.begin(9600);
  dht.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  if (millis() >= lecturaMillis + 3000) {
    StaticJsonDocument<1000> doc;
    JsonArray data = doc.createNestedArray("data");
    serializeJson(doc, Serial);
    serializeJson(doc, mySerial);

    // Si el LDR no está conectado al pin correcto o si no
    // funciona, no se enviarán datos
    getLightLDR();
    getTempDHT();
    getHumDHT();
    getSensDHT();

    doc["lumLDR"] = lumLDR;
    doc["tempDHT"] = tempDHT;
    doc["humDHT"] = humDHT;
    doc["sensDHT"] = sensDHT;

    if (mySerial.available() > 0) {
      serializeJsonPretty (doc, Serial);
      serializeJsonPretty (doc, mySerial);
    }
    lecturaMillis = millis();
  }
}
