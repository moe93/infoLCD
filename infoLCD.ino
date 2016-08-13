/* ***************************************************************
 *  DESCRIPTION:
 *   - A very basic program that displays local weather information
 *     such as (temp, humidity, heat index).
 *  
 *  AUTHOR:
 *   - Mohammad Odeh
 *  
 *  VERSION:
 *   - 0.4
 * ****************************************************************
 *  
 * LCD SETUP:
 *  VSS -> GND
 *  VDD -> +5V
 *  VO  -> Middle pin of potentiometer
 *  RS  -> PIN 7
 *  RW  -> GND
 *  E   -> PIN 8
 *  D4  -> PIN 9
 *  D5  -> PIN 10
 *  D6  -> PIN 11
 *  D7  -> PIN 12
 *  A   -> 220ohm -> PIN 4
 *  K   -> GND
 * 
 * WAKE UP PUSHBUTTON SETUP:
 *  Left pin  -> PIN 3
 *  Right pin -> GND
 * 
 * TEMP. + HUMIDITY SENSOR SETUP:
 *  +   -> VIN
 *  out -> PIN 2 (Also jump a 1Kohm from + to out)
 *  -   -> GND
 *  
 * POTENTIOMETER SEUP:
 *  Left pin  -> VIN
 *  Right Pin -> GND
 */

// Include needed libraries
#include <LowPower.h>
#include <DHT.h>
#include <LiquidCrystal.h>

// Define sensor type and pin
#define DHTPIN 2
#define DHTTYPE DHT22
// Define pushbutton pin + display on/off
#define button 3
#define displaySwitch 4

// LiquidCrystal name(rs, enable, d4, d5, d6, d7)
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  // Initialize temp sensor
  dht.begin();

  // Initialize pushbutton + display on/off
  pinMode(button, INPUT_PULLUP);
  pinMode(displaySwitch, OUTPUT);

  // Initialize LCD to 16x2
  digitalWrite(displaySwitch, HIGH);
  lcd.begin(16, 2);

  // Print greeting message
  lcd.print("Hello, Moe.");
  delay2s();
  lcd.clear();
}

void loop() {
  // Display message
  lcd.print("Going to sleep..");
  lcd.setCursor(0, 1);
  lcd.print("Display OFF");
  delay2s();
  // Turn OFF display
  lcd.noDisplay();
  lcd.clear();
  digitalWrite(displaySwitch, LOW);
  // Create an interrupt service routine (ISR)
  /* Here the interrupt cue is attached to the pin we specify; we tell the atmega to
  // watch for the specified change to occur (changing the pin's state from whatever to LOW)
  // and when that happens call the function (displayON)
  attachInterrupt(digitalPinToInterrupt(2or3), functionToCall, stateChange); */
  attachInterrupt(digitalPinToInterrupt(3), displayON, LOW);
  // Add delay() for stability
  delay(50);
  // powerDown the atmega
  LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF);
  // After interrupt is triggered and displayON is called the MC will continue code execution here
  detachInterrupt(digitalPinToInterrupt(3));
}

/* *************************************************************************** *
 * --------------------------- Auxiliary Functions --------------------------- *
 * *************************************************************************** *
 */

void displayON() {
  // Do readings first as this sensor is cheap and slow
  // Read humidity level
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float c = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(c, h, false);

  // Turn ON and display message
  lcd.print("Display ON");
  lcd.display();
  digitalWrite(displaySwitch, HIGH);
  delay(1500);
  lcd.clear();

  // Print greeting message
  lcd.print("Hello, Moe.");
  delay(1500);
  lcd.clear();

  //1st column, 1st row
  lcd.setCursor(0, 0);
  // print float with 1 decimal precision
  lcd.print(c, 1);
  lcd.print("C");
  delay(750);

  // 1st column, 2nd row
  lcd.setCursor(0, 1);
  lcd.print(f, 1);
  lcd.print("F");
  delay(750);

  // 5th column, 1st row
  lcd.setCursor(6, 0);
  lcd.print(hic, 1);
  lcd.print("HtIndx");
  delay(750);

  // 5th column, 2nd row
  lcd.setCursor(6, 1);
  lcd.print(h, 1);
  lcd.print("Hmdty");

  // Delay for stability
  delay(2500);
  lcd.clear();
}

void delay2s(){
  LowPower.powerDown(SLEEP_2S, ADC_OFF, BOD_OFF);
}