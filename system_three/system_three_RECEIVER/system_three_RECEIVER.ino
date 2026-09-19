// RECEIVER CODE

#define PIEZO_PIN 23
#define ANALOG_PIN 32

void setup() {
  Serial.begin(115200);

  pinMode(PIEZO_PIN, OUTPUT);
  pinMode(ANALOG_PIN, INPUT);

  tone(PIEZO_PIN, 440, 500);

  Serial.println("Receiver startup complete.");
}

void loop() {
  int adc = analogRead(ANALOG_PIN);

  if (adc > 500) {
    // Map the read voltage (1360 - 4095) back to duration (100ms - 300ms)
    int toneDuration = map(adc, 1360, 4095, 100, 300);

    Serial.print("Read ADC: ");
    Serial.print(adc);
    Serial.print(" -> Calculated Duration: ");
    Serial.print(toneDuration);
    Serial.println(" ms");

    tone(PIEZO_PIN, 440, toneDuration);
    delay(toneDuration + 20);
  } else {
    noTone(PIEZO_PIN);
    delay(10);
  }
}

