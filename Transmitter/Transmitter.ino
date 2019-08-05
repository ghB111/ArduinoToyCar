//#include <SPI.h>         
#include "nRF24L01.h"     
#include "RF24.h"         

RF24 radio(9, 10); 

//byte address[][6] = {"1Node", "2Node", "3Node", "4Node", "5Node", "6Node"};

const uint64_t pipe = 0xE8E8F0F0E1LL;

void setup() {
  Serial.begin(9600); 

  radio.begin(); 
  radio.setAutoAck(0);        // можно сделать проверку отправки
  radio.setRetries(0, 15);    
  //radio.enableAckPayload();    
  radio.setPayloadSize(32);     

  radio.openWritingPipe(0xE8E8F0F0E1LL);   
  radio.setChannel(0x60);  // можно поменять

  radio.setPALevel (RF24_PA_MAX); 
  radio.setDataRate (RF24_1MBPS); //надо с этим поиграться, пока поставил среднее

  radio.powerUp(); 
  radio.stopListening();  
}

void loop() {
  Serial.print("Sent: "); Serial.println(counter);
  radio.write(&counter, sizeof(counter));
  counter++;
  delay(10);
}
