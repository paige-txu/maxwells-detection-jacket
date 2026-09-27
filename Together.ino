#include <Adafruit_NeoPixel.h> // Include the Adafruit NeoPixel library
#include <Wire.h> 
#include <LiquidCrystal_I2C.h> //for the lcd display
#include <SPI.h>
#include <RF24.h> //for the RF board

#define LED_PIN 14 // NeoPixel LED strip
#define NUM_LEDS 8 // Number of LEDs

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

// Create an instance of the Adafruit_NeoPixel class
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800); 
//set the different color codes
uint32_t green = strip.Color(0, 255, 0);
uint32_t red = strip.Color(255, 0, 0);
uint32_t black = strip.Color(0, 0, 0);
uint32_t purple = strip.Color(128, 0, 128);
uint32_t blue = strip.Color(0, 0, 255);



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
  
  //LED
  strip.begin(); // Initialize the NeoPixel strip
  strip.show(); // Set initial color to black
  strip.setBrightness(100); // set brightness ONCE here, 0-255

}

void loop(){
  
//clear the led before each time? clear the row in the beginning? before each switch in equation? 
  
  equationDisplay();
  if (count == 4){
    equationFourLoop();
  }else if (count ==3){
    zoneThreeloop();
  }else if (count ==1 ){
    zoneOneloop();
  }else if (count ==2){
    zoneTwoLoop();
  }else{
    Serial.println("At zero");
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
      delayMicroseconds(100); // Short wait for signal detection
      if (radio.testRPD()) {  // Returns true if signal > -64dBm
        values[i]++;
      }
      radio.stopListening();
    }
  }

  strip.fill(black, 0);
  strip.show();
  int totalBars = 0;
  // Print the "Graph" to Serial Monitor
   for (int i = 0; i < num_channels; i++) {
    uint8_t bar_height = values[i] / 4; // Scale for display
    if (bar_height > 0) {
      totalBars += bar_height;   
    }
  }


  clearRow(1);
  lcd.print("Strength:");
  lcd.print(totalBars);

  delay(200);
  //NEW ADDED ADITION FOR LED
  int numLightLit = totalBars/10;
  if(numLightLit ==0){
    numLightLit=1;
  }
  strip.fill(red, 0, numLightLit);
  strip.show();

  //Serial.print(numLightLit);
 // Serial.print(":");
  //Serial.println(totalBars);


}


void equationDisplay()
{
   
  clearRow(0);
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


void zoneOneloop() {
  // Take 8 readings and average them
  // This smooths out the noise — one reading alone is jumpy
  int total = 0;
  for (int i = 0; i < 8; i++) {
    total += analogRead(36); // GPIO36
    delay(2);
  }
  int average = total / 8;

 //Show the reading on the lcd
  clearRow(1);
  lcd.print("Strength:");
  lcd.print(average);

  //new light, lighting up the amount of lights
  int numLightLit = average/105;

  if(numLightLit == 0){
    numLightLit = 1;
  }

  Serial.print(average);
  Serial.print(":");
  Serial.println(numLightLit);

  strip.fill(green, 0, numLightLit);
  strip.show();

  delay(1000);

  strip.fill(black, 0);
  strip.show();
  
  delay(100);

}

void zoneThreeloop() {
  int reading = analogRead(34);
  // show the reading on the screen
  clearRow(1);
  lcd.setCursor(0,1);
  lcd.print("Strength:");
  lcd.print(reading);

  // Visual bar
  int numLightLit = reading / 100;
   if(numLightLit == 0){
    numLightLit = 1;
  }
  strip.fill(blue, 0, numLightLit);
  strip.show();
  delay(300);
  strip.fill(black, 0);
  strip.show();
  delay(50);

}


void zoneTwoLoop() {
  // Read the internal hall sensor
  int total=0;
  for(int i=0;i<100;i++) {
    total+=hallRead();
  }
  total= total/100;

  //lcd display
   clearRow(1);
  lcd.setCursor(0,1);
  lcd.print("Strength:");
  lcd.print(total);


  //Serial.println(total);
  int numLightLit = abs(total) / 80;
   if(numLightLit == 0){
    numLightLit = 1;
  }
  strip.fill(purple, 0, numLightLit);
  strip.show();
  delay(1000);
  strip.fill(black, 0);
  delay(100);
  
}


void clearRow(int rowNum) {
  lcd.setCursor(0, rowNum);
  lcd.print("                "); // 16 spaces
  lcd.setCursor(0, rowNum); // Optional: reset cursor to 0,0
}

//EVERYtime we change the equation, reset the led to be black the whole thing and then set a small delay
//HOW to turn it off, will it be that quick?



