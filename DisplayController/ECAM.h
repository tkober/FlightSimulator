#include <LiquidCrystal.h>

extern LiquidCrystal ecam_lcd;

extern void ecam_setup();
extern void ecam_tick();

extern void read_flaps_position(char token);
extern void read_spoilers_position(char token);
extern void read_spoilers_armed(char token);
