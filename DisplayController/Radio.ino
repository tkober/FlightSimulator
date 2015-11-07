#ifdef CONTROLLER_A

#include "Radio.h"
#include "SerialReader.h"
#include "SimConnectInputs.h"
#include "Glyphs.h"
#include "Button.h"
#include "Pins.h"
#include "MFD.h"
#include "RotaryEncoder.h"


#define RADIO_LCD_ROW_COUNT   2
#define RADIO_LCD_COL_COUNT   16

#define RADIO_CALL_GLYH_INDEX         0
#define RADIO_CALL_GLYH               byte(RADIO_CALL_GLYH_INDEX)
  
#define RADIO_SWAP_LEFT_GLYPH_INDEX   1
#define RADIO_SWAP_LEFT_GLYPH         byte(RADIO_SWAP_LEFT_GLYPH_INDEX)

#define RADIO_SWAP_RIGHT_GLYPH_INDEX  2
#define RADIO_SWAP_RIGHT_GLYPH        byte(RADIO_SWAP_RIGHT_GLYPH_INDEX)


typedef enum {
  Megaherz = 0,
  Kiloherz, 
  FrequencyChangeSize
} FrequencyChange;


typedef enum {
  Digit1 = 0,
  Digit2, 
  Digit3,
  Digit4,
  TransponderChangeSize
} TransponderChange;


void swap_frequency();
void previous_radio();
void next_radio();
void show_current_radio();
void call();
void change_pressed();
void increment(int boost);
void decrement(int boost);

extern RadioPage com1_page;
void com1_page_will_appeaer();
void com1_page_render();
void com1_page_update_if_necessary();
void com1_page_change_button_pressed();
void com1_page_call();
void com1_page_swap();
void com1_page_increment();
void com1_page_decrement();

extern RadioPage com2_page;
void com2_page_will_appeaer();
void com2_page_render();
void com2_page_update_if_necessary();
void com2_page_change_button_pressed();
void com2_page_call();
void com2_page_swap();
void com2_page_increment();
void com2_page_decrement();

extern RadioPage nav1_page;
void nav1_page_will_appeaer();
void nav1_page_render();
void nav1_page_update_if_necessary();
void nav1_page_change_button_pressed();
void nav1_page_call();
void nav1_page_swap();
void nav1_page_increment();
void nav1_page_decrement();

extern RadioPage nav2_page;
void nav2_page_will_appeaer();
void nav2_page_render();
void nav2_page_update_if_necessary();
void nav2_page_change_button_pressed();
void nav2_page_call();
void nav2_page_swap();
void nav2_page_increment();
void nav2_page_decrement();

extern RadioPage transponder_page;
void transponder_page_will_appeaer();
void transponder_page_render();
void transponder_page_update_if_necessary();
void transponder_page_change_button_pressed();
void transponder_page_increment();
void transponder_page_decrement();


RadioPage com1_page = RADIO_PAGE_MAKE(&transponder_page, &com2_page, 
  com1_page_will_appeaer, 
  com1_page_render, 
  com1_page_update_if_necessary, 
  NULL, 
  com1_page_change_button_pressed, 
  com1_page_call, 
  com1_page_swap, 
  com1_page_increment, 
  com1_page_decrement
 );
 
RadioPage com2_page = RADIO_PAGE_MAKE(&com1_page, &nav1_page, 
  com2_page_will_appeaer, 
  com2_page_render, 
  com2_page_update_if_necessary, 
  NULL, 
  com2_page_change_button_pressed, 
  com2_page_call, 
  com2_page_swap, 
  com2_page_increment, 
  com2_page_decrement
);

RadioPage nav1_page = RADIO_PAGE_MAKE(&com2_page, &nav2_page, 
  nav1_page_will_appeaer, 
  nav1_page_render, 
  nav1_page_update_if_necessary, 
  NULL, 
  nav1_page_change_button_pressed, 
  nav1_page_call, 
  nav1_page_swap, 
  nav1_page_increment, 
  nav1_page_decrement
);

RadioPage nav2_page = RADIO_PAGE_MAKE(&nav1_page, &transponder_page,
  nav2_page_will_appeaer, 
  nav2_page_render, 
  nav2_page_update_if_necessary, 
  NULL, 
  nav2_page_change_button_pressed, 
  nav2_page_call, 
  nav2_page_swap, 
  nav2_page_increment, 
  nav2_page_decrement
);

RadioPage transponder_page = RADIO_PAGE_MAKE(&nav2_page, &com1_page,
  transponder_page_will_appeaer, 
  transponder_page_render, 
  transponder_page_update_if_necessary, 
  NULL, 
  transponder_page_change_button_pressed, 
  NULL, 
  NULL, 
  transponder_page_increment, 
  transponder_page_decrement
);


Button call_button = Button(RADIO_CALL_PIN, 1);
Button swap_freq_button = Button(RADIO_SWAP_FREQ_PIN, 1);
Button previous_radio_button = Button(RADIO_PREVIOUS_RADIO_PIN, 1);
Button next_radio_button = Button(RADIO_NEXT_RADIO_PIN, 1);
PushableRotaryEncoder change_rotary_encoder = PushableRotaryEncoder(RADIO_CHANGE_ENCODER_PIN_A, RADIO_CHANGE_ENCODER_PIN_B, RADIO_CHANGE_BUTTON_PIN, 1);
LiquidCrystal radio_lcd = LiquidCrystal(RADIO_LCD_PINS);

RadioPage *current_radio_page = &com1_page;

#define COM_FREQ_SIZE   7
char com1_freq[COM_FREQ_SIZE]       = {'-', '-', '-', '.', '-', '-', '-'};
char com1_sb_freq[COM_FREQ_SIZE]    = {'-', '-', '-', '.', '-', '-', '-'};
char com2_freq[COM_FREQ_SIZE]       = {'-', '-', '-', '.', '-', '-', '-'};
char com2_sb_freq[COM_FREQ_SIZE]    = {'-', '-', '-', '.', '-', '-', '-'};

#define NAV_FREQ_SIZE   6
char nav1_freq[NAV_FREQ_SIZE]       = {'-', '-', '-', '.', '-', '-'};
char nav1_sb_freq[NAV_FREQ_SIZE]    = {'-', '-', '-', '.', '-', '-'};
char nav2_freq[NAV_FREQ_SIZE]       = {'-', '-', '-', '.', '-', '-'};
char nav2_sb_freq[NAV_FREQ_SIZE]    = {'-', '-', '-', '.', '-', '-'};

#define TRANSPONDER_CODE_SIZE  4
char transponder_code[TRANSPONDER_CODE_SIZE] = {'-', '-', '-', '-'};

#define SOUND_ACTIVE_SIZE   1
char com1_sound_active[SOUND_ACTIVE_SIZE] = {'0'};
char com2_sound_active[SOUND_ACTIVE_SIZE] = {'0'};
char nav1_sound_active[SOUND_ACTIVE_SIZE] = {'0'};
char nav2_sound_active[SOUND_ACTIVE_SIZE] = {'0'};

int com1_updated = 0;
int com2_updated = 0;
int nav1_updated = 0;
int nav2_updated = 0;
int transponder_updated = 0;

FrequencyChange frequency_change = Megaherz;
TransponderChange transponder_change = Digit1;


// Public

void radio_setup() {
  swap_freq_button.setOnClick(swap_frequency);
  previous_radio_button.setOnClick(previous_radio);
  next_radio_button.setOnClick(next_radio);
  call_button.setOnClick(call);
  change_rotary_encoder.setOnClick(change_pressed);
  change_rotary_encoder.setOnRotateClockwise(increment);
  change_rotary_encoder.setOnRotateCounterClockwise(decrement);

  radio_lcd.createChar(RADIO_CALL_GLYH_INDEX, CALL_GLYPH);
  radio_lcd.createChar(RADIO_SWAP_LEFT_GLYPH_INDEX, SWAP_LEFT_GLYPH);
  radio_lcd.createChar(RADIO_SWAP_RIGHT_GLYPH_INDEX, SWAP_RIGHT_GLYPH);
  radio_lcd.begin(RADIO_LCD_COL_COUNT, RADIO_LCD_ROW_COUNT);
  show_current_radio();
}


void radio_tick() {  
  swap_freq_button.tick();
  previous_radio_button.tick();
  next_radio_button.tick();
  call_button.tick();
  change_rotary_encoder.tick();

  if (current_radio_page->update_if_necessary != NULL) {
    current_radio_page->update_if_necessary();
  }
}


void read_com1_freq(char token) {
  store_token(token, com1_freq, COM_FREQ_SIZE, &com1_updated);
}


void read_com1_sb_freq(char token) {
  store_token(token, com1_sb_freq, COM_FREQ_SIZE, &com1_updated);
}


void read_com2_freq(char token) {
  store_token(token, com2_freq, COM_FREQ_SIZE, &com2_updated);
}


void read_com2_sb_freq(char token) {
  store_token(token, com2_sb_freq, COM_FREQ_SIZE, &com2_updated);
}


void read_nav1_freq(char token) {
  store_token(token, nav1_freq, NAV_FREQ_SIZE, &nav1_updated);
}


void read_nav1_sb_freq(char token) {
  store_token(token, nav1_sb_freq, NAV_FREQ_SIZE, &nav1_updated);
}


void read_nav2_freq(char token) {
  store_token(token, nav2_freq, NAV_FREQ_SIZE, &nav2_updated);
}


void read_nav2_sb_freq(char token) {
  store_token(token, nav2_sb_freq, NAV_FREQ_SIZE, &nav2_updated);
}


void read_transponder_code(char token) {
  store_token(token, transponder_code, TRANSPONDER_CODE_SIZE, &transponder_updated);
}


void read_com1_sound_active(char token) {
  store_token(token, com1_sound_active, SOUND_ACTIVE_SIZE, &com1_updated);
}


void read_com2_sound_active(char token) {
  store_token(token, com2_sound_active, SOUND_ACTIVE_SIZE, &com2_updated);
}


void read_nav1_sound_active(char token) {
  store_token(token, nav1_sound_active, SOUND_ACTIVE_SIZE, &nav1_updated);
}


void read_nav2_sound_active(char token) {
  store_token(token, nav2_sound_active, SOUND_ACTIVE_SIZE, &nav2_updated);
}


// Private

void swap_frequency() {
  if (current_radio_page->swap != NULL) {
    current_radio_page->swap();
  }
}


void previous_radio() {
  if (current_radio_page->will_disappear != NULL) {
        current_radio_page->will_disappear();
    }
  current_radio_page = (RadioPage*)current_radio_page->previous_page;
  show_current_radio();
}


void next_radio() {
  if (current_radio_page->will_disappear != NULL) {
        current_radio_page->will_disappear();
    }
  current_radio_page = (RadioPage*)current_radio_page->next_page;
  show_current_radio();
}


void show_current_radio() {
  if (current_radio_page->will_appear != NULL) {
        current_radio_page->will_appear();
  }
  if (current_radio_page->render != NULL) {
    current_radio_page->render();
  }
}


void call() {
  if (current_radio_page->call != NULL) {
    current_radio_page->call();
  }
}


void change_pressed() {
  if (current_radio_page->change_button_pressed != NULL) {
    current_radio_page->change_button_pressed();
  }
}


void increment(int boost) {
  if (current_radio_page->increment != NULL) {
    current_radio_page->increment();
  }
}


void decrement(int boost) {
  if (current_radio_page->decrement != NULL) {
    current_radio_page->decrement();
  }
}


// COM1 Page
void com1_page_will_appeaer() {
  frequency_change = Megaherz;
  radio_lcd.clear();
}


void com1_page_render() {
  radio_lcd.setCursor(0, 0);
  radio_lcd.write("ACT   COM1");
  radio_lcd.write(com1_sound_active[0] == '1' ? RADIO_CALL_GLYH : ' ');
  radio_lcd.write("  S/B");
  radio_lcd.setCursor(0, 1);
  print_string_to_lcd(radio_lcd, com1_freq, COM_FREQ_SIZE);
  radio_lcd.write(RADIO_SWAP_LEFT_GLYPH);
  radio_lcd.write(RADIO_SWAP_RIGHT_GLYPH);
  print_string_to_lcd(radio_lcd, com1_sb_freq, COM_FREQ_SIZE);
  switch(frequency_change) {
    case Megaherz:
      radio_lcd.setCursor(11, 1);
      break;

    case Kiloherz:
      radio_lcd.setCursor(13, 1);
      break;
  }
  radio_lcd.cursor();
}


void com1_page_update_if_necessary() {
  if (com1_updated == 1) {
    com1_updated = 0;
    com1_page_render();
  }
}


void com1_page_change_button_pressed() {
  frequency_change = (FrequencyChange)((frequency_change + 1) % FrequencyChangeSize);
  if (current_radio_page->render != NULL) {
    current_radio_page->render();
  }
}


void com1_page_call() {
  Serial.println(SET_COM1_TO_TRANSMIT);   
}


void com1_page_swap() {
  Serial.println(SWAP_COM1);
}


void com1_page_increment() {
  switch (frequency_change) {
    case Megaherz:
      Serial.println(INCREMENT_COM1_MHZ);
      break;

    case Kiloherz:
      Serial.println(INCREMENT_COM1_KHZ);
      break;
  }
}


void com1_page_decrement() {
  switch (frequency_change) {
    case Megaherz:
      Serial.println(DECREMENT_COM1_MHZ);
      break;

    case Kiloherz:
      Serial.println(DECREMENT_COM1_KHZ);
      break;
  }
}


// COM2 Page
void com2_page_will_appeaer() {
  frequency_change = Megaherz;
  radio_lcd.clear();
}


void com2_page_render() {
  radio_lcd.setCursor(0, 0);
  radio_lcd.write("ACT   COM2");
  radio_lcd.write(com2_sound_active[0] == '1' ? RADIO_CALL_GLYH : ' ');
  radio_lcd.write("  S/B");
  radio_lcd.setCursor(0, 1);
  print_string_to_lcd(radio_lcd, com2_freq, COM_FREQ_SIZE);
  radio_lcd.write(RADIO_SWAP_LEFT_GLYPH);
  radio_lcd.write(RADIO_SWAP_RIGHT_GLYPH);
  print_string_to_lcd(radio_lcd, com2_sb_freq, COM_FREQ_SIZE);
  switch(frequency_change) {
    case Megaherz:
      radio_lcd.setCursor(11, 1);
      break;

    case Kiloherz:
      radio_lcd.setCursor(13, 1);
      break;
  }
  radio_lcd.cursor();
}


void com2_page_update_if_necessary() {
  if (com2_updated == 1) {
    com2_updated = 0;
    com2_page_render();
  }
}


void com2_page_change_button_pressed() {
  frequency_change = (FrequencyChange)((frequency_change + 1) % FrequencyChangeSize);
  if (current_radio_page->render != NULL) {
    current_radio_page->render();
  }
}


void com2_page_call() {
  Serial.println(SET_COM2_TO_TRANSMIT);
}


void com2_page_swap() {
  Serial.println(SWAP_COM2);   
}


void com2_page_increment() {
  switch (frequency_change) {
    case Megaherz:
      Serial.println(INCREMENT_COM2_MHZ);
      break;

    case Kiloherz:
      Serial.println(INCREMENT_COM2_KHZ);
      break;
  }
}


void com2_page_decrement() {
  switch (frequency_change) {
    case Megaherz:
      Serial.println(DECREMENT_COM2_MHZ);
      break;

    case Kiloherz:
      Serial.println(DECREMENT_COM2_KHZ);
      break;
  }
}


// NAV1 Page
void nav1_page_will_appeaer() {
  frequency_change = Megaherz;
  radio_lcd.clear();
}


void nav1_page_render() {
  radio_lcd.setCursor(0, 0);
  radio_lcd.write("ACT   NAV1");
  radio_lcd.write(nav1_sound_active[0] == '1' ? RADIO_CALL_GLYH : ' ');
  radio_lcd.write("  S/B");
  radio_lcd.setCursor(0, 1);
  print_string_to_lcd(radio_lcd, nav1_freq, NAV_FREQ_SIZE);
  radio_lcd.write(' ');
  radio_lcd.write(RADIO_SWAP_LEFT_GLYPH);
  radio_lcd.write(RADIO_SWAP_RIGHT_GLYPH);
  radio_lcd.write(' ');
  print_string_to_lcd(radio_lcd, nav1_sb_freq, NAV_FREQ_SIZE);
  switch(frequency_change) {
    case Megaherz:
      radio_lcd.setCursor(12, 1);
      break;

    case Kiloherz:
      radio_lcd.setCursor(15, 1);
      break;
  }
  radio_lcd.cursor();
}


void nav1_page_update_if_necessary() {
  if (nav1_updated == 1) {
    nav1_updated = 0;
    nav1_page_render();
  }
}


void nav1_page_change_button_pressed() {
  frequency_change = (FrequencyChange)((frequency_change + 1) % FrequencyChangeSize);
  if (current_radio_page->render != NULL) {
    current_radio_page->render();
  }
}


void nav1_page_call() {
  Serial.println(TOGGLE_NAV1_ID);
}


void nav1_page_swap() {
  
  Serial.println(SWAP_NAV1);
}


void nav1_page_increment() {
  switch (frequency_change) {
    case Megaherz:
      Serial.println(INCREMENT_NAV1_MHZ);
      break;

    case Kiloherz:
      Serial.println(INCREMENT_NAV1_KHZ);
      break;
  }   
}


void nav1_page_decrement() {
  switch (frequency_change) {
    case Megaherz:
      Serial.println(DECREMENT_NAV1_MHZ);
      break;

    case Kiloherz:
      Serial.println(DECREMENT_NAV1_KHZ);
      break;
  }  
}


// NAV2 Page
void nav2_page_will_appeaer() {
  frequency_change = Megaherz;
  radio_lcd.clear();
}


void nav2_page_render() {
  radio_lcd.setCursor(0, 0);
  radio_lcd.write("ACT   NAV2");
  radio_lcd.write(nav2_sound_active[0] == '1' ? RADIO_CALL_GLYH : ' ');
  radio_lcd.write("  S/B");
  radio_lcd.setCursor(0, 1);
  print_string_to_lcd(radio_lcd, nav2_freq, NAV_FREQ_SIZE);
  radio_lcd.write(' ');
  radio_lcd.write(RADIO_SWAP_LEFT_GLYPH);
  radio_lcd.write(RADIO_SWAP_RIGHT_GLYPH);
  radio_lcd.write(' ');
  print_string_to_lcd(radio_lcd, nav2_sb_freq, NAV_FREQ_SIZE);
  switch(frequency_change) {
    case Megaherz:
      radio_lcd.setCursor(12, 1);
      break;

    case Kiloherz:
      radio_lcd.setCursor(15, 1);
      break;
  }
  radio_lcd.cursor();
}


void nav2_page_update_if_necessary() {
  if (nav2_updated == 1) {
    nav2_updated = 0;
    nav2_page_render();
  }
}


void nav2_page_change_button_pressed() {
  frequency_change = (FrequencyChange)((frequency_change + 1) % FrequencyChangeSize);
  if (current_radio_page->render != NULL) {
    current_radio_page->render();
  }
}


void nav2_page_call() {
  Serial.println(TOGGLE_NAV2_ID);   
}


void nav2_page_swap() {
  Serial.println(SWAP_NAV2);
}


void nav2_page_increment() {
  switch (frequency_change) {
    case Megaherz:
      Serial.println(INCREMENT_NAV2_MHZ);
      break;

    case Kiloherz:
      Serial.println(INCREMENT_NAV2_KHZ);
      break;
  }  
}


void nav2_page_decrement() {
  switch (frequency_change) {
    case Megaherz:
      Serial.println(DECREMENT_NAV2_MHZ);
      break;

    case Kiloherz:
      Serial.println(DECREMENT_NAV2_KHZ);
      break;
  }  
}


// Transponder
void transponder_page_will_appeaer() {
  transponder_change = Digit1;
  radio_lcd.clear();
}


void transponder_page_render() {
  radio_lcd.setCursor(0, 0);
  radio_lcd.write("    TCAS/ATC    ");
  radio_lcd.setCursor(0, 1);
  radio_lcd.write("      ");
  print_string_to_lcd(radio_lcd, transponder_code, TRANSPONDER_CODE_SIZE);
  radio_lcd.write("      ");
  switch(transponder_change) {
    case Digit1:
      radio_lcd.setCursor(6, 1);
      break;

    case Digit2:
      radio_lcd.setCursor(7, 1);
      break;

    case Digit3:
      radio_lcd.setCursor(8, 1);
      break;

    case Digit4:
      radio_lcd.setCursor(9, 1);
      break;
  }
  radio_lcd.cursor();
}


void transponder_page_update_if_necessary() {
  if (transponder_updated == 1) {
    transponder_updated = 0;
    transponder_page_render();
  }
}


void transponder_page_change_button_pressed() {
  transponder_change = (TransponderChange)((transponder_change + 1) % TransponderChangeSize);
  if (current_radio_page->render != NULL) {
    current_radio_page->render();
  }
}


void transponder_page_increment() {
  switch(transponder_change) {
    case Digit1:
      Serial.println(INCREMENT_TRANSPONDER_DIGIT1);
      break;

    case Digit2:
      Serial.println(INCREMENT_TRANSPONDER_DIGIT2);
      break;

    case Digit3:
      Serial.println(INCREMENT_TRANSPONDER_DIGIT3);
      break;

    case Digit4:
      Serial.println(INCREMENT_TRANSPONDER_DIGIT4);
      break;
  }
}


void transponder_page_decrement() {
  switch(transponder_change) {
    case Digit1:
      Serial.println(DECREMENT_TRANSPONDER_DIGIT1);
      break;

    case Digit2:
      Serial.println(DECREMENT_TRANSPONDER_DIGIT2);
      break;

    case Digit3:
      Serial.println(DECREMENT_TRANSPONDER_DIGIT3);
      break;

    case Digit4:
      Serial.println(DECREMENT_TRANSPONDER_DIGIT4);
      break;
  }
}

#endif
