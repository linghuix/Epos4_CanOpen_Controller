/*
 * epos_bsp.h
 *
 *  Created on: Mar 2, 2020
 *      Author: test
 */

#ifndef _EPOS_BSP_H_
#define _EPOS_BSP_H_

#include <math.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdint.h>

// driver
#include "BSP.h"

// OS
#include "ucos_ii.h"

// Canopen
#include <dict.h>







// E_OBJ_DICTIONARY ?????,???define. 
// ????? EPOS2 Firmware Specification.pdf P110

//------------------------------------------------------------------------
//define variables  ??????
//------------------------------------------------------------------------

//typedef char                    int8;
//typedef int                     int16;
//typedef long                    int32;
typedef uint8_t                 Uint8;
typedef uint16_t                Uint16;
typedef uint32_t                Uint32;
typedef uint8_t                 BOOL;
typedef uint16_t                stcCAN;

#define NOT_USED 0

extern  Uint32  epos_err_LK;
extern  Uint32  epos_err_LH;
extern  Uint32  epos_err_RK;
extern  Uint32  epos_err_RH;

extern Uint32   buffer_size_LK;    
extern Uint32   buffer_size_LH;
extern Uint32   buffer_size_RK;
extern Uint32   buffer_size_RH;

extern Uint32   status_LK;          //0????,1?PPM

extern Uint32 pos_value_LK;
extern Uint32 pos_value_LH;
extern Uint32 pos_value_RK;
extern Uint32 pos_value_RH;

#define NODE_LK (1) /* epos 24/5???ID */
#define NODE_LH (2) /* epos 70/10???ID */
#define NODE_RK (3) /* epos 24/5???ID */
#define NODE_RH (4) /* epos 70/10???ID */

/************??*************/

#define PPMODE 0x01
#define MODE_NONE 0x00
#define ALL 0

#define MAX_ACC 30000
#define MAX_DEC 30000
#define MAX_F_ERR 40000	//500
#define MAX_P_V 2000		//2720//5000
#define QDEC MAX_ACC


enum PDO_Mapping
{
//	  Target_pos = 0x607A0020,					//RxPDO
//    Pos_offset = 0x60B00020,    //optional
//    Torque_offset = 0x60B20010, //optional for feed forward control.
//	  Target_torque = 0x60710010,

//	Torque_actual_value = 0x60770010,			//TxPDO
//    Velocity_actual_value = 0x606C0020,
//    Position_actual_value = 0x60640020,
	Current_actual_value = 0x30D10220,
};
	

enum Profile_Position_Mode
{
    Profile_Position_Mode = (uint8_t)(1),//??????

	/*CONFIGURE*/
    Soft_P_Limit_Min = 0x607D0120,              //????
	Soft_P_Limit_Max = 0x607D0220,              //????
	Max_Profile_Velocity = 0x607F0020,
	Max_motor_speed = 0x60800020,
	Max_gear_input_speed = 0x30030320,
	Q_deceleration = 0x60850020,
	Max_Acceleration = 0x60C50020,

	/*COMMAND*/
	Controlword = 0x60400010,                   //?????????
	Target_pos = 0x607A0020,
	Profile_Velocity = 0x60810020,              //?????????
	Profile_Acceleration = 0x60830020,          //????max Acceleration
	Profile_Deceleration = 0x60840020,          //min Deceleration
	Motion_Profile_Type = 0x60860010,
	/*OUT*/
	Pos_Demand_Value = 0x60620020,  //The output of the trajectory generator. 
	                                //It is used as input for the position control function.
	Statusword = 0x60410010,
};

enum Profile_Velocity_Mode
{
	Profile_Velocity_Mode  = 3,

	/*CONFIGURE*/
	/*
	Max_Profile_Velocity = 0x607F,
	Max_Acceleration = 0x60C5,
	Max_gear_input_speed
	Max_motor_speed
	Soft_P_Limit_Min = 0x607D0120,  //????
	Soft_P_Limit_Max = 0x607D0220,  //????
	Q_deceleration = ,
	*/

	/*COMMAND*/
	/*
	controlword
    Profile_Acceleration = 0x60830020,          //????max Acceleration
    Profile_Deceleration = 0x60840020,          //min Deceleration
    Motion_Profile_Type = 0x60860010,
	*/
    Target_Velocity = 0x60FF0020,          //???????

	/*OUT*/
	Velocity_Demand_Value = 0x606B0020,
	//Statusword
};


enum Homing_Mode
{

    Homing_Mode = (uint8_t)6,
    
    /*CONFIGURE*/
    /*Digital_input_properties
    Configuration_of_digital_inputs
    Digital_inputs
    //Motion_Profile_Type
	*/
    
    /*COMMAND*/
    //Controlword = 0x6040,
    Homing_Method = 0x6098,
    Homing_Speeds = 0x6099,
    Homing_Acceleration = 0x609A,
    Home_Offset = 0x607C,
    Current_Threshold = 0x2080,
    Home_Pos = 0x2081,

    /*OUT*/
    //Statusword = 0x6041,          //????
};

enum Cyclic_Synchronous_Position_Mode 
{
    Cyclic_Synchronous_Position_Mode = (uint8_t)8,
    
    /*CFG*/
    Nominal_current = 0x30010120,
    Motor_torque_constant = 0x30010520,
    Current_control_parameter_P = 0x30A00120,
    Current_control_parameter_I = 0x30A00220,
    Position_control_parameter_P = 0x30A10120,
    Position_control_parameter_I = 0x30A10220,
    Position_control_parameter_D = 0x30A10320,
    Position_control_parameter_FF_V = 0x30A10420,
    Position_control_parameter_FF_Acc = 0x30A10520,
    //Quick_stop_deceleraion
    //Profile deceleration
    Following_error_window = 0x60650020,
    //Software position limit
    Motor_rated_torque = 0x60760020,        //Nominal_current x Torque constant, write  directly is not allowed.
    //Max motor speed
    //Max gear input speed
    Interpolation_Time_Period_value = 0x60C20108,//time between two PDOs
	Interpolation_Time_index = 0x60C20208,//unit -3??ms

    /*COMMAND*/
    //Target_pos = 0x607A0020,
    Pos_offset = 0x60B00020,    //optional
    Torque_offset = 0x60B20010, //optional for feed forward control.
    
    /*OUT*/
    Torque_actual_value = 0x60770010,
    Velocity_actual_value = 0x606C0020,
    Position_actual_value = 0x60640020,
    //Statusword
};

enum Cyclic_Synchronous_Velocity_Mode
{
    Cyclic_Synchronous_Velocity_Mode = (uint8_t)9,
    
    /*CFG*/
    //Nominal_current 
    //Motor_torque_constant
    //Current_control_parameter_set
    Velocity_control_parameter_P = 0x30A20120,
    Velocity_control_parameter_I = 0x30A20220,
    Velocity_control_parameter_FF_V = 0x30A20320,
    Velocity_control_parameter_FF_Acc = 0x30A20420,
    //Quick_stop_deceleration
    //Profile_deceleration
    //Software_position_limit
    //Motor_rated_torque
    //Max_motor_speed
    //Max_gear_input_speed
    //Interpolation_Time_Period_value = 0x60C20108,
	//Interpolation_Time_index = 0x60C20208,s

    /*COMMAND*/
    V_offset = 0x60B10020,
    Target_v = 0x60FF0020,
    
    /*OUT*/
    /*
    Torque_actual_value = 0x60770010,
    Velocity_actual_value = 0x606C0020,
    Position_actual_value = 0x60640020,
    Statusword
    */
};

enum Cyclic_Synchronous_Torque_Mode 
{
    Cyclic_Synchronous_Torque_Mode  = 10,

	/*CONFIGURE*/
    //Nominal_current 
    //Motor_torque_constant
    //Max_motor_speed
    //Max_gear_input_speed
    //Current_control_parameter_set
    //Quick_stop_deceleration
    //Profile_deceleration
    //Motor_rated_torque
    //Software_position_limit
    //Interpolation_Time_Period_value = 0x60C20108
	//Interpolation_Time_index = 0x60C20208

	/*COMMAND*/
    //Torque_offset
	Target_torque = 0x60710010,

	/*OUT*/
    /*
    Torque_actual_value = 0x60770010,
    Velocity_actual_value = 0x606C0020,
    Position_actual_value = 0x60640020,
    Statusword
    */
};


//??  index+interger
enum E_OBJ_MODE
{
    OP_MODE = 0x60600008,           //Modes of operation, Sub-index 0x00, INTEGER8
	OP_MODE_Read = 0x60610008,      //Modes of operation display
};


enum Epos4_StateControlCommand
{
    Fault_Reset = 0x80,   //reference to 2.2.3 Device Control Commands
	Switch_on_Enable_operation = 0x0F,
	Shutdown = 0x06,
	Disable_voltage = 0x0,
};


enum E_OBJ_DICTIONARY
{
    OD_ERR_REG = 0x10010008,    //error register, Index 0x1001, Sub-index 0x00, UNSIGNED8

    /* ERROR */
    OD_ERR_COUNT = 0x10030020,  //number of errors
    OD_ERR_HIS1 = 0x10030120, //error history [1],Index 0x1003,Sub-index 0x01, UNSIGNED32
    OD_ERR_HIS2 = 0x10030220, //error history [2]
    OD_ERR_HIS3 = 0x10030320,
    OD_ERR_HIS4 = 0x10030420,
    OD_ERR_HIS5 = 0x10030520,

    OD_STORE = 0x10100120,      //Save all Parameters. SAVE_Code = 0x73617665,

    OD_CAN_BITRATE = 0x20010008,
};


#define EPOS_DELAY1  5          //5ms
#define EPOS_DELAY2  50
#define EPOS_DELAY3  62.5


//EPOS?????
typedef struct __Epos
{
    Uint8 node_ID;          //CANOPEN????EPOS????ID
    Uint8 cur_mode;         //EPOS?????
    
    Uint32 max_velocity;
    Uint32 error_window;
    Uint32 opt;             //EPOS????PPMODE??opt??
    Uint32 acc;             //EPOS????
    Uint32 dec;             //EPOS?????
    
}Epos;


#endif /* EPOS_EPOS_BSP_H_ */
