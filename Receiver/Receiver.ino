//#include <SPI.h>         
#include "nRF24L01.h"     
#include "RF24.h"     

RF24 radio(9,10);

//byte address[][6] = {"1Node","2Node","3Node","4Node","5Node","6Node"};  

const uint64_t pipe = 0xE8E8F0F0E1LL;

void setup(){
  Serial.begin(9600); 

  radioRSetUp();
  
  
}

int DATA[4] = {512, 512, 0, 0};

void loop() {

    if ( radio.available() ) radio.read(&DATA, sizeof(DATA)); else { dataFuckUp(); };
      
}

void radioRSetUp() {

  radio.begin();
  radio.setAutoAck(0);        // можно сделать проверку отправки        
  radio.setRetries(0,15);     
  //radio.enableAckPayload();    
  radio.setPayloadSize(32);     

  radio.openReadingPipe(1,pipe);      
  radio.setChannel(0x60); // можно поменять, и можно искать оптимальный в nrf_listen_air  

  radio.setPALevel (RF24_PA_MAX); 
  radio.setDataRate (RF24_1MBPS); //надо с этим поиграться, пока поставил среднее
  
  radio.powerUp(); 
  radio.startListening();  

}

void dataFuckUp() {

  //здесь можно зажигать красный светодиод ошибки. Вообще лучше эту функцию вызывать при потере связи

  for (int i = 0; i < 2; i++) {
    DATA[i] = 512;
    DATA[i+2] = 0;
  }

}
