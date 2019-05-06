#ifndef EVENT_HANDLER_H
#define EVENT_HANDLER_H

// constants won't change. They're used here to set pin numbers:
const int buttonPin = 6;     // the number of the pushbutton pin


class EventHandler {
  public:
    EventHandler(Workout &workout) : mWorkout(workout) {}

    void setup() {
      // initialize the pushbutton pin as an input:
      pinMode(buttonPin, INPUT);
    }


    void loop() {
      // read the state of the pushbutton value:
      int currentState = digitalRead(buttonPin);

      // if it has changed, it's time to change the mode
      long currentTime = millis();
      if ( currentState == HIGH ) {
        mWorkout.setNextMode();
      }

    }

  private:
    Workout &mWorkout;

};

#endif
