#include <LiquidCrystal.h>

#include "SerialReader.h"

#include "Radio.h"
#include "AP.h"
#include "ECAM.h"
#include "ElevatorTrim.h"
#include "RudderTrim.h"
#include "Gear.h"


int initialized = 0;


void setup() {
  serial_setup();
  radio_setup();
  gear_setup();
  ecam_setup();

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
  
  radio_tick();
  ecam_tick();
}


