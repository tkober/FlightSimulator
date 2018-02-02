#include <LiquidCrystal.h>

// Serial Com
#include "SerialReader.h"

// Panels
#include "ParkingBrake.h"
#include "Gear.h"
#include "Flaps.h"
//#include "Radio.h"
#include "Trim.h"


int initialized = 0;


void setup() {
  serial_setup();

  parking_brake_setup();
  gear_setup();
  flaps_setup();
  //radio_setup();
  trim_setup();
}


void loop() {
  read_serial();

  parking_brake_tick();
  gear_tick();
  flaps_tick();
  //radio_tick();
  trim_tick();
}
