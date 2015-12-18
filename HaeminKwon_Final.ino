#include "I2Cdev.h"
#include "MPU6050.h"

#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
#include "Wire.h"
#endif

MPU6050 accelgyro;

int16_t ax, ay, az;
int16_t gx, gy, gz;

#define OUTPUT_READABLE_ACCELGYRO

int buttonState;             
int lastButtonState = LOW;
long lastDebounceTime = 0;
long debounceDelay = 50;

#define LED_PIN 13
#define GREEN_LED_PIN 5
bool blinkState = false;

bool isOn = false;

#define NOTE_G7  3136

void setup() {
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
  Wire.begin();
#elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
  Fastwire::setup(400, true);
#endif
  Serial.begin(9600);
  
  accelgyro.initialize();
   
  // configure Arduino LED for
  pinMode(LED_PIN, OUTPUT);
  pinMode(GREEN_LED_PIN, OUTPUT);
}

void loop() {
  if (isOn) {
    accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

#ifdef OUTPUT_READABLE_ACCELGYRO

    Serial.println(az/400); Serial.print("\t");
    
    //divided by 400 to make the number smaller
    //az/400 is between 45 (down) and 12 , adjust when needed. 
    
    if (az / 400 < 16) {
      digitalWrite(GREEN_LED_PIN, HIGH);
      tone(9, NOTE_G7);
      Serial.println("BINGO");
    } else {
      noTone(9);
      digitalWrite(GREEN_LED_PIN, LOW);
    }
#endif
  }
  
      //make button to turn on the device
  int reading = digitalRead(6);
  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }
      
  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != buttonState) {
      buttonState = reading;
      //make blink
      if (buttonState == HIGH) {
      digitalWrite(5, HIGH);   
      delay(1000);              
      digitalWrite(5, LOW);    
      delay(1000);          
        isOn = !isOn;
      }
    }
  }
  lastButtonState = reading;
}
