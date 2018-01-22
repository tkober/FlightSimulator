#include "ParkingBrake.h"

#include "Button.h"
#include "Pins.h"
#include "SimConnectInputs.h"


Button parking_brake_on_button  = Button(PARKING_BRAKE_ON_PIN, 1);
Button parking_brake_off_button = Button(PARKING_BRAKE_OFF_PIN, 1);


void parking_brake_setup() {
  parking_brake_on_button.setOnClick(set_parking_brake_on);
  parking_brake_on_button.setRepeatOnHold(1);

  parking_brake_off_button.setOnClick(set_parking_brake_off);
  parking_brake_off_button.setRepeatOnHold(1);
}


void parking_brake_tick() {
  parking_brake_on_button.tick();
  parking_brake_off_button.tick();
}


void set_parking_brake_on(int buttonId) {
  Serial.println(PARKING_BRAKE_ON);
}


void set_parking_brake_off(int buttonId) {
  Serial.println(PARKING_BRAKE_OFF);
}
