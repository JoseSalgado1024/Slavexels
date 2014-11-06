#include <nRF24L01.h>
#include <RF24.h>
#include <RF24_config.h>
#include <SPI.h>

RF24 myRadio(9,10);
const uint64_t pipe = 0xE8E8F0F0E1LL;

int pkgIndex = 0; 
byte sendmethis[12];


void setup()
{
  Serial.begin(9600);
  myRadio.begin();
  myRadio.openWritingPipe(pipe);
  sendmethis[0]  = 255;
  sendmethis[1]  = 0;
  sendmethis[2]  = 0;
  sendmethis[3]  = 0;
  sendmethis[4]  = 0;
  sendmethis[5]  = 0;
  sendmethis[6]  = 0;
  sendmethis[7]  = 0;
  sendmethis[8]  = 0;
  sendmethis[9]  = 0;
  sendmethis[10] = 0;
  sendmethis[11] = 0;
}

void loop()
{
  myRadio.write(&sendmethis,sizeof(sendmethis) );
  Serial.println("Send OK!");
  delay(100);
}



