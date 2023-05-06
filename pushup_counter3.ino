#include "LedControl.h"
/*Now we need a LedControl to work with.
*** These pin numbers will probably not work with your hardware ***
pin 12 is connected to the DataIn
pin 11 is connected to the CLK
pin 10 is connected to LOAD
We have only a single MAX72XX.*/
LedControl lc=LedControl(12,11,10,1);
/* we always wait a bit between updates of the display */
unsigned long delaytime=500;
#define echoPin  2
#define trigPin  3
// DISABLE RESET PIN #define resetPin 4

float duration; //micro second
float d_cm;
//long numDisplay=100;
boolean trigUp = false;
boolean trigDown = false;
float counterPushUp=0;

void setup() {

  /* The MAX72XX is in power-saving mode on startup, we have to do a wakeup call */
  lc.shutdown(0,false);
  /* Set the brightness to a medium values */
  lc.setIntensity(0,2);
  /* and clear the display */
  lc.clearDisplay(0);

// initialize serial communication:
  Serial.begin(9600);
  pinMode(trigPin,OUTPUT);
  pinMode(echoPin,INPUT);
  //
  // DISABLE RESET PIN pinMode(resetPin,INPUT_PULLUP);
  //
///////////////////////  display.setBrightness(4);
///////////////////////  display.clear();
///////////////////////  delay(500);
///////////////////////  display.showNumberDecEx(numDisplay, false, true, 4, 0);
  delay(delaytime);
  MyDisplay(0); //to show 0000

}

void loop() {
  
  //reset by switch
  /* DISABLE RESET PIN 
   if(digitalRead(resetPin)==0){
    trigUp=false;
    trigDown=false;
    counterPushUp=0;
  } 
   */
  
  // generate pusle trigger
  digitalWrite(trigPin,LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin,HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin,LOW);
  // receive, convert time (us) to cm
  duration = pulseIn(echoPin,HIGH);
  d_cm = duration * 0.034/2.;
  
  //trigger body 
  if (d_cm < 10){
    trigDown = true;
  } else if (d_cm>15 && d_cm<=30){
    trigUp = true;
  } else if(d_cm >30) {
  }
  //counter
  if (trigUp==true && trigDown==true){
    counterPushUp=counterPushUp+0.5;
    trigUp=false;
    trigDown=false;
    MyDisplay(counterPushUp);
  }
   // Serial.println(d_cm);
//////////////////  display.showNumberDecEx(counterPushUp, false, true, 4, 0);
 delayMicroseconds(1000000);
  }

void MyDisplay(int VALUE){
    int v =VALUE;
    int ones;
    int tens;
    int hundreds;
    int thousands;
 
    ones=v%10;
    v=v/10;
    tens=v%10;
    v=v/10;
    hundreds=v%10;   
    v=v/10;
    thousands=v%10;  
 
    //Now print the number digit by digit
    lc.setDigit(0,3,(byte)thousands,false);
    lc.setDigit(0,2,(byte)hundreds,false);
    lc.setDigit(0,1,(byte)tens,false);
    lc.setDigit(0,0,(byte)ones,false);

}
