  #define SCL_PIN 13
  #define SDA_PIN 14

  //connect a wire between these two points and the program should send the byte
  #define test_pin_output = -1
  #define test_pin_input = -1

  //might be too low
  uint8_t wait_time = 50;

  uint8_t data = 159

  void setup() {
    Serial.begin(115200);
    set_wire(SDA_PIN, HIGH);
    set_wire(SCL_PIN, HIGH);

    pinMode(test_pin_input, INPUT);
    pinMode(test_pin_output, OUTPUT);
    digitalWrite(test_pin_output, HIGH);

  }

  void loop() {
    if (digitalRead(test_pin_input) == HIGH) {
      send_data(data, 0x02);
    }
  }


  void set_wire(int pin, bool signal) {
    if (signal) {
      pinMode(pin, INPUT_PULLUP);  // release the line and have it be pulled to high
    } else {
      digitalWrite(pin, LOW);
      pinMode(pin, OUTPUT); // drive the line to low
    }
  }

  void start() {
    set_wire(SDA_PIN, LOW);
    delayMicroseconds(wait_time);
    set_wire(SCL_PIN, LOW);
  }

  void stop() {
    set_wire(SDA_PIN, LOW);
    delayMicroseconds(wait_time);
    set_wire(SCL_PIN, HIGH);
    delayMicroseconds(wait_time);
    set_wire(SDA_PIN, HIGH);
    delayMicroseconds(wait_time);

  }

  bool send_byte(uint8_t byte) {
    for (int i = 7; i >= 0; i--) {

        // shifts the byte over so the right most bit is the one that should be transfered, the & 1 checks to see if the last bit is a 0 or 1
        bool bitValue = (byte >> i) & 1;

        set_wire(SDA_PIN, bitValue);
        delayMicroseconds(wait_time);
        set_wire(SCL_PIN, HIGH);
        delayMicroseconds(wait_time);
        set_wire(SCL_PIN, LOW);
    }

    // have master release SDA_PIN so the slave is free to pull it down to awk
    set_wire(SDA_PIN, HIGH);
    delayMicroseconds(wait_time);
    set_wire(SCL_PIN, HIGH);
    delayMicroseconds(wait_time);
    bool ack = !digitalRead(SDA_PIN);
    set_wire(SCL_PIN, LOW);
    return ack;
  } 

  bool address(uint8_t addr, bool reading) {
    return send_byte((addr << 1) | reading);
  }

  bool send_data(uint8_t data, uint8_t addr) {
    start();
    if (!address(addr, 0)) {
      stop();
      return false;
    }

    if (!send_byte(data)) {
      stop();
      return false;
    }

    stop();
    return true;
  }