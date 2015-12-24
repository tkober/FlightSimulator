#include <LiquidCrystal.h>

#include "Prefix.h"

#ifdef CONTROLLER_A
  #include <SPI.h>
  #include <EEPROM.h>
  #include <boards.h>
  #include <RBL_nRF8001.h>
  #include <Wire.h>
#endif



int initialized = 0;


void setup() {
  serial_setup();

#ifdef CONTROLLER_A
  ecam_setup();
  radio_setup();
  gear_setup();
  systems_setup();
#endif

#ifdef CONTROLLER_B
  ap_setup();
  control_surface_setup();
#endif

#ifdef CONTROLLER_C
  pfd_connector_setup();
#endif

  while(initialized == 0) {
    initialized = read_initialization();
  }
  initialized = 0;
  while(initialized == 0) {
    initialized = process_initialization_data();
  }
}

void loop() {
  read_serial();

#ifdef CONTROLLER_A
  ecam_tick();
  radio_tick();
  gear_tick();
  systems_tick();
#endif

#ifdef CONTROLLER_B
  ap_tick();
  control_surface_tick();
#endif

#ifdef CONTROLLER_C
  pfd_connector_tick();
#endif
}
