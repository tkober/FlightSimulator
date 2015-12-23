#include <LiquidCrystal.h>

extern LiquidCrystal ecam_lcd;

extern void ecam_setup();
extern void ecam_tick();

extern void read_n1_engine1(char token);
extern void read_n1_engine2(char token);
extern void read_egt_engine1(char token);
extern void read_egt_engine2(char token);
extern void read_fuel_flow_engine1(char token);
extern void read_fuel_flow_engine2(char token);
extern void read_thrust_levers_position_engine1(char token);
extern void read_thrust_levers_position_engine2(char token);

extern void read_flaps_position(char token);
extern void read_spoilers_position(char token);
extern void read_spoilers_armed(char token);
