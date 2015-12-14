#ifdef CONTROLLER_A

#include "ECAM.h"
#include "Glyphs.h"
#include "MFD.h"
#include "Keypad.h"
#include "Button.h"
#include "Pins.h"


void keypad_tick();

void previous_ecam();
void next_ecam();
void scroll_up();
void scroll_down();

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

#define FLAPS_POSITION_SIZE                        3
char flaps_position[FLAPS_POSITION_SIZE]         = {'0', '0', '0'};
#define SPOILERS_POSITION_SIZE                     3
char spoilers_position[SPOILERS_POSITION_SIZE]   = {'0', '0', '0'};
#define SPOILERS_ARMED_SIZE                        1
char spoilers_armed[SPOILERS_ARMED_SIZE]         = {'0'};

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


// Public

void ecam_setup() {
  nextPageButton.setOnClick(next_ecam);
  previousPageButton.setOnClick(previous_ecam);
  scrollUpButton.setOnClick(scroll_up);
  scrollDownButton.setOnClick(scroll_down);
  
  if (current_ecam_page->update_if_necessary != NULL) {
    current_ecam_page->update_if_necessary();
  }

  ecam1_lcd.write("ECAM 1");
}


void ecam_tick() {
  keypad_tick();
  nextPageButton.tick();
  previousPageButton.tick();
  scrollUpButton.tick();
  scrollDownButton.tick();
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
  Serial.println("previous_ecam()");
}


void next_ecam() {
  Serial.println("next_ecam()");
}


void scroll_up() {
  Serial.println("scroll_up()");
}


void scroll_down() {
  Serial.println("scroll_down()");
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
  ecam2_lcd.begin(ECAM_LCD_COL_COUNT, ECAM_LCD_ROW_COUNT);
  ecam2_lcd.clear();
}


void control_surfaces_page_render() {
  render_spoilers();
  render_flaps();
}


void control_surfaces_page_update_if_necessary() {
  if (flaps_updated == 1 || spoilers_updated == 1) {
    control_surfaces_page_render();
  }
}


void render_spoilers() {
  spoilers_full();
}


void render_flaps() {
  flaps_full();
}


// Fuel
void fuel_page_will_appear() {
    
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


















// TEST ONLY

void flaps_up() {
  ecam2_lcd.setCursor(0, 2);
  ecam2_lcd.print("Flaps             0%");
  ecam2_lcd.setCursor(0, 3);
  ecam2_lcd.print("Up");
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


void flaps_1() {
  ecam2_lcd.setCursor(0, 2);
  ecam2_lcd.print("Flaps           25%");
  ecam2_lcd.setCursor(0, 3);
  ecam2_lcd.print("1");
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


void flaps_2() {
  ecam2_lcd.setCursor(0, 2);
  ecam2_lcd.print("Flaps            50%");
  ecam2_lcd.setCursor(0, 3);
  ecam2_lcd.print("2");
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


void flaps_3() {
  ecam2_lcd.setCursor(0, 2);
  ecam2_lcd.print("Flaps            75%");
  ecam2_lcd.setCursor(0, 3);
  ecam2_lcd.print("3");
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


void flaps_full() {
  ecam2_lcd.setCursor(0, 2);
  ecam2_lcd.print("Flaps           100%");
  ecam2_lcd.setCursor(0, 3);
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


void spoilers_retracted() {
  ecam2_lcd.setCursor(0, 0);
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
  ecam2_lcd.print(" ARM   0% ");
  ecam2_lcd.write(SPOILER_BOTTOM_RETRACTED_GLYPH);
  ecam2_lcd.write(SPOILER_BOTTOM_RETRACTED_GLYPH);
  ecam2_lcd.write(SPOILER_BOTTOM_RETRACTED_GLYPH);
  ecam2_lcd.write(SPOILER_BOTTOM_RETRACTED_GLYPH);
  ecam2_lcd.write(SPOILER_BOTTOM_RETRACTED_GLYPH);
}


void spoilers_1() {
  ecam2_lcd.setCursor(0, 0);
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
  ecam2_lcd.print(" ARM  25% ");
  ecam2_lcd.write(SPOILER_BOTTOM_1_GLYPH);
  ecam2_lcd.write(SPOILER_BOTTOM_1_GLYPH);
  ecam2_lcd.write(SPOILER_BOTTOM_1_GLYPH);
  ecam2_lcd.write(SPOILER_BOTTOM_1_GLYPH);
  ecam2_lcd.write(SPOILER_BOTTOM_1_GLYPH);
}


void spoilers_2() {
  ecam2_lcd.setCursor(0, 0);
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
  ecam2_lcd.print(" ARM  50% ");
  ecam2_lcd.write(SPOILER_BOTTOM_2_GLYPH);
  ecam2_lcd.write(SPOILER_BOTTOM_2_GLYPH);
  ecam2_lcd.write(SPOILER_BOTTOM_2_GLYPH);
  ecam2_lcd.write(SPOILER_BOTTOM_2_GLYPH);
  ecam2_lcd.write(SPOILER_BOTTOM_2_GLYPH);
}


void spoilers_full() {
  ecam2_lcd.setCursor(0, 0);
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
  ecam2_lcd.print(" ARM 100% ");
  ecam2_lcd.write(SPOILER_BOTTOM_FULL_GLYPH);
  ecam2_lcd.write(SPOILER_BOTTOM_FULL_GLYPH);
  ecam2_lcd.write(SPOILER_BOTTOM_FULL_GLYPH);
  ecam2_lcd.write(SPOILER_BOTTOM_FULL_GLYPH);
  ecam2_lcd.write(SPOILER_BOTTOM_FULL_GLYPH);
}


#endif
