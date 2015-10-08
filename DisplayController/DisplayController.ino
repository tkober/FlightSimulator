#include <LiquidCrystal.h>

#include "Radio.h"
#include "AP.h"
#include "ECAM.h"
#include "ElevatorTrim.h"
#include "RudderTrim.h"

#define lcd   ecam_lcd

#define SWAP_NAV1     "A18"

#define CHANGE_RADIO_PIN  8
#define SWAP_PIN          9

enum Radio {
  COM1 = 0,
  COM2,
  NAV1,
  NAV2,
  RADIO_COUNT
};
typedef enum Radio Radio;

#define COM1_LABEL    "ACT   COM1   S/B"
#define COM2_LABEL    "ACT   COM2   S/B"
#define NAV1_LABEL    "ACT   NAV1   S/B"
#define NAV2_LABEL    "ACT   NAV2   S/B"

Radio currentRadio = COM1;

void (*token_processing)(char token) = determine_extraction_type;

int position_to_write = 0;

#define COM_FREQ_SIZE   7
char com1_freq[8]       = "---.---";
char com1_sb_freq[8]    = "---.---";
char com2_freq[8]       = "---.---";
char com2_sb_freq[8]    = "---.---";

#define NAV_FREQ_SIZE   6
char nav1_freq[7]       = "---.--";
char nav1_sb_freq[7]    = "---.--";
char nav2_freq[7]       = "---.--";
char nav2_sb_freq[7]    = "---.--";

int isSwapButtonPressed = 0;
int isChangeRadioButtonPressed = 0;

int updated = 0;

void setup() {
  pinMode(SWAP_PIN, INPUT);
  pinMode(CHANGE_RADIO_PIN, INPUT);

  Serial.begin(115200);

  lcd.begin(16, 2);
  printRadio();
}

void loop() {
  read_serial();

  if (digitalRead(SWAP_PIN) == 1) {
    if (isSwapButtonPressed == 0) {
      Serial.println(SWAP_NAV1);
    }
    isSwapButtonPressed = 1;
  } else {
    isSwapButtonPressed = 0;
  }

  if (digitalRead(CHANGE_RADIO_PIN) == 1) {
    if (isChangeRadioButtonPressed == 0) {
      currentRadio = (Radio)((currentRadio + 1) % RADIO_COUNT);
      updated = 1;
    }
    isChangeRadioButtonPressed = 1;
  } else {
    isChangeRadioButtonPressed = 0;
  }

  if (updated == 1) {
    printRadio();
    updated = 0;
  }
}


void printRadio() {
  lcd.clear();
  printLabel();
  printFrequncies();
}


void printLabel() {
  char *label;
  switch (currentRadio) {
    case COM1:
      label = COM1_LABEL;
      break;

    case COM2:
      label = COM2_LABEL;
      break;

    case NAV1:
      label = NAV1_LABEL;
      break;

    case NAV2:
      label = NAV2_LABEL;
      break;
  }
  lcd.setCursor(0, 0);
  lcd.print(label);
}


void printFrequncies() {
  lcd.setCursor(0, 1);
  printActiveFrequency();
  lcd.write("<");
  lcd.write(">");
  printStandbyFrequency();
}


void printActiveFrequency() {
  char *frequency;
  switch (currentRadio) {
    case COM1:
      lcd.print(com1_freq);
      break;

    case COM2:
      lcd.print(com2_freq);
      break;

    case NAV1:
      lcd.print(nav1_freq);
      lcd.write(" ");
      break;

    case NAV2:
      lcd.print(nav2_freq);
      lcd.write(" ");
      break;
  }
}


void printStandbyFrequency() {
  char *frequency;
  switch (currentRadio) {
    case COM1:
      lcd.print(com1_sb_freq);
      break;

    case COM2:
      lcd.print(com2_sb_freq);
      break;

    case NAV1:
      lcd.write(" ");
      lcd.print(nav1_sb_freq);
      break;

    case NAV2:
      lcd.write(" ");
      lcd.print(nav2_sb_freq);
      break;
  }
}



void read_serial() {
  if (Serial.available()) {
    char token = Serial.read();
    token_processing(token);
  }
}


void store_token(char token, char destination[], int size) {
  if (position_to_write < size) {
    destination[position_to_write] = token;
    position_to_write++;
    if (position_to_write >= size) {
      updated = 1;
      token_processing = determine_extraction_type;
    }
  } else {
    determine_extraction_type(token);
  }
}


void determine_extraction_type(char token) {
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


void read_com1_freq(char token) {
  store_token(token, com1_freq, COM_FREQ_SIZE);
}


void read_com1_sb_freq(char token) {
  store_token(token, com1_sb_freq, COM_FREQ_SIZE);
}


void read_com2_freq(char token) {
  store_token(token, com2_freq, COM_FREQ_SIZE);
}


void read_com2_sb_freq(char token) {
  store_token(token, com2_sb_freq, COM_FREQ_SIZE);
}


void read_nav1_freq(char token) {
  store_token(token, nav1_freq, NAV_FREQ_SIZE);
}


void read_nav1_sb_freq(char token) {
  store_token(token, nav1_sb_freq, NAV_FREQ_SIZE);
}


void read_nav2_freq(char token) {
  store_token(token, nav2_freq, NAV_FREQ_SIZE);
}


void read_nav2_sb_freq(char token) {
  store_token(token, nav2_sb_freq, NAV_FREQ_SIZE);
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

