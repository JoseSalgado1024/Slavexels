#include <Flash.h>
#include <hPix_conf.h>
#include <Thread.h>
#include <ThreadController.h>

#include <Event.h>
#include <Timer.h>



#define realTime 10
#define prH1     50
#define prH2     100
#define prL1     250
#define prL2     500

#define cSteps 5

//LED RGB PINS
#define   RED 3
#define GREEN 6
#define  BLUE 5



FLASH_TABLE(byte, act_table, 4 
/*| R  |  G  |  B  | SND  |*/, 
{
  0x00, 0x00, 0x00, 0x00}
, 
{
  0x00, 0x00, 0x00, 0x00}
);



ThreadController controll = ThreadController();

Thread* radThread     = new Thread();
Thread  rgbThread     = Thread();
Thread  soundThread   = Thread();


int aPntr = 0;
int aPass = 0;
int r, g, b;


void setup(){
  radThread->onRun(radThreadCallBack);
  radThread->setInterval(prH1);

  rgbThread.onRun(rgbThreadCallBack);
  rgbThread.setInterval(prL1);

  soundThread.onRun(soundThreadCallBack);
  soundThread.setInterval(prL1);

  controll.add(radThread);
  controll.add(&rgbThread ); 
  controll.add(&soundThread ); 
}


void loop(){
  controll.run();
}

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

void soundThreadCallBack(){
}
void radThreadCallBack(){
}




