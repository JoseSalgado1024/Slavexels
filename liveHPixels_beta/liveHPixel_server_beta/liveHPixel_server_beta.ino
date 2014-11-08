
#include <nRF24L01.h>
#include <RF24.h>
#include <RF24_config.h>
#include <SPI.h>

RF24 myRadio(9,10);
const uint64_t pipe = 0xE8E8F0F0E1LL;
 
int pkgIndex = 0; 
byte sendmethis[12];
boolean ready2Send = false;
byte message[14] ; 


byte cont = 0 ; 

void setup()
{
  Serial.begin(9600);
  myRadio.begin();
  myRadio.openWritingPipe(pipe);
  
  
  for (int i = 0 ;i<12;i++ )
      {
        sendmethis[i]=0;
      }
  //sendmethis[4]  = 0;
  //sendmethis[5]  = 0;
  //sendmethis[6]  = 0;
  //sendmethis[7]  = 0;
  //sendmethis[8]  = 0;
  //sendmethis[9]  = 0;
  //sendmethis[10] = 0;
  //sendmethis[11] = 0;
}
void loop()
{

}


void serialEvent() {
  while (Serial.available()) {
    message[cont++] = (byte)Serial.read(); 
    if (message[cont-1] == 13 && message[cont-2] == 10 ) {
      if (cont ==14 )
      {     
        commandComplete();
      }
      else
      {

      }
      cont = 0 ;
    }

  }
}



void commandComplete()
{
  for (int i = 0 ;i<12;i++ )
      {
        sendmethis[i]=message[i];
      }
  
  //sendmethis[0]  = message[0];
  //sendmethis[1]  = message[1];
  //sendmethis[2]  = message[2];
  //sendmethis[3]  = message[3];
  myRadio.write(&sendmethis,sizeof(sendmethis));
  delay(100);
}









