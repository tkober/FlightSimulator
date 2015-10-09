#include <LiquidCrystal.h>

extern LiquidCrystal radio_lcd;

extern void radio_setup();
extern void radio_tick();

extern void read_com1_freq(char token);
extern void read_com1_sb_freq(char token);
extern void read_com2_freq(char token);
extern void read_com2_sb_freq(char token);
extern void read_nav1_freq(char token);
extern void read_nav1_sb_freq(char token);
extern void read_nav2_freq(char token);
extern void read_nav2_sb_freq(char token);
