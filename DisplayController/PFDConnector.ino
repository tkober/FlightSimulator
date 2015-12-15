#ifdef CONTROLLER_C

#include <SPI.h>
#include <EEPROM.h>
#include <boards.h>
#include <RBL_nRF8001.h>
#include <Wire.h>


#include "PFDConnector.h"


#define SPEED_SIZE 3
char airspeed[SPEED_SIZE] = {'0', '0', '0'};
int airspeed_updated      = 0;

char ground_speed[SPEED_SIZE] = {'0', '0', '0'};
int ground_speed_updated      = 0;

#define ALTITUDE_SIZE 5
char altitude[ALTITUDE_SIZE]  = {'0', '0', '0', '0', '0'};
int altitude_updated          = 0;

#define HEADING_SIZE 3
char heading[HEADING_SIZE]  = {'0', '0', '0'};
int heading_updated         = 0;

char ground_clearance[ALTITUDE_SIZE]  = {'0', '0', '0', '0', '0'};
int ground_clearance_updated          = 0;

#define STATUS_SIZE 1
char aircraft_on_ground[STATUS_SIZE]  = {'0'};
int aircraft_on_ground_updated        = 0;

#define OFFSET_SIZE 4
char ils_loc_offset[OFFSET_SIZE]  = {'+', '0', '0', '0'};
int ils_loc_offset_updated        = 0;

char ils_gs_offset[OFFSET_SIZE] = {'+', '0', '0', '0'};
int ils_gs_offset_updated       = 0;

char reference_speed[SPEED_SIZE]  = {'0', '0', '0'};
int reference_speed_updated       = 0;

char speed_hold_active[STATUS_SIZE] = {'0'};
int speed_hold_active_updated       = 0;

char reference_altitude[ALTITUDE_SIZE]  = {'0', '0', '0', '0', '0'};
int reference_altitude_updated          = 0;

char altitude_hold_active[STATUS_SIZE] = {'0'};
int altitude_hold_active_updated       = 0;

#define ANGLE_SIZE 6
char pitch[ANGLE_SIZE]  = {'+', '0', '0', '0', '.', '0'};
int pitch_update        = 0;

char roll[ANGLE_SIZE] = {'+', '0', '0', '0', '.', '0'};
int roll_updated = 0;

#define ALTIMETER_SETTING_SIZE 5
char altimeter_setting[ALTIMETER_SETTING_SIZE]  = {'0', '0', '.', '0', '0'};
int altimeter_setting_updated                   = 0;

char overspeed_warning[STATUS_SIZE] = {'0'};
int overspeed_warning_updated       = 0;

char stall_warning[STATUS_SIZE] = {'0'};
int stall_warning_updated       = 0;

#define G_FORCE_SIZE 6
char g_force[G_FORCE_SIZE]  = {'+', '0', '0', '.', '0', '0'};
int g_force_updated         = 0;

#define ANGLE_OF_ATTACK_SIZE 5
char angle_of_attack[ANGLE_OF_ATTACK_SIZE]  = {'0', '0', '.', '0', '0'};
int angle_of_attack_updated                 = 0;

char to_ga_active[STATUS_SIZE]  = {'0'};
int to_ga_active_updated        = 0;


void pfd_connector_setup() {
  ble_set_name("PFD");
  ble_begin();
}


void pfd_connector_tick() {
  ble_do_events();

  if (airspeed_updated) {
    airspeed_updated = 0;
  }

  if (ground_speed_updated) {
    ground_speed_updated = 0;
  }

  if (altitude_updated) {
    altitude_updated = 0;
  }

  if (heading_updated) {
    heading_updated = 0;
  }

  if (ground_clearance_updated) {
    ground_clearance_updated = 0;
  }

  if (aircraft_on_ground_updated) {
    aircraft_on_ground_updated = 0;
  }

  if (ils_loc_offset_updated) {
    ils_loc_offset_updated = 0;
  }

  if (ils_gs_offset_updated) {
    ils_gs_offset_updated = 0;
  }

  if (reference_speed_updated) {
    reference_speed_updated = 0;
  }

  if (speed_hold_active_updated) {
    speed_hold_active_updated = 0;
  }

  if (reference_altitude_updated) {
    reference_altitude_updated = 0;
  }

  if (altitude_hold_active_updated) {
    altitude_hold_active_updated = 0;
  }

  if (pitch_update) {
    pitch_update = 0;
  }

  if (roll_updated) {
    roll_updated = 0;
  }

  if (altimeter_setting_updated) {
    altimeter_setting_updated = 0;
  }

  if (overspeed_warning_updated) {
    overspeed_warning_updated = 0;
  }

  if (stall_warning_updated) {
    stall_warning_updated = 0;
  }

  if (g_force_updated) {
    g_force_updated = 0;
  }

  if (angle_of_attack_updated) {
    angle_of_attack_updated = 0;
  }

  if (to_ga_active_updated) {
    to_ga_active_updated = 0;
  }
}




void pfd_read_airspeed(char token) {
  store_token(token, airspeed, SPEED_SIZE, &airspeed_updated);
}


void pfd_read_ground_speed(char token) {
  store_token(token, ground_speed, SPEED_SIZE, &ground_speed_updated);
}


void pfd_read_altitude(char token) {
  store_token(token, altitude, ALTITUDE_SIZE, &altitude_updated);
}


void pfd_read_heading(char token) {
  store_token(token, heading, HEADING_SIZE, &heading_updated);
}


void pfd_read_ground_clearance(char token) {
  store_token(token, ground_clearance, ALTITUDE_SIZE, &ground_clearance_updated);
}


void pfd_read_aircraft_on_ground(char token) {
  store_token(token, aircraft_on_ground, STATUS_SIZE, &aircraft_on_ground_updated);
}


void pfd_read_ils_loc_offset(char token) {
  store_token(token, ils_loc_offset, OFFSET_SIZE, &ils_loc_offset_updated);
}


void pfd_read_ils_gs_offset(char token) {
  store_token(token, ils_gs_offset, OFFSET_SIZE, &ils_gs_offset_updated);
}


void pfd_read_reference_speed(char token) {
  store_token(token, reference_speed, SPEED_SIZE, &reference_speed_updated);
}


void pfd_read_speed_hold_active(char token) {
  store_token(token, speed_hold_active, STATUS_SIZE, &speed_hold_active_updated);
}


void pfd_read_reference_altitude(char token) {
  store_token(token, reference_altitude, ALTITUDE_SIZE, &reference_altitude_updated);
}


void pfd_read_altitude_hold_active(char token) {
  store_token(token, altitude_hold_active, STATUS_SIZE, &altitude_hold_active_updated);
}


void pfd_read_pitch(char token) {
  store_token(token, pitch, ANGLE_SIZE, &pitch_updated);
}


void pfd_read_roll(char token) {
  store_token(token, roll, ANGLE_SIZE, &roll_updated);
}


void pfd_read_altimeter_setting(char token) {
  store_token(token, altimeter_setting, ALTIMETER_SETTING_SIZE, &altimeter_setting_updated);
}


void pfd_read_overspeed_warning(char token) {
  store_token(token, overspeed_warning, STATUS_SIZE, &overspeed_warning_updated);
}


void pfd_read_stall_warning(char token) {
  store_token(token, stall_warning, STATUS_SIZE, &stall_warning_updated);
}


void pfd_read_g_force(char token) {
  store_token(token, g_force, G_FORCE_SIZE, &g_force_updated);
}


void pfd_read_angle_of_attack(char token) {
  store_token(token, angle_of_attack, ANGLE_OF_ATTACK_SIZE, &angle_of_attack_updated);
}


void pfd_read_to_ga_active(char token) {
  store_token(token, to_ga_active, STATUS_SIZE, &to_ga_active_updated);
}


#endif
