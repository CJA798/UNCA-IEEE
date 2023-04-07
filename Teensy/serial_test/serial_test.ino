void setup() {
  Serial.begin(115200);   // initialize serial communication at 115200 baud rate
}

void loop() {
  if (Serial.available() > 0) {
    String inputString = Serial.readStringUntil('\n'); // Read the input string
    int intValue = inputString.toInt(); // Convert the input string to an integer
    uint16_t maskedValue = intValue & 0xFFFF; // Mask out all but the lowest 16 bits
    Serial.println(maskedValue, BIN);
    Serial.flush();                // clear the output buffer
  }
}
