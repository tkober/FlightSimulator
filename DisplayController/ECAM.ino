#ifdef CONTROLLER_A

#include "ECAM.h"
#include "Glyphs.h"
#include "MFD.h"
#include "Keypad.h"
#include "Button.h"
#include "Pins.h"
#include "Math.h"


void keypad_tick();

void previous_ecam();
void next_ecam();
void scroll_up();
void scroll_down();

void render_ecam_1();
void render_labels();
void render_engine_1(int n1, int egt, int ff, int thr);
void render_engine_2(int n1, int egt, int ff, int thr);
void render_thrust_lever(int col, int thr);

extern ECAMPage control_surfaces_page;
void control_surfaces_page_will_appear();
void control_surfaces_page_render();
void control_surfaces_page_update_if_necessary();

extern ECAMPage fuel_page;
void fuel_page_will_appear();
void fuel_page_render();
void fuel_page_update_if_necessary();

extern ECAMPage in_flight_settings_page;
void in_flight_settings_page_will_appear();
void in_flight_settings_page_render();
void in_flight_settings_page_update_if_necessary();
void in_flight_settings_page_scroll_down();
void in_flight_settings_page_scroll_up();
void in_flight_settings_page_handle_keypad_input(char key);


ECAMPage control_surfaces_page = ECAM_PAGE_MAKE(
  &in_flight_settings_page,
  &fuel_page,
  control_surfaces_page_will_appear,
  control_surfaces_page_render,
  control_surfaces_page_update_if_necessary,
  NULL,
  NULL,
  NULL,
  NULL
);

ECAMPage fuel_page = ECAM_PAGE_MAKE(
  &control_surfaces_page,
  &in_flight_settings_page,
  fuel_page_will_appear,
  fuel_page_render,
  fuel_page_update_if_necessary,
  NULL,
  NULL,
  NULL,
  NULL
);

ECAMPage in_flight_settings_page = ECAM_PAGE_MAKE(
  &in_flight_settings_page,
  &control_surfaces_page,
  in_flight_settings_page_will_appear,
  in_flight_settings_page_render,
  in_flight_settings_page_update_if_necessary,
  NULL,
  in_flight_settings_page_scroll_down,
  in_flight_settings_page_scroll_up,
  in_flight_settings_page_handle_keypad_input
);

#define ECAM_LCD_ROW_COUNT   4
#define ECAM_LCD_COL_COUNT   20

#define FLAPS_TOP_INDEX                 0
#define FLAPS_MIDDLE_INDEX              1
#define FLAPS_BOTTOM_INDEX              2
#define SPOILER_BOTTOM_RETRACTED_INDEX  3
#define SPOILER_BOTTOM_1_INDEX          4
#define SPOILER_BOTTOM_2_INDEX          5
#define SPOILER_BOTTOM_FULL_INDEX       6
#define SPOILER_TOP_FULL_INDEX          7

#define FLAPS_TOP_GLYPH                 byte(FLAPS_TOP_INDEX)
#define FLAPS_MIDDLE_GLYPH              byte(FLAPS_MIDDLE_INDEX)
#define FLAPS_BOTTOM_GLYPH              byte(FLAPS_BOTTOM_INDEX)
#define SPOILER_BOTTOM_RETRACTED_GLYPH  byte(SPOILER_BOTTOM_RETRACTED_INDEX)
#define SPOILER_BOTTOM_1_GLYPH          byte(SPOILER_BOTTOM_1_INDEX)
#define SPOILER_BOTTOM_2_GLYPH          byte(SPOILER_BOTTOM_2_INDEX)
#define SPOILER_BOTTOM_FULL_GLYPH       byte(SPOILER_BOTTOM_FULL_INDEX)
#define SPOILER_TOP_FULL_GLYPH          byte(SPOILER_TOP_FULL_INDEX)


#define THRUST_LEVERS_0_INDEX           0
#define THRUST_LEVERS_1_INDEX           1
#define THRUST_LEVERS_2_INDEX           2
#define THRUST_LEVERS_3_INDEX           3
#define THRUST_LEVERS_4_INDEX           4
#define THRUST_LEVERS_5_INDEX           5
#define THRUST_LEVERS_6_INDEX           6
#define THRUST_LEVERS_7_INDEX           7

#define THRUST_LEVERS_0_GLYPH           byte(THRUST_LEVERS_0_INDEX)
#define THRUST_LEVERS_1_GLYPH           byte(THRUST_LEVERS_1_INDEX)
#define THRUST_LEVERS_2_GLYPH           byte(THRUST_LEVERS_2_INDEX)
#define THRUST_LEVERS_3_GLYPH           byte(THRUST_LEVERS_3_INDEX)
#define THRUST_LEVERS_4_GLYPH           byte(THRUST_LEVERS_4_INDEX)
#define THRUST_LEVERS_5_GLYPH           byte(THRUST_LEVERS_5_INDEX)
#define THRUST_LEVERS_6_GLYPH           byte(THRUST_LEVERS_6_INDEX)
#define THRUST_LEVERS_7_GLYPH           byte(THRUST_LEVERS_7_INDEX)


#define FLAPS_POSITION_SIZE                        3
char flaps_position[FLAPS_POSITION_SIZE]         = {'0', '0', '0'};
#define SPOILERS_POSITION_SIZE                     3
char spoilers_position[SPOILERS_POSITION_SIZE]   = {'0', '0', '0'};
#define SPOILERS_ARMED_SIZE                        1
char spoilers_armed[SPOILERS_ARMED_SIZE]         = {'0'};

#define N1_RPM_SIZE                                               5
char n1_rpm_engine_1[N1_RPM_SIZE]                                 = {'0', '0', '0', '0', '0'};
char n1_rpm_engine_2[N1_RPM_SIZE]                                 = {'0', '0', '0', '0', '0'};
#define EGT_SIZE                                                  3
char egt_engine_1[EGT_SIZE]                                       = {'0', '0', '0'};
char egt_engine_2[EGT_SIZE]                                       = {'0', '0', '0'};
#define FUEL_FLOW_SIZE                                            4
char fuel_flow_engine_1[FUEL_FLOW_SIZE]                           = {'0', '0', '0', '0'};
char fuel_flow_engine_2[FUEL_FLOW_SIZE]                           = {'0', '0', '0', '0'};
#define THRUST_LEVERS_POSITION_SIZE                               4
char thrust_levers_position_engine_1[THRUST_LEVERS_POSITION_SIZE] = {'+', '0', '0', '0'};
char thrust_levers_position_engine_2[THRUST_LEVERS_POSITION_SIZE] = {'+', '0', '0', '0'};

#define KEYPAD_ROW_COUNT  4
#define KEYPAD_COL_COUNT  4
#define KEYPAD_KEYS       (char[KEYPAD_ROW_COUNT][KEYPAD_COL_COUNT]){ {'1','2','3','A'}, {'4','5','6','B'}, {'7','8','9','C'}, {'*','0','#','D'} }

Keypad keypad = Keypad( makeKeymap(KEYPAD_KEYS), KEYPAD_ROW_PINS, KEYPAD_COL_PINS, KEYPAD_ROW_COUNT, KEYPAD_COL_COUNT);

LiquidCrystal ecam1_lcd = LiquidCrystal(ECAM_1_LCD_PINS);
LiquidCrystal ecam2_lcd = LiquidCrystal(ECAM_2_LCD_PINS);

Button nextPageButton = Button(NEXT_PAGE_PIN, 1);
Button previousPageButton = Button(PREVIOUS_PAGE_PIN, 1);
Button scrollUpButton= Button(SCROLL_UP_PIN, 1);
Button scrollDownButton  = Button(SCROLL_DOWN_PIN, 1);

ECAMPage *current_ecam_page = &control_surfaces_page;

int control_surfaces_updated = 0;
int flaps_updated = 0;
int spoilers_updated = 0;
int fuel_updated = 0;

int ecam1_updated = 0;


// Public

void ecam_setup() {
  nextPageButton.setOnClick(next_ecam);
  previousPageButton.setOnClick(previous_ecam);
  scrollUpButton.setOnClick(scroll_up);
  scrollDownButton.setOnClick(scroll_down);

  ecam1_lcd.begin(ECAM_LCD_COL_COUNT, ECAM_LCD_ROW_COUNT);
  ecam2_lcd.begin(ECAM_LCD_COL_COUNT, ECAM_LCD_ROW_COUNT);

  ecam1_lcd.createChar(THRUST_LEVERS_0_INDEX, THRUST_LEVERS_POSITION_0);
  ecam1_lcd.createChar(THRUST_LEVERS_1_INDEX, THRUST_LEVERS_POSITION_1);
  ecam1_lcd.createChar(THRUST_LEVERS_2_INDEX, THRUST_LEVERS_POSITION_2);
  ecam1_lcd.createChar(THRUST_LEVERS_3_INDEX, THRUST_LEVERS_POSITION_3);
  ecam1_lcd.createChar(THRUST_LEVERS_4_INDEX, THRUST_LEVERS_POSITION_4);
  ecam1_lcd.createChar(THRUST_LEVERS_5_INDEX, THRUST_LEVERS_POSITION_5);
  ecam1_lcd.createChar(THRUST_LEVERS_6_INDEX, THRUST_LEVERS_POSITION_6);
  ecam1_lcd.createChar(THRUST_LEVERS_7_INDEX, THRUST_LEVERS_POSITION_7);

  render_ecam_1();
  ecam2_force_rerender();
}


void ecam_tick() {
  keypad_tick();
  nextPageButton.tick();
  previousPageButton.tick();
  scrollUpButton.tick();
  scrollDownButton.tick();

  if (current_ecam_page->update_if_necessary != NULL) {
    current_ecam_page->update_if_necessary();
  }

  if (ecam1_updated) {
    ecam1_updated = 0;
    render_ecam_1();
  }
}


void read_n1_engine1(char token) {
  store_token(token, n1_rpm_engine_1, N1_RPM_SIZE, &ecam1_updated);
}


void read_n1_engine2(char token) {
  store_token(token, n1_rpm_engine_2, N1_RPM_SIZE, &ecam1_updated);
}


void read_egt_engine1(char token) {
  store_token(token, egt_engine_1, EGT_SIZE, &ecam1_updated);
}


void read_egt_engine2(char token) {
  store_token(token, egt_engine_2, EGT_SIZE, &ecam1_updated);
}


void read_fuel_flow_engine1(char token) {
  store_token(token, fuel_flow_engine_1, FUEL_FLOW_SIZE, &ecam1_updated);
}


void read_fuel_flow_engine2(char token) {
  store_token(token, fuel_flow_engine_2, FUEL_FLOW_SIZE, &ecam1_updated);
}


void read_thrust_levers_position_engine1(char token) {
  store_token(token, thrust_levers_position_engine_1, THRUST_LEVERS_POSITION_SIZE, &ecam1_updated);
}


void read_thrust_levers_position_engine2(char token) {
  store_token(token, thrust_levers_position_engine_2, THRUST_LEVERS_POSITION_SIZE, &ecam1_updated);
}


void read_flaps_position(char token) {
  store_token(token, flaps_position, FLAPS_POSITION_SIZE, &control_surfaces_updated);
}


void read_spoilers_position(char token) {
  store_token(token, spoilers_position, SPOILERS_POSITION_SIZE, &control_surfaces_updated);
}


void read_spoilers_armed(char token) {
  store_token(token, spoilers_armed, SPOILERS_ARMED_SIZE, &control_surfaces_updated);
}


// Private

void keypad_tick() {
  char customKey = keypad.getKey();
  if (customKey && current_ecam_page->handle_keypad_input != NULL){
    current_ecam_page->handle_keypad_input(customKey);
  }
}


void previous_ecam() {
  current_ecam_page = (ECAMPage *)current_ecam_page->previous_page;
  ecam2_force_rerender();
}


void next_ecam() {
  current_ecam_page = (ECAMPage *)current_ecam_page->next_page;
  ecam2_force_rerender();
}


void ecam2_force_rerender() {
  if (current_ecam_page->will_appear) {
    current_ecam_page->will_appear();
  }
  if (current_ecam_page->render) {
    current_ecam_page->render();
  }
}


void scroll_up() {
  Serial.println("scroll_up()");
}


void scroll_down() {
  Serial.println("scroll_down()");
}


// ECAM 1

void render_ecam_1() {
  int n1 = int_from_string(n1_rpm_engine_1, N1_RPM_SIZE, 0);
  int egt = int_from_string(egt_engine_1, EGT_SIZE, 0);
  int ff = int_from_string(fuel_flow_engine_1, FUEL_FLOW_SIZE, 0);
  int thrust_lever_position = int_from_string(thrust_levers_position_engine_1, THRUST_LEVERS_POSITION_SIZE, 1);
  render_engine_1(n1, egt, ff, thrust_lever_position);

  render_labels();

  n1 = int_from_string(n1_rpm_engine_2, N1_RPM_SIZE, 0);
  egt = int_from_string(egt_engine_2, EGT_SIZE, 0);
  ff = int_from_string(fuel_flow_engine_2, FUEL_FLOW_SIZE, 0);
  thrust_lever_position = int_from_string(thrust_levers_position_engine_2, THRUST_LEVERS_POSITION_SIZE, 1);
  render_engine_2(n1, egt, ff, thrust_lever_position);

}


void render_labels() {
  ecam1_lcd.setCursor(9, 0);
  ecam1_lcd.print("N1");
  ecam1_lcd.setCursor(9, 1);
  ecam1_lcd.print("EGT");
  ecam1_lcd.setCursor(9, 2);
  ecam1_lcd.print("FF");
  ecam1_lcd.setCursor(9, 3);
  ecam1_lcd.print("THR");
}


void render_engine_1(int n1, int egt, int ff, int thr) {
  ecam1_lcd.setCursor(2, 0);
  if (n1 < 10000) {
    ecam1_lcd.print(" ");
  }
  if (n1 < 1000) {
    ecam1_lcd.print(" ");
  }
  if (n1 < 100) {
    ecam1_lcd.print(" ");
  }
  if (n1 < 10) {
    ecam1_lcd.print(" ");
  }
  ecam1_lcd.print(n1);

  ecam1_lcd.setCursor(2, 1);
  ecam1_lcd.print("  ");
  if (egt < 100) {
    ecam1_lcd.print(" ");
  }
  if (egt < 10) {
    ecam1_lcd.print(" ");
  }
  ecam1_lcd.print(egt);

  ecam1_lcd.setCursor(2, 2);
  ecam1_lcd.print(" ");
  if (ff < 1000) {
    ecam1_lcd.print(" ");
  }
  if (ff < 100) {
    ecam1_lcd.print(" ");
  }
  if (ff < 10) {
    ecam1_lcd.print(" ");
  }
  ecam1_lcd.print(ff);

  ecam1_lcd.setCursor(2, 3);
  int thr_abs = thr;
  if (thr < 0) {
    ecam1_lcd.print(" ");
    thr_abs = -thr;
  } else {
    ecam1_lcd.print("  ");
  }
  if (thr_abs < 100) {
    ecam1_lcd.print(" ");
  }
  if (thr_abs < 10) {
    ecam1_lcd.print(" ");
  }
  ecam1_lcd.print(thr);

  render_thrust_lever(0, thr);
}


void render_engine_2(int n1, int egt, int ff, int thr) {
  ecam1_lcd.setCursor(13, 0);
  ecam1_lcd.print(n1);
  if (n1 < 10000) {
    ecam1_lcd.print(" ");
  }
  if (n1 < 1000) {
    ecam1_lcd.print(" ");
  }
  if (n1 < 100) {
    ecam1_lcd.print(" ");
  }
  if (n1 < 10) {
    ecam1_lcd.print(" ");
  }

  ecam1_lcd.setCursor(13, 1);
  ecam1_lcd.print(egt);
  if (egt < 100) {
    ecam1_lcd.print(" ");
  }
  if (egt < 10) {
    ecam1_lcd.print(" ");
  }

  ecam1_lcd.setCursor(13, 2);
  ecam1_lcd.print(ff);
  if (ff < 1000) {
    ecam1_lcd.print(" ");
  }
  if (ff < 100) {
    ecam1_lcd.print(" ");
  }
  if (ff < 10) {
    ecam1_lcd.print(" ");
  }

  ecam1_lcd.setCursor(13, 3);
  ecam1_lcd.print(thr);
  if (thr < 0) {
    ecam1_lcd.print(" ");
  } else {
    if (thr < 100) {
      ecam1_lcd.print(" ");
    }
    if (thr < 10) {
      ecam1_lcd.print(" ");
    }
  }

  render_thrust_lever(19, thr);
}


void render_thrust_lever(int col, int thr) {
  thr = thr < 0 ? -thr : thr;
  int units = thr / 3.125;
  int filled = units / 8;
  int i;
  for (i = 0; i < filled; i++) {
    ecam1_lcd.setCursor(col, 3-i);
    ecam1_lcd.write(THRUST_LEVERS_7_GLYPH);
  }
  ecam1_lcd.setCursor(col, 3-i);
  switch (units % 8) {
    case 0:
    ecam1_lcd.write(THRUST_LEVERS_0_GLYPH);
    break;

    case 1:
    ecam1_lcd.write(THRUST_LEVERS_1_GLYPH);
    break;

    case 2:
    ecam1_lcd.write(THRUST_LEVERS_2_GLYPH);
    break;

    case 3:
    ecam1_lcd.write(THRUST_LEVERS_3_GLYPH);
    break;

    case 4:
    ecam1_lcd.write(THRUST_LEVERS_4_GLYPH);
    break;

    case 5:
    ecam1_lcd.write(THRUST_LEVERS_5_GLYPH);
    break;

    case 6:
    ecam1_lcd.write(THRUST_LEVERS_6_GLYPH);
    break;

    case 7:
    ecam1_lcd.write(THRUST_LEVERS_7_GLYPH);
    break;
  }
  i++;
  while (i < 4) {
    ecam1_lcd.setCursor(col, 3-i);
    ecam1_lcd.write(THRUST_LEVERS_0_GLYPH);
    i++;
  }
}


// Control Surface
void control_surfaces_page_will_appear() {
  ecam2_lcd.createChar(FLAPS_TOP_INDEX, FLAPS_TOP);
  ecam2_lcd.createChar(FLAPS_MIDDLE_INDEX, FLAPS_MIDDLE);
  ecam2_lcd.createChar(FLAPS_BOTTOM_INDEX, FLAPS_BOTTOM);
  ecam2_lcd.createChar(SPOILER_BOTTOM_RETRACTED_INDEX, SPOILER_BOTTOM_RETRACTED);
  ecam2_lcd.createChar(SPOILER_BOTTOM_1_INDEX, SPOILER_BOTTOM_1);
  ecam2_lcd.createChar(SPOILER_BOTTOM_2_INDEX, SPOILER_BOTTOM_2);
  ecam2_lcd.createChar(SPOILER_BOTTOM_FULL_INDEX, SPOILER_BOTTOM_FULL);
  ecam2_lcd.createChar(SPOILER_TOP_FULL_INDEX, SPOILER_TOP_FULL);
  ecam2_lcd.clear();
}


void control_surfaces_page_render() {
  render_spoilers();
  render_flaps();
}


void control_surfaces_page_update_if_necessary() {
  if (control_surfaces_updated == 1) {
    control_surfaces_page_render();
    control_surfaces_updated = 0;
  }
}


void render_spoilers() {
  ecam2_lcd.setCursor(0, 0);
  int spoilers = int_from_string(spoilers_position, SPOILERS_POSITION_SIZE, 0);
  int armed = int_from_string(spoilers_armed, SPOILERS_ARMED_SIZE, 0);
  if (spoilers == 0) {
    render_spoilers_retracted(spoilers, armed);
  } else if (spoilers <= 50) {
    render_spoilers_1(spoilers, armed);
  } else if (spoilers <= 75) {
    render_spoilers_2(spoilers, armed);
  } else {
    render_spoilers_full(spoilers, armed);
  }
}


void render_flaps() {
  ecam2_lcd.setCursor(0, 2);
  ecam2_lcd.print("Flaps            ");
  int flaps = int_from_string(flaps_position, FLAPS_POSITION_SIZE, 0);
  if (flaps < 10) {
    ecam2_lcd.print("  ");
  } else if (flaps < 99) {
    ecam2_lcd.print(" ");
  }
  ecam2_lcd.print(flaps);
  ecam2_lcd.setCursor(0, 3);

  if (flaps < 14) {
    render_flaps_up();
  } else if (flaps < 21) {
    render_flaps_1();
  } else if (flaps < 29) {
    render_flaps_2();
  } else if (flaps < 57) {
    render_flaps_3();
  } else {
    render_flaps_full();
  }
}


// Fuel
void fuel_page_will_appear() {
  ecam2_lcd.clear();
}


void fuel_page_render() {
  ecam2_lcd.setCursor(0, 0);
  ecam2_lcd.write("Fuel");
}


void fuel_page_update_if_necessary() {
  if (fuel_updated == 1) {
    fuel_page_render();
  }
}


// In Flight Settings
void in_flight_settings_page_will_appear() {
  ecam2_lcd.clear();
}


void in_flight_settings_page_render() {
  ecam2_lcd.setCursor(0, 0);
  ecam2_lcd.write("In Flight Settings");
}


void in_flight_settings_page_update_if_necessary() {

}


void in_flight_settings_page_scroll_down() {

}


void in_flight_settings_page_scroll_up() {

}


void in_flight_settings_page_handle_keypad_input(char key) {

}


// Flaps

void render_flaps_up() {
  ecam2_lcd.print("Up  ");
  ecam2_lcd.setCursor(12, 3);
  ecam2_lcd.write(FLAPS_TOP_GLYPH);
  ecam2_lcd.write(FLAPS_TOP_GLYPH);
  ecam2_lcd.write(FLAPS_TOP_GLYPH);
  ecam2_lcd.write(FLAPS_TOP_GLYPH);
  ecam2_lcd.write(FLAPS_TOP_GLYPH);
  ecam2_lcd.write(FLAPS_TOP_GLYPH);
  ecam2_lcd.write(FLAPS_TOP_GLYPH);
  ecam2_lcd.write(FLAPS_TOP_GLYPH);
}


void render_flaps_1() {
  ecam2_lcd.print("1   ");
  ecam2_lcd.setCursor(12, 3);
  ecam2_lcd.write(FLAPS_TOP_GLYPH);
  ecam2_lcd.write(FLAPS_TOP_GLYPH);
  ecam2_lcd.write(FLAPS_TOP_GLYPH);
  ecam2_lcd.write(FLAPS_TOP_GLYPH);
  ecam2_lcd.write(FLAPS_TOP_GLYPH);
  ecam2_lcd.write(FLAPS_TOP_GLYPH);
  ecam2_lcd.write(FLAPS_TOP_GLYPH);
  ecam2_lcd.write(FLAPS_MIDDLE_GLYPH);
}


void render_flaps_2() {
  ecam2_lcd.print("2   ");
  ecam2_lcd.setCursor(12, 3);
  ecam2_lcd.write(FLAPS_TOP_GLYPH);
  ecam2_lcd.write(FLAPS_TOP_GLYPH);
  ecam2_lcd.write(FLAPS_TOP_GLYPH);
  ecam2_lcd.write(FLAPS_TOP_GLYPH);
  ecam2_lcd.write(FLAPS_TOP_GLYPH);
  ecam2_lcd.write(FLAPS_TOP_GLYPH);
  ecam2_lcd.write(FLAPS_MIDDLE_GLYPH);
  ecam2_lcd.write(FLAPS_MIDDLE_GLYPH);
}


void render_flaps_3() {
  ecam2_lcd.print("3   ");
  ecam2_lcd.setCursor(12, 3);
  ecam2_lcd.write(FLAPS_MIDDLE_GLYPH);
  ecam2_lcd.write(FLAPS_TOP_GLYPH);
  ecam2_lcd.write(FLAPS_TOP_GLYPH);
  ecam2_lcd.write(FLAPS_TOP_GLYPH);
  ecam2_lcd.write(FLAPS_TOP_GLYPH);
  ecam2_lcd.write(FLAPS_TOP_GLYPH);
  ecam2_lcd.write(FLAPS_MIDDLE_GLYPH);
  ecam2_lcd.write(FLAPS_BOTTOM_GLYPH);
}


void render_flaps_full() {
  ecam2_lcd.print("Full");
  ecam2_lcd.setCursor(12, 3);
  ecam2_lcd.write(FLAPS_BOTTOM_GLYPH);
  ecam2_lcd.write(FLAPS_MIDDLE_GLYPH);
  ecam2_lcd.write(FLAPS_TOP_GLYPH);
  ecam2_lcd.write(FLAPS_TOP_GLYPH);
  ecam2_lcd.write(FLAPS_TOP_GLYPH);
  ecam2_lcd.write(FLAPS_MIDDLE_GLYPH);
  ecam2_lcd.write(FLAPS_BOTTOM_GLYPH);
  ecam2_lcd.write(FLAPS_BOTTOM_GLYPH);
}


// Spoilers

void render_spoilers_retracted(int percent, int armed) {
  ecam2_lcd.write(' ');
  ecam2_lcd.write(' ');
  ecam2_lcd.write(' ');
  ecam2_lcd.write(' ');
  ecam2_lcd.write(' ');
  ecam2_lcd.print(" SPOILERS ");
  ecam2_lcd.write(' ');
  ecam2_lcd.write(' ');
  ecam2_lcd.write(' ');
  ecam2_lcd.write(' ');
  ecam2_lcd.write(' ');

  ecam2_lcd.setCursor(0, 1);
  ecam2_lcd.write(SPOILER_BOTTOM_RETRACTED_GLYPH);
  ecam2_lcd.write(SPOILER_BOTTOM_RETRACTED_GLYPH);
  ecam2_lcd.write(SPOILER_BOTTOM_RETRACTED_GLYPH);
  ecam2_lcd.write(SPOILER_BOTTOM_RETRACTED_GLYPH);
  ecam2_lcd.write(SPOILER_BOTTOM_RETRACTED_GLYPH);
  render_spoilers_armed(armed);
  render_spoilers_percent(percent);
  ecam2_lcd.write(SPOILER_BOTTOM_RETRACTED_GLYPH);
  ecam2_lcd.write(SPOILER_BOTTOM_RETRACTED_GLYPH);
  ecam2_lcd.write(SPOILER_BOTTOM_RETRACTED_GLYPH);
  ecam2_lcd.write(SPOILER_BOTTOM_RETRACTED_GLYPH);
  ecam2_lcd.write(SPOILER_BOTTOM_RETRACTED_GLYPH);
}


void render_spoilers_1(int percent, int armed) {
  ecam2_lcd.write(' ');
  ecam2_lcd.write(' ');
  ecam2_lcd.write(' ');
  ecam2_lcd.write(' ');
  ecam2_lcd.write(' ');
  ecam2_lcd.print(" SPOILERS ");
  ecam2_lcd.write(' ');
  ecam2_lcd.write(' ');
  ecam2_lcd.write(' ');
  ecam2_lcd.write(' ');
  ecam2_lcd.write(' ');

  ecam2_lcd.setCursor(0, 1);
  ecam2_lcd.write(SPOILER_BOTTOM_1_GLYPH);
  ecam2_lcd.write(SPOILER_BOTTOM_1_GLYPH);
  ecam2_lcd.write(SPOILER_BOTTOM_1_GLYPH);
  ecam2_lcd.write(SPOILER_BOTTOM_1_GLYPH);
  ecam2_lcd.write(SPOILER_BOTTOM_1_GLYPH);
  render_spoilers_armed(armed);
  render_spoilers_percent(percent);
  ecam2_lcd.write(SPOILER_BOTTOM_1_GLYPH);
  ecam2_lcd.write(SPOILER_BOTTOM_1_GLYPH);
  ecam2_lcd.write(SPOILER_BOTTOM_1_GLYPH);
  ecam2_lcd.write(SPOILER_BOTTOM_1_GLYPH);
  ecam2_lcd.write(SPOILER_BOTTOM_1_GLYPH);
}


void render_spoilers_2(int percent, int armed) {
  ecam2_lcd.write(' ');
  ecam2_lcd.write(' ');
  ecam2_lcd.write(' ');
  ecam2_lcd.write(' ');
  ecam2_lcd.write(' ');
  ecam2_lcd.print(" SPOILERS ");
  ecam2_lcd.write(' ');
  ecam2_lcd.write(' ');
  ecam2_lcd.write(' ');
  ecam2_lcd.write(' ');
  ecam2_lcd.write(' ');

  ecam2_lcd.setCursor(0, 1);
  ecam2_lcd.write(SPOILER_BOTTOM_2_GLYPH);
  ecam2_lcd.write(SPOILER_BOTTOM_2_GLYPH);
  ecam2_lcd.write(SPOILER_BOTTOM_2_GLYPH);
  ecam2_lcd.write(SPOILER_BOTTOM_2_GLYPH);
  ecam2_lcd.write(SPOILER_BOTTOM_2_GLYPH);
  render_spoilers_armed(armed);
  render_spoilers_percent(percent);
  ecam2_lcd.write(SPOILER_BOTTOM_2_GLYPH);
  ecam2_lcd.write(SPOILER_BOTTOM_2_GLYPH);
  ecam2_lcd.write(SPOILER_BOTTOM_2_GLYPH);
  ecam2_lcd.write(SPOILER_BOTTOM_2_GLYPH);
  ecam2_lcd.write(SPOILER_BOTTOM_2_GLYPH);
}


void render_spoilers_full(int percent, int armed) {
  ecam2_lcd.write(SPOILER_TOP_FULL_GLYPH);
  ecam2_lcd.write(SPOILER_TOP_FULL_GLYPH);
  ecam2_lcd.write(SPOILER_TOP_FULL_GLYPH);
  ecam2_lcd.write(SPOILER_TOP_FULL_GLYPH);
  ecam2_lcd.write(SPOILER_TOP_FULL_GLYPH);
  ecam2_lcd.print(" SPOILERS ");
  ecam2_lcd.write(SPOILER_TOP_FULL_GLYPH);
  ecam2_lcd.write(SPOILER_TOP_FULL_GLYPH);
  ecam2_lcd.write(SPOILER_TOP_FULL_GLYPH);
  ecam2_lcd.write(SPOILER_TOP_FULL_GLYPH);
  ecam2_lcd.write(SPOILER_TOP_FULL_GLYPH);

  ecam2_lcd.setCursor(0, 1);
  ecam2_lcd.write(SPOILER_BOTTOM_FULL_GLYPH);
  ecam2_lcd.write(SPOILER_BOTTOM_FULL_GLYPH);
  ecam2_lcd.write(SPOILER_BOTTOM_FULL_GLYPH);
  ecam2_lcd.write(SPOILER_BOTTOM_FULL_GLYPH);
  ecam2_lcd.write(SPOILER_BOTTOM_FULL_GLYPH);
  render_spoilers_armed(armed);
  render_spoilers_percent(percent);
  ecam2_lcd.write(SPOILER_BOTTOM_FULL_GLYPH);
  ecam2_lcd.write(SPOILER_BOTTOM_FULL_GLYPH);
  ecam2_lcd.write(SPOILER_BOTTOM_FULL_GLYPH);
  ecam2_lcd.write(SPOILER_BOTTOM_FULL_GLYPH);
  ecam2_lcd.write(SPOILER_BOTTOM_FULL_GLYPH);
}


void render_spoilers_armed(int armed) {
  if (armed) {
    ecam2_lcd.print(" ARM ");
  } else {
    ecam2_lcd.print("     ");
  }
}


void render_spoilers_percent(int percent) {
  if (percent < 10) {
    ecam2_lcd.print("  ");
  } else if (percent < 100) {
    ecam2_lcd.print(" ");
  }
  ecam2_lcd.print(percent);
  ecam2_lcd.print("% ");
}


#endif
