#include <Flash.h>

#include <Thread.h>
#include <ThreadController.h>

#include <Event.h>
#include <Timer.h>

#define realTime 10

#define prH1   realTime*10
#define prH2   realTime*25
#define prL1   realTime*50
#define prL2   realTime*100


ThreadController controll = ThreadController();

Thread* radThread     = new Thread();
Thread  rgbThread     = Thread();
Thread  soundThread   = Thread();

void setup(){
  radThread->onRun(radThreadCallBack);
  radThread->setInterval(prH1);

  rgbThread.onRun(rgbThreadCallBack);
  rgbThread.setInterval(prH2);
  
  soundThread.onRun(soundThreadCallBack);
  soundThread.setInterval(prL1);

  controll.add(radThread);
  controll.add(&rgbThread ); 
  controll.add(&soundThread ); 
}


void loop(){
}

void rgbThreadCallBack(){
}
void soundThreadCallBack(){
}
void radThreadCallBack(){
}

