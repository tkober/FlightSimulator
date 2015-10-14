struct MFDPage {
  struct MFDPage *previous_page;
  struct MFDPage *next_page;
    
  void (*will_appear)();
  void (*render)();
  void (*update_if_necessary)();
  void (*scroll_down)();
  void (*scroll_up)();
  void (*will_disappear)();
};
typedef struct MFDPage MFDPage;
