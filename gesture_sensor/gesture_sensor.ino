
#include <Wire.h>
#include <SparkFun_APDS9960.h>

float sinVal;
int ledVal;

// Pins
#define APDS9960_INT    2 // Needs to be an interrupt pin

// Constants

// Global Variables
SparkFun_APDS9960 apds = SparkFun_APDS9960();
int isr_flag = 0;
static int cont = 0;

void setup() {
  // Set interrupt pin as input
  pinMode(APDS9960_INT, INPUT);
  pinMode(11, OUTPUT);
   
  // Initialize Serial port
  Serial.begin(9600);
  Serial.println();
  Serial.println(F("--------------------------------"));
  Serial.println(F("SparkFun APDS-9960 - GestureTest"));
  Serial.println(F("--------------------------------"));
  
  // Initialize interrupt service routine
  attachInterrupt(0, interruptRoutine, FALLING);

  // Initialize APDS-9960 (configure I2C and initial values)
  if ( apds.init() ) {
    Serial.println(F("APDS-9960 initialization complete"));
  } else {
    Serial.println(F("Something went wrong during APDS-9960 init!"));
  }
  
  // Start running the APDS-9960 gesture sensor engine
  if ( apds.enableGestureSensor(true) ) {
    Serial.println(F("Gesture sensor is now running"));
  } else {
    Serial.println(F("Something went wrong during gesture sensor init!"));
  }
}

// Chegar gestos 
void handleGesture() {
  static int cont = 0;
    if ( apds.isGestureAvailable() ) {
    switch ( apds.readGesture() ) {      
      case DIR_UP:
        cont = 4;
        Serial.println("CIMA ↑");
        break;
        
      case DIR_DOWN:
        cont = 0;
        Serial.println("BAIXO ↓");
        break;
        
      case DIR_LEFT:
        cont --;
        Serial.println("ESQUERDA ←");
        break;
        
      case DIR_RIGHT:
        cont ++;
        Serial.println("DIREITA →");
        break;
        
      case DIR_NEAR:
        Serial.println("PROXIMO");
        break;
        
      case DIR_FAR:
        Serial.println("DISTANTE");
        for (int x=0; x<180; x++) 
//        {
//          sinVal = (sin(x*(3.1412/180)));
//          ledVal = int(sinVal*255);
//          analogWrite(11, ledVal);
//          delay(25);
//        }
        break; 
        
      default:
        Serial.println("NULL");
    }
    
//conferir se o valor excede    
    if (cont >=4)
      cont= 4;
    if(cont <= 0)
        cont=0;

//Adaptar a luz ao nivel desejado 
    switch (cont){
      case 0:
        Serial.println(cont);
        analogWrite(11, 0);
      break;

      case 1:
        Serial.println(cont);
        analogWrite(11, 64);
      break;

      case 2:
        Serial.println(cont);
        analogWrite(11, 128);
      break;

      case 3:
        Serial.println(cont);
        analogWrite(11, 192);
      break;

      case 4:
        Serial.println(cont);
        analogWrite(11, 255);
      break;
      
      default:
        Serial.println("NULL");
    }
  }
}

void loop() {
  if( isr_flag == 1 ) {
    detachInterrupt(0);
    handleGesture();
    isr_flag = 0;
    attachInterrupt(0, interruptRoutine, FALLING);
  }
}

void interruptRoutine() {
  isr_flag = 1;
}
