#ifndef BUTTON_H
#define BUTTON_H

typedef void (*ButtonEventHandler)(int buttonId);


typedef enum {
  Normal  = 0,
  ClickStarted,
  Clicked,
  Repeat
} ButtonState;


class Button {

  public:
    Button(int pin, int pullUp);

    void setOnClick(ButtonEventHandler handler);
    void setClickTicks(int clickTicks);

    void setRepeatOnHold(int repeat);
    void setRepeatInterval(int repeatInterval);

    void setId(int id);
    int getId();

    void tick();

  private:
    int _pin;
    int _isPullUp;
    int _id;
    ButtonState _state;

    int _clickTicks;
    int _repeat;
    int _repeatInterval;

    ButtonEventHandler _onClick;

    unsigned long _startedTime;
    unsigned long _lastRepeat;
};

#endif
