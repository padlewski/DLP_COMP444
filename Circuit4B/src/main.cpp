#include <Arduino.h>

#include <LiquidCrystal.h>                  //the liquid crystal library contains commands for printing to the display
LiquidCrystal lcd(13, 12, 11, 10, 9, 8);    // tell the RedBoard what pins are connected to the display


#ifdef _EXPERIMENT_1_

float voltage = 0;
float degreesC = 0;
float degreesK = 0;
static const float K = 273.15f;

void setup() {

  lcd.begin(16, 2);
  lcd.clear();
}

void loop() {

  voltage = analogRead(A0) * 0.004882813;
  degreesC = (voltage - 0.5) * 100.0;
  // https://www.rapidtables.com/convert/temperature/how-celsius-to-kelvin.html
  degreesK = degreesC + K;

  lcd.clear();

  lcd.setCursor(0, 0);
  lcd.print("Degrees C: ");
  lcd.print(degreesC);

  lcd.setCursor(0, 1);  
  lcd.print("Degrees K: ");
  lcd.print(degreesK);

  delay(1000);  
}

#elif _EXPERIMENT_2_

float voltage = 0;
float degreesC = 0;
int l_val = 0;
int r_val = 0;
byte bar[8] = {
  B11011, B11011, B11011, B11011, B11011, B11011, B11011, B11011
};
static const byte i_bar = B00000000;
static const byte i_bar_left = B00000001;
static const int segs = 16;
char buff[segs];
int segments = 0;
String format = "%02d    %02dc    %02d";

void setup() {
  lcd.begin(16, 2);
  lcd.clear();
  lcd.createChar(0, bar);
}

void loop() {

  voltage = analogRead(A0) * 0.004882813;
  degreesC = (voltage - 0.5) * 100.0;
  if(degreesC < l_val || degreesC > r_val) {
    l_val = degreesC - 2;
    r_val = degreesC + 2;
    snprintf(buff, segs, format.c_str(), l_val, int(degreesC), r_val);
  }
  segments = ((degreesC - l_val) / 5) * 16;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(String(buff));
  lcd.setCursor(0, 1);  
  for (int i = 0 ; i < segments ; ++i) lcd.write(i_bar);
  delay(1000);  
}

#else

float voltage = 0;                          //the voltage measured from the TMP36
float degreesC = 0;                         //the temperature in Celsius, calculated from the voltage
float degreesF = 0;                         //the temperature in Fahrenheit, calculated from the voltage

void setup() {

  lcd.begin(16, 2);                         //tell the lcd library that we are using a display that is 16 characters wide and 2 characters high
  lcd.clear();                              //clear the display
}

void loop() {

  voltage = analogRead(A0) * 0.004882813;   //convert the analog reading, which varies from 0 to 1023, back to a voltage value from 0-5 volts
  degreesC = (voltage - 0.5) * 100.0;       //convert the voltage to a temperature in degrees Celsius
  degreesF = degreesC * (9.0 / 5.0) + 32.0; //convert the voltage to a temperature in degrees Fahrenheit

  lcd.clear();                              //clear the LCD

  lcd.setCursor(0, 0);                      //set the cursor to the top left position
  lcd.print("Degrees C: ");                 //print a label for the data
  lcd.print(degreesC);                      //print the degrees Celsius

  lcd.setCursor(0, 1);                      //set the cursor to the lower left position
  lcd.print("Degrees F: ");                 //Print a label for the data
  lcd.print(degreesF);                      //print the degrees Fahrenheit

  delay(1000);                              //delay for 1 second between each reading (this makes the display less noisy)
}

#endif