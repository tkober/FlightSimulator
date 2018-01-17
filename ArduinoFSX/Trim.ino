#include <stdlib.h>
#include "Trim.h"

#include "Pins.h"
#include "SimConnectInputs.h"
#include "Glyphs.h"
#include "Button.h"

int get_rudder_trim();
int get_elevator_trim();

void update_elevator_trim_display();
void update_rudder_trim_display();
void print_trim_indicator_to_display(LiquidCrystal lcd, int col_count, int cell_width, int row, int trim_position);

void trim_elevator_up(int boost);
void trim_elevator_down(int boost);

void trim_rudder_left(int boost);
void trim_rudder_right(int boost);

#define LCD_CELL_WIDTH                5

#define ELEVATOR_TRIM_LCD_ROW_COUNT   2
#define ELEVATOR_TRIM_LCD_COL_COUNT   16
#define RUDDER_TRIM_LCD_ROW_COUNT     2
#define RUDDER_TRIM_LCD_COL_COUNT     16

#define TRIM_INDICATOR_POSITION_0_GLYPH_INDEX 0
#define TRIM_INDICATOR_POSITION_1_GLYPH_INDEX 1
#define TRIM_INDICATOR_POSITION_2_GLYPH_INDEX 2
#define TRIM_INDICATOR_POSITION_3_GLYPH_INDEX 3
#define TRIM_INDICATOR_POSITION_4_GLYPH_INDEX 4
#define TRIM_INDICATOR_CLEAR_GLYPH_INDEX      5
#define TRIM_INDICATOR_TO_SCALE_1_GLYPH_INDEX 6
#define TRIM_INDICATOR_TO_SCALE_2_GLYPH_INDEX 7

#define TRIM_INDICATOR_CLEAR_GLYPH        byte(TRIM_INDICATOR_CLEAR_GLYPH_INDEX)
#define TRIM_INDICATOR_POSITION_0_GLYPH   byte(TRIM_INDICATOR_POSITION_0_GLYPH_INDEX)
#define TRIM_INDICATOR_POSITION_1_GLYPH   byte(TRIM_INDICATOR_POSITION_1_GLYPH_INDEX)
#define TRIM_INDICATOR_POSITION_2_GLYPH   byte(TRIM_INDICATOR_POSITION_2_GLYPH_INDEX)
#define TRIM_INDICATOR_POSITION_3_GLYPH   byte(TRIM_INDICATOR_POSITION_3_GLYPH_INDEX)
#define TRIM_INDICATOR_POSITION_4_GLYPH   byte(TRIM_INDICATOR_POSITION_4_GLYPH_INDEX)
#define TRIM_INDICATOR_TO_SCALE_1_GLYPH   byte(TRIM_INDICATOR_TO_SCALE_1_GLYPH_INDEX)
#define TRIM_INDICATOR_TO_SCALE_2_GLYPH   byte(TRIM_INDICATOR_TO_SCALE_2_GLYPH_INDEX)

#define TRIM_SIZE             4
char rudder_trim[TRIM_SIZE]   = {'-', '-', '-', '-'};
char elevator_trim[TRIM_SIZE] = {'-', '-', '-', '-'};

LiquidCrystal elevator_trim_lcd = LiquidCrystal(ELEVATOR_TRIM_LCD_PINS);
LiquidCrystal rudder_trim_lcd   = LiquidCrystal(RUDDER_TRIM_LCD_PINS);

Button rudderLeftButton   = Button(RUDDER_LEFT_BUTTON_PIN, 1);
Button rudderRightButton  = Button(RUDDER_RIGHT_BUTTON_PIN, 1);
Button elevatorUpButton   = Button(ELEVATOR_LEFT_BUTTON_PIN, 1);
Button elevatorDownButton = Button(ELEVATOR_RIGHT_BUTTON_PIN, 1);

int elevator_trim_updated = 0;
int rudder_trim_updated = 0;

// Public

void trim_setup() {
  elevator_trim_lcd.createChar(TRIM_INDICATOR_POSITION_0_GLYPH_INDEX, TRIM_INDICATOR_POSITION_0);
  elevator_trim_lcd.createChar(TRIM_INDICATOR_POSITION_1_GLYPH_INDEX, TRIM_INDICATOR_POSITION_1);
  elevator_trim_lcd.createChar(TRIM_INDICATOR_POSITION_2_GLYPH_INDEX, TRIM_INDICATOR_POSITION_2);
  elevator_trim_lcd.createChar(TRIM_INDICATOR_POSITION_3_GLYPH_INDEX, TRIM_INDICATOR_POSITION_3);
  elevator_trim_lcd.createChar(TRIM_INDICATOR_POSITION_4_GLYPH_INDEX, TRIM_INDICATOR_POSITION_4);
  elevator_trim_lcd.createChar(TRIM_INDICATOR_CLEAR_GLYPH_INDEX, TRIM_INDICATOR_CLEAR);
  elevator_trim_lcd.createChar(TRIM_INDICATOR_TO_SCALE_1_GLYPH_INDEX, TRIM_INDICATOR_TO_SCALE_1);
  elevator_trim_lcd.createChar(TRIM_INDICATOR_TO_SCALE_2_GLYPH_INDEX, TRIM_INDICATOR_TO_SCALE_2);

  rudder_trim_lcd.createChar(TRIM_INDICATOR_POSITION_0_GLYPH_INDEX, TRIM_INDICATOR_POSITION_0);
  rudder_trim_lcd.createChar(TRIM_INDICATOR_POSITION_1_GLYPH_INDEX, TRIM_INDICATOR_POSITION_1);
  rudder_trim_lcd.createChar(TRIM_INDICATOR_POSITION_2_GLYPH_INDEX, TRIM_INDICATOR_POSITION_2);
  rudder_trim_lcd.createChar(TRIM_INDICATOR_POSITION_3_GLYPH_INDEX, TRIM_INDICATOR_POSITION_3);
  rudder_trim_lcd.createChar(TRIM_INDICATOR_POSITION_4_GLYPH_INDEX, TRIM_INDICATOR_POSITION_4);
  rudder_trim_lcd.createChar(TRIM_INDICATOR_CLEAR_GLYPH_INDEX, TRIM_INDICATOR_CLEAR);

  elevator_trim_lcd.begin(ELEVATOR_TRIM_LCD_COL_COUNT, ELEVATOR_TRIM_LCD_ROW_COUNT);
  rudder_trim_lcd.begin(RUDDER_TRIM_LCD_COL_COUNT, RUDDER_TRIM_LCD_ROW_COUNT);
  update_elevator_trim_display();
  update_rudder_trim_display();

  rudderLeftButton.setOnClick(trim_rudder_left);
  rudderRightButton.setOnClick(trim_rudder_right);
  elevatorUpButton.setOnClick(trim_elevator_up);
  elevatorDownButton.setOnClick(trim_elevator_down);
}


void trim_tick() {
  rudderLeftButton.tick();
  rudderRightButton.tick();
  elevatorUpButton.tick();
  elevatorDownButton.tick();

  if (elevator_trim_updated == 1) {
    elevator_trim_updated = 0;
    update_elevator_trim_display();
  }
  if (rudder_trim_updated == 1) {
    rudder_trim_updated = 0;
    update_rudder_trim_display();
  }
}


void read_elevator_trim(char token) {
  store_token(token, elevator_trim, TRIM_SIZE, &elevator_trim_updated);
}


void read_ruddert_trim(char token) {
  store_token(token, rudder_trim, TRIM_SIZE, &rudder_trim_updated);
}

// Private

int get_rudder_trim() {
  char sign = rudder_trim[0];
  char digit1 = rudder_trim[1];
  char digit2 = rudder_trim[2];
  char digit3 = rudder_trim[3];
  char *value_string = "+000";
  value_string[0] = sign;
  value_string[1] = digit1;
  value_string[2] = digit2;
  value_string[3] = digit3;
  return atoi(value_string);
}


int get_elevator_trim() {
  char sign = elevator_trim[0];
  char digit1 = elevator_trim[1];
  char digit2 = elevator_trim[2];
  char digit3 = elevator_trim[3];
  char *value_string = "+000";
  value_string[0] = sign;
  value_string[1] = digit1;
  value_string[2] = digit2;
  value_string[3] = digit3;
  return atoi(value_string);
}


void update_elevator_trim_display() {
  char sign = elevator_trim[0];
  char digit1 = elevator_trim[1];
  char digit2 = elevator_trim[2];
  char digit3 = elevator_trim[3];
  char *value_string = "+000";
  value_string[0] = sign;
  value_string[1] = digit1;
  value_string[2] = digit2;
  value_string[3] = digit3;
  int trim_position = get_elevator_trim();

  elevator_trim_lcd.clear();
  elevator_trim_lcd.setCursor(0, 0);
  elevator_trim_lcd.write("DWN");
  elevator_trim_lcd.write(trim_position < 0 ? "<<" : "  ");
  elevator_trim_lcd.write(" ");
  if (digit1 == '0' && digit2 == '0') {
    elevator_trim_lcd.write(' ');
    elevator_trim_lcd.write(digit3);
    elevator_trim_lcd.write("% ");
  } else {
    elevator_trim_lcd.write(digit1 != '0' ? digit1 : ' ');
    elevator_trim_lcd.write(digit2);
    elevator_trim_lcd.write(digit3);
    elevator_trim_lcd.write('%');
  }
  elevator_trim_lcd.write("  ");
  elevator_trim_lcd.write(trim_position > 0 ? ">>" : "  ");
  elevator_trim_lcd.write("UP");
  print_trim_indicator_to_display(elevator_trim_lcd, ELEVATOR_TRIM_LCD_COL_COUNT, LCD_CELL_WIDTH, 1, trim_position);
}


void update_rudder_trim_display() {
  char sign = rudder_trim[0];
  char digit1 = rudder_trim[1];
  char digit2 = rudder_trim[2];
  char digit3 = rudder_trim[3];
  char *value_string = "+000";
  value_string[0] = sign;
  value_string[1] = digit1;
  value_string[2] = digit2;
  value_string[3] = digit3;
  int trim_position = atoi(value_string);

  rudder_trim_lcd.clear();
  rudder_trim_lcd.setCursor(0, 0);
  rudder_trim_lcd.write("L");
  rudder_trim_lcd.write(trim_position < 0 ? "<<" : "  ");
  rudder_trim_lcd.write("   ");
  if (digit1 == '0' && digit2 == '0') {
    rudder_trim_lcd.write(' ');
    rudder_trim_lcd.write(digit3);
    rudder_trim_lcd.write("% ");
  } else {
    rudder_trim_lcd.write(digit1 != '0' ? digit1 : ' ');
    rudder_trim_lcd.write(digit2);
    rudder_trim_lcd.write(digit3);
    rudder_trim_lcd.write('%');
  }
  rudder_trim_lcd.write("   ");
  rudder_trim_lcd.write(trim_position > 0 ? ">>" : "  ");
  rudder_trim_lcd.write('R');
  print_trim_indicator_to_display(rudder_trim_lcd, RUDDER_TRIM_LCD_COL_COUNT, LCD_CELL_WIDTH, 1, trim_position);
}


void print_trim_indicator_to_display(LiquidCrystal lcd, int col_count, int cell_width, int row, int trim_position) {
  lcd.setCursor(0, row);
  float step_size = (col_count * cell_width / 200.0f);
  int position = ceil((trim_position + 100) * step_size);
  position = position >= col_count * cell_width ? col_count * cell_width - 1 : position;

  for (int i = 0; i < col_count; i++) {
    int from = i * cell_width;
    int to = ((i + 1) * cell_width) - 1;
    if (position >= from && position <= to) {
      int marker = position % cell_width;
      switch (marker) {
        case 0:
          lcd.write(TRIM_INDICATOR_POSITION_0_GLYPH);
          break;

        case 1:
          lcd.write(TRIM_INDICATOR_POSITION_1_GLYPH);
          break;

        case 2:
          lcd.write(TRIM_INDICATOR_POSITION_2_GLYPH);
          break;

        case 3:
          lcd.write(TRIM_INDICATOR_POSITION_3_GLYPH);
          break;

        case 4:
          lcd.write(TRIM_INDICATOR_POSITION_4_GLYPH);
          break;
      }
    } else {
      lcd.write(TRIM_INDICATOR_CLEAR_GLYPH);
    }
  }
}


void trim_elevator_up(int buttonId) {
  Serial.println(TRIM_ELEVATOR_UP);
}


void trim_elevator_down(int buttonId) {
  Serial.println(TRIM_ELEVATOR_DOWN);
}


void trim_rudder_left(int buttonId) {
  Serial.println(TRIM_RUDDER_LEFT);
}


void trim_rudder_right(int buttonId) {
  Serial.println(TRIM_RUDDER_RIGHT);
}
