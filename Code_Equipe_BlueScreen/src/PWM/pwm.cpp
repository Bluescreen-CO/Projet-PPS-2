#include "PWM/pwm.h"


//Création de la classe PWM
PWM::PWM(int ledPin, int frequency, int channel, int resolution) {
    pwmLedPin = ledPin;
    pwmFrequency = frequency;
    pwmChannel = channel;
    pwmResolution = resolution;

    //Innitialiser le constructeur
    ledcSetup(pwmChannel, pwmFrequency, pwmResolution);
    ledcAttachPin(pwmLedPin, pwmChannel);
}

//Permet de clear au besoin
PWM::~PWM() {
    ledcDetachPin(pwmLedPin);
}

//Fonction qui permet d'appliquer la Valeur PWM sur la broche sélectionner (ledPin)
void PWM::applyValue(int dutyCycle) {
    ledcWrite(pwmChannel, dutyCycle);
}

//Fonction qui permet de lire la valeur PWM sur la boche sélectionner (lePin)
int PWM::getValue() {
    return ledcRead(pwmChannel);
}

// *Trouver en ligne, potentiellement mauvais* permet de retourner la valeur maximal du duty cycle
int PWM::getMaxDutyCycle() {
 return (1 << pwmResolution) - 1;
}

// Fonction qui permet de appliquer ledWrite sur plusieurs LED. Il faut identifié les LED comme led[0], led[1]... 
void PWM::attachLeds(int led[], int nb){
    
    for (int i = 0; i<=nb; i=i+1 )
        ledcWrite(pwmChannel, led[i]);
}


