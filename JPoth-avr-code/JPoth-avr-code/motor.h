/******************************************************************************
*Author: Kehnin Dyer, Tyler Martin, Nolan Check
*
*Function Prototypes for Driving the motor Properly
*
*
******************************************************************************/


#ifndef __MOTOR_H_
#define __MOTOR_H_

//init motor control pins
void motor_init();

//hax to delay a variable amount.
void delay_var_ms(int time);


//Steps the motor forward or backwards for a number of steps
//if dir > 0 forward else backwards
//TIME per step is TIME
void motor_step(char dir, int steps, int TIME);

#endif /* __MOTOR_H_ */