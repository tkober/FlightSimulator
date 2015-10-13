#include "ECAM.h"
#include "Glyphs.h"

LiquidCrystal ecam_lcd = LiquidCrystal(49, 47, 52, 50, 48, 46);     // ECAM

#define ECAM_LCD_ROW_COUNT   2
#define ECAM_LCD_COL_COUNT   16

#define FLAP_NODE_TOP_INDEX             0
#define FLAP_NODE_MIDDLE_INDEX          1
#define FLAP_NODE_BOTTOM_INDEX          2
#define FLAP_EDGE_PARALLEL_INDEX        3
#define FLAP_EDGE_UPWARD_1_INDEX        4
#define FLAP_EDGE_UPWARD_2_INDEX        5
#define FLAP_EDGE_DOWNWARD_1_INDEX      6
#define FLAP_EDGE_DOWNWARD_2_INDEX      7

#define FLAP_NODE_TOP_GLYPH           byte(FLAP_NODE_TOP_INDEX)
#define FLAP_NODE_MIDDLE_GLYPH        byte(FLAP_NODE_MIDDLE_INDEX)
#define FLAP_NODE_BOTTOM_GLYPH        byte(FLAP_NODE_BOTTOM_INDEX)
#define FLAP_EDGE_PARALLEL_GLYPH      byte(FLAP_EDGE_PARALLEL_INDEX)
#define FLAP_EDGE_UPWARD_1_GLYPH      byte(FLAP_EDGE_UPWARD_1_INDEX)
#define FLAP_EDGE_UPWARD_2_GLYPH      byte(FLAP_EDGE_UPWARD_2_INDEX)  
#define FLAP_EDGE_DOWNWARD_1_GLYPH    byte(FLAP_EDGE_DOWNWARD_1_INDEX)
#define FLAP_EDGE_DOWNWARD_2_GLYPH    byte(FLAP_EDGE_DOWNWARD_2_INDEX)


void ecam_setup() {
  ecam_lcd.createChar(FLAP_NODE_TOP_INDEX, FLAP_NODE_TOP);
  ecam_lcd.createChar(FLAP_NODE_MIDDLE_INDEX, FLAP_NODE_MIDDLE);
  ecam_lcd.createChar(FLAP_NODE_BOTTOM_INDEX, FLAP_NODE_BOTTOM);
  ecam_lcd.createChar(FLAP_EDGE_PARALLEL_INDEX, FLAP_EDGE_PARALLEL);
  ecam_lcd.createChar(FLAP_EDGE_UPWARD_1_INDEX, FLAP_EDGE_UPWARD_1);
  ecam_lcd.createChar(FLAP_EDGE_UPWARD_2_INDEX, FLAP_EDGE_UPWARD_2);
  ecam_lcd.createChar(FLAP_EDGE_DOWNWARD_1_INDEX, FLAP_EDGE_DOWNWARD_1);
  ecam_lcd.createChar(FLAP_EDGE_DOWNWARD_2_INDEX, FLAP_EDGE_DOWNWARD_2);
  ecam_lcd.begin(ECAM_LCD_COL_COUNT, ECAM_LCD_ROW_COUNT);
  flaps_3();
}


void ecam_tick() {
}


void flaps_up() {
  ecam_lcd.clear();
  ecam_lcd.print("Flaps             0%");
  ecam_lcd.setCursor(0, 1);
  ecam_lcd.print("Up");
  ecam_lcd.setCursor(5, 1);
  ecam_lcd.write(FLAP_NODE_TOP_GLYPH);
  ecam_lcd.write(FLAP_EDGE_PARALLEL_GLYPH);
  ecam_lcd.write(FLAP_NODE_TOP_GLYPH);
  ecam_lcd.write(FLAP_EDGE_PARALLEL_GLYPH);
  ecam_lcd.write(FLAP_NODE_TOP_GLYPH);
  ecam_lcd.write(FLAP_EDGE_PARALLEL_GLYPH);
  ecam_lcd.write(FLAP_NODE_TOP_GLYPH);
  ecam_lcd.write(FLAP_EDGE_PARALLEL_GLYPH);
  ecam_lcd.write(FLAP_NODE_TOP_GLYPH);
  ecam_lcd.write(FLAP_EDGE_PARALLEL_GLYPH);
  ecam_lcd.write(FLAP_NODE_TOP_GLYPH);
}


void flaps_1() {
  ecam_lcd.clear();
  ecam_lcd.print("Flaps            10%");
  ecam_lcd.setCursor(0, 1);
  ecam_lcd.print("1");
  ecam_lcd.setCursor(5, 1);
  ecam_lcd.write(FLAP_NODE_TOP_GLYPH);
  ecam_lcd.write(FLAP_EDGE_PARALLEL_GLYPH);
  ecam_lcd.write(FLAP_NODE_TOP_GLYPH);
  ecam_lcd.write(FLAP_EDGE_PARALLEL_GLYPH);
  ecam_lcd.write(FLAP_NODE_TOP_GLYPH);
  ecam_lcd.write(FLAP_EDGE_PARALLEL_GLYPH);
  ecam_lcd.write(FLAP_NODE_TOP_GLYPH);
  ecam_lcd.write(FLAP_EDGE_PARALLEL_GLYPH);
  ecam_lcd.write(FLAP_NODE_TOP_GLYPH);
  ecam_lcd.write(FLAP_EDGE_DOWNWARD_1_GLYPH);
  ecam_lcd.write(FLAP_NODE_MIDDLE_GLYPH);
}


void flaps_2() {
  ecam_lcd.clear();
  ecam_lcd.print("Flaps            30%");
  ecam_lcd.setCursor(0, 1);
  ecam_lcd.print("2");
  ecam_lcd.setCursor(5, 1);
  ecam_lcd.write(FLAP_NODE_MIDDLE_GLYPH);
  ecam_lcd.write(FLAP_EDGE_UPWARD_1_GLYPH);
  ecam_lcd.write(FLAP_NODE_TOP_GLYPH);
  ecam_lcd.write(FLAP_EDGE_PARALLEL_GLYPH);
  ecam_lcd.write(FLAP_NODE_TOP_GLYPH);
  ecam_lcd.write(FLAP_EDGE_PARALLEL_GLYPH);
  ecam_lcd.write(FLAP_NODE_TOP_GLYPH);
  ecam_lcd.write(FLAP_EDGE_PARALLEL_GLYPH);
  ecam_lcd.write(FLAP_NODE_TOP_GLYPH);
  ecam_lcd.write(FLAP_EDGE_DOWNWARD_1_GLYPH);
  ecam_lcd.write(FLAP_NODE_MIDDLE_GLYPH);
}


void flaps_3() {
  ecam_lcd.clear();
  ecam_lcd.print("Flaps            50%");
  ecam_lcd.setCursor(0, 1);
  ecam_lcd.print("3");
  ecam_lcd.setCursor(5, 1);
  ecam_lcd.write(FLAP_NODE_MIDDLE_GLYPH);
  ecam_lcd.write(FLAP_EDGE_UPWARD_1_GLYPH);
  ecam_lcd.write(FLAP_NODE_TOP_GLYPH);
  ecam_lcd.write(FLAP_EDGE_PARALLEL_GLYPH);
  ecam_lcd.write(FLAP_NODE_TOP_GLYPH);
  ecam_lcd.write(FLAP_EDGE_PARALLEL_GLYPH);
  ecam_lcd.write(FLAP_NODE_TOP_GLYPH);
  ecam_lcd.write(FLAP_EDGE_DOWNWARD_1_GLYPH);
  ecam_lcd.write(FLAP_NODE_MIDDLE_GLYPH);
  ecam_lcd.write(FLAP_EDGE_DOWNWARD_2_GLYPH);
  ecam_lcd.write(FLAP_NODE_BOTTOM_GLYPH);
}


void flaps_full() {
  ecam_lcd.clear();
  ecam_lcd.print("Flaps           100%");
  ecam_lcd.setCursor(0, 1);
  ecam_lcd.print("Full");
  ecam_lcd.setCursor(5, 1);
  ecam_lcd.write(FLAP_NODE_BOTTOM_GLYPH);
  ecam_lcd.write(FLAP_EDGE_UPWARD_2_GLYPH);
  ecam_lcd.write(FLAP_NODE_MIDDLE_GLYPH);
  ecam_lcd.write(FLAP_EDGE_UPWARD_1_GLYPH);
  ecam_lcd.write(FLAP_NODE_TOP_GLYPH);
  ecam_lcd.write(FLAP_EDGE_PARALLEL_GLYPH);
  ecam_lcd.write(FLAP_NODE_TOP_GLYPH);
  ecam_lcd.write(FLAP_EDGE_DOWNWARD_1_GLYPH);
  ecam_lcd.write(FLAP_NODE_MIDDLE_GLYPH);
  ecam_lcd.write(FLAP_EDGE_DOWNWARD_2_GLYPH);
  ecam_lcd.write(FLAP_NODE_BOTTOM_GLYPH);
}
