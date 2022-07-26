#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <LiquidCrystal.h>
#include <EEPROM.h>

const int buttonPin = 9; // the number of the pushbutton pin
const int potPin = A0; // potentiometer is connected to analog 0 pin
const int sensor_pin = 4;
const int tap_servo_pin = 5;

LiquidCrystal_I2C lcd(0x27,16,2);
Servo tap_servo;
int candy_count = 0;
bool fill_more = false;

void setup()
{
  Serial.begin(9600); // initialize the serial communication

  pinMode(sensor_pin,INPUT);
  pinMode(buttonPin, INPUT);

  tap_servo.attach(tap_servo_pin);

  lcd.init(); // initialize the lcd
  lcd.backlight(); // Print a message to the LCD.

  candy_count = EEPROM[0];

  if (candy_count <= 0)
  {
    candy_count = 3;
    EEPROM[0] = candy_count;
  }

  printCandies();
}

void printCandies()
{
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Candies left: ");
  lcd.setCursor(14,0);
  lcd.print(candy_count);
}

void updateCandies(int count)
{
  candy_count += count;
  EEPROM[0] = candy_count;

  fill_more = (candy_count <= 0)

  if (!fill_more)
  {
    printCandies();
    return;
  }

  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(" Fill more!" );
  delay(900);
}

void loop()
{
  if (fill_more) // Empty
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

  int sensorVal = digitalRead(sensor_pin);

  if (sensorVal == 0) // Hand-over
  {
     tap_servo.write(180); // Set servo arm to 180 degrees
     updateCandies(-1);
     delay(900);
     return;
  }

  tap_servo.write(0); // Reset servo arm
}
