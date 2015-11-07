#ifndef PINS_H
#define PINS_H

#define ANALOG_PIN(_pin)              (_pin+54)


// ===============================================================================================================================================================================
// Controller A Pins
// ===============================================================================================================================================================================
// ECAM / FMC Panel
// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ECAM 1
#define ECAM_1_LCD_RS_PIN             52
#define ECAM_1_LCD_E_PIN              50
#define ECAM_1_LCD_D4_PIN             48
#define ECAM_1_LCD_D5_PIN             46
#define ECAM_1_LCD_D6_PIN             44
#define ECAM_1_LCD_D7_PIN             42
#define ECAM_1_LCD_PINS               ECAM_1_LCD_RS_PIN, ECAM_1_LCD_E_PIN, ECAM_1_LCD_D4_PIN, ECAM_1_LCD_D5_PIN, ECAM_1_LCD_D6_PIN, ECAM_1_LCD_D7_PIN

// ECAM 2 / FMC
#define ECAM_2_LCD_RS_PIN             53
#define ECAM_2_LCD_E_PIN              51
#define ECAM_2_LCD_D4_PIN             49
#define ECAM_2_LCD_D5_PIN             47
#define ECAM_2_LCD_D6_PIN             45
#define ECAM_2_LCD_D7_PIN             43
#define ECAM_2_LCD_PINS               ECAM_2_LCD_RS_PIN, ECAM_2_LCD_E_PIN, ECAM_2_LCD_D4_PIN, ECAM_2_LCD_D5_PIN, ECAM_2_LCD_D6_PIN, ECAM_2_LCD_D7_PIN

// Next Page
#define NEXT_PAGE_PIN                 ANALOG_PIN(8)

// Previous Page
#define PREVIOUS_PAGE_PIN             ANALOG_PIN(9)

// Keypad
#define KEYPAD_ROW_1_PIN              ANALOG_PIN(0)
#define KEYPAD_ROW_2_PIN              ANALOG_PIN(1)
#define KEYPAD_ROW_3_PIN              ANALOG_PIN(2)
#define KEYPAD_ROW_4_PIN              ANALOG_PIN(3)
#define KEYPAD_COL_1_PIN              ANALOG_PIN(4)
#define KEYPAD_COL_2_PIN              ANALOG_PIN(5)
#define KEYPAD_COL_3_PIN              ANALOG_PIN(6)
#define KEYPAD_COL_4_PIN              ANALOG_PIN(7)
#define KEYPAD_ROW_PINS               (byte[]){KEYPAD_ROW_1_PIN, KEYPAD_ROW_2_PIN, KEYPAD_ROW_3_PIN, KEYPAD_ROW_4_PIN}
#define KEYPAD_COL_PINS               (byte[]){KEYPAD_COL_1_PIN, KEYPAD_COL_2_PIN, KEYPAD_COL_3_PIN, KEYPAD_COL_4_PIN}

// Scroll Up
#define SCROLL_UP_PIN                 ANALOG_PIN(10)

// Scroll Down
#define SCROLL_DOWN_PIN               ANALOG_PIN(11)

// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Radio Panel
// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Radio LCD
#define RADIO_LCD_RS_PIN              40
#define RADIO_LCD_E_PIN               38
#define RADIO_LCD_D4_PIN              36
#define RADIO_LCD_D5_PIN              34
#define RADIO_LCD_D6_PIN              32
#define RADIO_LCD_D7_PIN              30
#define RADIO_LCD_PINS                RADIO_LCD_RS_PIN, RADIO_LCD_E_PIN, RADIO_LCD_D4_PIN, RADIO_LCD_D5_PIN, RADIO_LCD_D6_PIN, RADIO_LCD_D7_PIN

// Call
#define RADIO_CALL_PIN                ANALOG_PIN(12)

// Swap Frequency
#define RADIO_SWAP_FREQ_PIN           ANALOG_PIN(13)

// Previous Radio
#define RADIO_PREVIOUS_RADIO_PIN      ANALOG_PIN(14)

// Next Radio
#define RADIO_NEXT_RADIO_PIN          ANALOG_PIN(15)

// Change Frequency
#define RADIO_CHANGE_BUTTON_PIN       28
#define RADIO_CHANGE_ENCODER_PIN_A    26
#define RADIO_CHANGE_ENCODER_PIN_B    24

// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Gear Panel
// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// LEDs
#define NOSE_GEAR_EXTENDED_LED_PIN    41
#define NOSE_GEAR_UNLOCKED_LED_PIN    39
#define LEFT_GEAR_EXTENDED_LED_PIN    37
#define LEFT_GEAR_UNLOCKED_LED_PIN    35
#define RIGHT_GEAR_EXTENDED_LED_PIN   33
#define RIGHT_GEAR_UNLOCKED_LED_PIN   31

// Gear Switch
#define GEAR_SWITCH_PIN_A             29
#define GEAR_SWITCH_PIN_B             27

// Parking Brake Toggle
#define TOGGLE_PARKING_BRAKE_PIN      25

// Parking Brake LED
#define PARKING_BRAKE_LED_PIN         23

// ===============================================================================================================================================================================





// ===============================================================================================================================================================================
// Controller B Pins
// ===============================================================================================================================================================================
// AP Panel
// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// LCD 1 (SPD / HDG / CRS)
#define AP_LCD_1_RS_PIN               52
#define AP_LCD_1_E_PIN                50  
#define AP_LCD_1_D4_PIN               48
#define AP_LCD_1_D5_PIN               46
#define AP_LCD_1_D6_PIN               44
#define AP_LCD_1_D7_PIN               42
#define AP_LCD_1_PINS                 AP_LCD_1_RS_PIN, AP_LCD_1_E_PIN, AP_LCD_1_D4_PIN, AP_LCD_1_D5_PIN, AP_LCD_1_D6_PIN, AP_LCD_1_D7_PIN

// LCD 2 (ALT / VS)
#define AP_LCD_2_RS_PIN               53
#define AP_LCD_2_E_PIN                51  
#define AP_LCD_2_D4_PIN               49
#define AP_LCD_2_D5_PIN               47
#define AP_LCD_2_D6_PIN               45  
#define AP_LCD_2_D7_PIN               43
#define AP_LCD_2_PINS                 AP_LCD_2_RS_PIN, AP_LCD_2_E_PIN, AP_LCD_2_D4_PIN, AP_LCD_2_D5_PIN, AP_LCD_2_D6_PIN, AP_LCD_2_D7_PIN

// Change SPD Encoder
#define AP_CHANGE_SPD_BUTTON_PIN      ANALOG_PIN(0)
#define AP_CHANGE_SPD_ENCODER_PIN_A   ANALOG_PIN(1)
#define AP_CHANGE_SPD_ENCODER_PIN_B   ANALOG_PIN(2)

// Change HDG Encoder
#define AP_CHANGE_HDG_BUTTON_PIN      ANALOG_PIN(3)
#define AP_CHANGE_HDG_ENCODER_PIN_A   ANALOG_PIN(4)
#define AP_CHANGE_HDG_ENCODER_PIN_B   ANALOG_PIN(5)

// Change CRS Encoder
#define AP_CHANGE_CRS_BUTTON_PIN      ANALOG_PIN(6)
#define AP_CHANGE_CRS_ENCODER_PIN_A   ANALOG_PIN(7)
#define AP_CHANGE_CRS_ENCODER_PIN_B   ANALOG_PIN(8)

// Change ALT Encoder
#define AP_CHANGE_ALT_BUTTON_PIN      ANALOG_PIN(9)
#define AP_CHANGE_ALT_ENCODER_PIN_A   ANALOG_PIN(10)
#define AP_CHANGE_ALT_ENCODER_PIN_B   ANALOG_PIN(11)

// Change VS Encoder
#define AP_CHANGE_VS_BUTTON_PIN       ANALOG_PIN(12)
#define AP_CHANGE_VS_ENCODER_PIN_A    ANALOG_PIN(13)
#define AP_CHANGE_VS_ENCODER_PIN_B    ANALOG_PIN(14)

// LEDs
#define AP_STATUS_PIN                 28
#define A_THR_STATUS_PIN              26

// Toglle AP
#define TOGGLE_AP_PIN                 40

// Toglle A/THR
#define TOGGLE_A_THR_PIN              38

// Toglle LOC
#define TOGGLE_LOC_PIN                36

// Toglle APR
#define TOGGLE_APR_PIN                34

// Toglle FD
#define TOGGLE_FD_PIN                 32
  
// Toglle NAV GPS
#define TOGGLE_NAV_GPS_PIN            30

// Toglle FL Change
#define TOGGLE_FL_CHANGE_PIN          ANALOG_PIN(15)

// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Flight Controls Panel
// -------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Rudder LCD
#define RUDDER_TRIM_LCD_RS_PIN        41
#define RUDDER_TRIM_LCD_E_PIN         39  
#define RUDDER_TRIM_LCD_D4_PIN        37
#define RUDDER_TRIM_LCD_D5_PIN        35
#define RUDDER_TRIM_LCD_D6_PIN        33
#define RUDDER_TRIM_LCD_D7_PIN        31
#define RUDDER_TRIM_LCD_PINS          RUDDER_TRIM_LCD_RS_PIN, RUDDER_TRIM_LCD_E_PIN, RUDDER_TRIM_LCD_D4_PIN, RUDDER_TRIM_LCD_D5_PIN, RUDDER_TRIM_LCD_D6_PIN, RUDDER_TRIM_LCD_D7_PIN

// Elevator LCD
#define ELEVATOR_TRIM_LCD_RS_PIN      24
#define ELEVATOR_TRIM_LCD_E_PIN       22  
#define ELEVATOR_TRIM_LCD_D4_PIN      29
#define ELEVATOR_TRIM_LCD_D5_PIN      27
#define ELEVATOR_TRIM_LCD_D6_PIN      25
#define ELEVATOR_TRIM_LCD_D7_PIN      23
#define ELEVATOR_TRIM_LCD_PINS        ELEVATOR_TRIM_LCD_RS_PIN, ELEVATOR_TRIM_LCD_E_PIN, ELEVATOR_TRIM_LCD_D4_PIN, ELEVATOR_TRIM_LCD_D5_PIN, ELEVATOR_TRIM_LCD_D6_PIN, ELEVATOR_TRIM_LCD_D7_PIN

// Change Rudder 
#define CHANGE_RUDDER_BUTTON_PIN      14
#define CHANGE_RUDDER_ENCODER_PIN_A   15
#define CHANGE_RUDDER_ENCODER_PIN_B   16

// Change Elevator
#define CHANGE_ELEVATOR_BUTTON_PIN    17
#define CHANGE_ELEVATOR_ENCODER_PIN_A 18
#define CHANGE_ELEVATOR_ENCODER_PIN_B 19

// Flaps Switch
#define FLAPS_SWITCH_PIN_A            20
#define FLAPS_SWITCH_PIN_B            21

// Arm Spoilers
#define ARM_SPOILERS_PIN              5

// Retract Spoilers
#define RETRACT_SPOILERS_PIN          4

// Extend Spoilers  
#define EXTENC_SPOILERS_PIN           3

// Retract Flaps
#define RETRACT_FLAPS_PIN             2
// ===============================================================================================================================================================================

#endif
