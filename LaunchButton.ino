//Have the go go power rangers sound effect play, when the device is turned on. 
#include <Wire.h>
#include <SPI.h>
#include "RF24.h"
#include <Adafruit_NeoPixel.h>
#include "Adafruit_LEDBackpack.h"
#include "Adafruit_GFX.h"
#include "pitches.h"
#define PIN 9
Adafruit_NeoPixel pixel = Adafruit_NeoPixel(1, PIN, NEO_GRB + NEO_KHZ800);

const int RedButton = 2;
const int GreenButton = 5;
const int RED_LED = 3;
const int GREEN_LED = 6;
const int NeoPixel = 9;
const int Piezo = 4;

int RedButtonState = 0;
int GreenButtonState = 0;
int Transmit = 1;
int Transmit2 = 2;

int countDown = 5; // Determines how long countdown will be
int sound = 450; //Pitch for the piezo
int countDelay = 500; //Time for the countdown

bool radioNumber = 0;
RF24 radio(9,10);
byte addresses[][6] = {"1Node","2Node"};
Adafruit_7segment matrix = Adafruit_7segment();

int melody[] = {
  NOTE_E7, 0, NOTE_E7, NOTE_D7, NOTE_E7, NOTE_G7,0, NOTE_E7,
};

int noteDurations[] = {
  8, 8, 4, 8, 4, 4, 16, 4
};

void setup() {
  Serial.begin(9600);
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(RedButton, INPUT);
  pinMode(GreenButton, INPUT);
  pixel.begin();
  pixel.show(); // Initialize all pixels to 'off'
  matrix.begin(0x70);

  radio.begin();
  radio.setChannel(108);
  radio.setPALevel(RF24_PA_LOW);
  radio.openWritingPipe(addresses[0]);
  PowerRangers();
  delay(500);
}

void loop() {
  GreenPixel();
  RedButtonState = digitalRead(RedButton);
  GreenButtonState = digitalRead(GreenButton);
  digitalWrite(GREEN_LED, HIGH);

  matrix.print(0, DEC);
  matrix.writeDisplay();

  if (RedButtonState == HIGH){
    countdown();
  } 

  if (GreenButtonState == HIGH){
    checkCommunication();
  }
  else {
    digitalWrite(RED_LED, LOW);
  }
}

void countdown(){
  for (int i = countDown; i >= 1; i--)
  {
    digitalWrite(RED_LED, HIGH);
    tone(Piezo, sound);
    matrix.print(i, DEC);
    matrix.writeDisplay();
    RedPixel();
    delay(countDelay);
    noTone(Piezo);
    OffPixel();
    delay(countDelay);

    if ( i == 3){
      radio.write(&Transmit, sizeof(Transmit));
    }
  }

  matrix.print(0, DEC);
  matrix.writeDisplay();
}


//Change this after I test button/LED
void checkCommunication(){
  delay(400);
  digitalWrite(GREEN_LED, LOW);
  radio.write(&Transmit2, sizeof(Transmit2));
  BluePixel();
  delay(150);
  OffPixel();
  delay(200);
  BluePixel();
  delay(150);
  OffPixel();
  delay(200);
  digitalWrite(GREEN_LED, LOW);
}

void PowerRangers(){
  for (int thisNote = 0; thisNote < 8; thisNote++) {
    BluePixel();
      int noteDuration = 800 / noteDurations[thisNote];
      tone(Piezo, melody[thisNote], noteDuration);
      int pauseBetweenNotes = noteDuration * 1.30;
      
      delay(pauseBetweenNotes); 
    OffPixel();
  }
  noTone(Piezo);
}

void RedPixel(){
  pixel.setPixelColor(0, pixel.Color(25,0,0));
  pixel.show();
}

void BluePixel(){
  pixel.setPixelColor(0, pixel.Color(0,0,25));
  pixel.show();
}

void YellowPixel(){
  pixel.setPixelColor(0, pixel.Color(25,20,0));
  pixel.show();
}

void GreenPixel(){
  pixel.setPixelColor(0, pixel.Color(0,25,0));
  pixel.show();
}

void OffPixel(){
  pixel.setPixelColor(0, pixel.Color(0,0,0));
  pixel.show();
}

