#include <LowPower.h>
#include <DHT.h>
#include <LiquidCrystal.h>

// Define sensor type and pin
#define DHTPIN 2
#define DHTTYPE DHT22
// Define pushbutton pin + display on/off
#define button 3
#define displaySwitch 4

// LiquidCrystal(rs, enable, d4, d5, d6, d7)
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
  delay(1000);
}

void loop() {
  lcd.clear();
  lcd.print("Display OFF");
  delay(1000);
  lcd.noDisplay();
  digitalWrite(displaySwitch, LOW);
  // Create an interrupt service routine (ISR)
  attachInterrupt(digitalPinToInterrupt(3), displayON, LOW);
  LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF);
  detachInterrupt(digitalPinToInterrupt(3));
}

void displayON() {
  digitalWrite(displaySwitch, HIGH);
  lcd.clear();
  lcd.display();
  delay(1000);
  // Read humidity level
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float c = dht.readTemperature();
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(c, h, false);
  lcd.print("Display ON");
  delay(1000);
  lcd.clear();
  //1st column, 1st row
  lcd.setCursor(0, 0);
  // print float with 1 decimal precision
  lcd.print(c, 1);
  lcd.print("C");
  delay(500);

  // 6th column, 1st row
  lcd.setCursor(7, 0);
  lcd.print(h, 1);
  lcd.print("Humid");
  delay(500);

  // 1st column, 2nd row
  lcd.setCursor(0, 1);
  lcd.print(hic, 1);
  lcd.print(" H.I.");

  // Delay for stability
  delay(5000);
  lcd.clear();
}
