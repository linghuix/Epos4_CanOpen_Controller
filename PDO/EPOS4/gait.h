#ifndef __gait_H__
#define __gait_H__
#include "stdint.h"

extern int test_angle[114];


//extern float hip_flexion[450], knee_flexion[450], knee_flexion_RE40[450];

extern float hip_flexion[50], knee_flexion[50];
extern int hip[400], knee[400];

#define ArrayMaxSize 10
typedef struct {
	int * ArrayName[ArrayMaxSize];
	uint16_t RunNumber;
}trajectory;


void firstPos(uint32_t * pos);

#endif
