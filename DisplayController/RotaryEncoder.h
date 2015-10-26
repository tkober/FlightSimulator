#ifndef ROTARY_ENCODER_H
#define ROTARY_ENCODER_H


typedef void (*RotaryEncoderEventHandler)(int boost);


class PushableRotaryEncoder {
  
  public:
    PushableRotaryEncoder(int rotaryPinA, int rotaryPinB, int buttonPin, int pullUp);

    void setOnRotateClockwise(RotaryEncoderEventHandler handler);
    void setOnRotateCounterClockwise(RotaryEncoderEventHandler handler);
    void setOnClick(ButtonEventHandler handler);
    void setClickTicks(int clickTicks);
    void setDebounceFilter(int debounceFilter);
    void setBoostActivationCount(int boostActivationCount);
    void setBoostActivationInterval(int boostActivationInterval);
    
    void tick();
  
  private:
    int _rotaryPinA;
    int _rotaryPinB;
    int _buttonPin;
    int _pullUp;
    Button _button = Button(_buttonPin, _pullUp);

    int _oldState;
    int _lastTime;
    int _boostCount;
    
    int _debounceFilter;
    int _boostActivationCount;
    int _boostActivationInterval;

    RotaryEncoderEventHandler _onRotateClockwise;
    RotaryEncoderEventHandler _onRotateCounterClockwise;
};

#endif
