#ifndef ROTARY_ENCODER_H
#define ROTARY_ENCODER_H


typedef void (*RotaryEncoderEventHandler)();


class PushableRotaryEncoder {
  
  public:
    PushableRotaryEncoder(int rotaryPin1, int rotaryPin2, int buttonPin, int pullUp);

    void setOnRotateClockwise(RotaryEncoderEventHandler handler);
    void setOnRotateCounterClockwise(RotaryEncoderEventHandler handler);
    void setOnClick(ButtonEventHandler handler);
    void setClickTicks(int clickTicks);
    
    void tick();
  
  private:
    int _rotaryPin1;
    int _rotaryPin2;
    int _buttonPin;
    int _pullUp;
    Button _button = Button(_buttonPin, _pullUp);

    RotaryEncoderEventHandler _onRotateClockwise;
    RotaryEncoderEventHandler _onRotateCounterClockwise;
};

#endif
