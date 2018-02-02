#include "Flaps.h"

#include "Pins.h"
#include "SimConnectInputs.h"

#define AIRBUS_FLAPS_UP_INPUT       "C17000"
#define AIRBUS_FLAPS_1F_INPUT       "C17020"
#define AIRBUS_FLAPS_2_INPUT        "C17028"
#define AIRBUS_FLAPS_3_INPUT        "C17056"
#define AIRBUS_FLAPS_FULL_INPUT     "C17100"

typedef enum {
  FlapsUnkown = 0,
  FlapsUp,
  Flaps1F,
  Flaps2,
  Flaps3,
  FlapsFull
} FlapsPosition;

typedef struct FlapsPotiRange {
  int from;
  int to;
  FlapsPosition position;
} FlapsPotiRange;

#define FLAPS_POTI_RANGE_MAKE(_from, _to, _position)  { _from, _to, _position }

FlapsPotiRange FLAPS_UP_POTI_RANGE    = FLAPS_POTI_RANGE_MAKE(0, 100, FlapsUp);
FlapsPotiRange FLAPS_1F_POTI_RANGE    = FLAPS_POTI_RANGE_MAKE(150, 350, Flaps1F);
FlapsPotiRange FLAPS_2_POTI_RANGE     = FLAPS_POTI_RANGE_MAKE(400, 600, Flaps2);
FlapsPotiRange FLAPS_3_POTI_RANGE     = FLAPS_POTI_RANGE_MAKE(650, 850, Flaps3);
FlapsPotiRange FLAPS_FULL_POTI_RANGE  = FLAPS_POTI_RANGE_MAKE(900, 1024, FlapsFull);

#define FLAPS_POTI_RANGES 5
FlapsPotiRange flaps_poti_ranges[FLAPS_POTI_RANGES];

FlapsPosition previous_flaps_position = FlapsUnkown;
long last_update = millis();

#define UPDATE_INTERVAL 2000L

extern FlapsPosition flapsPositionFromPotiValue(int value);
extern void update_flaps(FlapsPosition position);

void flaps_setup() {
  flaps_poti_ranges[0] = FLAPS_UP_POTI_RANGE;
  flaps_poti_ranges[1] = FLAPS_1F_POTI_RANGE;
  flaps_poti_ranges[2] = FLAPS_2_POTI_RANGE;
  flaps_poti_ranges[3] = FLAPS_3_POTI_RANGE;
  flaps_poti_ranges[4] = FLAPS_FULL_POTI_RANGE;
}


void flaps_tick() {
  int flaps_poti_value = analogRead(FLAPS_POTI_PIN);
  FlapsPosition flapsPosition = flapsPositionFromPotiValue(flaps_poti_value);

  if (flapsPosition != previous_flaps_position || (millis() - last_update > UPDATE_INTERVAL) ) {
    previous_flaps_position = flapsPosition;
    update_flaps(flapsPosition);
    last_update = millis();
  }
}


FlapsPosition flapsPositionFromPotiValue(int value) {
  for (int i = 0; i < FLAPS_POTI_RANGES; i++) {
    FlapsPotiRange range = flaps_poti_ranges[i];
    if (value >= range.from && value <= range.to) {
      return range.position;
    }
  }
  return FlapsUnkown;
}


void update_flaps(FlapsPosition position) {
  switch (position) {

    case FlapsUp:
      Serial.println(AIRBUS_FLAPS_UP_INPUT);
      break;

    case Flaps1F:
      Serial.println(AIRBUS_FLAPS_1F_INPUT);
      break;

    case Flaps2:
      Serial.println(AIRBUS_FLAPS_2_INPUT);
      break;

    case Flaps3:
      Serial.println(AIRBUS_FLAPS_3_INPUT);
      break;

    case FlapsFull:
      Serial.println(AIRBUS_FLAPS_FULL_INPUT);
      break;

    default:
      break;

  }
}
