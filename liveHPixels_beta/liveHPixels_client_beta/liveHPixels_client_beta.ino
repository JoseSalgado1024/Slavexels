#include <Event.h>
#include <Timer.h>

#include <nRF24L01.h>
#include <RF24.h>
#include <RF24_config.h>
#include <SPI.h>
#include "Tones.h"
#include "config.h"


RF24 myRadio(9,10);
const uint64_t pipe = 0xE8E8F0F0E1LL;

char sChar; 
boolean printMe = false;
int messageLength = 12;
int msg[1];
    byte theMessage[12];
Timer t;

int tones[6];

int st = 0;
int dataIndex;


void setup()
{
  Serial.begin(57600);
  myRadio.begin();
  myRadio.openReadingPipe(1,pipe);
  myRadio.startListening();
  rgb_setup();
  setupTones();
  int tickEvent = t.every(50,soundTime);
}

void loop()
{
   t.update();
  if (myRadio.available()){

    myRadio.read  (&theMessage,sizeof(theMessage)); 
    dataIndex = (humanId)*4;
    rgb_set_values(theMessage[dataIndex],theMessage[dataIndex+1],theMessage[dataIndex+2]);
    playTone(theMessage[dataIndex+3]);
    
    Serial.print("Data Index: ");
    Serial.println(dataIndex);
    
    
    for (int i= 0 ;i<4;i++ )
      {
        Serial.print(theMessage[dataIndex+i]);
        Serial.print(',');
        
      }
       Serial.println(" ");
    for (int i= 0 ;i<12;i++ )
      {
         
                Serial.print(theMessage[i]);
                Serial.print(',');
           
      }
          Serial.println('-');
  }

}


//INIT RGB DEVICE
void rgb_setup()
{
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);
  pinMode(RED, OUTPUT);
  digitalWrite(GREEN, HIGH);
  digitalWrite(BLUE, HIGH);
  digitalWrite(RED, HIGH);
}


void rgb_set_values(byte r,byte g,byte b)
{
  analogWrite(GREEN,255 - g);
  analogWrite(BLUE, 255 - b);
  analogWrite(RED,  255 - r);
}



void setupTones(){

  tones[0] = 0;
  tones[1] = NOTE_D5;
  tones[2] = NOTE_E1;
  tones[3] = NOTE_DS2;
  tones[4] = NOTE_B5;
  tones[5] = NOTE_DS7;
  tones[6] = NOTE_DS8;
  tones[7] = NOTE_CS4; 
}

void soundTime(){
  if(st>0){
    st-=1;
  }else{
    noTone(SPEAKER);
  }
}

void playTone(int tone2Play){
  if(tones[tone2Play] !=0){
  noTone(SPEAKER);
  tone(SPEAKER,tones[tone2Play]);
  st = 10;
  }
}

