#ifdef CONTROLLER_A

#include "Systems.h"
#include "Keypad.h"
#include "Pins.h"


#define SWITCH_1_1  0x11
#define SWITCH_2_1  0x21
#define SWITCH_3_1  0x31
#define SWITCH_4_1  0x41

#define SWITCH_1_2  0x12
#define SWITCH_2_2  0x22
#define SWITCH_3_2  0x32
#define SWITCH_4_2  0x42

#define SWITCH_1_3  0x13
#define SWITCH_2_3  0x23
#define SWITCH_3_3  0x33
#define SWITCH_4_3  0x43

#define SWITCH_1_4  0x14
#define SWITCH_2_4  0x24
#define SWITCH_3_4  0x34
#define SWITCH_4_4  0x44

#define SWITCH_1_5  0x15
#define SWITCH_2_5  0x25
#define SWITCH_3_5  0x35
#define SWITCH_4_5  0x45

#define SWITCH_MATRIX_ROW_COUNT   4
#define SWITCH_MATRIX_COL_COUNT   5
#define SWITCH_MATRIX_KEYS        (char[SWITCH_MATRIX_ROW_COUNT][SWITCH_MATRIX_COL_COUNT]){   \
  {SWITCH_1_1, SWITCH_1_2, SWITCH_1_3, SWITCH_1_4, SWITCH_1_5},                               \
  {SWITCH_2_1, SWITCH_2_2, SWITCH_2_3, SWITCH_2_4, SWITCH_2_5},                               \
  {SWITCH_3_1, SWITCH_3_2, SWITCH_3_3, SWITCH_3_4, SWITCH_3_5},                               \
  {SWITCH_4_1, SWITCH_4_2, SWITCH_4_3, SWITCH_4_4, SWITCH_4_5}                                \
}

Keypad switchMatrix = Keypad( makeKeymap(SWITCH_MATRIX_KEYS), SWITCH_MATRIX_ROW_PINS, SWITCH_MATRIX_COL_PINS, SWITCH_MATRIX_ROW_COUNT, SWITCH_MATRIX_COL_COUNT);


void systems_setup() {

}


void systems_tick() {
  char customKey = switchMatrix.getKey();
  if (customKey) {

    switch (customKey) {

      // Col 1
      case SWITCH_1_1:
        Serial.println("E18"); // Battery On
        break;

      case SWITCH_2_1:
        Serial.println("E17"); // Battery Off
        break;

      case SWITCH_3_1:
        Serial.println("E21"); // Engine 1 Generator On
        break;

      case SWITCH_4_1:
        Serial.println("E20"); // Engine 1 Generator Off
        break;


      // Col 2
      case SWITCH_1_2:
        Serial.println("E31"); // APU Gen On
        break;

      case SWITCH_2_2:
        Serial.println("E32"); // APU Gen Off
        break;

      case SWITCH_3_2:
        Serial.println("E24"); // Engine 2 Generator On
        break;

      case SWITCH_4_2:
        Serial.println("E23"); // Engine 2 Generator Off
        break;


      // Col 3
      case SWITCH_1_3:
        Serial.println("F32"); // Engine 1 Fuel Valve On
        break;

      case SWITCH_2_3:
        Serial.println("F33"); // Engine 1 Fuel Valve Off
        break;

      case SWITCH_3_3:
        Serial.println("F021"); // Engine 1 Fuel Pump On
        break;

      case SWITCH_4_3:
        Serial.println("F020"); // Engine 1 Fuel Pump Off
        break;


      // Col 4
      case SWITCH_1_4:
        Serial.println("C11"); // APU Start
        break;

      case SWITCH_2_4:
        Serial.println("E30"); // APU Stop
        break;

      case SWITCH_3_4:
        Serial.println("F06"); // Crossfeed Valve On
        break;

      case SWITCH_4_4:
        Serial.println("F07"); // Crossfeed Valve Off
        break;


      // Col 5
      case SWITCH_1_5:
        Serial.println("F35"); // Engine 2 Fuel Valve On
        break;

      case SWITCH_2_5:
        Serial.println("F36"); // Engine 2 Fuel Valve Off
        break;

      case SWITCH_3_5:
        Serial.println("F031"); // Engine 2 Fuel Pump On
        break;

      case SWITCH_4_5:
        Serial.println("F030"); // Engine 2 Fuel Pump Off
        break;


    }

  }
}

#endif
