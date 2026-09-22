void setup() {
  Serial.begin(115200);
  Serial.println("Input a whole number from 0 to 255.");
}

void loop() {
  if (Serial.available() == 0) {
    return;
  }

  String input = Serial.readStringUntil('\n');
  input.trim();

  if (input.length() == 0) {
    return;
  }

  if (input.length() > 3) {
    Serial.println("Error: enter a whole number from 0 to 255.");
    return;
  }

  for (unsigned int i = 0; i < input.length(); i++) {
    if (input[i] < '0' || input[i] > '9') {
      Serial.println("Error: enter a whole number from 0 to 255.");
      return;
    }
  }

  int value = input.toInt();
  if (value > 255) {
    Serial.println("Error: enter a whole number from 0 to 255.");
    return;
  }

  Serial.print("Received: ");
  Serial.println(value);
}
