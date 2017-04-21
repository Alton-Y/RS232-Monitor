/*
  Software serial

 */
#include <LiquidCrystal.h>

String inputString = "";         // a string to hold incoming data
volatile int stringComplete = 0;  // "0 false","1 newGPS", "2 STOP" or "3 GO"
const byte ledPin = 13;

int GPSindx1 = 0;
int GPSindx2 = 0;
volatile byte GOstate = HIGH;
volatile byte STOPstate = LOW;

LiquidCrystal lcd(12, 11, 7, 4, 8, 5);

void setup()
{

  // Open serial communication with rs232
  Serial.begin(115200);

  inputString.reserve(200);
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("hello, world!");

  pinMode(ledPin, OUTPUT);

  pinMode(2, INPUT);
  digitalWrite(2, HIGH);
  attachInterrupt(digitalPinToInterrupt(2), GO, RISING);

  pinMode(3, INPUT);
  digitalWrite(3, HIGH);
  attachInterrupt(digitalPinToInterrupt(3), STOP, RISING);
}

void loop()
{
  // while there is data coming in, read it
  // and send to the hardware serial port:
  serialEvent();

  if (stringComplete) {
    
      // print the string when a newline arrives:

      //Serial.println(inputString);

      GPSindx1 = inputString.indexOf('G');
      GPSindx2 = inputString.indexOf('G', GPSindx1 + 1);

      if (GPSindx1 == -1 || GPSindx2 == -1 || (GPSindx1 == -1 && GPSindx2 == -1)) {
        lcd.clear();
        lcd.setCursor(0, 0); //over, down
        lcd.print(inputString);
        delay(500);
        inputString = "";
        stringComplete = 0;
      }
      
      else {
        lcd.clear();
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
        stringComplete = 0;

      }

  
  }

  digitalWrite(ledPin, !GOstate);
}

void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    if (inChar == '\n') {
      stringComplete = 1;
      return;
    }
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:

  }


}


void GO() {
  static unsigned long last_go_time = 0;
  unsigned long go_time = millis();
  if (go_time - last_go_time > 200)
  {
    GOstate = LOW;
    STOPstate = HIGH;
    //SEND GO COMMAND!
    Serial.println("g");
    //stringComplete = 2;
    //serialEvent();
    //lcd.clear();
    //lcd.setCursor(0, 0);
    //lcd.print(inputString);
  }
  last_go_time = go_time;

}


void STOP() {
  static unsigned long last_stop_time = 0;
  unsigned long stop_time = millis();
  if (stop_time - last_stop_time > 200)
  {
    GOstate = HIGH;
    STOPstate = LOW;
    //SEND GO COMMAND!
    Serial.println("s");
    //stringComplete = 3;
    //serialEvent();
    //lcd.clear();
    //lcd.setCursor(0, 0);
    //lcd.print(inputString);
  }
  last_stop_time = stop_time;

}

