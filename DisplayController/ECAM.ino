#include "ECAM.h"
#include "Glyphs.h"

LiquidCrystal ecam_lcd = LiquidCrystal(49, 47, 52, 50, 48, 46);     // ECAM

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
    delay(2000);

    ecam_lcd.clear();
    spoilers_1();
    flaps_1();
    delay(2000);

    ecam_lcd.clear();
    spoilers_2();
    flaps_2();
    delay(2000);

    ecam_lcd.clear();
    spoilers_full();
    flaps_3();
    delay(2000);

    ecam_lcd.clear();
    spoilers_full();
    flaps_full();
    delay(2000);

    ecam_lcd.clear();
    spoilers_retracted();
    flaps_up();
}


void ecam_tick() {
}


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

