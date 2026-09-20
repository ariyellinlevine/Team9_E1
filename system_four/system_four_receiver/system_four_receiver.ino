#define SCL_PIN 13
#define SDA_PIN 14

uint8_t address = 0x02;

bool receiving = 0;   // 1 = master wants to read

enum State { IDLE, ADDR, DATA };
State state = IDLE;

bool scl = 1;
bool sda = 1;
bool last_scl = 1;
bool last_sda = 1;

bool scl_rising = 0;
bool scl_falling = 0;

bool acking = 0;

uint8_t bit_count = 0;
uint8_t active_data = 0x00;

void setup() {
  Serial.begin(115200);
  pinMode(SCL_PIN, INPUT_PULLUP);
  set_wire(SDA_PIN, HIGH);
}

void set_wire(int pin, bool signal) {
  if (signal) {
    pinMode(pin, INPUT_PULLUP);  // release the line and have it pull high
  } else {
    digitalWrite(pin, LOW);
    pinMode(pin, OUTPUT);        // drive the line to low
  }
}

void loop() {
  scl = digitalRead(SCL_PIN);    // no "bool", these are the globals
  sda = digitalRead(SDA_PIN);

  scl_rising  = scl && !last_scl;
  scl_falling = !scl && last_scl;

  // stop check
  if (scl && last_scl && !last_sda && sda) {
    reset();
    set_wire(SDA_PIN, HIGH);
    state = IDLE;
  }

  switch (state) {
    case IDLE:
      // checks if scl is holding steady, then if so checks if SDA has fallen
      if (scl && last_scl) {
        if (!sda && last_sda) {
          reset();
          state = ADDR;
        }
      }
      break;

    case ADDR:
      if (read_data()) {
        receiving = active_data & 1;
        check_address();
      }
      break;

    case DATA:
      if (read_data()) {
        if (awk()) {                       // ack finished
          Serial.println(active_data, HEX);
          reset();                         // stay in DATA for more bytes
        }
      }
      break;
  }

  last_scl = scl;
  last_sda = sda;
}

bool read_data() {
  if (scl_rising && bit_count < 8) {
    active_data = (active_data << 1) | sda;
    bit_count++;
  }
  return bit_count >= 8;
}

void check_address() {
  if ((active_data >> 1) == address && !receiving) {
    if (awk()) {
      reset();
      state = DATA;
    }
  } else {
    state = IDLE;
  }
}

// Returns true only once the ack is completely finished
bool awk() {
  if (scl_falling) {
    if (!acking) {
      set_wire(SDA_PIN, LOW);    // first falling edge: ack
      acking = true;
    } else {
      set_wire(SDA_PIN, HIGH);   // second falling edge: release
      acking = false;
      return true;
    }
  }
  return false;                  // still waiting
}

void reset() {
  bit_count = 0;
  active_data = 0;
  acking = false;
}