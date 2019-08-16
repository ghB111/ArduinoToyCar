//#include <SPI.h>         
#include "nRF24L01.h"     
#include "RF24.h" 

#include <Servo.h>

/*
 *#define _DEBUGGING
 */


Servo servo;
RF24 radio(9,10);

//byte address[][6] = {"1Node","2Node","3Node","4Node","5Node","6Node"};  

const uint64_t pipe = 0xE8E8F0F0E1LL;

void setup(){
  #ifdef _DEBUGGING
  Serial.begin(9600); 
  #endif
  radioRSetUp();
  
  servo.attach(6);

  for (int i = 2; i <= 4; i++) {
    pinMode(i, OUTPUT);
  }
}

//int DATA[4] = {512, 512, 0, 0};

uint16_t DATA[2] = {512, 512};

int angle = 90;
//bool DoIBeep = 0;
int driveSpeed = 512;

unsigned long now;
unsigned long lastDataReceivedTime;

const uint16_t alarm = 1000;

void loop() {
    
    now = millis();
    
    if ( radio.available() ) {
      #ifdef _DEBUGGING
        radio.read(&DATA, sizeof(DATA));
        Serial.print("Received: ");
        printData();
      #endif
      lastDataReceivedTime = now;
    } else {
      if ( lastDataReceivedTime + alarm <= now ) { resetData(); }
    }

    driveSpeed = DATA[0];
    drive(driveSpeed);
    
    angle = 90 + map(DATA[1], 0, 1023, -30, 30);
    servo.write(angle);

    //DoIBeep = DATA[2];
    //if (DoIBeep) { beep(); } //бибикалку пока не сделали
      
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

void resetData() {

  //здесь можно зажигать красный светодиод ошибки.

  for (int i = 0; i < 2; i++) {
    DATA[i] = 512;
    //DATA[i+2] = 0;
  }

}

//void beep() {
  //это я оставил на всякий случай
//}

void drive(int vel) {

  if ( vel >= 512 ) {
    digitalWrite(2, HIGH); 
    digitalWrite(4, LOW); 
    analogWrite(3, map(vel, 512, 1023, 0, 255) ); 
  }
  else {
    digitalWrite(2, LOW); 
    digitalWrite(4, HIGH);
    analogWrite(3, map(vel, 511, 0, 0, 255) );
  }

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
