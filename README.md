# mi_prueba
arduino

a mica NodeMCU 1.0 v3 ESP8266

  A0          --->                      Modulo LDR out  
  D1//gpio5   --->  SCL            ---> RTC DS1307 + oled ssd1306 132x28 + BMP2280  
  D2//gpio4   --->  SDA            ---> RTC DS1307 + oled ssd1306 132x28 + BMP2280  
  D3//gpio    --->  res4.7k a vcc+  
  D4//gpio2   --->  res4.7k a vcc+ ---> DHT out  
  D5//gpio14  --->  SCLK           ---> Modulo // adaptador MicroSD Card  
  D6//gpio12  --->  MISO           ---> Modulo // adaptador MicroSD Card  
  D7//gpio13  --->  MOSI           ---> Modulo // adaptador MicroSD Card  
  D8//gpio15  --->  res10k a gnd   ---> CC/SS  Modulo // adaptador MicroSD Card  
  
  
ESP8266-01s  
  
 TX--->RX  
 RX--->TX  
 chpd/en-->pullUp4.7k/+3.3vcc  
 GPIO 0--->pullDown4.7k/GND  
 GPIO 2--->pullUp/+3.3vcc  
 vcc--->vcc  
 gnd--->gnd  
 (luego de la programacion solo son necesarios vcc & gnd)  
  
  
 
 
