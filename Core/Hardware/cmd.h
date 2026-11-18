#ifndef __CMD_H__
#define __CMD_H__

//设置速度的枚举命令
typedef enum{
	SPEED1, //低速
	SPEED2, //中速
	SPEED3  //高速
}SPEED;

//设置方向的枚举命令
typedef enum{
	STOP,
	FORWARD,
	BACK,
	LEFT,
	RIGHT
}DIR;

extern volatile SPEED Cmd_Speed;
extern volatile DIR Cmd_Dir;

#endif