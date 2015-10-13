#include "Radio.h"
#include "SerialReader.h"
#include "SimConnectInputs.h"
#include "Glyphs.h"


void print_radio();
void print_label();
void print_frequncies();
void print_active_frequency();
void print_standby_frequency();
void change_frequency();


#define COM1_LABEL    "ACT   COM1   S/B"
#define COM2_LABEL    "ACT   COM2   S/B"
#define NAV1_LABEL    "ACT   NAV1   S/B"
#define NAV2_LABEL    "ACT   NAV2   S/B"

#define SWAP_FREQUENCY_PIN    9
#define CHANGE_RADIO_PIN      8

#define RADIO_LCD_ROW_COUNT   2
#define RADIO_LCD_COL_COUNT   16

#define RADIO_CALL_GLYH_INDEX 0
#define RADIO_CALL_GLYH       byte(RADIO_CALL_GLYH_INDEX)

enum Radio {
  COM1 = 0,
  COM2,
  NAV1,
  NAV2,
  RADIO_COUNT
};
typedef enum Radio Radio;

LiquidCrystal radio_lcd = LiquidCrystal(19, 18, 17, 16, 15, 14);

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

Radio currentRadio = COM1;
int radio_updated = 0;
int isSwapFrequencyButtonPressed = 0;
int isChangeRadioButtonPressed = 0;


// Public

void radio_setup() {
  pinMode(CHANGE_RADIO_PIN, INPUT_PULLUP);
  pinMode(SWAP_FREQUENCY_PIN, INPUT_PULLUP);

  radio_lcd.createChar(RADIO_CALL_GLYH_INDEX, CALL_GLYPH);
  radio_lcd.begin(RADIO_LCD_COL_COUNT, RADIO_LCD_ROW_COUNT);
  print_radio();
}


void radio_tick() {
  if (digitalRead(SWAP_FREQUENCY_PIN) == 1) {
    if (isSwapFrequencyButtonPressed == 0) {
      change_frequency();
    }
    isSwapFrequencyButtonPressed = 1;
  } else {
    isSwapFrequencyButtonPressed = 0;
  }

  if (digitalRead(CHANGE_RADIO_PIN) == 1) {
    if (isChangeRadioButtonPressed == 0) {
      currentRadio = (Radio)((currentRadio + 1) % RADIO_COUNT);
      radio_updated = 1;
    }
    isChangeRadioButtonPressed = 1;
  } else {
    isChangeRadioButtonPressed = 0;
  }

  if (radio_updated == 1) {
    print_radio();
    radio_updated = 0;
  }
}


void read_com1_freq(char token) {
  store_token(token, com1_freq, COM_FREQ_SIZE, &radio_updated);
}


void read_com1_sb_freq(char token) {
  store_token(token, com1_sb_freq, COM_FREQ_SIZE, &radio_updated);
}


void read_com2_freq(char token) {
  store_token(token, com2_freq, COM_FREQ_SIZE, &radio_updated);
}


void read_com2_sb_freq(char token) {
  store_token(token, com2_sb_freq, COM_FREQ_SIZE, &radio_updated);
}


void read_nav1_freq(char token) {
  store_token(token, nav1_freq, NAV_FREQ_SIZE, &radio_updated);
}


void read_nav1_sb_freq(char token) {
  store_token(token, nav1_sb_freq, NAV_FREQ_SIZE, &radio_updated);
}


void read_nav2_freq(char token) {
  store_token(token, nav2_freq, NAV_FREQ_SIZE, &radio_updated);
}


void read_nav2_sb_freq(char token) {
  store_token(token, nav2_sb_freq, NAV_FREQ_SIZE, &radio_updated);
}


// Private

void print_radio() {
  radio_lcd.clear();
  print_label();
  print_frequncies();
}


void print_label() {
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
  radio_lcd.setCursor(0, 0);
  radio_lcd.print(label);
}


void print_frequncies() {
  radio_lcd.setCursor(0, 1);
  print_active_frequency();
  radio_lcd.write("<");
  radio_lcd.write(">");
  print_standby_frequency();
}


void print_active_frequency() {
  char *frequency;
  switch (currentRadio) {
    case COM1:
      radio_lcd.print(com1_freq);
      break;

    case COM2:
      radio_lcd.print(com2_freq);
      break;

    case NAV1:
      radio_lcd.print(nav1_freq);
      radio_lcd.write(" ");
      break;

    case NAV2:
      radio_lcd.print(nav2_freq);
      radio_lcd.write(" ");
      break;
  }
}


void print_standby_frequency() {
  char *frequency;
  switch (currentRadio) {
    case COM1:
      radio_lcd.print(com1_sb_freq);
      break;

    case COM2:
      radio_lcd.print(com2_sb_freq);
      break;

    case NAV1:
      radio_lcd.write(" ");
      radio_lcd.print(nav1_sb_freq);
      break;

    case NAV2:
      radio_lcd.write(" ");
      radio_lcd.print(nav2_sb_freq);
      break;
  }
}


void change_frequency() {
  char *code = "";
  switch (currentRadio) {
    case COM1:
      code = SWAP_COM1;
      break;

    case COM2:
      code = SWAP_COM2;
      break;

    case NAV1:
      code = SWAP_NAV1;
      break;

    case NAV2:
      code = SWAP_NAV2;
      break;
  }
  Serial.println(code);
}

