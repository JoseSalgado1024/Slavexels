
#define GREEN 6
#define BLUE 5
#define RED 3
#define delayTime 200


void rgb_setup()
  {
      pinMode(GREEN, OUTPUT);
      pinMode(BLUE, OUTPUT);
      pinMode(RED, OUTPUT);
      digitalWrite(GREEN, HIGH);
      digitalWrite(BLUE, HIGH);
      digitalWrite(RED, HIGH);
  }


void setup() {
Serial.begin(9600);
rgb_setup();
}

int redVal;
int blueVal;
int greenVal;
 int incoming ;
 
void loop() {
  
    if (Serial.available() > 0) {
    incoming = Serial.read();
     Serial.write(" pong: ");
     Serial.write(incoming);
     
  }
 
 
  int redVal = 255;
  int blueVal = 0;
  int greenVal = 0;
  if (incoming == 1)
    {
      rgb_set_values(255,0,0);
    }
      if (incoming == 2)
    {
      rgb_set_values(0,255,0);
    }
      if (incoming == 3)
    {
      rgb_set_values(0,0,255);
    }
/*
  for( int i = 0 ; i < 255 ; i += 1 ){
    greenVal += 1;
    redVal -= 1;
    analogWrite( GREEN, 255 - greenVal );
    analogWrite( RED, 255 - redVal );

    delay( delayTime );
  }
 
  redVal = 0;
  blueVal = 0;
  greenVal = 255;
  for( int i = 0 ; i < 255 ; i += 1 ){
    blueVal += 1;
    greenVal -= 1;
    analogWrite( BLUE, 255 - blueVal );
    analogWrite( GREEN, 255 - greenVal );

    delay( delayTime );
  }
 
  redVal = 0;
  blueVal = 255;
  greenVal = 0;
  for( int i = 0 ; i < 255 ; i += 1 ){
    redVal += 1;
    blueVal -= 1;
    analogWrite( RED, 255 - redVal );
    analogWrite( BLUE, 255 - blueVal );

    delay( delayTime );
  }
*/
}




//SETTING... OH SI
void rgb_set_values(byte r,byte g,byte b)
  {

      analogWrite(GREEN,255- r);
      analogWrite(BLUE, 255-g);
      analogWrite(RED,255- b);
  }
  
