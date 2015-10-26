#include "RotaryEncoder.h"


#define LATCHSTATE                          3
#define DEFAULT_DEBOUNCE_FILTER             50
#define DEFAULT_BOOST_ACTIVATION_COUNT      15
#define DEFAULT_BOOST_ACTIVATION_INTERVAL   500


const int8_t KNOBDIR[] = {
  0, -1,  1,  0,
  1,  0,  0, -1,
  -1,  0,  0,  1,
  0,  1, -1,  0  
};


PushableRotaryEncoder::PushableRotaryEncoder(int rotaryPinA, int rotaryPinB, int buttonPin, int pullUp) {
  _rotaryPinA = rotaryPinA;
  _rotaryPinB = rotaryPinB;
  _buttonPin = buttonPin;
  _pullUp = pullUp;
  _button = Button(buttonPin, pullUp);
  pinMode(rotaryPinA, pullUp == 1 ? INPUT_PULLUP : INPUT);
  pinMode(rotaryPinB, pullUp == 1 ? INPUT_PULLUP : INPUT);
  
  _oldState = 3;
  _lastTime = millis();
  _debounceFilter = DEFAULT_DEBOUNCE_FILTER;
  _boostCount = 0;
  _boostActivationCount = DEFAULT_BOOST_ACTIVATION_COUNT;
  _boostActivationInterval = DEFAULT_BOOST_ACTIVATION_INTERVAL;
}


void PushableRotaryEncoder::setOnRotateClockwise(RotaryEncoderEventHandler handler) {
  _onRotateClockwise = handler;
}


void PushableRotaryEncoder::setOnRotateCounterClockwise(RotaryEncoderEventHandler handler) {
  _onRotateCounterClockwise = handler;
}


void PushableRotaryEncoder::setOnClick(ButtonEventHandler handler) {
  _button.setOnClick(handler);
}


void PushableRotaryEncoder::setClickTicks(int clickTicks) {
  _button.setClickTicks(clickTicks);
}


void PushableRotaryEncoder::setDebounceFilter(int debounceFilter) {
  _debounceFilter = debounceFilter;
}


void PushableRotaryEncoder::setBoostActivationCount(int boostActivationCount) {
  _boostActivationCount = boostActivationCount;
}


void PushableRotaryEncoder::setBoostActivationInterval(int boostActivationInterval) {
  _boostActivationInterval = boostActivationInterval;
}

    
void PushableRotaryEncoder::tick() {
  _button.tick();

  int sigA = digitalRead(_rotaryPinA);
  int sigB = digitalRead(_rotaryPinB);
  int8_t thisState = sigA | (sigB << 1);

  if (_oldState != thisState) {
    int direction = KNOBDIR[thisState | (_oldState<<2)];
    
    if (thisState == LATCHSTATE) {
      int now = millis();
      if (now - _lastTime >= _debounceFilter) {
        if (now - _lastTime <= _boostActivationInterval) {
          _boostCount++;
        } else {
          _boostCount = 0;
        }
        _lastTime = now;
        if (direction > 0) {
          if (_onRotateClockwise) {
            _onRotateClockwise(_boostCount >= _boostActivationCount);
          }
        } else {
          if (_onRotateCounterClockwise) {
            _onRotateCounterClockwise(_boostCount >= _boostActivationCount);
          }
        }
      }
    }
    
    _oldState = thisState;
  } 
}

