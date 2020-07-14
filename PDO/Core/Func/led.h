#ifndef __LED_H
#define	__LED_H

#include <conf_gpio.h>

/* the macro definition to trigger the led on or off 
 * 1 - off
 - 0 - on
 */
#define LED_GPIO GPIOC,GPIO_PIN_13

#define LEDON	HAL_GPIO_WritePin(LED_GPIO, GPIO_PIN_RESET);

#define LEDOFF	HAL_GPIO_WritePin(LED_GPIO, GPIO_PIN_SET);

#define LEDSwich HAL_GPIO_TogglePin(LED_GPIO);

void LED_init(void);

#endif /* __LED_H */
