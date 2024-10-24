#include "Arduino.h"
#include "L298N/L298N.h"


//Fonction prend 3 variables en entrés in 1 et 2 pour la dirrection et ena pour le PMW
MotorControl::MotorControl(int in1, int in2, int in3, int in4, int ena) {
  _in1 = in1;
  _in2 = in2;
  _in3 = in3;
  _in4 = in4;

  _ena = ena;


  pinMode(_in1, OUTPUT);
  pinMode(_in2, OUTPUT);
  pinMode(_in3, OUTPUT);
  pinMode(_in4, OUTPUT);
  pinMode(_ena, OUTPUT);
}


//HIGH/LOW = avant 
//LOW/HIGH = arrière
//LOW/LOW = 



//3 fonctions pour avancer 
void MotorControl::forward_centre(int speed) {
  digitalWrite(_in1, HIGH);
  digitalWrite(_in2, LOW);
  digitalWrite(_in3, HIGH);
  digitalWrite(_in4, LOW);
  analogWrite(_ena, speed);
}

void MotorControl::forward_gauche(int speed) {
  digitalWrite(_in1, LOW);
  digitalWrite(_in2, HIGH);
  digitalWrite(_in3, HIGH);
  digitalWrite(_in4, LOW);
  analogWrite(_ena, speed);
}

void MotorControl::forward_droit(int speed) {
  digitalWrite(_in1, HIGH);
  digitalWrite(_in2, LOW);
  digitalWrite(_in3, LOW);
  digitalWrite(_in4, HIGH);
  analogWrite(_ena, speed);
}


//3 fonctions pour reculer 
void MotorControl::backward_centre(int speed) {
  digitalWrite(_in1, LOW);
  digitalWrite(_in2, HIGH);
  digitalWrite(_in3, LOW);
  digitalWrite(_in4, HIGH);
  analogWrite(_ena, speed);
}

void MotorControl::backward_gauche(int speed) {
  digitalWrite(_in1, HIGH);
  digitalWrite(_in2, LOW);
  digitalWrite(_in3, LOW);
  digitalWrite(_in4, HIGH);
  analogWrite(_ena, speed);
}

void MotorControl::backward_droit(int speed) {
  digitalWrite(_in1, LOW);
  digitalWrite(_in2, HIGH);
  digitalWrite(_in3, HIGH);
  digitalWrite(_in4, LOW);
  analogWrite(_ena, speed);
}

void MotorControl::stop() {
  digitalWrite(_in1, LOW);
  digitalWrite(_in2, LOW);
  digitalWrite(_in3, LOW);
  digitalWrite(_in4, LOW);
  analogWrite(_ena, 0);
}