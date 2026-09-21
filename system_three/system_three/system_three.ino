#include <DacESP32.h>

#define PIEZO_PIN 32

DacESP32 dac(PIEZO_PIN);

char inputBuffer[32];
bool cmd_ready = false; 

unsigned long toneStartTime = 0;
unsigned long toneDurationMs = 0;
bool isPlaying = false;

void readSerialStream();
void handleCommand();
void handleToneTimer();

void setup(){
  Serial.begin(115200);
  Serial.println("System 3 startup complete.");
}

void loop(){
  readSerialStream(); 
  handleCommand(); 
  handleToneTimer();
}

void readSerialStream() {
  static byte index = 0;
  char end_marker = '\n'; 

  while(Serial.available() > 0){
    char inc = Serial.read(); 

    if(inc == end_marker){
      inputBuffer[index] = '\0'; 
      index = 0; 
      cmd_ready = true; 
      return; 
    }
    
    // Ignore carriage return if using 'Both NL & CR' in serial monitor
    if(inc == '\r') continue;

    if(!isDigit(inc)){
      Serial.print("Received non-numeric character: "); 
      Serial.println(inc); 
      return; 
    }
    
    if(index < 31) { // Prevent buffer overflow
      inputBuffer[index] = inc; 
      index++; 
    }
  }
}

void handleCommand(){
  if(!cmd_ready) return;

  int seconds = atoi(inputBuffer); 
  
  if(seconds > 0){
    Serial.print("Playing 440Hz tone for ");
    Serial.print(seconds);
    Serial.println(" seconds...");
    
    dac.outputCW(440, (dac_cosine_atten_t)0); // Start 440Hz at Max Volume
    toneDurationMs = seconds * 1000UL; // Convert to milliseconds
    toneStartTime = millis();
    isPlaying = true;
  }
  else{
    Serial.println("Received invalid duration. Must be > 0."); 
  }

  cmd_ready = false; 
}

void handleToneTimer() {
  // Non-blocking timer check to disable the tone
  if(isPlaying) {
    if(millis() - toneStartTime >= toneDurationMs) {
      dac.disable(); 
      isPlaying = false;
      Serial.println("Tone finished. Muting the piezo...");
    }
  }
}