#include <LiquidCrystal.h>

// TODO
// - [ ] read battery level
// - [ ] display battery level
// - [ ] error when
//   - [ ] below (min threshold - 5%)
//   - [ ] below 0%
//   - [ ] above 100%
// - [ ] magic code to get into admin mode
// - [ ] when in admin mode, can set min level between 40 and 90%
// - [ ] default min level is 70%
// - [ ] happy solarpunk text
//
// - [ ] when below min threshold, switch relay
// - [ ] don't switch relay back until past (min threshold + 5%)


/*******************************************************
using code from https://www.dfrobot.com/wiki/index.php/Arduino_LCD_KeyPad_Shield_(SKU:_DFR0009)
- Mark Bramwell, July 2010

********************************************************/

#define LCD_IN_PIN A0
#define BATTERY_IN_PIN A1

// select the pins used on the LCD panel
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

// define some values used by the panel and buttons
int lcd_key     = 0;
int lcd_key_in  = 0;
#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5

void setup()
{
 setup_lcd();
}

void loop()
{
 double battery = read_battery_voltage();
 lcd.setCursor(0, 0);
 lcd.print(battery);

 lcd.setCursor(9,1);            // move cursor to second line "1" and 9 spaces over
 lcd.print(millis()/1000);      // display seconds elapsed since power-up

 lcd.setCursor(0,1);            // move to the begining of the second line
 lcd_key = read_lcd_buttons();  // read the buttons

 switch (lcd_key)               // depending on which button was pushed, we perform an action
 {
   case btnRIGHT:
     {
     lcd.print("RIGHT ");
     break;
     }
   case btnLEFT:
     {
     lcd.print("LEFT   ");
     break;
     }
   case btnUP:
     {
     lcd.print("UP    ");
     break;
     }
   case btnDOWN:
     {
     lcd.print("DOWN  ");
     break;
     }
   case btnSELECT:
     {
     lcd.print("SELECT");
     break;
     }
     case btnNONE:
     {
     lcd.print("NONE  ");
     break;
     }
 }

}



void setup_lcd () {
  lcd.begin(16, 2);
}

void setup_battery_in () {
  analogReference(INTERNAL); // set reference voltage used for analog input to 1.1v
  pinMode(BATTERY_IN_PIN, INPUT);
}

// read the buttons
int read_lcd_buttons()
{
 lcd_key_in = analogRead(LCD_IN_PIN); // read the value from the sensor 
 if (lcd_key_in > 1000) return btnNONE; // we make this the 1st option for speed reasons since it will be the most likely result
 if (lcd_key_in < 50)   return btnRIGHT;  
 if (lcd_key_in < 250)  return btnUP; 
 if (lcd_key_in < 450)  return btnDOWN; 
 if (lcd_key_in < 650)  return btnLEFT; 
 if (lcd_key_in < 850)  return btnSELECT;  

 return btnNONE; // when all others fail, return this...
}

// ripped from https://hackingmajenkoblog.wordpress.com/2016/02/01/making-accurate-adc-readings-on-the-arduino/
long read_vcc () {
  long result;
  // Read 1.1V reference against AVcc
  ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  delay(2); // Wait for Vref to settle
  ADCSRA |= _BV(ADSC); // Convert
  while (bit_is_set(ADCSRA,ADSC));
  result = ADCL;
  result |= ADCH<<8;
  result = 1125300L / result; // Back-calculate AVcc in mV
  return result;
}

double read_battery_voltage () {
  double vcc = read_vcc() / 1000.0;
  unsigned int battery_in = analogRead(BATTERY_IN_PIN);
  double measured_voltage = (battery_in / 1024.0) * vcc;
  double battery_voltage = mapf(measured_voltage, 0.0, 1.1252, 0.0, 14.8472);
  return battery_voltage;
}

float mapf (float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
