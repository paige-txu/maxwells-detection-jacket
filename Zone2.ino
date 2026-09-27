void setup() {
  Serial.begin(115200);
}
void loop() {
get_magnumber();
  

}

void get_magnumber() {
  // Read the internal hall sensor
  int total=0;
  for(int i=0;i<100;i++) {
    total+=hallRead();
  }
  total= total/100;
  Serial.println(total);
  delay(1000);
  
}
