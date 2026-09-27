#include <Adafruit_NeoPixel.h> // Include the Adafruit NeoPixel library
#include <Wire.h> 
#include <LiquidCrystal_I2C.h> //for the lcd display
#include <SPI.h>
#include <RF24.h> //for the RF board

//button display
const int buttonPin = 27; //button pin
int buttonState = 0; //is the button pressed
String message = ""; // display of which equation is used 

//SDA->21,SCL->22 for displayer thing
LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display
int count = 0;

RF24 radio(4, 5); // CE, CSN pins

const uint8_t num_channels = 126;
uint8_t values[num_channels];

void setup() {
  Serial.begin(115200);
  //radio init
  radio.begin();
  radio.setAutoAck(false);
  radio.startListening();
  radio.stopListening(); // We just want to scan the carrier
  Serial.println("nRF24L01 WiFi Signal Strength Scanner");


   //Display init
  lcd.init();// initialize the lcd 
  lcd.backlight(); // Turns on the LCD backlight.
  lcd.print("Hello, world!");   // Print a message to the LCD.
  
}

void loop(){
  

  equationDisplay();
  if (count == 4){
    equationFourLoop();
  }else{
    Serial.println("NotEquation");
     delay(1000);
     clearRow(1);
  }

 
}

void equationFourLoop() {
  memset(values, 0, sizeof(values));

  // Scan all channels 100 times to get an average activity level
  int scan_rep = 100;
  while (scan_rep--) {
    for (int i = 0; i < num_channels; i++) {
      radio.setChannel(i);
      radio.startListening();
      delayMicroseconds(128); // Short wait for signal detection
      if (radio.testRPD()) {  // Returns true if signal > -64dBm
        values[i]++;
      }
      radio.stopListening();
    }
  }

  int totalBars = 0;
  // Print the "Graph" to Serial Monitor
  for (int i = 0; i < num_channels; i++) {
    uint8_t bar_height = values[i] / 4; // Scale for display
    if (bar_height > 0) {
      Serial.print(i < 10 ? "0" : "");
      Serial.print(i);
      Serial.print(": ");
      for (int j = 0; j < bar_height; j++) Serial.print("|");
      Serial.println();
      totalBars += bar_height;
      
    }
  }
  

  clearRow(1);
  lcd.print("Strength:");
  lcd.print(totalBars);
  Serial.println(totalBars);
  Serial.println("---------------------------------------");
  
}








void equationDisplay()
{
  
  //start of new thing 
  clearRow(0);
  //lcd.print("NUMBER");
  //lcd.setCursor(0,1);
  buttonState = digitalRead(buttonPin);
  //Serial.println(buttonState);
  if(buttonState == LOW){
   
     if(count == 4){
      count = 1;
    }else{
      count++;
    }

  }
  // check to see what message should be displayed
    if(count == 4){
      message = "Equation 4";
    }else if(count ==3) {
      message = "Equation 3";
    }else if (count ==2){
      message = "Equations 2";
    }else if (count ==1){
      message = "Equation 1";
    }else{
      message = "Welcome to Demo";
    }

  lcd.print(message);
 
}


void clearRow(int rowNum) {
  lcd.setCursor(0, rowNum);
  lcd.print("                "); // 16 spaces
  lcd.setCursor(0, rowNum); // Optional: reset cursor to 0,0
}
