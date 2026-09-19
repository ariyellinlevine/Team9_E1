// EMITTER CODE

#define OUTPUT_PIN 25

const char* morseLetters[] = {
  ".-",   "-...", "-.-.", "-..",  ".",    "..-.", "--.",  "....", "..",   // A-I
  ".---", "-.-",  ".-..", "--",   "-.",   "---",  ".--.", "--.-", ".-.",  // J-R
  "...",  "-",    "..-",  "...-", ".--",  "-..-", "-.--", "--.."          // S-Z
};
const int UNIT_TIME = 100;   // within morse specs for dot duration. dash is T*3
enum Gap {
  DOT = UNIT_TIME,
  DASH = UNIT_TIME * 3,
  INTRA_CHAR = UNIT_TIME,
  INTER_CHAR = UNIT_TIME * 3, 
  WORD = UNIT_TIME * 7
};

void setup() {
  Serial.begin(115200);

  dacWrite(OUTPUT_PIN, 0);
  delay(3000);
}

void loop() {
  sendMorse("Hello world this is UMD Loop");
}

void sendMorse(String text){
  text.toUpperCase();
  
  for(int i = 0; i < text.length(); i++){
    char c = text.charAt(i);

    if(c == ' '){
      delay(WORD - INTER_CHAR);
      continue;
    }
    
    const char* morse_sequence = getMorseSequence(c);
    
    if(morse_sequence != NULL){
      playMorseCode(morse_sequence);
      delay(INTER_CHAR);
    }
  }
}

const char* getMorseSequence(char c){
  if(c >= 'A' and c <= 'Z'){
    return morseLetters[c - 'A'];
  }
  return NULL; // invalid char
}

void playMorseCode(const char* sequence) {
  for (int i = 0; sequence[i] != '\0'; i++) {
    char symbol = sequence[i];
    int duration = (symbol == '.') ? DOT : DASH;

    int dacValue = map(duration, DOT, DASH, 85, 255);

    dacWrite(OUTPUT_PIN, dacValue);
    delay(30); // Short pulse for analog voltage value timing

    dacWrite(OUTPUT_PIN, 0);
    delay(duration + INTRA_CHAR);
  }
}

