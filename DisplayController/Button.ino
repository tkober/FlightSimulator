#include "Button.h"


#define DEFAULT_CLICK_TICKS  600


Button::Button(int pin, int pullUp) {
  _pin = pin;
  _clickTicks = DEFAULT_CLICK_TICKS;
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


void Button::tick()
{
  int now = millis();
  
  if (digitalRead(_pin) == HIGH) {
    switch (_state) {
      case Normal:
        _startedTime = now;
        _state = ClickStarted;
        break;
    
      case ClickStarted:
        if (_startedTime - now >= _clickTicks) {
          _state = Clicked;
          if (_onClick) {
            _onClick();
          }
        }
        break;
    
      case Clicked:
        break;
     } 
  } else {
   _state =  Normal;
  }
}
