/***************************************************************************
  This is a library for the APDS9960 digital proximity, ambient light, RGB, and gesture sensor

  This sketch puts the sensor in proximity mode and enables the interrupt
  to fire when proximity goes over a set value

  Designed specifically to work with the Adafruit APDS9960 breakout
  ----> http://www.adafruit.com/products/3595

  These sensors use I2C to communicate. The device's I2C address is 0x39

  Adafruit invests time and resources providing this open source code,
  please support Adafruit andopen-source hardware by purchasing products
  from Adafruit!

  Written by Dean Miller for Adafruit Industries.
  BSD license, all text above must be included in any redistribution
 ***************************************************************************/

#include "Adafruit_APDS9960.h"

//the pin that the interrupt is attached to
#define INT_PIN 3
#define LED_PIN 9

//create the APDS9960 object
Adafruit_APDS9960 apds;

byte brightness = 255;

void setup() {
  Serial.begin(115200);
  pinMode(INT_PIN, INPUT_PULLUP);
  pinMode(LED_PIN, OUTPUT);
  if (!apds.begin()) {
    Serial.println("failed to initialize device! Please check your wiring.");
  }
  else Serial.println("Device initialized!");

  //enable proximity mode
  apds.enableProximity(true);

  //set the interrupt threshold to fire when proximity reading goes above 175
  apds.setProximityInterruptThreshold(0, 175);

  //enable the proximity interrupt
  //apds.enableProximityInterrupt();
}

void up_dow() {
  while (apds.readProximity() >= 20) {

    int led_status = digitalRead(LED_PIN);
    if (led_status)
      brightness -= 25;
    else
      brightness += 25;
    analogWrite(LED_PIN, brightness);
    delay(1000);
    Serial.println("Brilho:");
    Serial.println(brightness);
    if (brightness > 255) {
      brightness = 255;
    }
    if (brightness < 0) {
      brightness = 0;
    }
  }
}

void on() {
  analogWrite(LED_PIN, 255);
  Serial.println("ON");
  delay(1000);
}
void off() {
  analogWrite(LED_PIN, 0);
  Serial.println("OFF");
  delay(1000);
}

void loop() {


  Serial.println(apds.readProximity());
  if (apds.readProximity() >= 20) {
    int led_status = digitalRead(LED_PIN);
    Serial.println("state:");
    Serial.println(led_status);
    if (led_status) {
      off();
    } else {
      on();
      
    }
    Serial.println("DIMMER:");
    up_dow();
  }

}
