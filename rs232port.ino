/*
  Software serial

 */
#include <LiquidCrystal.h>

String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
void setup()
{

  // Open serial communications
  Serial.begin(115200);

  inputString.reserve(200);
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("hello, world!");
}

void loop()
{
  // while there is data coming in, read it
  // and send to the hardware serial port:
  serialEvent();

  // print the string when a newline arrives:
  if (stringComplete) {

    //Serial.println(inputString);
    lcd.clear();


    int GPSindx1 = inputString.indexOf('G');
    int GPSindx2 = inputString.indexOf('G', GPSindx1 + 1);

    lcd.setCursor(0, 0); //over, down
    //lcd.print(inputString.length());
    for (int i = GPSindx1; i < (GPSindx2); i++) {
      lcd.print(inputString[i]);
    }

    lcd.setCursor(0, 1); //over, down
    for (int i = GPSindx2; i < inputString.length() - 1; i++) {
      lcd.print(inputString[i]);
    }

    // clear the string:
    inputString = "";
    stringComplete = false;
  }

}

void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    if (inChar == '\n') {
      stringComplete = true;
      return;
    }
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:

  }
}




