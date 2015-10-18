#ifndef PINS_H
#define PINS_H

#define ANALOG_PIN(_pin)              (_pin+54)


// AP1
#define AP_LCD_1_RS_PIN               44
#define AP_LCD_1_E_PIN                42  
#define AP_LCD_1_D4_PIN               40
#define AP_LCD_1_D5_PIN               38
#define AP_LCD_1_D6_PIN               36
#define AP_LCD_1_D7_PIN               34
#define AP_LCD_1_PINS                 AP_LCD_1_RS_PIN, AP_LCD_1_E_PIN, AP_LCD_1_D4_PIN, AP_LCD_1_D5_PIN, AP_LCD_1_D6_PIN, AP_LCD_1_D7_PIN


// AP2
#define AP_LCD_2_RS_PIN               32
#define AP_LCD_2_E_PIN                30  
#define AP_LCD_2_D4_PIN               28
#define AP_LCD_2_D5_PIN               26
#define AP_LCD_2_D6_PIN               24  
#define AP_LCD_2_D7_PIN               22
#define AP_LCD_2_PINS                 AP_LCD_2_RS_PIN, AP_LCD_2_E_PIN, AP_LCD_2_D4_PIN, AP_LCD_2_D5_PIN, AP_LCD_2_D6_PIN, AP_LCD_2_D7_PIN


// Radio
#define RADIO_LCD_RS_PIN              19
#define RADIO_LCD_E_PIN               18
#define RADIO_LCD_D4_PIN              17
#define RADIO_LCD_D5_PIN              16
#define RADIO_LCD_D6_PIN              15
#define RADIO_LCD_D7_PIN              14
#define RADIO_LCD_PINS                RADIO_LCD_RS_PIN, RADIO_LCD_E_PIN, RADIO_LCD_D4_PIN, RADIO_LCD_D5_PIN, RADIO_LCD_D6_PIN, RADIO_LCD_D7_PIN

#define RADIO_CALL_PIN                ANALOG_PIN(0)
#define RADIO_SWAP_FREQ_PIN           ANALOG_PIN(1)
#define RADIO_PREVIOUS_RADIO_PIN      ANALOG_PIN(2)
#define RADIO_NEXT_RADIO_PIN          ANALOG_PIN(3)

#define RADIO_CHANGE_PRESS_PIN        4


// ECAM
#define ECAM_LCD_RS_PIN               49
#define ECAM_LCD_E_PIN                47
#define ECAM_LCD_D4_PIN               52
#define ECAM_LCD_D5_PIN               50
#define ECAM_LCD_D6_PIN               48
#define ECAM_LCD_D7_PIN               46
#define ECAM_LCD_PINS                 ECAM_LCD_RS_PIN, ECAM_LCD_E_PIN, ECAM_LCD_D4_PIN, ECAM_LCD_D5_PIN, ECAM_LCD_D6_PIN, ECAM_LCD_D7_PIN


// Elevator trim
#define ELEVATOR_TRIM_LCD_RS_PIN      33
#define ELEVATOR_TRIM_LCD_E_PIN       31
#define ELEVATOR_TRIM_LCD_D4_PIN      29
#define ELEVATOR_TRIM_LCD_D5_PIN      27
#define ELEVATOR_TRIM_LCD_D6_PIN      25
#define ELEVATOR_TRIM_LCD_D7_PIN      23
#define ELEVATOR_TRIM_LCD_PINS        ELEVATOR_TRIM_LCD_RS_PIN, ELEVATOR_TRIM_LCD_E_PIN, ELEVATOR_TRIM_LCD_D4_PIN, ELEVATOR_TRIM_LCD_D5_PIN, ELEVATOR_TRIM_LCD_D6_PIN, ELEVATOR_TRIM_LCD_D7_PIN

  
// Rudder trim
#define RUDDER_TRIM_LCD_RS_PIN        45
#define RUDDER_TRIM_LCD_E_PIN         43
#define RUDDER_TRIM_LCD_D4_PIN        41
#define RUDDER_TRIM_LCD_D5_PIN        39
#define RUDDER_TRIM_LCD_D6_PIN        37
#define RUDDER_TRIM_LCD_D7_PIN        35
#define RUDDER_TRIM_LCD_PINS          RUDDER_TRIM_LCD_RS_PIN, RUDDER_TRIM_LCD_E_PIN, RUDDER_TRIM_LCD_D4_PIN, RUDDER_TRIM_LCD_D5_PIN, RUDDER_TRIM_LCD_D6_PIN, RUDDER_TRIM_LCD_D7_PIN


// Gear
#define NOSE_GEAR_EXTENDED_LED_PIN    13
#define NOSE_GEAR_UNLOCKED_LED_PIN    12
#define LEFT_GEAR_EXTENDED_LED_PIN    11
#define LEFT_GEAR_UNLOCKED_LED_PIN    10
#define RIGHT_GEAR_EXTENDED_LED_PIN   9
#define RIGHT_GEAR_UNLOCKED_LED_PIN   8

#endif
