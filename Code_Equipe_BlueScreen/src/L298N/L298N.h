#ifndef MotorControl_h
#define MotorControl_h

#include "Arduino.h"

//in1, in2 roue gauche et in3, in4 roue droite

class MotorControl {
public:
  MotorControl(int in1, int in2, int int3, int in4, int ena);

  void forward_centre(int speed);
  void forward_gauche(int speed);
  void forward_droit(int speed);

  void backward_centre(int speed);
  void backward_gauche(int speed);
  void backward_droit(int speed);
  
  void stop();

private:
  int _in1;
  int _in2;
  int _in3;
  int _in4;
  int _ena;
};

#endif