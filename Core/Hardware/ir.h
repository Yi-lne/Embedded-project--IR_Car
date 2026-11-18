#ifndef __IR_H__
#define __IR_H__
#include "stm32f1xx_hal.h"

typedef enum {
	IR_IDLE,				//空闲状态，检测9ms低电平时间
	IR_START_HIGH,	//检测4.5ms或2.25ms高电平时间
	IR_RECEIVE_DATA,//接收32bit数据
	IR_REAPT_LOW		//检测重复码的560us数据
}IR_State;

void IR_StateMachine(uint16_t t_dur);
void IR_Set_is_hold();
uint8_t IR_GetIsHold();
const uint8_t* IR_GetDir();
uint16_t IR_GetDur();
uint8_t IR_GetData();

#endif