#include <LiquidCrystal.h>

#include "SerialReader.h"

#include "Radio.h"
#include "AP.h"
#include "ECAM.h"
#include "ElevatorTrim.h"
#include "RudderTrim.h"


void setup() {
  serial_setup();
  radio_setup();
  ecam_setup();
}

void loop() {
  read_serial();
  radio_tick();
  ecam_tick();
}


