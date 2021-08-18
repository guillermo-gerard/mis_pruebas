# Mis Pruebas  
  (esto no tiene mucho sentido)  
  Pin Map  
  
  
	arduino  UNO  
  
		   
		   D0     ---> RX ---> mientras se usen impedira programacion  
		   D1     ---> TX ---> mientras se usen impedira programacion  
		   D2     --->   
		   D3     --->
		   D4     --->
		   D5     --->
		   D6     --->
		   D7     --->  
		   D8     --->
		   D9     ---> 
		   D10    ---> Módulo SPI CC/SS //  
		   D11    ---> MOSI ---> Modulo //  
		   D12    ---> MISO ---> Modulo //  
		   D13    ---> SCLK ---> Modulo //   
		   D14 A0  
		   D15 A1
		   D16 A2 
		   D17 A3 
		   D18 A4 ---> SDA pullUp4.7k/+3.3vcc --->  
		   D19 A5 ---> SCL pullUp4.7k/+3.3vcc --->  
		   vcc    ---> vcc  
		   gnd    ---> gnd  
		  
		  
	NodeMCU amica  1.0 v3 ESP8266  
		  
		    gpio17 A0   --->   
  		  gpio10 SDD3 --->
  		  gpio9  SDD2 --->
  		  gpio8  SD1  ---> mosi
  		  gpio11 CMD  ---> cs
  		  gpio7  SD0  ---> miso
  		  gpio6  SCLK ---> sclk
  		  gpio16 D0   ---> pinWakeUP ---> (solo para despertar)  
  		  gpio5  D1   ---> SCL pullUp4.7k/+3.3vcc 
  		  gpio4  D2   ---> SDA pullUp4.7k/+3.3vcc 
  		  gpio0  D3   ---> pullUp4.7k/+3.3vcc
  		  gpio2  D4   ---> pullUp4.7k/+3.3vcc ---> 
  		  gpio14 D5   ---> SCLK ---> Modulo //  
  		  gpio12 D6   ---> MISO ---> Modulo //  
  		  gpio13 D7   ---> MOSI ---> Modulo //  
  		  gpio15 D8   ---> pullDown4.7k/GND ---> Modulo SPI CC/SS //  
  		  gpio3  RX   --->
  		  gpio1  TX   --->  
		  
		  
	ESP8266-01s  
		  
		  gpio 0 gpio0   ---> pullUp4.7k/+3.3vcc ... reemplazar por pullDown10k/GND para programar por ttl  
		  gpio 1 TX/SDA  ---> a RX para programar por ttl  // a TX para programar desde otro micro  
		  gpio 2 gpio2   ---> pullUp4.7k/+3.3vcc  
		  gpio 3 RX/SCL  ---> a TX para programar pot ttl  // a RX para progroamar dsde otro micro  
		  EN chpd        ---> pullUp4.7k/+3.3vcc  
		  vcc            ---> vcc  
		  gnd            ---> gnd  
   
		  (luego de la programacion solo son necesarios vcc & gnd)  
		  
		  
		  
