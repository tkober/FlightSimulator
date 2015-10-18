#include "RotaryEncoder.h"



PushableRotaryEncoder::PushableRotaryEncoder(int rotaryPin1, int rotaryPin2, int buttonPin, int pullUp) {
  _rotaryPin1 = rotaryPin1;
  _rotaryPin2 = rotaryPin2;
  _buttonPin = buttonPin;
  _pullUp = pullUp;
  _button = Button(buttonPin, pullUp);
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

    
void PushableRotaryEncoder::tick() {
  _button.tick();
}

