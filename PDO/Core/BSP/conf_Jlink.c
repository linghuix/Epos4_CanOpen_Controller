/*
 * conf_Jlink.c
 *
 *  Created on: Mar 14, 2020
 *      Author: test
 */

#include "device_bsp.h"

void Jlink_Init(void)
{
	  /** ENABLE: Full SWJ (JTAG-DP + SW-DP): Reset State
	  */
	  __HAL_AFIO_REMAP_SWJ_ENABLE();
}
