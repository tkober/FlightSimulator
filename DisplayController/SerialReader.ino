#include "SerialReader.h"
#include "Radio.h"

#define BAUD_RATE   115200

void determine_extraction_type(char token);
void determine_radio_ap_parameter(char token);
void determine_indication_sys1_parameter(char token);
void determine_sys2_other_parameter(char token);


// Public

void (*token_processing)(char token) = determine_extraction_type;
int position_to_write = 0;


void serial_setup() {
  Serial.begin(BAUD_RATE);
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
  int bla = digitalRead(1);
  switch (token) {
    case '=':
      token_processing = determine_radio_ap_parameter;
      break;

    case '<':
      token_processing = determine_indication_sys1_parameter;
      break;

    case '?':
      token_processing = determine_sys2_other_parameter;
      break;
  }
  position_to_write = 0;
}


// Type: Radio / Autopilot
void determine_radio_ap_parameter(char token) {
  switch (token) {
    case 'A':
      token_processing = read_com1_freq;
      break;

    case 'B':
      token_processing = read_com1_sb_freq;
      break;

    case 'C':
      token_processing = read_com2_freq;
      break;

    case 'D':
      token_processing = read_com2_sb_freq;
      break;

    case 'E':
      token_processing = read_nav1_freq;
      break;

    case 'F':
      token_processing = read_nav1_sb_freq;
      break;

    case 'G':
      token_processing = read_nav2_freq;
      break;

    case 'H':
      token_processing = read_nav2_sb_freq;
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

    default:
      token_processing = determine_extraction_type;
      determine_extraction_type(token);
      break;
  }
}


// Type: System2 / Other
void determine_sys2_other_parameter(char token) {
  switch (token) {

    default:
      token_processing = determine_extraction_type;
      determine_extraction_type(token);
      break;
  }
}
