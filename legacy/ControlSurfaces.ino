#ifdef CONTROLLER_B

#include <stdlib.h>

#include "ControlSurfaces.h"
#include "Pins.h"
#include "Glyphs.h"
#include "Button.h"
#include "RotaryEncoder.h"
#include "SimConnectInputs.h"


int get_rudder_trim();
int get_elevator_trim();

void update_elevator_trim_display();
void update_rudder_trim_display();
void print_trim_indicator_to_display(LiquidCrystal lcd, int col_count, int cell_width, int row, int trim_position);

void trim_elevator_up(int boost);
void trim_elevator_down(int boost);
void reset_elevator();

void trim_rudder_left(int boost);
void trim_rudder_right(int boost);
void reset_rudder();

void increment_flaps();
void decrement_flaps();
void retract_flaps();

void arm_spoilers();
void extend_spoilers();
void retract_spoilers();

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

#define CONTROLS_STATUS_SIZE  1
char spoilers_armed[CONTROLS_STATUS_SIZE] = {'0'};

LiquidCrystal elevator_trim_lcd = LiquidCrystal(ELEVATOR_TRIM_LCD_PINS);
LiquidCrystal rudder_trim_lcd   = LiquidCrystal(RUDDER_TRIM_LCD_PINS);

PushableRotaryEncoder rudderTrimRotaryEncoder = PushableRotaryEncoder(CHANGE_RUDDER_ENCODER_PIN_A, CHANGE_RUDDER_ENCODER_PIN_B, CHANGE_RUDDER_BUTTON_PIN, 1);
PushableRotaryEncoder elevatorTrimRotaryEncoder = PushableRotaryEncoder(CHANGE_ELEVATOR_ENCODER_PIN_A, CHANGE_ELEVATOR_ENCODER_PIN_B, CHANGE_ELEVATOR_BUTTON_PIN, 1);

Button incrementFlapsButton = Button(FLAPS_SWITCH_PIN_A, 1);
Button decrementFlapsButton = Button(FLAPS_SWITCH_PIN_B, 1);
Button retractFlapsButton = Button(RETRACT_FLAPS_PIN, 1);

Button armSpoilersButton = Button(ARM_SPOILERS_PIN, 1);
Button extendSpoilersButton = Button(EXTEND_SPOILERS_PIN, 1);
Button retractSpoilersButton = Button(RETRACT_SPOILERS_PIN, 1);

Button resetElevatorButton = Button(RESET_ELEVATOR_PIN, 1);

int elevator_trim_updated = 0;
int rudder_trim_updated = 0;
int spoilers_updated = 0;

int trim_rudder_neutral = 0;
int trim_elevator_neutral = 0;


// Public

void control_surface_setup() {
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

  rudderTrimRotaryEncoder.setOnRotateClockwise(trim_rudder_right);
  rudderTrimRotaryEncoder.setOnRotateCounterClockwise(trim_rudder_left);
  rudderTrimRotaryEncoder.setOnClick(reset_rudder);

  elevatorTrimRotaryEncoder.setOnRotateClockwise(trim_elevator_down);
  elevatorTrimRotaryEncoder.setOnRotateCounterClockwise(trim_elevator_up);

  incrementFlapsButton.setOnClick(decrement_flaps);
  decrementFlapsButton.setOnClick(increment_flaps);
  retractFlapsButton.setOnClick(retract_flaps);

  armSpoilersButton.setOnClick(arm_spoilers);
  extendSpoilersButton.setOnClick(extend_spoilers);
  retractSpoilersButton.setOnClick(retract_spoilers);

  resetElevatorButton.setOnClick(reset_elevator);
}


void control_surface_tick() {
  resetElevatorButton.tick();
  rudderTrimRotaryEncoder.tick();
  elevatorTrimRotaryEncoder.tick();
  incrementFlapsButton.tick();
  decrementFlapsButton.tick();
  retractFlapsButton.tick();
  armSpoilersButton.tick();
  extendSpoilersButton.tick();
  retractSpoilersButton.tick();

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

void controls_read_spoilers_armed(char token) {
  store_token(token, spoilers_armed, CONTROLS_STATUS_SIZE, &spoilers_updated);
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

  if (trim_elevator_neutral) {
    reset_elevator();
  }
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

  if (trim_rudder_neutral) {
    reset_rudder();
  }
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



void trim_elevator_up(int boost) {
  Serial.println(TRIM_ELEVATOR_UP);
}


void trim_elevator_down(int boost) {
  Serial.println(TRIM_ELEVATOR_DOWN);
}


void reset_elevator() {
  trim_elevator_neutral = 1;
  int position = get_elevator_trim();
  if (position < 0) {
    trim_elevator_up(0);
  } else if (position > 0) {
    trim_elevator_down(0);
  } else {
    trim_elevator_neutral = 0;
  }
}


void trim_rudder_left(int boost) {
  Serial.println(TRIM_RUDDER_LEFT);
}


void trim_rudder_right(int boost) {
  Serial.println(TRIM_RUDDER_RIGHT);
}


void reset_rudder() {
  trim_rudder_neutral = 1;
  int position = get_rudder_trim();
  if (position < 0) {
    trim_rudder_right(0);
  } else if (position > 0) {
    trim_rudder_left(0);
  } else {
    trim_rudder_neutral = 0;
  }
}


void increment_flaps() {
  Serial.println(INCREMENT_FLAPS);
}


void decrement_flaps() {
  Serial.println(DECREMENT_FLAPS);
}


void retract_flaps() {
  Serial.println(RETRACT_FLAPS);
}


void arm_spoilers() {
  if (spoilers_armed[0] == '1') {
    Serial.println(ARM_SPOILERS_OFF);
  } else {
    Serial.println(ARM_SPOILERS_ON);
  }
}


void extend_spoilers() {
  Serial.println(EXTEND_SPOILERS);
}


void retract_spoilers() {
  Serial.println(RETRACT_SPOILERS);
}


#endif
