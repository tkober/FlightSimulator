#include "Gear.h"
#include "Pins.h"
#include "SimConnectInputs.h"


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

typedef enum {
  GearLeverPositionUnkown = 0,
  GearLeverPositionUp,
  GearLeverPositionDown
} GearLeverPosition;

typedef struct GearLeverPotiRange {
  int from;
  int to;
  GearLeverPosition position;
} GearLeverPotiRange;

#define GEAR_LEVER_POTI_RANGE_MAKE(_from, _to, _position)  { _from, _to, _position }

GearLeverPotiRange GEAR_UP_POTI_RANGE   = GEAR_LEVER_POTI_RANGE_MAKE(0, 100, GearLeverPositionUp);
GearLeverPotiRange GEAR_DOWN_POTI_RANGE = GEAR_LEVER_POTI_RANGE_MAKE(900, 1024, GearLeverPositionDown);

#define GEAR_LEVER_POTI_RANGES 2
GearLeverPotiRange gear_lever_poti_ranges[GEAR_LEVER_POTI_RANGES];

GearLeverPosition previous_gear_lever_position = GearLeverPositionUnkown;
long last_gear_lever_update = millis();

#define UPDATE_INTERVAL 2000L

extern GearLeverPosition gearLeverFromPotiValue(int value);
extern void update_gear_lever(GearLeverPosition position);


// Public

void gear_setup() {
  pinMode(NOSE_GEAR_EXTENDED_LED_PIN, OUTPUT);
  pinMode(NOSE_GEAR_UNLOCKED_LED_PIN, OUTPUT);
  pinMode(LEFT_GEAR_EXTENDED_LED_PIN, OUTPUT);
  pinMode(LEFT_GEAR_UNLOCKED_LED_PIN, OUTPUT);
  pinMode(RIGHT_GEAR_EXTENDED_LED_PIN, OUTPUT);
  pinMode(RIGHT_GEAR_UNLOCKED_LED_PIN, OUTPUT);
  update_gear_leds();

  gear_lever_poti_ranges[0] = GEAR_UP_POTI_RANGE;
  gear_lever_poti_ranges[1] = GEAR_DOWN_POTI_RANGE;
}


void gear_tick() {
  if (gear_updated == 1) {
    update_gear_leds();
    gear_updated = 0;
  }

  int gear_lever_poti_value = analogRead(GEAR_POTI_PIN);
  GearLeverPosition gear_lever_position = gearLeverFromPotiValue(gear_lever_poti_value);

  if (gear_lever_position != previous_gear_lever_position || (millis() - last_gear_lever_update > UPDATE_INTERVAL) ) {
    previous_gear_lever_position = gear_lever_position;
    update_gear_lever(gear_lever_position);
    last_gear_lever_update = millis();
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
  show_gear_position(position_for_gear(NoseGear), gearRetractable, NOSE_GEAR_EXTENDED_LED_PIN, NOSE_GEAR_UNLOCKED_LED_PIN);
  show_gear_position(position_for_gear(LeftGear), gearRetractable, LEFT_GEAR_EXTENDED_LED_PIN, LEFT_GEAR_UNLOCKED_LED_PIN);
  show_gear_position(position_for_gear(RightGear), gearRetractable, RIGHT_GEAR_EXTENDED_LED_PIN, RIGHT_GEAR_UNLOCKED_LED_PIN);
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


GearLeverPosition gearLeverFromPotiValue(int value) {
  for (int i = 0; i < GEAR_LEVER_POTI_RANGES; i++) {
    GearLeverPotiRange range = gear_lever_poti_ranges[i];
    if (value >= range.from && value <= range.to) {
      return range.position;
    }
  }
  return GearLeverPositionUnkown;
}


void update_gear_lever(GearLeverPosition position) {
  switch (position) {

    case GearLeverPositionUp:
      Serial.println(GEAR_UP);
      break;

    case GearLeverPositionDown:
      Serial.println(GEAR_DOWN);
      break;

    default:
      break;

  }
}
