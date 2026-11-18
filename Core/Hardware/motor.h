#ifndef __MOTOR_H
#define __MOTOR_H
#include "cmd.h"

void Motor_StateMachine(uint8_t motor_is_hold);
void Motor_SetStop();
void Motor_SetForward();
void Motor_SetBack();
void Motor_SetLeft();
void Motor_SetRight();
void Motor_Set_F_B_Speed();
SPEED Motor_GetSpeed();

#endif