#ifndef BBCAR_H
#define BBCAR_H
#include "parallax_servo.h"

void bbcar_init( PwmOut &pin_servo0, PwmOut &pin_servo1 );
void ServoStop();
void ServoCtrl( int speed );
void ServoTurn( int speed, double turn );
void Buzz();

#endif
