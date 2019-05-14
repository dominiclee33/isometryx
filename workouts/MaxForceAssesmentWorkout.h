#ifndef MAXFORCE_WORKOUT_H
#define MAXFORCE_WORKOUT_H


const unsigned int MAX_FORCE_NUM_REPS = 8;
const unsigned int MAX_FORCE_NUM_SETS = 1;
const unsigned long MAX_FORCE_REP_HANGING_DURATION_MS = 10000;
const unsigned long MAX_FORCE_REST_DURATION_MS = 120000;
const float MAX_FORCE_MAX_PRECENT_LIMIT = 80.0f;

/**
 * This workout is used for max force assessment
 * 
 * this is the best of 8 reps
 */
class MaxForceAssessmentWorkout : public Workout {
  public:
    MaxForceAssessmentWorkout() : Workout( ONE_HAND,
                                ASSESSMENT_MAXFORCE,
                                MAX_FORCE_NUM_REPS, 
                                MAX_FORCE_NUM_SETS, 
                                MAX_FORCE_REP_HANGING_DURATION_MS, 
                                MAX_FORCE_REST_DURATION_MS,
                                MAX_FORCE_MAX_PRECENT_LIMIT) {}

  protected:

    
    void onRepFinish(){
      
      // update maxForce for left and right hands
      if( mHangingMode == TWO_HANDS){
        if( mMaxForceLeft + mMaxForceRight < mCurrentRepMaxForce){
          mMaxForceLeft = mCurrentRepMaxForce / 2.0f;
          mMaxForceRight = mMaxForceLeft;
        }
      }else{
        if( mCurrentHand == LEFT && mMaxForceLeft < mCurrentRepMaxForce){
          mMaxForceLeft = mCurrentRepMaxForce;
        }
        if( mCurrentHand == RIGHT && mMaxForceRight < mCurrentRepMaxForce){
          mMaxForceRight = mCurrentRepMaxForce;
        }
      }

      if (climber.getMaxForceL() < mMaxForceLeft ) {
        Serial.print(F("New personnal best for left hand : "));
        Serial.println(mMaxForceLeft);
        climber.setMaxForceL(mMaxForceLeft);
      }

      if (climber.getMaxForceR() < mMaxForceRight) {
        Serial.print(F("New personnal best for right hand : "));
        Serial.println(mMaxForceRight);
        climber.setMaxForceR(mMaxForceRight);
      }
    }

    void render() {

      display.clearDisplay();
      display.setTextSize(2);
      display.setTextColor(WHITE);
      display.setCursor(0, 0);

      switch (getState()) {

        case IDLE : 
          display.println(F("Max Force"));
          display.print(getNumReps());
          display.println(F(" reps"));
          display.print(valueToString(getClimber().getMaxForceL(), 1));
          display.print(F(" "));
          display.println(valueToString(getClimber().getMaxForceR(), 1));
          break;

        case HANGING :
          if(mHangingMode == ONE_HAND){
            if(mCurrentHand == LEFT){
              display.print(F("L:"));
              display.print(valueToString(mMaxForceLeft, 1));
              display.print(F(" "));
              display.print(valueToString(getClimber().getMaxForceL(), 1));
            }else{
              display.print(F("R:"));
              display.print(valueToString(mMaxForceRight, 1));
              display.print(F(" "));
              display.println(valueToString(getClimber().getMaxForceR(), 1));
            }
          }else{
            display.print(F("B:"));
            display.print(valueToString(mMaxForceLeft+mMaxForceRight, 1));
            display.print(F(" "));
            display.println(valueToString(getClimber().getMaxForceL()+ getClimber().getMaxForceR(), 1));
          }
          
          display.print(valueToString(getStats().last(), 1));
          display.print(F(" "));
          display.println(valueToString(getStats().last(), 1));

          if (percentMax() > 2) {
            display.print(valueToString(percentMax(), 0));
            display.print(F("% "));
          } else {
            display.print(F("  0% "));
          }
          display.print(valueToString(timeOverLimit(), 0));
          display.println(F("s"));

          display.print(valueToString(getRepsCount(), 0));
          display.print(F("/"));
          display.println(valueToString(getNumReps(), 0));
          break;
        
        case RESTING :
          display.println(F("Rest (s)"));

          display.print(valueToString(restingTime(), 0));
          display.print(F("/"));
          display.println(valueToString(restDurationMs(), 0));
                    display.print(valueToString(mMaxForceLeft, 1));
          display.print(F(" "));
          display.println(valueToString(mMaxForceRight, 1));
          break;
          
        case DONE :
          display.println(F("Done !"));
          display.print(valueToString(mMaxForceLeft, 1));
          display.print(F(" "));
          display.println(valueToString(mMaxForceRight, 1));
          break;

        case SWITCHING_HANDS :
          display.println(F("time to \nswitch"));
          break;

        default:
          display.println(F("..."));
          break;
      }


      display.display();
    }

    void onRepStart(){}
    void onRepAbort(){}
    void onRestStart(){}
    void onRestFinish(){}
    void onSetFinish(){}
    void onWorkoutFinish(){}

  private:
    float mMaxForceLeft = 0.0f;
    float mMaxForceRight = 0.0f;

};

#endif