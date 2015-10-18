#ifndef MFD_H
#define MFD_H


#define MFD_PAGE_MEMBERS  void *previous_page;                  \
                          void *next_page;                      \
                                                                \
                          void (*will_appear)();                \
                          void (*render)();                     \
                          void (*update_if_necessary)();        \
                          void (*will_disappear)();             


typedef struct MFDPage {
  MFD_PAGE_MEMBERS
} MFDPage;


typedef struct RadioPage {
  MFD_PAGE_MEMBERS
  void (*change_button_pressed)();
  void (*call)();
  void (*swap)();
  void (*increment)();
  void (*decrement)();
} RadioPage;
#define RADIO_PAGE_MAKE(_previous_page, _next_page, _will_appear, _render, _update_if_necessary, _will_disappear, _change_button_pressed, _call, _swap, _increment, _decrement) {_previous_page, _next_page, _will_appear, _render, _update_if_necessary, _will_disappear, _change_button_pressed, _call, _swap, _increment, _decrement}


typedef struct ECAMPage {
  MFD_PAGE_MEMBERS
  void (*scroll_down)();
  void (*scroll_up)();
  void (*handle_keypad_input)(char key);
} ECAMPage;


extern void print_string_to_lcd(LiquidCrystal lcd, char string[], int size);

#endif
