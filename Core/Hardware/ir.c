#include "ir.h"
#include "stm32f1xx_hal.h"                  // Device header
#include "tim.h"
#include "cmd.h"

static volatile IR_State current_state = IR_IDLE; //设置当前状态
static volatile uint8_t is_hold = 0; //判断是否为重复码
static volatile uint8_t is_ready_receive = 0; //判断接收到的逻辑0/1脉冲是否为第二个脉冲、
static volatile uint8_t data_count = 0; //记录接收的数据位数
static volatile uint32_t IR_Data = 0; //记录红外32位数据
static volatile uint32_t Last_RepeatTime = 0; //记录上一次接收到重复码的时间
//addr：地址码，addr_anti：地址反码，data：数据码，data_anti：数据反码
static volatile uint8_t addr = 0, addr_anti = 0, data = 0, data_anti = 0;
volatile SPEED Cmd_Speed = SPEED1;
volatile DIR Cmd_Dir = STOP;
static volatile uint16_t IR_Dur; //记录时间差

void IR_StateMachine(uint16_t t_dur)
{
	
	switch(current_state){
		case IR_IDLE:
			if(t_dur>8500 && t_dur<9500){
				current_state = IR_START_HIGH;
			}
			break;
		
		case IR_START_HIGH:
			//判断 is_hold == 0 是为了防止重复码接收过程中误入接收数据的状态
			if(t_dur>3800 && t_dur<4800 && is_hold == 0){
				current_state = IR_RECEIVE_DATA;
			}else if(t_dur>2050 && t_dur<2600){
				current_state = IR_REAPT_LOW;
			}else{
				current_state = IR_IDLE;
			}
			break;
		
		case IR_RECEIVE_DATA:
			if(is_ready_receive == 0){ //检测数据逻辑0/1的首个脉冲信号时长
				if(t_dur>500 && t_dur<650){
					is_ready_receive = 1;
				}else{
					//错误复位
					current_state = IR_IDLE;
					data_count = 0;
					IR_Data = 0;
					is_ready_receive = 0;
				}
			}else{
				IR_Dur = t_dur;
				if(t_dur>500 && t_dur<650){//检测数据逻辑0的第二个脉冲信号时长
					data_count++;
					IR_Data = IR_Data<<1;
				}else if(t_dur>1500 && t_dur<1800){//检测数据逻辑1的第二个脉冲信号时长
					data_count++;
					IR_Data = (IR_Data<<1) | 0x01;
				}else{
					//错误复位
					current_state = IR_IDLE;
					data_count = 0;
					IR_Data = 0;
					is_ready_receive = 0;
				}
				is_ready_receive = 0;
				
				if(data_count >= 32){
					addr = IR_Data>>24&0xFF;
					addr_anti = IR_Data>>16&0xFF;
					data = IR_Data>>8&0xFF;				
					data_anti = IR_Data&0xFF;
					
					//校验数据并设置命令
					if(addr == 0x00 && addr+addr_anti == 0xFF && data+data_anti == 0xFF){
						switch(data){
							//设置速度1命令（低速）
							case 0xA2: Cmd_Speed = SPEED1; break;
							
							//设置速度2命令（中速）
							case 0x62: Cmd_Speed = SPEED2; break;
							
							//设置速度3命令（高速）
							case 0xE2: Cmd_Speed = SPEED3; break;
							
							//设置前进命令
							case 0x18: Cmd_Dir = FORWARD; break;
							
							//设置后退命令
							case 0x4A: Cmd_Dir = BACK; break;
							
							//设置左转命令
							case 0x10: Cmd_Dir = LEFT; break;
							
							//设置右转命令
							case 0x5A: Cmd_Dir = RIGHT; break;
							default: Cmd_Dir = STOP;
						}
					}
					current_state = IR_IDLE;
					data_count = 0;
					IR_Data = 0;
					is_ready_receive = 0;
				}
			}
			break;
		
		case IR_REAPT_LOW:
			if(t_dur>500 && t_dur<650){
				Last_RepeatTime = HAL_GetTick();
				current_state = IR_IDLE;
				is_hold = 1;
			}
			break;
		
		default:
			current_state = IR_IDLE;
	}
}

void IR_Set_is_hold()
{
	if(HAL_GetTick() - Last_RepeatTime > 300){
		is_hold = 0;
	}
	if(data == 0xA2 || data == 0x62 || data == 0xE2){
		is_hold = 0;
	}
}

uint8_t IR_GetIsHold()
{
	return is_hold;
}

uint16_t IR_GetDur()
{
	return IR_Dur;
}

uint8_t IR_GetData()
{
	return data;
}
	
const uint8_t* IR_GetDir()
{
	switch(Cmd_Dir){
		case FORWARD: return "FORWARD";
		case BACK: return "BACK   ";
		case LEFT: return "LEFT   ";
		case RIGHT: return "RIGHT  ";
		default: return "STOP   ";
	}
}