#include <Flash.h>
#include "hPix_conf.h"

//BECOUSE DOES NOT SUPPORT THREADS, THIS LIB GENERATE SOME SIMILIAR EXTRUCTURE
//USING THE SAME TECNIC THAT MS IN HIM WINDOWS'95
#include <Thread.h>
#include <ThreadController.h>

#include <Event.h>
#include <Timer.h>



#define realTime 10
#define prH1     realTime*10
#define prH2     realTime*25
#define prL1     realTime*50
#define prL2     realTime*100

//------------- LEDS STUFF -------------//
#define cSteps 10

//LED RGB PINS
#define  RED   3
#define  GREEN 6
#define  BLUE  5

//------------- SOUND STUFF -------------//
//SOUND HRDWR CONFIGURATION
#define speakerPin 8
int toneDuration = 0;

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

//------------- RADIO STUFF -------------//
//CANAL SERVER -> CLIENTE
const uint64_t pipe = 0xE8E8F0F0E1LL;


//ADD HERE YOUR ROUTUNE TABLE
FLASH_TABLE(byte, act_table, 4 
        /*| R  |  G  |  B  | SND  |*/, 
        {255,  0x00, 0x00, 3}, 
        {0x00,  255, 0x00, 0x00},
        {0x00, 0x00, 255,  0x00}, 
        {0x00,  255, 0x00, 1},
        {255,  0x00, 0x00, 0x00}, 
        {0x00,  255, 0x00, 0x00},
        {0x00, 0x00, 255,  0x00}, 
        {0x00, 0xFF, 0x00, 2},
        {0xFF, 0x00, 0x00, 0x00}, 
        {0xAF, 0xFF, 0x00, 0x00},
        {0x00, 0x00, 0xFF, 0x00}, 
        {0x00, 0xFF, 0x00, 0},
        {255,  0xA1, 0xDF, 0x00}, 
        {0x00,  255, 0x00, 0x00},
        {0x00, 0x00, 0xFF, 0x00}, 
        {0x00,  255, 0x00, 2},
        {255,  0x00, 0x00, 0x00}, 
        {0x00,  255, 0x00, 0x00},
        {0x00, 0x0F, 255,  0x00}, 
        {0x00, 0xFF, 0x00, 2},
        {0xFF, 0x00, 0x00, 0x00}, 
        {0x00, 0xFF, 0x00, 0x00},
        {0x00, 0x00, 0xFF, 0x00}, 
        {0xFF, 0xFF, 0x00, 0xFF},
        );


//FIRST IS FIRST, WE MUST CREATE SOME SCHEDULE 3 HANDLE OURS THREADS
ThreadController controll = ThreadController();

Thread* radThread     = new Thread();
Thread  rgbThread     = Thread();
Thread  soundThread   = Thread();


int aPntr = 0;
int aPass = 0;
int r, g, b;

boolean mustPlay = true;

void setup(){
   //SETTING-UP THE THREADS
    radThread->onRun(radThreadCallBack);
    radThread->setInterval(prH1);
  
    rgbThread.onRun(rgbThreadCallBack);
    rgbThread.setInterval(prL1);
  
    soundThread.onRun(soundThreadCallBack);
    soundThread.setInterval(prL1);
  
    controll.add(radThread);
    controll.add(&rgbThread); 
    controll.add(&soundThread); 
}


void loop(){
  //NOTHING IS MOST BEATIFUL THAT A CLEAN LOOP! :)
  controll.run();
}

/*-------------FUNCTIONS CALLBACK 4 THE TREADS!---------------*/
void rgbThreadCallBack(){
  if (aPntr < act_table.rows()) {
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
      aPntr++;
      paintLeds(act_table[aPntr][0], act_table[aPntr][1], act_table[aPntr][2]);
    }

  }
}


//RADIO HANDLER THREAD CALLBACK
void radThreadCallBack(){
}

//SOUND HANDLER THREAD CALLBACK
void soundThreadCallBack(){
  aTone = act_table[aPntr][3];
  if(mustPlay && (aTone == 0 || aTone != lsTone)){
    toneDuration++;
    lsTone = act_table[aPntr][3];
    tone(speakerPin,tones_table[act_table[aPntr][3]][0]);
    mustPlay = (prL1*2)/toneDuration <= tones_table[act_table[aPntr][3]][1];
  }else{
    noTone(speakerPin);
    toneDuration=0;
  }
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







