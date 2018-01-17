#ifndef PINS_H
#define PINS_H

#define ANALOG_PIN(_pin)              (_pin+54)

// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Radio Panel
// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Keypad
#define KEYPAD_ROW_1_PIN              ANALOG_PIN(5)
#define KEYPAD_ROW_2_PIN              ANALOG_PIN(6)
#define KEYPAD_ROW_3_PIN              ANALOG_PIN(7)
#define KEYPAD_ROW_4_PIN              ANALOG_PIN(8)
#define KEYPAD_COL_1_PIN              ANALOG_PIN(9)
#define KEYPAD_COL_2_PIN              ANALOG_PIN(10)
#define KEYPAD_COL_3_PIN              ANALOG_PIN(11)
#define KEYPAD_COL_4_PIN              ANALOG_PIN(12)
#define KEYPAD_ROW_PINS               (byte[]){KEYPAD_ROW_1_PIN, KEYPAD_ROW_2_PIN, KEYPAD_ROW_3_PIN, KEYPAD_ROW_4_PIN}
#define KEYPAD_COL_PINS               (byte[]){KEYPAD_COL_1_PIN, KEYPAD_COL_2_PIN, KEYPAD_COL_3_PIN, KEYPAD_COL_4_PIN}

// Upper LCD
#define RADIO_UPPER_LCD_RS_PIN        7
#define RADIO_UPPER_LCD_E_PIN         6
#define RADIO_UPPER_LCD_D4_PIN        5
#define RADIO_UPPER_LCD_D5_PIN        4
#define RADIO_UPPER_LCD_D6_PIN        3
#define RADIO_UPPER_LCD_D7_PIN        2
#define RADIO_UPPER_LCD_PINS          RADIO_UPPER_LCD_RS_PIN, RADIO_UPPER_LCD_E_PIN, RADIO_UPPER_LCD_D4_PIN, RADIO_UPPER_LCD_D5_PIN, RADIO_UPPER_LCD_D6_PIN, RADIO_UPPER_LCD_D7_PIN

// Lower LCD
#define RADIO_LOWER_LCD_RS_PIN        49
#define RADIO_LOWER_LCD_E_PIN         47
#define RADIO_LOWER_LCD_D4_PIN        45
#define RADIO_LOWER_LCD_D5_PIN        43
#define RADIO_LOWER_LCD_D6_PIN        41
#define RADIO_LOWER_LCD_D7_PIN        39
#define RADIO_LOWER_LCD_PINS          RADIO_LOWER_LCD_RS_PIN, RADIO_LOWER_LCD_E_PIN, RADIO_LOWER_LCD_D4_PIN, RADIO_LOWER_LCD_D5_PIN, RADIO_LOWER_LCD_D6_PIN, RADIO_LOWER_LCD_D7_PIN

// Buttons
#define SCROLL_UP_PIN                 ANALOG_PIN(13)
#define SCROLL_DOWN_PIN               ANALOG_PIN(14)
#define RADIO_L1_PIN                  ANALOG_PIN(15)
#define RADIO_L2_PIN                  52
#define RADIO_L3_PIN                  50
#define RADIO_L4_PIN                  48
#define RADIO_R1_PIN                  46
#define RADIO_R2_PIN                  44
#define RADIO_R3_PIN                  42
#define RADIO_R4_PIN                  40
#define RADIO_R5_PIN                  38

// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Gear Panel
// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// LEDs
#define NOSE_GEAR_EXTENDED_LED_PIN    13
#define NOSE_GEAR_UNLOCKED_LED_PIN    12
#define LEFT_GEAR_EXTENDED_LED_PIN    11
#define LEFT_GEAR_UNLOCKED_LED_PIN    10
#define RIGHT_GEAR_EXTENDED_LED_PIN   9
#define RIGHT_GEAR_UNLOCKED_LED_PIN   8

// Gear Switch
#define GEAR_POTI_PIN                 ANALOG_PIN(1)

// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Trim Panel
// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Rudder LCD
#define RUDDER_TRIM_LCD_RS_PIN        37
#define RUDDER_TRIM_LCD_E_PIN         35
#define RUDDER_TRIM_LCD_D4_PIN        33
#define RUDDER_TRIM_LCD_D5_PIN        31
#define RUDDER_TRIM_LCD_D6_PIN        29
#define RUDDER_TRIM_LCD_D7_PIN        27

#define RUDDER_TRIM_LCD_PINS          RUDDER_TRIM_LCD_RS_PIN, RUDDER_TRIM_LCD_E_PIN, RUDDER_TRIM_LCD_D4_PIN, RUDDER_TRIM_LCD_D5_PIN, RUDDER_TRIM_LCD_D6_PIN, RUDDER_TRIM_LCD_D7_PIN

// Elevator LCD
#define ELEVATOR_TRIM_LCD_RS_PIN      36
#define ELEVATOR_TRIM_LCD_E_PIN       34
#define ELEVATOR_TRIM_LCD_D4_PIN      32
#define ELEVATOR_TRIM_LCD_D5_PIN      30
#define ELEVATOR_TRIM_LCD_D6_PIN      28
#define ELEVATOR_TRIM_LCD_D7_PIN      26

#define ELEVATOR_TRIM_LCD_PINS        ELEVATOR_TRIM_LCD_RS_PIN, ELEVATOR_TRIM_LCD_E_PIN, ELEVATOR_TRIM_LCD_D4_PIN, ELEVATOR_TRIM_LCD_D5_PIN, ELEVATOR_TRIM_LCD_D6_PIN, ELEVATOR_TRIM_LCD_D7_PIN

// Change Rudder
#define RUDDER_LEFT_BUTTON_PIN        25
#define RUDDER_RIGHT_BUTTON_PIN       23

// Change Elevator
#define ELEVATOR_LEFT_BUTTON_PIN      24
#define ELEVATOR_RIGHT_BUTTON_PIN     22

// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Flaps Panel
// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// Gear Switch
#define FLAPS_POTI_PIN                ANALOG_PIN(2)

// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Parking Brake Panel
// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

// Parking Brake Switch
#define PARKING_BRAKE_ON_PIN          53
#define PARKING_BRAKE_OFF_PIN         51

#endif
