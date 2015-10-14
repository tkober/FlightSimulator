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


typedef struct ECAMPage {
  MFD_PAGE_MEMBERS
  void (*scroll_down)();
  void (*scroll_up)();
  void (*handle_keypad_input)(char key);
} ECAMPage;

