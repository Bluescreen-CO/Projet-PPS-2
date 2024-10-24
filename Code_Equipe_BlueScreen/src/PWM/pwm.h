#ifndef PWM_h
#define PWM_h
#include "Arduino.h"

class PWM{
    private:
    int pwmLedPin;
    int pwmFrequency;
    int pwmChannel;
    int pwmResolution;

    public:
    PWM(int ledPin,int frequency, int channel, int resolution);
    ~PWM();
    void applyValue(int dutyCycle);
    int getValue();
    int getMaxDutyCycle();
    void attachLeds(int led[], int nb);
    //void autreLed(int nbLed, int tab[]);
};


#endif