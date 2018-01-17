#include "Radio.h"
#include "SerialReader.h"
#include "SimConnectInputs.h"
#include "Glyphs.h"
#include "Button.h"
#include "Pins.h"
#include "Keypad.h"

typedef struct RadioEntry {
  // Values
  int size;
  char *standby;
  char *active;

  // Rendering
  int rendering_size;
  char *name;

  // call
  char *call;

  // Updated
  int updated;

  // Inputs
  char *set_stby_input_prefix;
  char *swap_input;
  char *toggle_call_input;
} RadioEntry;
#define RADIO_ENTRY_MAKE(_size, _standby, _active, _rendering_size, _name, _call, _updated, _set_stby_input_prefix, _swap_input, _toggle_call_input) {_size, _standby, _active, _rendering_size, _name, _call, _updated, _set_stby_input_prefix, _swap_input, _toggle_call_input}

RadioEntry com1 = RADIO_ENTRY_MAKE(
  // Values
  7, "---.---", "---.---",
  // Rendering
  6, "COM1",
  // Call
  "0",
  // Updated
  0,
  // Inputs
  "XXX", "XXX", "XXX"
);

RadioEntry com2 = RADIO_ENTRY_MAKE(
  // Values
  7, "---.---", "---.---",
  // Rendering
  6, "COM2",
  // Call
  "0",
  // Updated
  0,
  // Inputs
  "XXX", "XXX", "XXX"
);

RadioEntry nav1 = RADIO_ENTRY_MAKE(
  // Values
  6, "---.--", "---.--",
  // Rendering
  6, "NAV1",
  // Call
  "0",
  // Updated
  0,
  // Inputs
  "XXX", "XXX", "XXX"
);

RadioEntry nav2 = RADIO_ENTRY_MAKE(
  // Values
  6, "---.--", "---.--",
  // Rendering
  6, "NAV2",
  // Call
  "0",
  // Updated
  0,
  // Inputs
  "XXX", "XXX", "XXX"
);

#define TRANSPONDER_CODE_SIZE  4
char transponder_code[TRANSPONDER_CODE_SIZE] = {'-', '-', '-', '-'};
int transponder_updated = 0;

#define RADIO_LCD_ROW_COUNT   4
#define RADIO_LCD_COL_COUNT   20

#define RADIO_CALL_GLYPH_INDEX        0
#define RADIO_CALL_GLYPH              byte(RADIO_CALL_GLYPH_INDEX)

#define PLACEHOLDER_GLYPH_INDEX       1
#define PLACEHOLDER_GLYPH             byte(PLACEHOLDER_GLYPH_INDEX)

#define SCROLL_UP_GLYPH_INDEX         2
#define SCROLL_UP_GLYPH               byte(SCROLL_UP_GLYPH_INDEX)

#define SCROLL_DOWN_GLYPH_INDEX       3
#define SCROLL_DOWN_GLYPH             byte(SCROLL_DOWN_GLYPH_INDEX)

LiquidCrystal upper_radio_lcd = LiquidCrystal(RADIO_UPPER_LCD_PINS);
LiquidCrystal lower_radio_lcd = LiquidCrystal(RADIO_LOWER_LCD_PINS);

Button l1_button = Button(RADIO_L1_PIN, 1);
Button l2_button = Button(RADIO_L2_PIN, 1);
Button l3_button = Button(RADIO_L3_PIN, 1);
Button l4_button = Button(RADIO_L4_PIN, 1);

Button r1_button = Button(RADIO_R1_PIN, 1);
Button r2_button = Button(RADIO_R2_PIN, 1);
Button r3_button = Button(RADIO_R3_PIN, 1);
Button r4_button = Button(RADIO_R4_PIN, 1);
Button r5_button = Button(RADIO_R5_PIN, 1);

Button scroll_up_button   = Button(SCROLL_UP_PIN, 1);
Button scroll_down_button = Button(SCROLL_DOWN_PIN, 1);

#define KEYPAD_ROW_COUNT  4
#define KEYPAD_COL_COUNT  4
#define KEYPAD_KEYS       (char[KEYPAD_ROW_COUNT][KEYPAD_COL_COUNT]){ {'1','2','3','-'}, {'4','5','6','-'}, {'7','8','9','C'}, {'.','0','-','-'} }

Keypad keypad = Keypad( makeKeymap(KEYPAD_KEYS), KEYPAD_ROW_PINS, KEYPAD_COL_PINS, KEYPAD_ROW_COUNT, KEYPAD_COL_COUNT);

#define SCRATCHPAD_SIZE   6
int scratchpad_index = 0;
char scratchpad[SCRATCHPAD_SIZE]    = {'-', '-', '-', '-', '-', '-'};

int should_render_upper_lcd = 0;
int should_render_lower_lcd = 0;

#define RADIO_ENTRY_COUNT 2
RadioEntry radio_entries[RADIO_ENTRY_COUNT];
int top_entry_index = 0;

// Public

void radio_setup() {
  radio_entries[0] = com1;
  radio_entries[1] = com2;
  radio_entries[2] = nav1;
  radio_entries[3] = nav2;

  l1_button.setOnClick(l1_4_pressed);
  l2_button.setOnClick(l1_4_pressed);
  l3_button.setOnClick(l1_4_pressed);
  l4_button.setOnClick(l1_4_pressed);
  r1_button.setOnClick(r1_4_pressed);
  r2_button.setOnClick(r1_4_pressed);
  r3_button.setOnClick(r1_4_pressed);
  r4_button.setOnClick(r1_4_pressed);
  r5_button.setOnClick(r5_pressed);

  scroll_up_button.setOnClick(scroll_up_button_pressed);
  scroll_down_button.setOnClick(scroll_down_button_pressed);

  l1_button.setId(0);
  r1_button.setId(0);

  l2_button.setId(1);
  r2_button.setId(1);

  l3_button.setId(2);
  r3_button.setId(2);

  l4_button.setId(3);
  r4_button.setId(3);

  upper_radio_lcd.createChar(RADIO_CALL_GLYPH_INDEX, CALL_GLYPH);
  upper_radio_lcd.createChar(PLACEHOLDER_GLYPH_INDEX, PLACEHOLDER);
  upper_radio_lcd.createChar(SCROLL_UP_GLYPH_INDEX, SCROLL_UP);
  upper_radio_lcd.createChar(SCROLL_DOWN_GLYPH_INDEX, SCROLL_DOWN);
  upper_radio_lcd.begin(RADIO_LCD_COL_COUNT, RADIO_LCD_ROW_COUNT);

  lower_radio_lcd.createChar(PLACEHOLDER_GLYPH_INDEX, PLACEHOLDER_GLYPH);
  lower_radio_lcd.begin(RADIO_LCD_COL_COUNT, RADIO_LCD_ROW_COUNT);

  render_upper_display(upper_radio_lcd);
  render_lower_display(lower_radio_lcd);
}


void radio_tick() {
  keypad_tick();

  l1_button.tick();
  l2_button.tick();
  l3_button.tick();
  l4_button.tick();
  r1_button.tick();
  r2_button.tick();
  r3_button.tick();
  r4_button.tick();
  r5_button.tick();

  scroll_up_button.tick();
  scroll_down_button.tick();

  if (should_render_upper_lcd) {
    render_upper_display(upper_radio_lcd);
    should_render_upper_lcd = 0;
  }

  if (should_render_lower_lcd) {
    render_lower_display(lower_radio_lcd);
    should_render_lower_lcd = 0;
  }
}

void read_com1_freq(char token) {
  store_token(token, com1.active, com1.size, &com1.updated);
}


void read_com1_sb_freq(char token) {
  store_token(token, com1.standby, com1.size, &com1.updated);
}


void read_com2_freq(char token) {
  store_token(token, com2.active, com2.size, &com2.updated);
}


void read_com2_sb_freq(char token) {
  store_token(token, com2.standby, com2.size, &com2.updated);
}


void read_nav1_freq(char token) {
  store_token(token, nav1.active, nav1.size, &nav1.updated);
}


void read_nav1_sb_freq(char token) {
  store_token(token, nav1.standby, nav1.size, &nav1.updated);
}


void read_nav2_freq(char token) {
  store_token(token, nav2.active, nav2.size, &nav2.updated);
}


void read_nav2_sb_freq(char token) {
  store_token(token, nav2.standby, nav2.size, &nav2.updated);
}


void read_transponder_code(char token) {
  store_token(token, transponder_code, TRANSPONDER_CODE_SIZE, &transponder_updated);
}


void read_com1_sound_active(char token) {
  store_token(token, com1.call, 1, &com1.updated);
}


void read_com2_sound_active(char token) {
  store_token(token, com2.call, 1, &com2.updated);
}


void read_nav1_sound_active(char token) {
  store_token(token, nav1.call, 1, &nav1.updated);
}


void read_nav2_sound_active(char token) {
  store_token(token, nav2.call, 1, &nav2.updated);
}

// Private

void keypad_tick() {
  char customKey = keypad.getKey();
  if (customKey && customKey != '-') {

    // Clear?
    if (customKey == 'C') {
      clear_scratchpad();
    }

    // Append to scratchpad if not already full
    if (scratchpad_index < SCRATCHPAD_SIZE) {
      scratchpad[scratchpad_index] = customKey;

      should_render_upper_lcd = (scratchpad_index == 0) ? 1 : should_render_upper_lcd;
      should_render_lower_lcd = 1;
      scratchpad_index++;
    }
  }
}

void clear_scratchpad() {
  for (int i = 0; i < SCRATCHPAD_SIZE; i++) {
    scratchpad[i] = '-';
  }
  should_render_lower_lcd = 1;
  should_render_upper_lcd = 1;
}

void l1_4_pressed(int buttonId) {
  // Toggle Call
  RadioEntry entry = radio_entries[top_entry_index + buttonId];
  Serial.println(entry.toggle_call_input);
}

void r1_4_pressed(int buttonId) {
  RadioEntry entry = radio_entries[top_entry_index + buttonId];
  if (scratchpad_index > 0) {
    // TODO: Set new value
  } else {
    // Swap st/by and act
    Serial.println(entry.swap_input);
  }
}

void r5_pressed(int buttonId) {

}

void scroll_up_button_pressed(int buttonId) {
  if (scratchpad_index > 0) {
    scratchpad_index--;
    should_render_upper_lcd = 1;
  }
}

void scroll_down_button_pressed(int buttonId) {
  if (top_entry_index < RADIO_ENTRY_COUNT - 4) {
    scratchpad_index++;
    should_render_upper_lcd = 1;
  }
}

void render_upper_display(LiquidCrystal lcd) {
  // Scroll Up Indicator
  lcd.setCursor(0, 0);
  if (top_entry_index > 0) {
    lcd.write(SCROLL_UP_GLYPH);
  } else {
    lcd.print(" ");
  }

  // Entry 0
  render_radio_entry(lcd, radio_entries[top_entry_index]);

  // Entry 1
  lcd.setCursor(1, 1);
  render_radio_entry(lcd, radio_entries[top_entry_index+1]);

  // Entry 2
  lcd.setCursor(1, 2);
  render_radio_entry(lcd, radio_entries[top_entry_index+2]);

  // Scroll Down Indicator
  lcd.setCursor(0, 3);
  if (top_entry_index < RADIO_ENTRY_COUNT - 4) {
    lcd.write(SCROLL_DOWN_GLYPH);
  } else {
    lcd.print(" ");
  }

  // Entry 3
  render_radio_entry(lcd, radio_entries[top_entry_index+3]);
}

void render_radio_entry(LiquidCrystal lcd, struct RadioEntry entry) {
  for (int i = 0; i < entry.rendering_size; i++) {
    lcd.write(entry.active[i]);
  }

  lcd.write(' ');
  lcd.print(entry.name);
  lcd.write(entry.call[0] == '1' ? RADIO_CALL_GLYPH : ' ');
  lcd.write(' ');

  if (scratchpad_index > 0) {
    lcd.write(PLACEHOLDER_GLYPH);
    lcd.write(PLACEHOLDER_GLYPH);
    lcd.write(PLACEHOLDER_GLYPH);
    lcd.write('.');
    lcd.write(PLACEHOLDER_GLYPH);
    lcd.write(PLACEHOLDER_GLYPH);
  } else {
    for (int i = 0; i < entry.rendering_size; i++) {
      lcd.write(entry.standby[i]);
    }
  }
}


void render_lower_display(LiquidCrystal lcd) {
  // Transponder
  lcd.setCursor(0, 0);
  lcd.print("TRANSPONDER     ");
  if (scratchpad_index > 0) {
    lcd.write(PLACEHOLDER_GLYPH);
    lcd.write(PLACEHOLDER_GLYPH);
    lcd.write(PLACEHOLDER_GLYPH);
    lcd.write(PLACEHOLDER_GLYPH);
  } else {
    lcd.print(transponder_code);
  }

  // Scratchpad
  lcd.setCursor(0,2);
  lcd.print("--------------------");
  lcd.setCursor(0, 3);
  for (int i = 0; i < SCRATCHPAD_SIZE; i++) {
    if (scratchpad[i] == '-') {
      break;
    } else {
      lcd.print(scratchpad[i]);
    }
  }
}
