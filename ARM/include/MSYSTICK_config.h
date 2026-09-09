/*
 * MSYSTICK_config.h
 *
 *  Created on: Aug 19, 2026
 *      Author: Abdelrhmanaj
 */

#ifndef MSYSTICK_CONFIG_H_
#define MSYSTICK_CONFIG_H_


/****************************************************************/
/*                    Clock Source                              */
/****************************************************************/

/*
 * Choose SysTick Clock Source:
 *
 * MSYSTICK_AHB_DIV_8
 * MSYSTICK_AHB
 */

#define MSYSTICK_CLOCK_SOURCE      MSYSTICK_AHB_DIV_8



/****************************************************************/
/*                    Interrupt Mode                            */
/****************************************************************/

/*
 * Choose:
 *
 * 0 -> Polling
 * 1 -> Interrupt
 */

#define MSYSTICK_INTERRUPT_ENABLE     0



/****************************************************************/
/*                    SysTick Frequency                         */
/****************************************************************/

/*
 * System Clock Frequency in Hz
 *
 * Example:
 *
 * HSI = 16 MHz
 *
 * #define MSYSTICK_CLOCK_FREQUENCY 8000000
 */

#define MSYSTICK_CLOCK_FREQUENCY      16000000UL


#endif /* MSYSTICK_CONFIG_H_ */
