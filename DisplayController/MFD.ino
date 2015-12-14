#include "MFD.h"


void print_string_to_lcd(LiquidCrystal lcd, char string[], int size) {
  for (int i = 0; i < size; i++) {
    lcd.write(string[i]);
  }
}

