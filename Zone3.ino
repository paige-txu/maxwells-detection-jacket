// Zone 3 test — LM358 4-pin module
// Coil Lead 1 → IN
// Coil Lead 2 → GND
// OUT → GPIO34
// VCC → 3.3V, GND → GND


void setup() {
  Serial.begin(115200);
  Serial.println("Zone 3 Faraday coil test");
  Serial.println("Static magnet = near zero");
  Serial.println("Moving magnet = number rises");
}

void zoneThreeloop() {
  int reading = analogRead(34);

  Serial.print("Reading: ");
  Serial.print(reading);
  Serial.print("  /4095    ");

  // Visual bar
  int bars = reading / 200;
  bars = constrain(bars, 0, 20);
  for (int i = 0; i < bars; i++) Serial.print("█");
  Serial.println();

  delay(50);
}