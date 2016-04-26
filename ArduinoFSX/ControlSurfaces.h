#ifndef CONTROL_SURFACE_H
#define CONTROL_SURFACE_H

#include <LiquidCrystal.h>

extern LiquidCrystal elevator_trim_lcd;
extern LiquidCrystal rudder_trim_lcd;

extern void control_surface_setup();
extern void control_surface_tick();

extern void read_elevator_trim(char token);
extern void read_ruddert_trim(char token);
extern void controls_read_spoilers_armed(char token);

#endif
