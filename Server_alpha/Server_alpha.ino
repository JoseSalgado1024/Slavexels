#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <RF24_config.h>

#include <Timer.h>


//SOME NICE & UTIL CODES!
const String STARTC = "_START";
const String RESETC = "_RESET";
const String WAIT   = "|WAIT|";


Timer t;

//end of line char :D 
int msg[1];

RF24 radio(9,10);
const uint64_t pipe = 0xE8E8F0F0E1LL;

//FLAGS
boolean sendData = false;
boolean cReady = false;
boolean cycleRun = false;


int thePass = -40;
String sendmethis = "";


void setup()
{
  Serial.begin(9600);
  radio.begin();
  int tickEvent = t.every(250,makePass);
  radio.openWritingPipe(pipe);
 
}

void loop(){
    t.update();
  if(sendData)
    {
     int messageSize = sendmethis.length();
     for (int i = 0; i < messageSize; i++) {
     int charToSend[1];
     charToSend[0] = sendmethis.charAt(i);
     radio.write(charToSend,1);
    }  
     msg[0] = 2; 
     radio.write(msg,1);
     sendmethis = "";
     sendData = true; 
     if(sendmethis == STARTC){sendData =false;}
    }

 }
      

void sendCode(String theMessage)
 {
  int messageSize = theMessage.length();
  for (int i = 0; i < messageSize; i++) {
    int charToSend[1];
    charToSend[0] = theMessage.charAt(i);
    radio.write(charToSend,1);
  }  
 
  msg[0] = 2; 
  radio.write(msg,1);
}

void initCycles(){

}

void makePass(){
  sendData = true;
  thePass++;
  if (cReady && cycleRun && (thePass > 0))
  {
    sendmethis = STARTC;
    
    int addC = 6-sendmethis.length();
    for(int i = 0; i < addC ; i++)
    {
      sendmethis += " ";
     }
   }else if(!cReady && !cycleRun ) {
     sendmethis = RESETC;
     cReady=true;
   }else if(cReady && !cycleRun && (thePass > -10)){
     sendmethis = WAIT;
     cycleRun = true;
   }
  
  
}
