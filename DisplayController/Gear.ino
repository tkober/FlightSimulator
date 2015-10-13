#include "Gear.h"


#define NOSE_GEAR_EXTENDED_LED      13
#define NOSE_GEAR_UNLOCKED_LED      12
#define LEFT_GEAR_EXTENDED_LED      11
#define LEFT_GEAR_UNLOCKED_LED      10
#define RIGHT_GEAR_EXTENDED_LED     9
#define RIGHT_GEAR_UNLOCKED_LED     8


#define GEAR_POSITION_SIZE  3
char gear_position[GEAR_POSITION_SIZE+1]        = "222";

#define GEAR_RETRACTABKE_SIZE 1
char gear_retractable[GEAR_RETRACTABKE_SIZE+1]  = "1";


int gear_updated = 0;


typedef enum {
  NoseGear = 0,
  LeftGear,
  RightGear
} Gear;


typedef enum {
  GearPositionRetracted = 0,
  GearPositionUnlocked,
  GearPositionExtended
} GearPosition;


void update_gear_leds();
GearPosition position_for_gear(Gear gear);
void show_gear_position(GearPosition gearPosition, int retractable, int extended_led, int unlocked_led);


// Public

void gear_setup() {
  pinMode(NOSE_GEAR_EXTENDED_LED, OUTPUT);
  pinMode(NOSE_GEAR_UNLOCKED_LED, OUTPUT);
  pinMode(LEFT_GEAR_EXTENDED_LED, OUTPUT);
  pinMode(LEFT_GEAR_UNLOCKED_LED, OUTPUT);
  pinMode(RIGHT_GEAR_EXTENDED_LED, OUTPUT);
  pinMode(RIGHT_GEAR_UNLOCKED_LED, OUTPUT);
  update_gear_leds();  
}


void gear_tick() {
  if (gear_updated == 1) {
    update_gear_leds();
    gear_updated = 0;
  }
}


void read_gear_position(char token) {
  store_token(token, gear_position, GEAR_POSITION_SIZE, &gear_updated);
}


void read_gear_retractable(char token) {
  store_token(token, gear_retractable, GEAR_RETRACTABKE_SIZE, &gear_updated);
}


// Private

void update_gear_leds() {
  int gearRetractable = gear_retractable[0] == '1' ? 1 : 0;
  show_gear_position(position_for_gear(NoseGear), gearRetractable, NOSE_GEAR_EXTENDED_LED, NOSE_GEAR_UNLOCKED_LED);
  show_gear_position(position_for_gear(LeftGear), gearRetractable, LEFT_GEAR_EXTENDED_LED, LEFT_GEAR_UNLOCKED_LED);
  show_gear_position(position_for_gear(RightGear), gearRetractable, RIGHT_GEAR_EXTENDED_LED, RIGHT_GEAR_UNLOCKED_LED);
}


GearPosition position_for_gear(Gear gear) {
  char value = gear_position[gear];
  switch (value) {
    case '0':
      return GearPositionRetracted;

    case '2':
      return GearPositionExtended;
    
    default:
      return GearPositionUnlocked;
  }
}


void show_gear_position(GearPosition gearPosition, int retractable, int extended_led, int unlocked_led) {
  if (retractable == 0) {
    digitalWrite(extended_led, LOW);
    digitalWrite(unlocked_led, LOW);
    return;
  }
  switch (gearPosition) {
    case GearPositionRetracted:
      digitalWrite(extended_led, LOW);
      digitalWrite(unlocked_led, LOW);
      break;
      
    case GearPositionExtended:
      digitalWrite(extended_led, HIGH);
      digitalWrite(unlocked_led, LOW);
      break;
      
    case GearPositionUnlocked:
      digitalWrite(extended_led, LOW);
      digitalWrite(unlocked_led, HIGH);
      break;
  }
}

