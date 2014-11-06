#include <Flash.h>

#include <nRF24L01.h>
#include <RF24.h>
#include <RF24_config.h>
#include <SPI.h>

//CONGFIG FILES
#include "hPix_conf.h"
#include "action_table.h"
#include "callBacks.h"
#include "sound.h"


//BECOUSE DOES NOT SUPPORT THREADS, THIS LIB GENERATE SOME SIMILIAR EXTRUCTURE
//USING THE SAME TECNIC THAT MS IN HIM WINDOWS'95
#include <Thread.h>
#include <ThreadController.h>

#include <Event.h>
#include <Timer.h>


//TONES TABLE
FLASH_TABLE(int, tones_table, 2  
       /* | TONE   |  TIME  |*/,
        { 0        ,    0   }, 
        { NOTE_B5  ,    5   }, 
        { NOTE_B5  ,    3   },
        { NOTE_D3  ,    3   }, 
        { NOTE_B0  ,    4   }, 
        { NOTE_GS1 ,    4   },
        { NOTE_DS8 ,    7   }
        ); 
        
int lsTone = 0;
int aTone  = 0;

//SOUNDS!

class Sounds {

   int speakerPin;
   int lsSound;
   int lsSoundDur;

  Sounds() {
   restSpeaker();
  }
  void soundUpDate(int _pointer){
    if(lsSound != _pointer){
      lsSound = _pointer;
      lsSoundDur = 100*tones_table[act_table[_pointer][3]][1];
      tone(speakerPin,tones_table[act_table[_pointer][3]][0]);
    } 
    if(lsSoundDur <= 0){
      noTone(speakerPin);
    }
   lsSoundDur -=10;
  }


   void restSpeaker(){
    lsSound=0;
    lsSoundDur=0;
    setupSpeaker(8);
    noTone(speakerPin);
  }

  void setupSpeaker(int newPin){
    speakerPin = newPin; 
  }
}

Sounds theSound = new Sounds();


//------------- RADIO STUFF -------------//
RF24 radio(9,10);




//FIRST IS FIRST, WE MUST CREATE SOME SCHEDULE 3 HANDLE OURS THREADS
ThreadController controll = ThreadController();

Thread* radThread     = new Thread();
Thread  rgbThread     = Thread();
Thread  soundThread   = Thread();
Thread couterThread   = Thread();


int aPntr = 0;
int aPass = 0;
int r, g, b;

boolean mustPlay = true;

void setup(){
  Serial.begin(9600);
  Serial.println("Inicializar THREADS...");
  //SETTING-UP THE THREADS
    radThread->onRun(radThreadCallBack);
    radThread->setInterval(50);
  
    rgbThread.onRun(rgbThreadCallBack);
    rgbThread.setInterval(20);
  
    couterThread.onRun(counterThreadCallBack);
    couterThread.setInterval(100);
    
    soundThread.onRun(soundThreadCallBack);
    soundThread.setInterval(250); 
  
    //controll.add(radThread);
    controll.add(&rgbThread); 
    controll.add(&soundThread);
    controll.add(&couterThread);  
  Serial.println(">> THREADS DONE!");   
}


void loop(){
  //NOTHING IS MOST BEATIFUL THAT A CLEAN LOOP! :)
  controll.run();
  theSound.soundUpDate(aPntr);
}

/*-------------FUNCTIONS CALLBACK 4 THE TREADS!---------------*/
void rgbThreadCallBack(){
     Serial.println(">> THREADS RGB");
     
  if (aPntr < act_table.rows()) {
    boolean fade = lsR == act_table[aPntr][0] && lsG==act_table[aPntr][1] && lsB==act_table[aPntr][2];
    if (aPass < cSteps) {
      //FADES BETWEEN COLOR ACTUAL RGB AND NEXT RGB
      // RED COLOR
      r = act_table[aPntr][0]-(act_table[aPntr][0]-act_table[aPntr+1][0])/cSteps*aPass;
      // GREEN COLOR
      g = act_table[aPntr][1]-(act_table[aPntr][1]-act_table[aPntr+1][1])/cSteps*aPass;
      // BLUE COLOR
      b = act_table[aPntr][2]-(act_table[aPntr][2]-act_table[aPntr+1][2])/cSteps*aPass;
      aPass++;
      paintLeds(r,g,b);
    } 
    else {
      aPass=0;
      paintLeds(act_table[aPntr][0], act_table[aPntr][1], act_table[aPntr][2]);
    }

  }
}


//RADIO HANDLER THREAD CALLBACK
void counterThreadCallBack(){
   Serial.println(">> THREADS COUNTER"); 
  if(onRun){
    aPntr++;
  }else{
    aPntr = 0;
  }
   
}

//RADIO HANDLER THREAD CALLBACK
void radThreadCallBack(){
     Serial.println(">> THREADS COUNTER");
  if (radio.available()){
    bool done = false;  
     // done =
      radio.read(msg, 1); 
      char theChar = msg[0];
      if (msg[0] != 2){
        theMessage.concat(theChar);
        }
      else {
       if (theMessage.length() == messageLength) { 
         //char codeS=convertCode(theMessage);
         Serial.print("data arrived: ");
         Serial.println(theMessage);
                 /* switch(codeS){
          case 'r' :{ initDev(); reseted = true; break;}
          case 'w' :{ syncDev = true; break;}
          case 's' : {playRoutine = true; break;}
        }  */
       }
       theMessage= "theMessage"; 
      }
   }
}

char convertCode(String Message){
  char rCode = 'c';
  if(Message == RESETC){
      rCode = 'r';
  }else if(Message == STARTC){
      rCode = 's';
  }else if(Message == WAIT){
       rCode = 'w';
  }
  return rCode;
  
}


//SOUND HANDLER THREAD CALLBACK
void soundThreadCallBack(){

}

void paintLeds(int _r, int _g, int _b){
  analogWrite(GREEN,255- r);
  analogWrite(BLUE, 255-g);
  analogWrite(RED,255- b);
}

void rgb_setup()
{
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);
  pinMode(RED, OUTPUT);
  digitalWrite(GREEN, HIGH);
  digitalWrite(BLUE, HIGH);
  digitalWrite(RED, HIGH);
}







