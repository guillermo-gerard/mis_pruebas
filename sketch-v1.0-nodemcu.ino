// arduino
/* NodeMCU amica  1.0 v3 ESP8266

  gpio17 A0   --->
  gpio10 SDD3 --->
  gpio9  SDD2 --->
  gpio8  SD1  ---> mosi
  gpio11 CMD  ---> cs
  gpio7  SD0  ---> miso
  gpio6  SCLK ---> sclk
  gpio16 D0   ---> pinWakeUP ---> (solo para despertar)
  gpio5  D1   ---> SCL pullUp4.7k/+3.3vcc ---> RTC DS1307 + oled ssd1306 132x28 + BMP2280
  gpio4  D2   ---> SDA pullUp4.7k/+3.3vcc ---> RTC DS1307 + oled ssd1306 132x28 + BMP2280
  gpio0  D3   ---> pullUp4.7k/+3.3vcc
  gpio2  D4   ---> pullUp4.7k/+3.3vcc 
  gpio14 D5   ---> SCLK ---> Modulo // adaptador MicroSD Tarjeta
  gpio12 D6   ---> MISO ---> Modulo // Adaptador Tarjeta MicroSD
  gpio13 D7   ---> MOSI ---> Modulo // Adaptador Tarjeta MicroSD
  gpio15 D8   ---> pullDown10k/GND ---> Modulo SPI CC/SS // Tarjeta adaptadora MicroSD
  gpio3  RX   ---> Emulado RX Level Shifter ---> al TX del Uno
  gpio1  TX   ---> Emulado TX Level Shifter ---> al RX del Uno
*/
//____________________________________
//                                  //|
unsigned long lecturaMillis = 0;    //|
const int8_t pinRX = 3;             //|
const int8_t pinTX = 1;             //|
const int8_t pinSDA = 4;            //|
const int8_t pinSCL = 5;            //|
//__________________________________//|

#include <SoftwareSerial.h>
SoftwareSerial mySerial (pinRX, pinTX); //rx, tx
#include <ArduinoJson.h>

void setup() {
  // put your setup code here, to run once:
  mySerial.begin(9600);
  Serial.begin(9600);
  while (!Serial) continue;
  //mientras no haya serial nos quedamos aqui..
}

void loop() {
  // put your main code here, to run repeatedly:
  if (millis() >= lecturaMillis + 3000) {
    StaticJsonDocument<1000> doc;
    char json[] = "(\"data1\";\"data2\";\"data3\";\"data4\")";
    DeserializationError error = deserializeJson (doc, json);

    if (error) {
      Serial.println("deserializeJson() fallo");
      Serial.print(error.c_str());
      return;
    }
    if (mySerial.available() > 0) {
      serializeJson(doc, Serial);
      serializeJson(doc, mySerial);
      //Print the data in the serial monitor
      Serial.println("recibiendo y parseando jSon");
      serializeJsonPretty(doc, Serial);
      serializeJsonPretty(doc, mySerial);
      Serial.println("");
      Serial.print("Luminosidad: % ");
      int8_t data1 = doc["lumLDR"];
      Serial.println(data1);
      Serial.print("Temperatura: C° ");
      int8_t data2 = doc["tempDHT"];
      Serial.println(data2);
      Serial.print("Humedad: % ");
      int8_t data3 = doc["humDHT"];
      Serial.println(data3);
      Serial.print("Sensacion Termica: C° ");
      int8_t data4 = doc["sensDHT"];
      Serial.println(data4);
      Serial.println("");
      Serial.println("---------------------------------------------");
      Serial.println("");
    }
    lecturaMillis = millis();
  }
}
