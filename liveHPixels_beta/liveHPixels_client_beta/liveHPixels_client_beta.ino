#include <nRF24L01.h>
#include <RF24.h>
#include <RF24_config.h>
#include <SPI.h>

RF24 myRadio(9,10);
const uint64_t pipe = 0xE8E8F0F0E1LL;

char sChar; 
boolean printMe = false;
int messageLength = 12;
int msg[1];

int humanId = 1;

#define   RED 3
#define GREEN 6
#define  BLUE 5


void setup()
{
  Serial.begin(57600);
  myRadio.begin();
  myRadio.openReadingPipe(1,pipe);
  myRadio.startListening();
  rgb_setup();
  
}

void loop()
{
  if (myRadio.available()){
      byte theMessage[12];
      myRadio.read  (&theMessage,sizeof(theMessage)); 
      int dataIndex = (humanId-1)*2;
      rgb_set_values(theMessage[dataIndex],theMessage[dataIndex+1],theMessage[dataIndex+2]);
      for(int j = 0;j<4;j++){
        Serial.print(theMessage[j]);
        Serial.print("|");
      }
      Serial.println(dataIndex); 
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
      analogWrite(GREEN,255- g);
      analogWrite(BLUE, 255-b);
      analogWrite(RED,255- r);
  }
