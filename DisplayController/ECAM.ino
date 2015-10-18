#include "ECAM.h"
#include "Glyphs.h"
#include "MFD.h"
#include "Keypad.h"
#include "Button.h"
#include "Pins.h"


void keypad_tick();


#define KEYPAD_ROW_COUNT  4
#define KEYPAD_COL_COUNT  4
#define KEYPAD_ROW_PINS   (byte[]){ ANALOG_PIN(15), ANALOG_PIN(14), ANALOG_PIN(13), ANALOG_PIN(12) }
#define KEYPAD_COL_PINS   (byte[]){ ANALOG_PIN(11), ANALOG_PIN(10), ANALOG_PIN(9), ANALOG_PIN(8) } 
#define KEYPAD_KEYS       (char[KEYPAD_ROW_COUNT][KEYPAD_COL_COUNT]){ {'1','2','3','A'}, {'4','5','6','B'}, {'7','8','9','C'}, {'*','0','#','D'} }

Keypad keypad = Keypad( makeKeymap(KEYPAD_KEYS), KEYPAD_ROW_PINS, KEYPAD_COL_PINS, KEYPAD_ROW_COUNT, KEYPAD_COL_COUNT); 

LiquidCrystal ecam_lcd = LiquidCrystal(ECAM_LCD_PINS);

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

int control_surfaces_updated = 0;


// Public

void ecam_setup() {
  ecam_lcd.createChar(FLAPS_TOP_INDEX, FLAPS_TOP);
  ecam_lcd.createChar(FLAPS_MIDDLE_INDEX, FLAPS_MIDDLE);
  ecam_lcd.createChar(FLAPS_BOTTOM_INDEX, FLAPS_BOTTOM);
  ecam_lcd.createChar(SPOILER_BOTTOM_RETRACTED_INDEX, SPOILER_BOTTOM_RETRACTED);
  ecam_lcd.createChar(SPOILER_BOTTOM_1_INDEX, SPOILER_BOTTOM_1);
  ecam_lcd.createChar(SPOILER_BOTTOM_2_INDEX, SPOILER_BOTTOM_2);
  ecam_lcd.createChar(SPOILER_BOTTOM_FULL_INDEX, SPOILER_BOTTOM_FULL);
  ecam_lcd.createChar(SPOILER_TOP_FULL_INDEX, SPOILER_TOP_FULL);
  ecam_lcd.begin(ECAM_LCD_COL_COUNT, ECAM_LCD_ROW_COUNT);
  
  ecam_lcd.clear();
  spoilers_retracted();
  flaps_up();
}


void ecam_tick() {
  keypad_tick();
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
  if (customKey){
    // TODO: Do something with the key
  }
}





// TEST ONLY

void flaps_up() {
  ecam_lcd.setCursor(0, 2);
  ecam_lcd.print("Flaps             0%");
  ecam_lcd.setCursor(0, 3);
  ecam_lcd.print("Up");
  ecam_lcd.setCursor(12, 3);
  ecam_lcd.write(FLAPS_TOP_GLYPH);
  ecam_lcd.write(FLAPS_TOP_GLYPH);
  ecam_lcd.write(FLAPS_TOP_GLYPH);
  ecam_lcd.write(FLAPS_TOP_GLYPH);
  ecam_lcd.write(FLAPS_TOP_GLYPH);
  ecam_lcd.write(FLAPS_TOP_GLYPH);
  ecam_lcd.write(FLAPS_TOP_GLYPH);
  ecam_lcd.write(FLAPS_TOP_GLYPH);
}


void flaps_1() {
  ecam_lcd.setCursor(0, 2);
  ecam_lcd.print("Flaps           25%");
  ecam_lcd.setCursor(0, 3);
  ecam_lcd.print("1");
  ecam_lcd.setCursor(12, 3);
  ecam_lcd.write(FLAPS_TOP_GLYPH);
  ecam_lcd.write(FLAPS_TOP_GLYPH);
  ecam_lcd.write(FLAPS_TOP_GLYPH);
  ecam_lcd.write(FLAPS_TOP_GLYPH);
  ecam_lcd.write(FLAPS_TOP_GLYPH);
  ecam_lcd.write(FLAPS_TOP_GLYPH);
  ecam_lcd.write(FLAPS_TOP_GLYPH);
  ecam_lcd.write(FLAPS_MIDDLE_GLYPH);
}


void flaps_2() {
  ecam_lcd.setCursor(0, 2);
  ecam_lcd.print("Flaps            50%");
  ecam_lcd.setCursor(0, 3);
  ecam_lcd.print("2");
  ecam_lcd.setCursor(12, 3);
  ecam_lcd.write(FLAPS_TOP_GLYPH);
  ecam_lcd.write(FLAPS_TOP_GLYPH);
  ecam_lcd.write(FLAPS_TOP_GLYPH);
  ecam_lcd.write(FLAPS_TOP_GLYPH);
  ecam_lcd.write(FLAPS_TOP_GLYPH);
  ecam_lcd.write(FLAPS_TOP_GLYPH);
  ecam_lcd.write(FLAPS_MIDDLE_GLYPH);
  ecam_lcd.write(FLAPS_MIDDLE_GLYPH);
}


void flaps_3() {
  ecam_lcd.setCursor(0, 2);
  ecam_lcd.print("Flaps            75%");
  ecam_lcd.setCursor(0, 3);
  ecam_lcd.print("3");
  ecam_lcd.setCursor(12, 3);
  ecam_lcd.write(FLAPS_MIDDLE_GLYPH);
  ecam_lcd.write(FLAPS_TOP_GLYPH);
  ecam_lcd.write(FLAPS_TOP_GLYPH);
  ecam_lcd.write(FLAPS_TOP_GLYPH);
  ecam_lcd.write(FLAPS_TOP_GLYPH);
  ecam_lcd.write(FLAPS_TOP_GLYPH);
  ecam_lcd.write(FLAPS_MIDDLE_GLYPH);
  ecam_lcd.write(FLAPS_BOTTOM_GLYPH);
}


void flaps_full() {
  ecam_lcd.setCursor(0, 2);
  ecam_lcd.print("Flaps           100%");
  ecam_lcd.setCursor(0, 3);
  ecam_lcd.print("Full");
  ecam_lcd.setCursor(12, 3);
  ecam_lcd.write(FLAPS_BOTTOM_GLYPH);
  ecam_lcd.write(FLAPS_MIDDLE_GLYPH);
  ecam_lcd.write(FLAPS_TOP_GLYPH);
  ecam_lcd.write(FLAPS_TOP_GLYPH);
  ecam_lcd.write(FLAPS_TOP_GLYPH);
  ecam_lcd.write(FLAPS_MIDDLE_GLYPH);
  ecam_lcd.write(FLAPS_BOTTOM_GLYPH);
  ecam_lcd.write(FLAPS_BOTTOM_GLYPH);
}


void spoilers_retracted() {
  ecam_lcd.setCursor(0, 0);
  ecam_lcd.write(' ');
  ecam_lcd.write(' ');
  ecam_lcd.write(' ');
  ecam_lcd.write(' ');
  ecam_lcd.write(' ');
  ecam_lcd.print(" SPOILERS ");
  ecam_lcd.write(' ');
  ecam_lcd.write(' ');
  ecam_lcd.write(' ');
  ecam_lcd.write(' ');
  ecam_lcd.write(' ');
  
  ecam_lcd.setCursor(0, 1);
  ecam_lcd.write(SPOILER_BOTTOM_RETRACTED_GLYPH);
  ecam_lcd.write(SPOILER_BOTTOM_RETRACTED_GLYPH);
  ecam_lcd.write(SPOILER_BOTTOM_RETRACTED_GLYPH);
  ecam_lcd.write(SPOILER_BOTTOM_RETRACTED_GLYPH);
  ecam_lcd.write(SPOILER_BOTTOM_RETRACTED_GLYPH);
  ecam_lcd.print(" ARM   0% ");
  ecam_lcd.write(SPOILER_BOTTOM_RETRACTED_GLYPH);
  ecam_lcd.write(SPOILER_BOTTOM_RETRACTED_GLYPH);
  ecam_lcd.write(SPOILER_BOTTOM_RETRACTED_GLYPH);
  ecam_lcd.write(SPOILER_BOTTOM_RETRACTED_GLYPH);
  ecam_lcd.write(SPOILER_BOTTOM_RETRACTED_GLYPH);
}


void spoilers_1() {
  ecam_lcd.setCursor(0, 0);
  ecam_lcd.write(' ');
  ecam_lcd.write(' ');
  ecam_lcd.write(' ');
  ecam_lcd.write(' ');
  ecam_lcd.write(' ');
  ecam_lcd.print(" SPOILERS ");
  ecam_lcd.write(' ');
  ecam_lcd.write(' ');
  ecam_lcd.write(' ');
  ecam_lcd.write(' ');
  ecam_lcd.write(' ');
  
  ecam_lcd.setCursor(0, 1);
  ecam_lcd.write(SPOILER_BOTTOM_1_GLYPH);
  ecam_lcd.write(SPOILER_BOTTOM_1_GLYPH);
  ecam_lcd.write(SPOILER_BOTTOM_1_GLYPH);
  ecam_lcd.write(SPOILER_BOTTOM_1_GLYPH);
  ecam_lcd.write(SPOILER_BOTTOM_1_GLYPH);
  ecam_lcd.print(" ARM  25% ");
  ecam_lcd.write(SPOILER_BOTTOM_1_GLYPH);
  ecam_lcd.write(SPOILER_BOTTOM_1_GLYPH);
  ecam_lcd.write(SPOILER_BOTTOM_1_GLYPH);
  ecam_lcd.write(SPOILER_BOTTOM_1_GLYPH);
  ecam_lcd.write(SPOILER_BOTTOM_1_GLYPH);
}


void spoilers_2() {
  ecam_lcd.setCursor(0, 0);
  ecam_lcd.write(' ');
  ecam_lcd.write(' ');
  ecam_lcd.write(' ');
  ecam_lcd.write(' ');
  ecam_lcd.write(' ');
  ecam_lcd.print(" SPOILERS ");
  ecam_lcd.write(' ');
  ecam_lcd.write(' ');
  ecam_lcd.write(' ');
  ecam_lcd.write(' ');
  ecam_lcd.write(' ');
  
  ecam_lcd.setCursor(0, 1);
  ecam_lcd.write(SPOILER_BOTTOM_2_GLYPH);
  ecam_lcd.write(SPOILER_BOTTOM_2_GLYPH);
  ecam_lcd.write(SPOILER_BOTTOM_2_GLYPH);
  ecam_lcd.write(SPOILER_BOTTOM_2_GLYPH);
  ecam_lcd.write(SPOILER_BOTTOM_2_GLYPH);
  ecam_lcd.print(" ARM  50% ");
  ecam_lcd.write(SPOILER_BOTTOM_2_GLYPH);
  ecam_lcd.write(SPOILER_BOTTOM_2_GLYPH);
  ecam_lcd.write(SPOILER_BOTTOM_2_GLYPH);
  ecam_lcd.write(SPOILER_BOTTOM_2_GLYPH);
  ecam_lcd.write(SPOILER_BOTTOM_2_GLYPH);
}


void spoilers_full() {
  ecam_lcd.setCursor(0, 0);
  ecam_lcd.write(SPOILER_TOP_FULL_GLYPH);
  ecam_lcd.write(SPOILER_TOP_FULL_GLYPH);
  ecam_lcd.write(SPOILER_TOP_FULL_GLYPH);
  ecam_lcd.write(SPOILER_TOP_FULL_GLYPH);
  ecam_lcd.write(SPOILER_TOP_FULL_GLYPH);
  ecam_lcd.print(" SPOILERS ");
  ecam_lcd.write(SPOILER_TOP_FULL_GLYPH);
  ecam_lcd.write(SPOILER_TOP_FULL_GLYPH);
  ecam_lcd.write(SPOILER_TOP_FULL_GLYPH);
  ecam_lcd.write(SPOILER_TOP_FULL_GLYPH);
  ecam_lcd.write(SPOILER_TOP_FULL_GLYPH);
  
  ecam_lcd.setCursor(0, 1);
  ecam_lcd.write(SPOILER_BOTTOM_FULL_GLYPH);
  ecam_lcd.write(SPOILER_BOTTOM_FULL_GLYPH);
  ecam_lcd.write(SPOILER_BOTTOM_FULL_GLYPH);
  ecam_lcd.write(SPOILER_BOTTOM_FULL_GLYPH);
  ecam_lcd.write(SPOILER_BOTTOM_FULL_GLYPH);
  ecam_lcd.print(" ARM 100% ");
  ecam_lcd.write(SPOILER_BOTTOM_FULL_GLYPH);
  ecam_lcd.write(SPOILER_BOTTOM_FULL_GLYPH);
  ecam_lcd.write(SPOILER_BOTTOM_FULL_GLYPH);
  ecam_lcd.write(SPOILER_BOTTOM_FULL_GLYPH);
  ecam_lcd.write(SPOILER_BOTTOM_FULL_GLYPH);
}

