#include <SPI.h>
#include "RF24.h"

const int Button = 2;
const int LED =  3;
const int Piezo = 4;
int buttonState = 0;
int Transmit = 1;

int sound = 400; //Pitch for the piezo
int count = 500; //Time for the countdown

bool radioNumber = 0;
RF24 radio(9,10);
byte addresses[][6] = {"1Node","2Node"};

void setup() {
  Serial.begin(115200);
  pinMode(LED, OUTPUT);
  pinMode(Button, INPUT);
  radio.begin();
  
  radio.setChannel(108);
  radio.setPALevel(RF24_PA_LOW);
  radio.openWritingPipe(addresses[0]); 
  delay(500);
}

void loop() {
  buttonState = digitalRead(Button);

  if (buttonState == HIGH){
    countdown(); 
  } 
  
  else{
    digitalWrite(LED, LOW);
  }
}

void countdown(){
  for (int i = 1; i <= 5; i++)
  {
    digitalWrite(LED, HIGH);
    tone(Piezo, sound);
    delay(count);
    digitalWrite(LED, LOW);
    noTone(Piezo);
    delay(count);
  }
  radio.write(&Transmit, sizeof(Transmit));
}


