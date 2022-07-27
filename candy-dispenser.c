#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <LiquidCrystal.h>
#include <EEPROM.h>

const int buttonPin = 9; // the number of the pushbutton pin
const int potPin = A0; // potentiometer is connected to analog 0 pin
const int sensorPin = 4;
const int tapServoPin = 5;
const int startingCandies = 3;

LiquidCrystal_I2C lcd(0x27,16,2);
Servo tapServo;
int candyCount = 0;
bool fillMore = false;

void setup()
{
  Serial.begin(9600); // initialize the serial communication

  pinMode(sensorPin,INPUT);
  pinMode(buttonPin, INPUT);

  tapServo.attach(tapServoPin);

  lcd.init(); // initialize the lcd
  lcd.backlight(); // Print a message to the LCD.

  int savedCandies = EEPROM[0];
  if (savedCandies <= 0)
    savedCandies = startingCandies;

  updateCandies(savedCandies);
}

void printCandies()
{
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Candies left: ");
  lcd.setCursor(14,0);
  lcd.print(candyCount);
}

void updateCandies(int candies)
{
  candyCount = candies;
  EEPROM[0] = candyCount;

  fillMore = (candyCount <= 0)

  if (!fillMore)
  {
    printCandies();
    return;
  }

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(" Fill more!" );
}

void loop()
{
  if (fillMore) // Empty
  {
    int potValue = analogRead(potPin); // get a reading from the potentiometer, assign the name potValue

    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(potValue);

    int buttonState = digitalRead(buttonPin); // Read button state

    if (buttonState == HIGH) // button push
      updateCandies(potValue);

    return;
  }

  // Sensor + Servo

  int sensorVal = digitalRead(sensorPin);

  if (sensorVal == 0) // Hand-over
  {
     tapServo.write(180); // Set servo arm to 180 degrees
     updateCandies(candyCount - 1);
     return;
  }

  tapServo.write(0); // Reset servo arm
}
