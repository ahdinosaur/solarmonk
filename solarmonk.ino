#include <LiquidCrystal.h>

// TODO
// - [x] read battery level
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
// - [ ] read amperage
//
// - [ ] when below min threshold, switch relay
//   - keep track of whether load is on
// - [ ] don't switch relay back until past (min threshold + 5%)
//   - if load is on, give more wiggle room


/*******************************************************
using code from https://www.dfrobot.com/wiki/index.php/Arduino_LCD_KeyPad_Shield_(SKU:_DFR0009)
- Mark Bramwell, July 2010

********************************************************/

#define V_REF 1.1

#define PIN_LCD_IN A0

// 14.8472 = Vin when solving for
//   R2  = R1   * (1 / ((Vin / Vout) - 1)
//   820 = 10e3 * (1 / ((Vin / 1.1) - 1))
// which is the resistive divider equation being used
#define V_BATTERY_MAX 14.8472
#define PIN_BATTERY_IN A1

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

double battery_charge;
double battery_voltage;
int battery_status;
#define BATTERY_HEALTHY 0
#define BATTERY_BELOW_MIN 1
#define BATTERY_BELOW_THRESHOLD 2
#define BATTERY_ABOVE_MAX 3

double threshold = 0.5;

void setup()
{
 setup_lcd();
 setup_battery_in();
}

void loop()
{ 
 lcd_key = read_lcd_buttons();

 battery_voltage = read_battery_voltage();
 battery_charge = get_battery_charge(battery_voltage);
 battery_status = get_battery_status(battery_charge);
 print_battery_status(battery_status, battery_charge);
 print_battery_voltage(battery_voltage);

 delay(100);
}



void setup_lcd () {
  lcd.begin(16, 2);
}

void setup_battery_in () {
  analogReference(INTERNAL); // set reference voltage used for analog input to 1.1v
  pinMode(PIN_BATTERY_IN, INPUT);
}

// read the buttons
int read_lcd_buttons()
{
 lcd_key_in = analogRead(PIN_LCD_IN); // read the value from the sensor 
 if (lcd_key_in > 1000) return btnNONE; // we make this the 1st option for speed reasons since it will be the most likely result
 if (lcd_key_in < 50)   return btnRIGHT;  
 if (lcd_key_in < 250)  return btnUP; 
 if (lcd_key_in < 450)  return btnDOWN; 
 if (lcd_key_in < 650)  return btnLEFT; 
 if (lcd_key_in < 850)  return btnSELECT;  

 return btnNONE; // when all others fail, return this...
}

double read_battery_voltage () {
  unsigned int battery_in = analogRead(PIN_BATTERY_IN);
  double measured_voltage = (battery_in / 1024.0) * V_REF;
  double battery_voltage = mapf(measured_voltage, 0.0, V_REF, 0.0, V_BATTERY_MAX);
  return battery_voltage;
}

double get_battery_charge (double battery_voltage) {
  double battery_charge = mapf(battery_voltage, 11.8, 12.7, 0, 1);
  return battery_charge;
}

void print_battery_status (int battery_status, double battery_charge) {
 switch (battery_status) {
   case BATTERY_HEALTHY:
     lcd.setCursor(0, 0);
     lcd.print(battery_charge * 100);
     lcd.setCursor(5, 0);
     lcd.print('%');
     print_clear(6, 0, 9);
     break;
   case BATTERY_BELOW_MIN:
     lcd.setCursor(0, 0);
     lcd.print("BELOW MIN!");
     print_clear(10, 0, 5);
     break;
   case BATTERY_BELOW_THRESHOLD:
     lcd.setCursor(0, 0);
     lcd.print("BELOW THRESHOLD!");
     break;
   case BATTERY_ABOVE_MAX:
     lcd.setCursor(0, 0);
     lcd.print("ABOVE MAX");
     print_clear(10, 0, 5);
     break;
 }
}


int get_battery_status (double battery_charge) {
  if (battery_charge <= 0) {
    return BATTERY_BELOW_MIN;
  } else if (battery_charge < threshold) {
    return BATTERY_BELOW_THRESHOLD;
  } else if (battery_charge > 1) {
    return BATTERY_ABOVE_MAX;
  } else {
    return BATTERY_HEALTHY;
  }
}
void print_battery_voltage (double battery_voltage) {
  lcd.setCursor(0, 1);
  lcd.print(battery_voltage);
  lcd.setCursor(5, 2);
  lcd.print('V');
}

float mapf (float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void print_clear (int col, int row, int len) {
  lcd.setCursor(col, row);
  for (int i = 0; i < len; i++) {
    lcd.write(' ');
  }
}

