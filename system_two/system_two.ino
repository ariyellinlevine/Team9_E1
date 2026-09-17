#include <ESP32Servo.h>

#define SERVO_PIN 5           // H1
Servo servo; 

void setup() {
  Serial.begin(115200); 

  Serial.println("Hello World");

  servo.setPeriodHertz(50); 
  servo.attach(SERVO_PIN); 
  servo.write(90);           // reset before operation
}

void loop() {

}
