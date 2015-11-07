#include "SerialReader.h"
#include "Radio.h"
#include "Gear.h"
#include "AP.h"
#include "Extractions.h"

#define BAUD_RATE   115200

char init_buffer[200];
int init_fill_position = 0;
int init_process_position = 0;


void determine_extraction_type(char token);
void determine_radio_ap_parameter(char token);
void determine_indication_sys1_parameter(char token);
void determine_sys2_other_parameter(char token);


// Public

void (*token_processing)(char token) = DEFAULT_TOKEN_PROCESSING;
int position_to_write = 0;


void serial_setup() {
  Serial.begin(BAUD_RATE);
}


int read_initialization() {
  if (Serial.available()) {
    char token = Serial.read();
    if (token == '\n') {
      return 1;
    }
    init_buffer[init_fill_position] = token;
    init_fill_position++;
  }
  return 0;
}


int process_initialization_data() {
  if (init_process_position < init_fill_position) {
    char token = init_buffer[init_process_position];
    init_process_position++;
    token_processing(token);
    return 0;
  } else {
    return 1;
  }
}


void read_serial() {
  if (Serial.available()) {
    char token = Serial.read();
    token_processing(token);
  }
}


void store_token(char token, char destination[], int size, int *updated) {
  if (position_to_write < size) {
    destination[position_to_write] = token;
    position_to_write++;
    if (position_to_write >= size) {
      token_processing = determine_extraction_type;
      *updated = 1;
    }
  } else {
    determine_extraction_type(token);
  }
}


// Private

void determine_extraction_type(char token) {
  switch (token) {
    case RADIO_AP_EXTRACTIONS:
      token_processing = determine_radio_ap_parameter;
      break;

    case INDICATIONS_SYSTEM_1_EXTRACTIONS:
      token_processing = determine_indication_sys1_parameter;
      break;

    case SYSTEM_2_OTHERS_EXTRACTIONS:
      token_processing = determine_sys2_other_parameter;
      break;
  }
  position_to_write = 0;
}


// Type: Radio / Autopilot
void determine_radio_ap_parameter(char token) {
  switch (token) {
    case COM_1_FREQ_EXTRACTION:
      token_processing = READ_COM1_FREQ;
      break;

    case COM_1_SB_FREQ_EXTRACTION:
      token_processing = READ_COM1_SB_FREQ;
      break;

    case COM_2_FREQ_EXTRACTION:
      token_processing = READ_COM2_FREQ;
      break;

    case COM_2_SB_FREQ_EXTRACTION:
      token_processing = READ_COM2_SB_FREQ;
      break;

    case NAV_1_FREQ_EXTRACTION:
      token_processing = READ_NAV1_FREQ;
      break;

    case NAV_1_SB_FREQ_EXTRACTION:
      token_processing = READ_NAV1_SB_FREQ;
      break;

    case NAV_2_FREQ_EXTRACTION:
      token_processing = READ_NAV2_FREQ;
      break;

    case NAV_2_SB_FREQ_EXTRACTION:
      token_processing = READ_NAV2_SB_FREQ;
      break;

    case TRANSPONDER_CODE_EXTRACTION:
      token_processing = READ_TRANSPONDER_CODE;
      break;

    case COM_1_SOUND_ACTIVE_EXTRACTION:
      token_processing = READ_COM1_SOUND_ACTIVE;
      break;

    case COM_2_SOUND_ACTIVE_EXTRACTION:
      token_processing = READ_COM2_SOUND_ACTIVE;
      break;

    case NAV_1_SOUND_ACTIVE_EXTRACTION:
      token_processing = READ_NAV1_SOUND_ACTIVE;
      break;

    case NAV_2_SOUND_ACTIVE_EXTRACTION:
      token_processing = READ_NAV2_SOUND_ACTIVE;
      break;

    case AP_AIRSPEED_SET_EXTRACTION:
      token_processing = READ_SPEED;
      break;

    case AP_AIRSPEED_LOCK_ACTIVE_EXTRACTION:
      token_processing = READ_SPEED_ACTIVE;
      break;

    case AP_HEADING_SET_EXTRACTION:
      token_processing = READ_HEADING;
      break;

    case AP_HEADING_LOCK_ACTIVE_EXTRACTION:
      token_processing = READ_HEADING_ACTIVE;
      break;

    case AP_COURSE_SET_EXTRACTION:
      token_processing = READ_COURSE;
      break;

    case AP_ACTIVE_EXTRACTION:
      token_processing = READ_AP_STATUS;
      break;

    case A_THR_ARMED_EXTRACTION:
      token_processing = READ_A_THR_STATUS;
      break;

    case AP_ALTITUDE_SET_EXTRACTION:
      token_processing = READ_ALTITUDE;
      break;

    case AP_VERTICAL_SPEED_SET_EXTRACTION:
      token_processing = READ_VERTICAL_SPEED;
      break;

    case AP_ALTITUDE_LOCK_ACTIVE_EXTRACTION:
      token_processing = READ_ALTITUDE_ACTIVE;
      break;

    case AP_APR_HOLD_ACTIVE_EXTRACTION:
      token_processing = READ_APR_HOLD;
      break;

    case AP_NAV_HOLD_EXTRACTION:
      token_processing = READ_NAV_HOLD;
      break;

    case AP_GLIDESLOPE_HOLD_EXTRACTION:
      token_processing = READ_GLIDESLOPE_HOLD;
      break;
      
    default:
      token_processing = determine_extraction_type;
      determine_extraction_type(token);
      break;
  }
}


// Type: Indication / System1
void determine_indication_sys1_parameter(char token) {
  switch (token) {
    case TRIM_POSITION_EXTRACTION:
      token_processing = READ_ELEVATOR_TRIM;
      break;

    case PARK_BRAKE_POSITION:
      token_processing = READ_PARKING_BRAKE_STATUS;
      break;
    
    default:
      token_processing = determine_extraction_type;
      determine_extraction_type(token);
      break;
  }
}


// Type: System2 / Other
void determine_sys2_other_parameter(char token) {
  switch (token) {
    case GEAR_POSITION_EXTRACTION: 
      token_processing = READ_GEAR_POSITION;
      break;

    case GEAR_RETRACTABLE_EXTRACTION: 
      token_processing = READ_GEAR_RETRACTABLE;
      break;

    case RUDDER_TRIM_EXTRACTION:
      token_processing = READ_RUDDERT_TRIM;
      break;

    case PARK_BRAKE_POSITION:
      token_processing = READ_PARKING_BRAKE_STATUS;
      break;
    
    default:
      token_processing = determine_extraction_type;
      determine_extraction_type(token);
      break;
  }
}
