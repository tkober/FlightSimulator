#include "AP.h"
#include "Pins.h"
#include "MFD.h"
#include "Glyphs.h"
#include "SimConnectInputs.h"


void update_ap1_display();
void update_ap2_display();
void update_status_leds();

void incrementSpeed(int boost);
void decrementSpeed(int boost);
void toggleSpeedHold();

void incrementHeading(int boost);
void decrementHeading(int boost);
void toggleHeadingHold();

void incrementCourse(int boost);
void decrementCourse(int boost);

void incrementAltitude(int boost);
void decrementAltitude(int boost);
void toggleAlitiudeHold();

void incrementVerticalSpeed(int boost);
void decrementVerticalSpeed(int boost);
void levelOff();


#define AP1_LCD_ROW_COUNT   2
#define AP1_LCD_COL_COUNT   16
#define AP2_LCD_ROW_COUNT   2
#define AP2_LCD_COL_COUNT   16

#define ACTIVATED_INDICATOR_GLYPH_INDEX 0
#define ACTIVATED_INDICATOR_GLYPH       byte(ACTIVATED_INDICATOR_GLYPH_INDEX)


#define SPEED_SIZE                        3
char speed[SPEED_SIZE]                    = {'-', '-', '-'};
#define HEADING_SIZE                      3
char heading[HEADING_SIZE]                = {'-', '-', '-'};
#define COURSE_SIZE                       3
char course[COURSE_SIZE]                  = {'-', '-', '-'};
#define ALTITUDE_SIZE                     5
char altitude[ALTITUDE_SIZE]              = {'-', '-', '-', '-', '-'};
#define VERTICAL_SPEED_SIZE               5
char vertical_speed[VERTICAL_SPEED_SIZE]  = {'-', '-', '-', '-', '-'};

#define AP_COMPONENT_STATUS_SIZE  1
char speed_hold_active[AP_COMPONENT_STATUS_SIZE]      = {'0'};
char heading_hold_active[AP_COMPONENT_STATUS_SIZE]    = {'0'};
char altitude_hold_active[AP_COMPONENT_STATUS_SIZE]   = {'0'};
char ap_active[AP_COMPONENT_STATUS_SIZE]              = {'0'};
char a_thr_active[AP_COMPONENT_STATUS_SIZE]           = {'0'};

LiquidCrystal ap_lcd_1 = LiquidCrystal(AP_LCD_1_PINS);
LiquidCrystal ap_lcd_2 = LiquidCrystal(AP_LCD_2_PINS);

PushableRotaryEncoder changeSpeedRotaryEncoder = PushableRotaryEncoder(AP_CHANGE_SPD_ENCODER_PIN_A, AP_CHANGE_SPD_ENCODER_PIN_B, AP_CHANGE_SPD_BUTTON_PIN, 1);
PushableRotaryEncoder changeHeadingRotaryEncoder = PushableRotaryEncoder(AP_CHANGE_HDG_ENCODER_PIN_A, AP_CHANGE_HDG_ENCODER_PIN_B, AP_CHANGE_HDG_BUTTON_PIN, 1);
PushableRotaryEncoder changeCourseRotaryEncoder = PushableRotaryEncoder(AP_CHANGE_CRS_ENCODER_PIN_A, AP_CHANGE_CRS_ENCODER_PIN_B, AP_CHANGE_CRS_BUTTON_PIN, 1);
PushableRotaryEncoder changeAltitudeRotaryEncoder = PushableRotaryEncoder(AP_CHANGE_ALT_ENCODER_PIN_A, AP_CHANGE_ALT_ENCODER_PIN_B, AP_CHANGE_ALT_BUTTON_PIN, 1);
PushableRotaryEncoder changeVerticalSpeedRotaryEncoder = PushableRotaryEncoder(AP_CHANGE_VS_ENCODER_PIN_A, AP_CHANGE_VS_ENCODER_PIN_B, AP_CHANGE_VS_BUTTON_PIN, 1);

int ap1_updated = 0;
int ap2_updated = 0;
int status_updated = 0;


// Public

void ap_setup() {
  changeSpeedRotaryEncoder.setOnRotateClockwise(incrementSpeed);
  changeSpeedRotaryEncoder.setOnRotateCounterClockwise(decrementSpeed);
  changeSpeedRotaryEncoder.setOnClick(toggleSpeedHold);
    
  changeHeadingRotaryEncoder.setOnRotateClockwise(incrementHeading);
  changeHeadingRotaryEncoder.setOnRotateCounterClockwise(decrementHeading);
  changeHeadingRotaryEncoder.setOnClick(toggleHeadingHold);
    
  changeCourseRotaryEncoder.setOnRotateClockwise(incrementCourse);
  changeCourseRotaryEncoder.setOnRotateCounterClockwise(decrementCourse);
    
  changeAltitudeRotaryEncoder.setOnRotateClockwise(incrementAltitude);
  changeAltitudeRotaryEncoder.setOnRotateCounterClockwise(decrementAltitude);
  changeAltitudeRotaryEncoder.setOnClick(toggleAlitiudeHold);
    
  changeVerticalSpeedRotaryEncoder.setOnRotateClockwise(incrementVerticalSpeed);
  changeVerticalSpeedRotaryEncoder.setOnRotateCounterClockwise(decrementVerticalSpeed);
  changeVerticalSpeedRotaryEncoder.setOnClick(levelOff);
    
  ap_lcd_1.createChar(ACTIVATED_INDICATOR_GLYPH_INDEX, ACTIVED_INDICATOR);
  ap_lcd_1.begin(AP1_LCD_COL_COUNT, AP1_LCD_ROW_COUNT);
  ap_lcd_2.createChar(ACTIVATED_INDICATOR_GLYPH_INDEX, ACTIVED_INDICATOR);
  ap_lcd_2.begin(AP2_LCD_COL_COUNT, AP2_LCD_ROW_COUNT);

  pinMode(AP_STATUS_PIN, OUTPUT);
  pinMode(A_THR_STATUS_PIN, OUTPUT);
    
  update_ap1_display();
  update_ap2_display();
  update_status_leds();
}


void ap_tick() {
  changeSpeedRotaryEncoder.tick();
  changeHeadingRotaryEncoder.tick();
  changeCourseRotaryEncoder.tick();
  changeAltitudeRotaryEncoder.tick();
  changeVerticalSpeedRotaryEncoder.tick();
  if (ap1_updated == 1) {
    ap1_updated = 0;
    update_ap1_display();
  }
  if (ap2_updated == 1) {
    ap2_updated = 0;
    update_ap2_display();
  }
  if (status_updated == 1) {
    status_updated = 0;
    update_status_leds();
  }
}


void read_speed(char token) {
  store_token(token, speed, SPEED_SIZE, &ap1_updated);
}


void read_speed_active(char token) {
  store_token(token, speed_hold_active, AP_COMPONENT_STATUS_SIZE, &ap1_updated);
}


void read_heading(char token) {
  store_token(token, heading, HEADING_SIZE, &ap1_updated);
}


void read_heading_active(char token) {
  store_token(token, heading_hold_active, AP_COMPONENT_STATUS_SIZE, &ap1_updated);
}


void read_course(char token) {
  store_token(token, course, COURSE_SIZE, &ap1_updated);
}


void read_altitude(char token) {
  store_token(token, altitude, ALTITUDE_SIZE, &ap2_updated);
}


void read_altitude_active(char token) {
  store_token(token, altitude_hold_active, AP_COMPONENT_STATUS_SIZE, &ap2_updated);
}


void read_vertical_speed(char token) {
  store_token(token, vertical_speed, VERTICAL_SPEED_SIZE, &ap2_updated);
}


void read_ap_status(char token) {
  store_token(token, ap_active, AP_COMPONENT_STATUS_SIZE, &status_updated);
}


void read_a_thr_status(char token) {
  store_token(token, a_thr_active, AP_COMPONENT_STATUS_SIZE, &status_updated);
}


// Private

void update_ap1_display() {
  ap_lcd_1.clear();
  ap_lcd_1.setCursor(0, 0);
  ap_lcd_1.write("SPD   HDG   CRS ");
  ap_lcd_1.setCursor(0, 1);
  print_string_to_lcd(ap_lcd_1, speed, SPEED_SIZE);
  ap_lcd_1.write(speed_hold_active[0] == '1' ? ACTIVATED_INDICATOR_GLYPH : ' ');
  ap_lcd_1.write("  ");
  print_string_to_lcd(ap_lcd_1, heading, HEADING_SIZE);
  ap_lcd_1.write(heading_hold_active[0] == '1' ? ACTIVATED_INDICATOR_GLYPH : ' ');
  ap_lcd_1.write("  ");
  print_string_to_lcd(ap_lcd_1, course, COURSE_SIZE);
}


void update_ap2_display() {
  ap_lcd_2.clear();
  ap_lcd_2.setCursor(0, 0);
  ap_lcd_2.write("ALT       V/S   ");
  ap_lcd_2.setCursor(0, 1);
  print_string_to_lcd(ap_lcd_2, altitude, ALTITUDE_SIZE);
  ap_lcd_2.write(altitude_hold_active[0] == '1' ? ACTIVATED_INDICATOR_GLYPH : ' ');
  ap_lcd_2.write("    ");
  print_string_to_lcd(ap_lcd_2, vertical_speed, VERTICAL_SPEED_SIZE);
}


void update_status_leds() {
  digitalWrite(AP_STATUS_PIN, ap_active[0] == '1' ? HIGH : LOW);
  digitalWrite(A_THR_STATUS_PIN, a_thr_active[0] == '1' ? HIGH : LOW);
}


// SPD
void incrementSpeed(int boost) {
  Serial.println(INCREMENT_REFERENCE_SPEED);
}


void decrementSpeed(int boost) {
  Serial.println(DECREMENT_REFERENCE_SPEED);
}


void toggleSpeedHold() {
  Serial.println(SPEED_HEADING_HOLD);
}


// HDG
void incrementHeading(int boost) {
  Serial.println(INCREMENT_REFERENCE_HEADING);
}


void decrementHeading(int boost) {
  Serial.println(DECREMENT_REFERENCE_HEADING);
}


void toggleHeadingHold() {
  Serial.println(TOGGLE_HEADING_HOLD);
}


// CRS
void incrementCourse(int boost) {
  Serial.println(INCREMENT_REFERENCE_COURSE);
}


void decrementCourse(int boost) {
  Serial.println(DECREMENT_REFERENCE_COURSE);
}


// ALT
void incrementAltitude(int boost) {
  Serial.println(INCREMENT_REFERENCE_ALTITUDE);
}


void decrementAltitude(int boost) {
  Serial.println(DECREMENT_REFERENCE_ALTITUDE);
}


void toggleAlitiudeHold() {
  Serial.println(TOGGLE_ALTITUDE_HOLD);
}


// V/S
void incrementVerticalSpeed(int boost) {
  Serial.println(INCREMENT_REFERENCE_VERTICAL_SPEED);
}


void decrementVerticalSpeed(int boost) {
  Serial.println(DECREMENT_REFERENCE_VERTICAL_SPEED);
}


void levelOff() {

}
