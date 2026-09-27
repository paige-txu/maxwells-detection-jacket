

void setup() {
  Serial.begin(115200);
}

void zoneOneloop() {
  // Take 8 readings and average them
  // This smooths out the noise — one reading alone is jumpy
  int total = 0;
  for (int i = 0; i < 8; i++) {
    total += analogRead(36); // GPIO36
    delay(2);
  }
  int average = total / 8;

  Serial.print("E-field reading: ");
  Serial.print(average);

  int bars = average / 100;
  bars = constrain(bars, 0, 20);
  for (int i = 0; i < bars; i++) Serial.print("█");
  Serial.println();

  delay(100);

}
