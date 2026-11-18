#include "stm32f1xx_hal.h"                  // Device header
#include "cmd.h"
#include "motor.h"
#include "tim.h"

static volatile uint16_t left_speed = 0, right_speed = 0;

//当方向为前进和后退时设置速度
void Motor_Set_F_B_Speed()
{
	switch(Cmd_Speed){
		case SPEED1:
			left_speed = 600;
			right_speed = 600;
			break;
		
		case SPEED2:
			left_speed = 700;
			right_speed = 700;
			break;
		
		case SPEED3:
			left_speed = 800;
			right_speed = 800;
			break;
	}
}

void Motor_StateMachine(uint8_t motor_is_hold)
{
	if(motor_is_hold == 0){
		Motor_SetStop();
		return;
	}
	switch(Cmd_Dir){
		case STOP:
			Motor_SetStop();
			break;
		
		case FORWARD:
			Motor_SetForward();
			break;
		
		case BACK:
			Motor_SetBack();
			break;
		
		case LEFT:
			Motor_SetLeft();
			break;
		
		case RIGHT:
			Motor_SetRight();
			break;
	}
}

void Motor_SetStop()
{
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_SET);
}

void Motor_SetForward()
{
	Motor_Set_F_B_Speed();
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_RESET);
	
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, left_speed);
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, right_speed+100);
}

void Motor_SetBack()
{
	Motor_Set_F_B_Speed();
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_SET);
	
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, left_speed);
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, right_speed);
}

void Motor_SetLeft()
{
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_RESET);
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 450);
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, 950);
}

void Motor_SetRight()
{
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_11, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_RESET);
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, 950);
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_2, 450);
}

SPEED Motor_GetSpeed()
{
	return Cmd_Speed;
}