#include "Button.h"


#define DEFAULT_CLICK_TICKS     100
#define DEFAULT_REPEAT_INTERVAL 1000


Button::Button(int pin, int pullUp) {
  _pin = pin;
  _clickTicks = DEFAULT_CLICK_TICKS;
  _isPullUp = pullUp;
  _repeat = 0;
  _repeatInterval = DEFAULT_REPEAT_INTERVAL;
  pinMode(pin, pullUp == 1 ? INPUT_PULLUP : INPUT);
}


void Button::setOnClick(ButtonEventHandler handler)
{
  _onClick = handler;
}


void Button::setClickTicks(int clickTicks)
{
  _clickTicks = clickTicks;
}


void Button::setRepeatOnHold(int repeat)
{
  _repeat = repeat;
}


void Button::setRepeatInterval(int repeatInterval)
{
  _repeatInterval = repeatInterval;
}


void Button::setId(int id)
{
  _id = id;
}


int Button::getId()
{
  return _id;
}


void Button::tick()
{
  unsigned long now = millis();
  int triggeringState = _isPullUp == 1 ? LOW : HIGH;
  if (digitalRead(_pin) == triggeringState) {
    switch (_state) {
      case Normal:
        _startedTime = now;
        _state = ClickStarted;
        break;

      case ClickStarted:
        if (now - _startedTime >= _clickTicks) {
          if (_repeat) {
            _state = Repeat;
            _lastRepeat = millis();
          } else {
            _state = Clicked;
          }
          if (_onClick != NULL) {
            _onClick(_id);
          }
        }
        break;

      case Clicked:
        break;

      case Repeat:
        if (now - _lastRepeat >= _repeatInterval) {
          _lastRepeat = millis();
          if (_onClick != NULL) {
            _onClick(_id);
          }
        }
        break;
     }
  } else {
   _state =  Normal;
  }
}
