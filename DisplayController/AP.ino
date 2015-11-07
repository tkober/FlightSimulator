#ifdef CONTROLLER_B

#include "AP.h"
#include "Pins.h"
#include "MFD.h"
#include "Glyphs.h"
#include "SimConnectInputs.h"
#include "Button.h"
#include "RotaryEncoder.h"
#include "Math.h"


typedef enum {
  AltitudeChangeHundred = 0,
  AltitudeChangeThousand,
  AltitudeChangeSize
} AltitudeChange;


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

void toggle_ap_pressed();
void toggle_a_thr_pressed();

void toggle_apr_pressed();
void toggle_loc_pressed();

void toggle_altitude_change_pressed();

void toggle_flight_director_pressed();

void toggle_nav_gps_pressed();

int value_for_altitude_change(AltitudeChange altitudeChange);


#define AP1_LCD_ROW_COUNT   2
#define AP1_LCD_COL_COUNT   16
#define AP2_LCD_ROW_COUNT   2
#define AP2_LCD_COL_COUNT   16

#define ACTIVATED_INDICATOR_GLYPH_INDEX 0
#define ACTIVATED_INDICATOR_GLYPH       byte(ACTIVATED_INDICATOR_GLYPH_INDEX)

#define GLIDESLOPE_GLYPH_INDEX          1
#define GLIDESLOPE_GLYPH                byte(GLIDESLOPE_GLYPH_INDEX)

#define NAV_HOLD_GLYPH_INDEX            1
#define NAV_HOLD_GLYPH                  byte(NAV_HOLD_GLYPH_INDEX)

#define APR_HOLD_GLYPH_INDEX            2
#define APR_HOLD_GLYPH                  byte(APR_HOLD_GLYPH_INDEX)


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
char nav_1_lock_active[AP_COMPONENT_STATUS_SIZE]      = {'0'};
char approach_hold_active[AP_COMPONENT_STATUS_SIZE]   = {'0'};
char glideslope_hold_active[AP_COMPONENT_STATUS_SIZE]  = {'0'};

LiquidCrystal ap_lcd_1 = LiquidCrystal(AP_LCD_1_PINS);
LiquidCrystal ap_lcd_2 = LiquidCrystal(AP_LCD_2_PINS);

PushableRotaryEncoder changeSpeedRotaryEncoder = PushableRotaryEncoder(AP_CHANGE_SPD_ENCODER_PIN_A, AP_CHANGE_SPD_ENCODER_PIN_B, AP_CHANGE_SPD_BUTTON_PIN, 1);
PushableRotaryEncoder changeHeadingRotaryEncoder = PushableRotaryEncoder(AP_CHANGE_HDG_ENCODER_PIN_A, AP_CHANGE_HDG_ENCODER_PIN_B, AP_CHANGE_HDG_BUTTON_PIN, 1);
PushableRotaryEncoder changeCourseRotaryEncoder = PushableRotaryEncoder(AP_CHANGE_CRS_ENCODER_PIN_A, AP_CHANGE_CRS_ENCODER_PIN_B, AP_CHANGE_CRS_BUTTON_PIN, 1);
PushableRotaryEncoder changeAltitudeRotaryEncoder = PushableRotaryEncoder(AP_CHANGE_ALT_ENCODER_PIN_A, AP_CHANGE_ALT_ENCODER_PIN_B, AP_CHANGE_ALT_BUTTON_PIN, 1);
PushableRotaryEncoder changeVerticalSpeedRotaryEncoder = PushableRotaryEncoder(AP_CHANGE_VS_ENCODER_PIN_A, AP_CHANGE_VS_ENCODER_PIN_B, AP_CHANGE_VS_BUTTON_PIN, 1);

Button toggleAPButton = Button(TOGGLE_AP_PIN, 1);
Button toggleATHRButton = Button(TOGGLE_A_THR_PIN, 1);

Button toggleAPRButton = Button(TOGGLE_APR_PIN, 1);
Button toggleLOCButton = Button(TOGGLE_LOC_PIN, 1);

Button toggleAltitudeChangeButton = Button(TOGGLE_FL_CHANGE_PIN, 1);

Button toggleFlightDirectorButton = Button(TOGGLE_FD_PIN, 1);

Button toggleNAVGPSButton = Button(TOGGLE_NAV_GPS_PIN, 1);

int ap1_updated = 0;
int ap2_updated = 0;
int status_updated = 0;

AltitudeChange altitudeChange = AltitudeChangeHundred;


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
  changeCourseRotaryEncoder.setOnClick(NULL);
    
  changeAltitudeRotaryEncoder.setOnRotateClockwise(incrementAltitude);
  changeAltitudeRotaryEncoder.setOnRotateCounterClockwise(decrementAltitude);
  changeAltitudeRotaryEncoder.setOnClick(toggleAlitiudeHold);
    
  changeVerticalSpeedRotaryEncoder.setOnRotateClockwise(incrementVerticalSpeed);
  changeVerticalSpeedRotaryEncoder.setOnRotateCounterClockwise(decrementVerticalSpeed);
  changeVerticalSpeedRotaryEncoder.setOnClick(levelOff);

  toggleAPButton.setOnClick(toggle_ap_pressed);
  toggleATHRButton.setOnClick(toggle_a_thr_pressed);

  toggleAPRButton.setOnClick(toggle_apr_pressed);
  toggleLOCButton.setOnClick(toggle_loc_pressed);
    
  toggleAltitudeChangeButton.setOnClick(toggle_altitude_change_pressed);

  toggleFlightDirectorButton.setOnClick(toggle_flight_director_pressed);

  toggleNAVGPSButton.setOnClick(toggle_nav_gps_pressed);
    
  ap_lcd_1.createChar(ACTIVATED_INDICATOR_GLYPH_INDEX, ACTIVED_INDICATOR);
  ap_lcd_1.createChar(NAV_HOLD_GLYPH_INDEX, NAV_HOLD);
  ap_lcd_1.createChar(APR_HOLD_GLYPH_INDEX, APR_HOLD);
  ap_lcd_1.begin(AP1_LCD_COL_COUNT, AP1_LCD_ROW_COUNT);
  
  ap_lcd_2.createChar(ACTIVATED_INDICATOR_GLYPH_INDEX, ACTIVED_INDICATOR);
  ap_lcd_2.createChar(GLIDESLOPE_GLYPH_INDEX, GLIDESLOPE);
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
  toggleAPButton.tick();
  toggleATHRButton.tick();
  toggleAPRButton.tick();
  toggleLOCButton.tick();
  toggleAltitudeChangeButton.tick();
  toggleFlightDirectorButton.tick();
  toggleNAVGPSButton.tick();
  
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


void read_apr_hold_status(char token) {
  store_token(token, approach_hold_active, AP_COMPONENT_STATUS_SIZE, &ap1_updated);
}


void read_nav_hold_status(char token) {
  store_token(token, nav_1_lock_active, AP_COMPONENT_STATUS_SIZE, &ap1_updated);
}


void read_glideslope_hold_status(char token) {
  store_token(token, glideslope_hold_active, AP_COMPONENT_STATUS_SIZE, &ap2_updated);
}


// Private

void update_ap1_display() {
  ap_lcd_1.clear();
  ap_lcd_1.setCursor(0, 0);
  ap_lcd_1.write("SPD   HDG    CRS");
  ap_lcd_1.setCursor(0, 1);
  print_string_to_lcd(ap_lcd_1, speed, SPEED_SIZE);
  ap_lcd_1.write(speed_hold_active[0] == '1' ? ACTIVATED_INDICATOR_GLYPH : ' ');
  ap_lcd_1.write("  ");
  print_string_to_lcd(ap_lcd_1, heading, HEADING_SIZE);
  ap_lcd_1.write(heading_hold_active[0] == '1' ? ACTIVATED_INDICATOR_GLYPH : ' ');
  if (nav_1_lock_active[0] == '1') {
    ap_lcd_1.write(NAV_HOLD_GLYPH);
  } else if (approach_hold_active[0] == '1') {
    ap_lcd_1.write(APR_HOLD_GLYPH);
  } else {
    ap_lcd_1.write(' ');
  }
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
  ap_lcd_2.write(glideslope_hold_active[0] == '1' ? GLIDESLOPE_GLYPH : ' ');
  ap_lcd_2.write("   ");
  print_string_to_lcd(ap_lcd_2, vertical_speed, VERTICAL_SPEED_SIZE);
  switch (altitudeChange) {
      case AltitudeChangeHundred:
          ap_lcd_2.setCursor(2, 1);
          break;
            
      case AltitudeChangeThousand:
          ap_lcd_2.setCursor(1, 1);
          break;
  }
  ap_lcd_2.cursor();
}


void update_status_leds() {
  digitalWrite(AP_STATUS_PIN, ap_active[0] == '1' ? HIGH : LOW);
  digitalWrite(A_THR_STATUS_PIN, a_thr_active[0] == '1' ? HIGH : LOW);
}


// SPD
void incrementSpeed(int boost) {
  int change = boost == 1 ? 10 : 1;
  int spd = int_from_string(speed, SPEED_SIZE, 0);
  spd += change;
  spd = min(spd, 999);
  Serial.write(SET_REFERENCE_SPEED);
  Serial.println(spd);
}


void decrementSpeed(int boost) {
  int change = boost == 1 ? 10 : 1;
  int spd = int_from_string(speed, SPEED_SIZE, 0);
  spd -= change;
  spd = max(spd, 0);
  Serial.write(SET_REFERENCE_SPEED);
  Serial.println(spd);
}


void toggleSpeedHold() {
  Serial.println(SPEED_HEADING_HOLD);
}


// HDG
void incrementHeading(int boost) {
  int change = boost == 1 ? 10 : 1;
  int hdg = int_from_string(heading, HEADING_SIZE, 0);
  hdg += change;
  hdg = hdg % 360;
  Serial.write(SET_REFERENCE_HEADING);
  Serial.println(hdg);
}


void decrementHeading(int boost) {
  int change = boost == 1 ? 10 : 1;
  int hdg = int_from_string(heading, HEADING_SIZE, 0);
  hdg -= change;
  if (hdg < 1) {
    hdg += 360;
  }
  Serial.write(SET_REFERENCE_HEADING);
  Serial.println(hdg);
}


void toggleHeadingHold() {
  if (heading_hold_active[0] == '1') {
    Serial.println(TOGGLE_HEADING_HOLD);
  } else {
    int hdg = int_from_string(heading, HEADING_SIZE, 0);
    Serial.println(TOGGLE_HEADING_HOLD);
    Serial.write(SET_REFERENCE_HEADING);
    Serial.println(hdg);
  }
}


// CRS
void incrementCourse(int boost) {
  int change = boost == 1 ? 10 : 1;
  int crs = int_from_string(course, COURSE_SIZE, 0);
  crs += change;
  crs = crs % 360;
  Serial.write(SET_REFERENCE_COURSE);
  Serial.println(crs);
}


void decrementCourse(int boost) {
  int change = boost == 1 ? 10 : 1;
  int crs = int_from_string(course, COURSE_SIZE, 0);
  crs -= change;
  if (crs < 1) {
    crs += 360;
  }
  Serial.write(SET_REFERENCE_COURSE);
  Serial.println(crs);
}


// ALT
void incrementAltitude(int boost) {
  int alt = int_from_string(altitude, ALTITUDE_SIZE, 0);
  alt += value_for_altitude_change(altitudeChange);
  alt = min(alt, 99999);
  Serial.write(SET_REFERENCE_ALTITUDE);
  Serial.println(alt);
}


void decrementAltitude(int boost) {
  int alt = int_from_string(altitude, ALTITUDE_SIZE, 0);
  alt -= value_for_altitude_change(altitudeChange);
  alt = max(alt, 0);
  Serial.write(SET_REFERENCE_ALTITUDE);
  Serial.println(alt);
}


void toggleAlitiudeHold() {
  if (altitude_hold_active[0] == '1') {
    Serial.println(TOGGLE_ALTITUDE_HOLD);
  } else {
    int alt = int_from_string(altitude, ALTITUDE_SIZE, 0);
    Serial.println(TOGGLE_ALTITUDE_HOLD);
    Serial.write(SET_REFERENCE_ALTITUDE);
    Serial.println(alt);
  }
}


// V/S
void incrementVerticalSpeed(int boost) {
  Serial.println(INCREMENT_REFERENCE_VERTICAL_SPEED);
}


void decrementVerticalSpeed(int boost) {
  Serial.println(DECREMENT_REFERENCE_VERTICAL_SPEED);
}


void levelOff() {
  if (altitude_hold_active[0] == '1') {
    Serial.println(TOGGLE_ALTITUDE_HOLD);
    Serial.println(TOGGLE_ALTITUDE_HOLD);
  } else {
    Serial.println(TOGGLE_ALTITUDE_HOLD);
  }
}


void toggle_ap_pressed() {
  Serial.println(TOGGLE_AP);
}


void toggle_a_thr_pressed() {
  Serial.println(TOGGLE_A_THR);
}


void toggle_apr_pressed() {
  Serial.println(TOGGLE_APR);
}


void toggle_loc_pressed() {
  Serial.println(TOGGLE_LOC);
}


void toggle_altitude_change_pressed() {
  altitudeChange = (AltitudeChange)((altitudeChange + 1) % AltitudeChangeSize);
  update_ap2_display();
}


void toggle_flight_director_pressed() {
  Serial.println(TOGGLE_FD);
}


void toggle_nav_gps_pressed() {
  Serial.println(TOGGLE_NAV_GPS);
}


int value_for_altitude_change(AltitudeChange altitudeChange) {
  switch (altitudeChange) {
    case AltitudeChangeHundred:
      return 100;

    case AltitudeChangeThousand:
      return 1000;
  }
}


#endif
