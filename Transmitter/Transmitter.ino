//#include <SPI.h>         
#include "nRF24L01.h"     
#include "RF24.h" 

/*
 *#define _DEBUGGING
 */


#define thrashHold 15
#define tH thrashHold


#define thrashHold 15
#define tH thrashHold


RF24 radio(9, 10); 

//byte address[][6] = {"1Node", "2Node", "3Node", "4Node", "5Node", "6Node"};

const uint64_t pipe = 0xE8E8F0F0E1LL;

void setup() {
  #ifdef _DEBUGGING
  Serial.begin(9600); 
  #endif
  
  //pinMode(2, INPUT);
  //pinMode(3, INPUT);

  radioTSetUp();
    
}

//int DATA[4] = {512, 512, 0, 1}; //последняя единица для дебаггинга

uint16_t DATA[2] = {512, 512};
             /* 0 - Y - Speed
             ** 1 - X - Rotation Angle
             ** 2 - switch_1 - Beeping
             ** 3 - switch_2 - Anything else
             */


bool sendSuccess = 0;

void loop() {

  for (int i = 0; i < 2; i ++) {
    DATA[i] = analogRead(i);
    //DATA[i+2] = digitalRead(i); 
  }
  for (int i = 0; i <= 1; i++) {
    if ( DATA[i] > 512 - tH && DATA[i] < 512 + tH) DATA[i] = 512;
  }

  sendSuccess = radio.write(&DATA, sizeof(DATA));
  #ifdef _DEBUGGING
    if ( sendSuccess ) Serial.print("Send succesfully: ");
    else Serial.println("SEND FAILED: ");
 
    printData();
  #endif 
 
}



void radioTSetUp() {

  radio.begin(); 
  radio.setAutoAck(0);        // можно сделать проверку отправки
  radio.setRetries(0, 15);    
  //radio.enableAckPayload();    
  radio.setPayloadSize(32);     

  radio.openWritingPipe(pipe);   
  radio.setChannel(0x60);  // можно поменять, и можно искать оптимальный в nrf_listen_air  

  radio.setPALevel (RF24_PA_MAX); 
  radio.setDataRate (RF24_1MBPS); //надо с этим поиграться, пока поставил среднее

  radio.powerUp(); 
  radio.stopListening();

}
#ifdef _DEBUGGING
void printData() {

  //for (int i = 0; i < 4; i++) {
  for (int i = 0; i < 2; i++) {  
  
    Serial.print(DATA[i]);
    Serial.print(" ");
  
  }
  Serial.println();
}
#endif
