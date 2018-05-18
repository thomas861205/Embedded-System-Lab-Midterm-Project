#include "bbcar.h"
extern parallax_servo *servo0_ptr, *servo1_ptr;
extern Ticker servo_ticker;
extern PwmOut buzzer;
Timeout buzz_out;

void bbcar_init( PwmOut &pin_servo0, PwmOut &pin_servo1 ){
    buzzer.period(.003);
    static parallax_servo servo0(pin_servo0);
    static parallax_servo servo1(pin_servo1);
    servo0_ptr = &servo0;
    servo1_ptr = &servo1;
    servo_ticker.attach(&servo_control, .5);
    servo0 = 0; servo1 = 0;
}

void ServoStop(){
    servo0_ptr->set_speed(0);
    servo1_ptr->set_speed(0);
    servo0_ptr->set_factor(1);
    servo1_ptr->set_factor(1);
    return;
}

void ServoCtrl( int speed ){
    servo0_ptr->set_speed(speed);
    servo1_ptr->set_speed(-speed);
    return;
}

void ServoTurn( int speed, double turn ){
    static int last_speed = 0;
    if(last_speed!=speed){
        servo0_ptr->set_speed(speed);
        servo1_ptr->set_speed(-speed);
    }
    if(turn>0){
        servo0_ptr->set_factor(turn);
        servo1_ptr->set_factor(1);
    }
    if(turn<0){
        servo0_ptr->set_factor(1);
        servo1_ptr->set_factor(-turn);
    }
    return;
}

void buzz_off(){ buzzer = 0; }
void Buzz(){
    buzzer = 0.5;
    buzz_out.attach(buzz_off,.5);
}

