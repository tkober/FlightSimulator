#include <LiquidCrystal.h>

extern LiquidCrystal elevator_trim_lcd;
extern LiquidCrystal rudder_trim_lcd;

void trim_setup();
void trim_tick();

extern void read_elevator_trim(char token);
extern void read_ruddert_trim(char token);
