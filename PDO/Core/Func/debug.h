
#ifndef __debug_H
#define __debug_H


#include "conf_usart.h"


#define BufferSize 5000
#define PORT huart1


/* Definition of error and warning macros */
/* -------------------------------------- */
#define MYMSG(...)				printf (__VA_ARGS__)//;printf("--%s,%d\r\n",__FILE__, __LINE__)
#define MSG_deviceTest(...) 	MYMSG(__VA_ARGS__)
#define Task_MSG(...) 			MYMSG(__VA_ARGS__)
#define MMSG(...) 				MYMSG(__VA_ARGS__)
#define CAN_SEND_MSG(...)		//MYMSG(__VA_ARGS__)
#define CAN_RCV_MSG(...)		//MYMSG(__VA_ARGS__)
#define TPDO_MSG(...)			//printf("TPDO: ");MYMSG(__VA_ARGS__)
#define SYNC_MSG(...)			//printf("SYNC: ");MYMSG(__VA_ARGS__)
#define ROW_MSG(...)			MYMSG(__VA_ARGS__)					//需要发送到PC等地方进行进一步处理的信息
#define REMOTE_RECEIVEMSG(...)	printf("REMOTE: ");MYMSG(__VA_ARGS__)
#define REMOTE_HANDLEMSG(...)	printf("REMOTE_Handle: ");MYMSG(__VA_ARGS__)
#define ERROR(s,...)			printf("#ERROR %d# ",s);MYMSG(__VA_ARGS__);printf("\t--%s,%d\r\n",__FILE__, __LINE__)	//发送严重错误，必须指明错误语句和地点

/*
 Definition of MSG_ERR
 ---------------------
#ifdef DEBUG_ERR_CONSOLE_ON
#    define MSG_ERR(num, string, value) MSG("\r\n%s,%d : 0X%x %s 0X%x\r\n",__FILE__, __LINE__,num, string, value);
#else
#    define MSG_ERR(num, string, value)
#endif


 Definition of MSG_WAR
 ---------------------
#ifdef DEBUG_WAR_CONSOLE_ON
#    define MSG_WAR(num, string, value) MSG("\r\n%s,%d : 0X%x %s 0X%x \r\n",__FILE__, __LINE__,num, string, value);
#else
#    define MSG_WAR(num, string, value)
#endif
*/


struct Buffer{
	char data[BufferSize];
	uint16_t in;
	uint16_t out;
};



char getDebugBuffer(void);
uint8_t addDebugBuffer(char c);
void debug_init(void);
void debug_IRQ(void);

	#ifdef TEST_ON
	void test_printf(void);
	TEST test_SpeedOfBuffer_printf(void);
	#endif
#endif
