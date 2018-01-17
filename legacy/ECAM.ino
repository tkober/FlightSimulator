#ifdef CONTROLLER_A

#include "ECAM.h"
#include "Glyphs.h"
#include "MFD.h"
#include "Keypad.h"
#include "Button.h"
#include "Pins.h"
#include "Math.h"


enum FuelUnit {
  Percent,
  Tons
};


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
void fuel_page_scroll_down();
void fuel_page_scroll_up();
void fuel_page_handle_keypad_input(char key);
void render_fuel_info();
void render_fuel_configuration();
void render_fuel_value(unsigned long value, unsigned long kg_per_percent);
void render_x_feed_valve();
void toggle_fuel_unit();
void render_fuel_configuration_value(char *value);
void render_scrollbar(int up, int down);
void configure_fuel_with_input(int index);
void clear_fuel_configuration_ipnut();

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
  fuel_page_scroll_down,
  fuel_page_scroll_up,
  fuel_page_handle_keypad_input
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

#define FLAPS_1_INDEX                   0
#define FLAPS_2_INDEX                   1
#define FLAPS_3_INDEX                   2
#define FLAPS_4_INDEX                   3
#define FLAPS_5_INDEX                   4

#define SPOILER_BOTTOM_RETRACTED_INDEX  5
#define SPOILER_BOTTOM_1_INDEX          6
#define SPOILER_BOTTOM_2_INDEX          7

#define FLAPS_1_GLYPH                   byte(FLAPS_1_INDEX)
#define FLAPS_2_GLYPH                   byte(FLAPS_2_INDEX)
#define FLAPS_3_GLYPH                   byte(FLAPS_3_INDEX)
#define FLAPS_4_GLYPH                   byte(FLAPS_4_INDEX)
#define FLAPS_5_GLYPH                   byte(FLAPS_5_INDEX)

#define SPOILER_BOTTOM_RETRACTED_GLYPH  byte(SPOILER_BOTTOM_RETRACTED_INDEX)
#define SPOILER_BOTTOM_1_GLYPH          byte(SPOILER_BOTTOM_1_INDEX)
#define SPOILER_BOTTOM_2_GLYPH          byte(SPOILER_BOTTOM_2_INDEX)


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


#define FRAME_UPPER_RIGHT_INDEX         0
#define FRAME_UPPER_LEFT_INDEX          1
#define FRAME_BOTTOM_RIGHT_INDEX        2
#define FRAME_BOTTOM_LEFT_INDEX         3
#define FRAME_VERTICAL_INDEX            4
#define FRAME_HORIZONTAL_INDEX          5
#define X_FEED_VALVE_OPEN_INDEX         6
#define X_FEED_VALVE_CLOSED_INDEX       7

#define FRAME_UPPER_RIGHT_GLYPH         byte(FRAME_UPPER_RIGHT_INDEX)
#define FRAME_UPPER_LEFT_GLYPH          byte(FRAME_UPPER_LEFT_INDEX)
#define FRAME_BOTTOM_RIGHT_GLYPH        byte(FRAME_BOTTOM_RIGHT_INDEX)
#define FRAME_BOTTOM_LEFT_GLYPH         byte(FRAME_BOTTOM_LEFT_INDEX)
#define FRAME_VERTICAL_GLYPH            byte(FRAME_VERTICAL_INDEX)
#define FRAME_HORIZONTAL_GLYPH          byte(FRAME_HORIZONTAL_INDEX)
#define X_FEED_VALVE_OPEN_GLYPH         byte(X_FEED_VALVE_OPEN_INDEX)
#define X_FEED_VALVE_CLOSED_GLYPH       byte(X_FEED_VALVE_CLOSED_INDEX)

#define SCROLL_DOWN_INDEX               0
#define SCROLL_UP_INDEX                 1
#define PLACEHOLDER_INDEX               2

#define SCROLL_DOWN_GLYPH               byte(SCROLL_DOWN_INDEX)
#define SCROLL_UP_GLYPH                 byte(SCROLL_UP_INDEX)
#define PLACEHOLDER_GLYPH               byte(PLACEHOLDER_INDEX)


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

#define FUEL_SIZE 3
char fuel_left[FUEL_SIZE]   = {'0', '0', '0'};
char fuel_center[FUEL_SIZE] = {'0', '0', '0'};
char fuel_right[FUEL_SIZE]  = {'0', '0', '0'};
FuelUnit fuel_unit = Percent;
#define FUEL_CONFIGURATION_SIZE     4
char fuel_kg_per_percent_left[FUEL_CONFIGURATION_SIZE] = {PLACEHOLDER_INDEX, PLACEHOLDER_INDEX, PLACEHOLDER_INDEX, PLACEHOLDER_INDEX};
char fuel_kg_per_percent_center[FUEL_CONFIGURATION_SIZE] = {PLACEHOLDER_INDEX, PLACEHOLDER_INDEX, PLACEHOLDER_INDEX, PLACEHOLDER_INDEX};
char fuel_kg_per_percent_right[FUEL_CONFIGURATION_SIZE] = {PLACEHOLDER_INDEX, PLACEHOLDER_INDEX, PLACEHOLDER_INDEX, PLACEHOLDER_INDEX};
char fuel_configuration_input[FUEL_CONFIGURATION_SIZE] = {'_', '_', '_', '_'};
int fuel_configuration_input_position = 0;
int is_configuring_fuel = 0;
int fuel_configuration_clip_position = 0;
#define X_FEED_VALVE_SIZE             1
char x_feed_valve[X_FEED_VALVE_SIZE]  = {'0'};

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


void read_fuel_left(char token) {
  store_token(token, fuel_left, FUEL_SIZE, &fuel_updated);
}


void read_fuel_center(char token) {
  store_token(token, fuel_center, FUEL_SIZE, &fuel_updated);
}


void read_fuel_right(char token) {
  store_token(token, fuel_right, FUEL_SIZE, &fuel_updated);
}


void read_x_feed_valve(char token) {
  store_token(token, x_feed_valve, X_FEED_VALVE_SIZE, &fuel_updated);
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
  if (current_ecam_page->scroll_up) {
    current_ecam_page->scroll_up();
  }
}


void scroll_down() {
  if (current_ecam_page->scroll_down) {
    current_ecam_page->scroll_down();
  }
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
  ecam2_lcd.createChar(FLAPS_1_INDEX, FLAPS_1);
  ecam2_lcd.createChar(FLAPS_2_INDEX, FLAPS_2);
  ecam2_lcd.createChar(FLAPS_3_INDEX, FLAPS_3);
  ecam2_lcd.createChar(FLAPS_4_INDEX, FLAPS_4);
  ecam2_lcd.createChar(FLAPS_5_INDEX, FLAPS_5);

  ecam2_lcd.createChar(SPOILER_BOTTOM_RETRACTED_INDEX, SPOILER_BOTTOM_RETRACTED);
  ecam2_lcd.createChar(SPOILER_BOTTOM_1_INDEX, SPOILER_BOTTOM_1);
  ecam2_lcd.createChar(SPOILER_BOTTOM_2_INDEX, SPOILER_BOTTOM_2);
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
  } else {
    render_spoilers_2(spoilers, armed);
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
  if (is_configuring_fuel) {
    ecam2_lcd.createChar(SCROLL_DOWN_INDEX, SCROLL_DOWN);
    ecam2_lcd.createChar(SCROLL_UP_INDEX, SCROLL_UP);
    ecam2_lcd.createChar(PLACEHOLDER_INDEX, PLACEHOLDER);
  } else {
    ecam2_lcd.createChar(FRAME_UPPER_RIGHT_INDEX, FRAME_UPPER_RIGHT);
    ecam2_lcd.createChar(FRAME_UPPER_LEFT_INDEX, FRAME_UPPER_LEFT);
    ecam2_lcd.createChar(FRAME_BOTTOM_RIGHT_INDEX, FRAME_BOTTOM_RIGHT);
    ecam2_lcd.createChar(FRAME_BOTTOM_LEFT_INDEX, FRAME_BOTTOM_LEFT);
    ecam2_lcd.createChar(FRAME_VERTICAL_INDEX, FRAME_VERTICAL);
    ecam2_lcd.createChar(FRAME_HORIZONTAL_INDEX, FRAME_HORIZONTAL);

    ecam2_lcd.createChar(X_FEED_VALVE_OPEN_INDEX, X_FEED_VALVE_OPEN);
    ecam2_lcd.createChar(X_FEED_VALVE_CLOSED_INDEX, X_FEED_VALVE_CLOSED);
  }

  ecam2_lcd.clear();
}


void fuel_page_render() {
  if (is_configuring_fuel) {
    render_fuel_configuration();
  } else {
    render_fuel_info();
  }
}


void fuel_page_update_if_necessary() {
  if (fuel_updated == 1) {
    fuel_page_render();
    fuel_updated = 0;
  }
}


void fuel_page_scroll_down() {
  if (is_configuring_fuel) {
    if (fuel_configuration_clip_position == 0) {
      fuel_configuration_clip_position = 1;
      ecam2_lcd.clear();
      fuel_updated = 1;
    }
  }
}


void fuel_page_scroll_up() {
  if (is_configuring_fuel) {
    if (fuel_configuration_clip_position == 1) {
      fuel_configuration_clip_position = 0;
      ecam2_lcd.clear();
      fuel_updated = 1;
    }
  }
}


void fuel_page_handle_keypad_input(char key) {
  if (is_configuring_fuel) {
    switch (key) {
      case '*':
        is_configuring_fuel = 0;
        fuel_page_will_appear();
        fuel_updated = 1;
        break;

      case '#':
        clear_fuel_configuration_ipnut();
        break;

      case 'A':
        configure_fuel_with_input(0);
        break;

      case 'B':
        configure_fuel_with_input(1);
        break;

      case 'C':
        break;

      case 'D':
        break;

      default:
        if (fuel_configuration_input_position < FUEL_CONFIGURATION_SIZE) {
          fuel_configuration_input[fuel_configuration_input_position] = key;
          fuel_updated = 1;
          fuel_configuration_input_position++;
        }
        break;
    }
  } else {
    switch (key) {
      case 'A':
        toggle_fuel_unit();
        break;

      case 'B':
        is_configuring_fuel = 1;
        fuel_page_will_appear();
        fuel_updated = 1;
        break;

      default:
        break;
    }
  }
}


void render_fuel_info() {
  int left = int_from_string(fuel_left, FUEL_SIZE, 0);
  int center = int_from_string(fuel_center, FUEL_SIZE, 0);
  int right = int_from_string(fuel_right, FUEL_SIZE, 0);

  int leftKg = int_from_string(fuel_kg_per_percent_left, FUEL_CONFIGURATION_SIZE, 0);
  int centerKg = int_from_string(fuel_kg_per_percent_center, FUEL_CONFIGURATION_SIZE, 0);
  int rightKg = int_from_string(fuel_kg_per_percent_right, FUEL_CONFIGURATION_SIZE, 0);

  ecam2_lcd.setCursor(0, 0);
  switch (fuel_unit) {

    case Percent:
      ecam2_lcd.write("A> t   ");
      break;

    case Tons:
      ecam2_lcd.write("A> %   ");
      break;
  }
  ecam2_lcd.write(FRAME_UPPER_LEFT_GLYPH);
  ecam2_lcd.write(FRAME_HORIZONTAL_GLYPH);
  ecam2_lcd.write(FRAME_HORIZONTAL_GLYPH);
  ecam2_lcd.write(FRAME_HORIZONTAL_GLYPH);
  ecam2_lcd.write(FRAME_HORIZONTAL_GLYPH);
  ecam2_lcd.write(FRAME_UPPER_RIGHT_GLYPH);
  ecam2_lcd.write(" CFG <B");

  ecam2_lcd.setCursor(0, 1);
  ecam2_lcd.write(FRAME_UPPER_LEFT_GLYPH);
  ecam2_lcd.write(FRAME_HORIZONTAL_GLYPH);
  ecam2_lcd.write(FRAME_HORIZONTAL_GLYPH);
  ecam2_lcd.write(FRAME_HORIZONTAL_GLYPH);
  ecam2_lcd.write(FRAME_HORIZONTAL_GLYPH);
  ecam2_lcd.write(FRAME_UPPER_RIGHT_GLYPH);
  ecam2_lcd.write(" ");
  ecam2_lcd.write(FRAME_VERTICAL_GLYPH);
  render_fuel_value(center, centerKg);
  ecam2_lcd.write(FRAME_VERTICAL_GLYPH);
  ecam2_lcd.write(" ");
  ecam2_lcd.write(FRAME_UPPER_LEFT_GLYPH);
  ecam2_lcd.write(FRAME_HORIZONTAL_GLYPH);
  ecam2_lcd.write(FRAME_HORIZONTAL_GLYPH);
  ecam2_lcd.write(FRAME_HORIZONTAL_GLYPH);
  ecam2_lcd.write(FRAME_HORIZONTAL_GLYPH);
  ecam2_lcd.write(FRAME_UPPER_RIGHT_GLYPH);

  ecam2_lcd.setCursor(0, 2);
  ecam2_lcd.write(FRAME_VERTICAL_GLYPH);
  render_fuel_value(left, leftKg);
  ecam2_lcd.write(FRAME_VERTICAL_GLYPH);
  render_x_feed_valve();
  ecam2_lcd.write(FRAME_VERTICAL_GLYPH);
  ecam2_lcd.write("    ");
  ecam2_lcd.write(FRAME_VERTICAL_GLYPH);
  render_x_feed_valve();
  ecam2_lcd.write(FRAME_VERTICAL_GLYPH);
  render_fuel_value(right, rightKg);
  ecam2_lcd.write(FRAME_VERTICAL_GLYPH);

  ecam2_lcd.setCursor(0, 3);
  ecam2_lcd.write(FRAME_BOTTOM_LEFT_GLYPH);
  ecam2_lcd.write(FRAME_HORIZONTAL_GLYPH);
  ecam2_lcd.write(FRAME_HORIZONTAL_GLYPH);
  ecam2_lcd.write(FRAME_HORIZONTAL_GLYPH);
  ecam2_lcd.write(FRAME_HORIZONTAL_GLYPH);
  ecam2_lcd.write(FRAME_BOTTOM_RIGHT_GLYPH);
  ecam2_lcd.write(" ");
  ecam2_lcd.write(FRAME_BOTTOM_LEFT_GLYPH);
  ecam2_lcd.write(FRAME_HORIZONTAL_GLYPH);
  ecam2_lcd.write(FRAME_HORIZONTAL_GLYPH);
  ecam2_lcd.write(FRAME_HORIZONTAL_GLYPH);
  ecam2_lcd.write(FRAME_HORIZONTAL_GLYPH);
  ecam2_lcd.write(FRAME_BOTTOM_RIGHT_GLYPH);
  ecam2_lcd.write(" ");
  ecam2_lcd.write(FRAME_BOTTOM_LEFT_GLYPH);
  ecam2_lcd.write(FRAME_HORIZONTAL_GLYPH);
  ecam2_lcd.write(FRAME_HORIZONTAL_GLYPH);
  ecam2_lcd.write(FRAME_HORIZONTAL_GLYPH);
  ecam2_lcd.write(FRAME_HORIZONTAL_GLYPH);
  ecam2_lcd.write(FRAME_BOTTOM_RIGHT_GLYPH);
}


void render_fuel_configuration() {
  ecam2_lcd.setCursor(0, 0);
  ecam2_lcd.write("*> EXIT");
  ecam2_lcd.setCursor(0, 1);
  ecam2_lcd.write("#> CLEAR");
  ecam2_lcd.setCursor(0, 3);
  ecam2_lcd.write(" >");
  render_fuel_configuration_value(fuel_configuration_input);
  ecam2_lcd.write("[kg/%]");
  if (fuel_configuration_clip_position == 0) {
    ecam2_lcd.setCursor(12, 0);
    ecam2_lcd.write("LEFT <A");
    ecam2_lcd.setCursor(14, 1);
    render_fuel_configuration_value(fuel_kg_per_percent_left);
    ecam2_lcd.setCursor(10, 2);
    ecam2_lcd.write("CENTER <B");
    ecam2_lcd.setCursor(14, 3);
    render_fuel_configuration_value(fuel_kg_per_percent_center);
    render_scrollbar(0, 1);
  } else {
    ecam2_lcd.setCursor(10, 0);
    ecam2_lcd.write("CENTER <A");
    ecam2_lcd.setCursor(14, 1);
    render_fuel_configuration_value(fuel_kg_per_percent_center);
    ecam2_lcd.setCursor(11, 2);
    ecam2_lcd.write("RIGHT <B");
    ecam2_lcd.setCursor(14, 3);
    render_fuel_configuration_value(fuel_kg_per_percent_right);
    render_scrollbar(1, 0);
  }
}


void render_fuel_configuration_value(char *value) {
  for (int i = 0; i < FUEL_CONFIGURATION_SIZE; i++) {
    ecam2_lcd.write(value[i]);
  }
}


void render_scrollbar(int up, int down) {
  if (up) {
    ecam2_lcd.setCursor(19, 0);
    ecam2_lcd.write(SCROLL_UP_GLYPH);
  }
  if (down) {
    ecam2_lcd.setCursor(19, 3);
    ecam2_lcd.write(SCROLL_DOWN_GLYPH);
  }
}



void render_fuel_value(unsigned long value, unsigned long kg_per_percent) {
  unsigned long valueInKg;
  unsigned long valueInTons;
  unsigned long decimals;
  switch (fuel_unit) {

    case Percent:
        if (value < 100) {
          ecam2_lcd.write(" ");
          if (value < 10) {
            ecam2_lcd.write(" ");
          }
        }
        ecam2_lcd.print(value);
        ecam2_lcd.write("%");
      break;

    case Tons:
      if (kg_per_percent > 0) {
        valueInKg = value * kg_per_percent;
        if (valueInKg < 1000) {
          // .123
          ecam2_lcd.write(".");
          if (valueInKg < 100) {
            ecam2_lcd.write("0");
            if (valueInKg < 10) {
              ecam2_lcd.write("0");
            }
          }
          ecam2_lcd.print(valueInKg);
        } else {
          valueInTons = valueInKg / 1000;
          if (valueInTons < 10) {
            // 1.23
            ecam2_lcd.print(valueInTons);
            ecam2_lcd.write(".");
            decimals = (valueInKg % 1000) / 10;
            if (decimals < 10) {
              ecam2_lcd.write("0");
            }
            ecam2_lcd.print(decimals);
          } else if (valueInTons < 100)  {
            // 12.3
            ecam2_lcd.print(valueInTons);
            ecam2_lcd.write(".");
            decimals = (valueInKg % 1000) / 100;
            ecam2_lcd.print(decimals);
          } else {
            // 123
            ecam2_lcd.write(" ");
            ecam2_lcd.print(valueInTons);
          }
        }
      } else {
        ecam2_lcd.write("????");
      }
      break;
  }
}


void render_x_feed_valve() {
  if (x_feed_valve[0] == '1') {
    ecam2_lcd.write(X_FEED_VALVE_OPEN_GLYPH);
  } else {
    ecam2_lcd.write(X_FEED_VALVE_CLOSED_GLYPH);
  }
}


void toggle_fuel_unit() {
  switch (fuel_unit) {
    case Percent:
      fuel_unit = Tons;
      break;

    case Tons:
      fuel_unit = Percent;
      break;
  }
  fuel_updated = 1;
}


void configure_fuel_with_input(int index) {
  if (is_number_string(fuel_configuration_input, FUEL_CONFIGURATION_SIZE, 0)) {
    char *value_to_configure;
    if (fuel_configuration_clip_position == 0) {
      value_to_configure = index == 0 ? fuel_kg_per_percent_left : fuel_kg_per_percent_center;
    } else {
      value_to_configure = index == 0 ? fuel_kg_per_percent_center : fuel_kg_per_percent_right;
    }
    for (int i = 0; i < FUEL_CONFIGURATION_SIZE; i++) {
      value_to_configure[i] = fuel_configuration_input[i];
    }
    clear_fuel_configuration_ipnut();
  }
}


void clear_fuel_configuration_ipnut() {
  for (int i = 0; i < FUEL_CONFIGURATION_SIZE; i++) {
    fuel_configuration_input[i] = '_';
  }
  fuel_configuration_input_position = 0;
  fuel_updated = 1;
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
  ecam2_lcd.write(FLAPS_1_GLYPH);
  ecam2_lcd.write(FLAPS_1_GLYPH);
  ecam2_lcd.write(FLAPS_1_GLYPH);
  ecam2_lcd.write(FLAPS_1_GLYPH);
  ecam2_lcd.write(FLAPS_1_GLYPH);
  ecam2_lcd.write(FLAPS_1_GLYPH);
  ecam2_lcd.write(FLAPS_1_GLYPH);
  ecam2_lcd.write(FLAPS_1_GLYPH);
}


void render_flaps_1() {
  ecam2_lcd.print("1   ");
  ecam2_lcd.setCursor(12, 3);
  ecam2_lcd.write(FLAPS_2_GLYPH);
  ecam2_lcd.write(FLAPS_1_GLYPH);
  ecam2_lcd.write(FLAPS_1_GLYPH);
  ecam2_lcd.write(FLAPS_1_GLYPH);
  ecam2_lcd.write(FLAPS_1_GLYPH);
  ecam2_lcd.write(FLAPS_1_GLYPH);
  ecam2_lcd.write(FLAPS_1_GLYPH);
  ecam2_lcd.write(FLAPS_2_GLYPH);
}


void render_flaps_2() {
  ecam2_lcd.print("2   ");
  ecam2_lcd.setCursor(12, 3);
  ecam2_lcd.write(FLAPS_3_GLYPH);
  ecam2_lcd.write(FLAPS_2_GLYPH);
  ecam2_lcd.write(FLAPS_1_GLYPH);
  ecam2_lcd.write(FLAPS_1_GLYPH);
  ecam2_lcd.write(FLAPS_1_GLYPH);
  ecam2_lcd.write(FLAPS_1_GLYPH);
  ecam2_lcd.write(FLAPS_2_GLYPH);
  ecam2_lcd.write(FLAPS_3_GLYPH);
}


void render_flaps_3() {
  ecam2_lcd.print("3   ");
  ecam2_lcd.setCursor(12, 3);
  ecam2_lcd.write(FLAPS_4_GLYPH);
  ecam2_lcd.write(FLAPS_3_GLYPH);
  ecam2_lcd.write(FLAPS_2_GLYPH);
  ecam2_lcd.write(FLAPS_1_GLYPH);
  ecam2_lcd.write(FLAPS_1_GLYPH);
  ecam2_lcd.write(FLAPS_2_GLYPH);
  ecam2_lcd.write(FLAPS_3_GLYPH);
  ecam2_lcd.write(FLAPS_4_GLYPH);
}


void render_flaps_full() {
  ecam2_lcd.print("Full");
  ecam2_lcd.setCursor(12, 3);
  ecam2_lcd.write(FLAPS_4_GLYPH);
  ecam2_lcd.write(FLAPS_3_GLYPH);
  ecam2_lcd.write(FLAPS_2_GLYPH);
  ecam2_lcd.write(FLAPS_1_GLYPH);
  ecam2_lcd.write(FLAPS_2_GLYPH);
  ecam2_lcd.write(FLAPS_3_GLYPH);
  ecam2_lcd.write(FLAPS_4_GLYPH);
  ecam2_lcd.write(FLAPS_5_GLYPH);
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
