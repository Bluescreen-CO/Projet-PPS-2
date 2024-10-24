#include "Arduino.h"
#include "led.h"



Led::Led(int pin)
{
    broche = pin;
    pinMode(broche, OUTPUT);

}

void Led::allumer()
{
    digitalWrite(broche, 1);
}

void Led::Eteindre()
{
    digitalWrite(broche, 0);
}


Led::~Led()
{

}