

typedef struct {
  MFDPage previous_page;
  MFDPage next_page;
  
  void (*will_appear)();
  void (*render)();
  void (*update_if_necessary)();
  void (*scroll_down)();
  void (*scroll_up)()
  void (*will_disappear)();
} MFDPage;
