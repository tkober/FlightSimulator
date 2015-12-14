#ifdef CONTROLLER_C

#include <SPI.h>
#include <EEPROM.h>
#include <boards.h>
#include <RBL_nRF8001.h>
#include <Wire.h>


#include "PFDConnector.h"


void pfd_connector_setup() {
  ble_set_name("PFD"); 
  ble_begin();
}


void pfd_connector_tick() {
  ble_do_events();
}


#endif
