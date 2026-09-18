#include <ESP32Servo.h>

#define SERVO_PIN 5  // H1
Servo servo;

int servo_angle = 90;
int input = 90;
const int MIN_ANGLE = 0; 
const int MAX_ANGLE = 180; 

char inputBuffer[32];
bool cmd_ready = false; 

void setup() {
  Serial.begin(115200);
  Serial.println("Starting E1 System Two...");

  servo.setPeriodHertz(50);
  servo.attach(SERVO_PIN);
  servo.write(90);  // reset before operation

  Serial.println("Startup complete.");
}

void loop() {
  readSerialStream(); 
  handleCommand(); 

  // simpleInputHandler(); 
}

void readSerialStream() {
  static byte index = 0;
  char end_marker = '\n';  // change based on Serial Monitor

  while(Serial.available() > 0){
    char inc = Serial.read(); 

    if(inc == end_marker){
      inputBuffer[index] = '\0'; 

      index = 0; 
      cmd_ready = true; 
      return; 
    }
    if(!isDigit(inc)){
      Serial.print("Recieved non-numeric character: "); 
      Serial.println(inc); 
      return; 
    }
    inputBuffer[index] = inc; 
    index++; 
  }
}

void handleCommand(){
  if(!cmd_ready) return;

  int angle = atoi(inputBuffer);  // array to integer
  if(angle <= MAX_ANGLE && angle >= MIN_ANGLE){
    Serial.print("Moving servo to: "); 
    Serial.println(angle); 
    servo.write(angle); 
  }
  else{
    Serial.print("Recieved invalid angle input: "); 
    Serial.println(angle); 
  }

  // input buffer doesn't need to be cleaned up
  // since it holds an ending character
  cmd_ready = false; 
}

void simpleInputHandler() {
  // simple function using Serial.read()
  // Blocking call, so causes lag/reset issues
  if (Serial.available() > 0) {
    input = Serial.parseInt(); 
    Serial.print("Recieved angle input: ");
    Serial.println(input);
    if (input >= 0 && input <= 180) {
      servo_angle = input;
    }
    servo.write(servo_angle);
  }
}
