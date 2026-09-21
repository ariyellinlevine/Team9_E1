#define RED_PIN 5
#define GREEN_PIN 18
#define BLUE_PIN 12

void setup() {
  Serial.begin(115200);

  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);

  digitalWrite(RED_PIN, HIGH);
  delay(250);
  digitalWrite(RED_PIN, LOW);
  digitalWrite(GREEN_PIN, HIGH);
  delay(250);
  digitalWrite(GREEN_PIN, LOW);
  digitalWrite(BLUE_PIN, HIGH);
  delay(250);
  digitalWrite(BLUE_PIN, LOW);

  Serial.println("booted up");
}

void loop() {
  if (Serial.available() > 0) {
    digitalWrite(RED_PIN, LOW);
    digitalWrite(GREEN_PIN, LOW);
    digitalWrite(BLUE_PIN, LOW); 

    String input = Serial.readStringUntil('\n');
    input.trim();

    if (input.startsWith("#")) {
      input = input.substring(1);
    }

    if (input.length() != 6) {
      Serial.println("Invalid string.");
      return;
    }
    uint32_t hex = strtoul(input.c_str(), NULL, 16);

    uint8_t red = (hex >> 16) & 0xFF;   // shift and mask
    uint8_t green = (hex >> 8) & 0xFF;
    uint8_t blue = hex & 0xFF;

    Serial.println(red);
    Serial.println(green);
    Serial.println(blue);

    if(red > green &* red > blue){
      digitalWrite(RED_PIN, HIGH);
    }
    else if(green > red && green > blue){
      digitalWrite(GREEN_PIN, HIGH); 
    }
    else if(blue > red && blue > green){
      digitalWrite(BLUE_PIN, HIGH);
    }
    else if(red > blue && red == green){
      digitalWrite(RED_PIN, HIGH);
      digitalWrite(GREEN_PIN, HIGH);
    }
    else if(red > green && red == blue){
      digitalWrite(RED_PIN, HIGH);
      digitalWrite(BLUE_PIN, HIGH);
    }
    else if(green > red && green == blue){
      digitalWrite(GREEN_PIN, HIGH);
      digitalWrite(BLUE_PIN, HIGH);
    }
    else if(red == green && green == blue && red != 0){
      digitalWrite(RED_PIN, HIGH);
      digitalWrite(GREEN_PIN, HIGH);
      digitalWrite(BLUE_PIN, HIGH);
    }
  }
}
