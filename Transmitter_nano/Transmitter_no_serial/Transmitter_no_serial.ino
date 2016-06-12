#include <SPI.h>

#include <nRF24L01.h>
#include <RF24_config.h>
#include <printf.h>
#include <RF24.h>
#include <dht.h>



// Настройка пинов для CE, CSN
RF24 radio(9, 10);
 
 const uint64_t pipes[2] = { 0xABCDABCD71LL, 0x544d52687CLL };   // Radio pipe addresses for the 2 nodes to communicate.
 
short int data[3];

DHT sensor = DHT();


// ещё объявления ...
 
void setup(void) {
    analogWrite(6, 170);
  // важно, чтобы было 57600 бауд
 // Serial.begin(57600);
  radio.begin();
 
  radio.setDataRate(RF24_1MBPS);
  radio.setPALevel(RF24_PA_MAX);
  radio.setChannel(1);
  radio.setAutoAck(1);
  radio.setRetries(2,15);
  radio.setCRCLength(RF24_CRC_8);
 
  radio.openWritingPipe(pipes[0]); 
  radio.openReadingPipe(1,pipes[1]); 
  //radio.startListening();
  //radio.printDetails();
   

 radio.powerUp();
  sensor.attach(A0);
delay(2000);

}
 
void loop(void) {

sensor.update();
  
switch (sensor.getLastError())
    {
        case DHT_ERROR_OK:
            data[0]=sensor.getTemperatureInt();
            data[1]=sensor.getHumidityInt();  
             break;
        case DHT_ERROR_START_FAILED_1:
         //   Serial.println("Error: start failed (stage 1)");
            break;
        case DHT_ERROR_START_FAILED_2:
         //   Serial.println("Error: start failed (stage 2)");
            break;
        case DHT_ERROR_READ_TIMEOUT:
         //   Serial.println("Error: read timeout");
            break;
        case DHT_ERROR_CHECKSUM_FAILURE:
          //  Serial.println("Error: checksum error");
            break;
    }
  
  // Send to RPi
    radio.write( &data,3);
    //if ( radio.write( &data,3) ) {
    //   Serial.println("Send successful\n\r"); 
    //   for(int i=0; i<2; i++){
    //       Serial.println(data[i]);
     //     }
      // Serial.println("1:");
   // } else {
   //    Serial.println("Send failed\n\r");
   //    //Serial.println("0:");  
   // }
 
     delay(1000);    
  }
