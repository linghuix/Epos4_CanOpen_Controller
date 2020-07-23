#ifndef __gait_H__
#define __gait_H__
#include "stdint.h"

extern int test_angle[114];


extern float hip_flexion[450], knee_flexion[450], knee_flexion_RE40[450];

#define ArrayMaxSize 10
typedef struct {
	int * ArrayName[ArrayMaxSize];
	uint16_t RunNumber;
}trajectory;


#endif
