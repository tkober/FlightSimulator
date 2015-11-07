#include <LiquidCrystal.h>

extern LiquidCrystal ap_lcd_1;
extern LiquidCrystal ap_lcd_2;

extern void ap_setup();
extern void ap_tick();

extern void read_speed(char token);
extern void read_speed_active(char token);
extern void read_heading(char token);
extern void read_heading_active(char token);
extern void read_course(char token);
extern void read_course_active(char token);
extern void read_altitude(char token);
extern void read_altitude_active(char token);
extern void read_vertical_speed(char token);
extern void read_ap_status(char token);
extern void read_a_thr_status(char token);
extern void read_apr_hold_status(char token);
extern void read_nav_hold_status(char token);
extern void read_glideslope_hold_status(char token);
