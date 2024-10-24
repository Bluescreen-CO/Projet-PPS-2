#ifndef led_h
#define led_h
#include "Arduino.h"
class Led
{
private:
    int broche;
    float valeur;

public:
    Led(int broche);
    ~Led();
    void allumer();
    void Eteindre();

};

#endif