#include <ESP32Servo.h>

#define SERVO_PIN 5  // H1
Servo servo;

int servo_angle = 90;
int input = 0;

void setup() {
  Serial.begin(115200);
  Serial.println("Starting E1 System Two...");

  servo.setPeriodHertz(50);
  servo.attach(SERVO_PIN);
  servo.write(90);  // reset before operation

  Serial.println("Startup complete."); 
}

void loop() {
  if (Serial.available() > 0) {
    // TODO: Implement with Serial.read() to use non-blocking calls
    // as well as handle input faster and ignore newline characters
    input = Serial.parseInt();
    Serial.print("Recieved angle input: ");
    Serial.println(input);  
    if (input >= 0 && input <= 180) {
      servo_angle = input;
    }
    servo.write(servo_angle);
  }
}
