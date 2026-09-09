/*
 * MSYSTICK_interface.h
 *
 *  Created on: Aug 19, 2026
 *      Author: Abdelrhmanaj
 */

#ifndef MSYSTICK_INTERFACE_H_
#define MSYSTICK_INTERFACE_H_


/****************************************************************/
/*                      SysTick Clock Source                    */
/****************************************************************/

typedef enum
{
    MSYSTICK_AHB_DIV_8 = 0,
    MSYSTICK_AHB = 1

} MSYSTICK_ClockSource;


/****************************************************************/
/*                      SysTick Functions                       */
/****************************************************************/


/*
 * Initialize SysTick
 */
void MSYSTICK_voidInit(void);


/*
 * Set the required delay in milliseconds
 */
void MSYSTICK_voidDelay_ms(u32 Copy_u32Time);


/*
 * Set the required delay in microseconds
 */
void MSYSTICK_voidDelay_us(u32 Copy_u32Time);


/*
 * Set SysTick reload value
 */
void MSYSTICK_voidSetReloadValue(u32 Copy_u32ReloadValue);


/*
 * Start SysTick
 */
void MSYSTICK_voidStart(void);


/*
 * Stop SysTick
 */
void MSYSTICK_voidStop(void);


/*
 * Get SysTick counter value
 */
u32 MSYSTICK_u32GetCurrentValue(void);


/*
 * Check whether SysTick counting is finished
 *
 * Return:
 * 1 -> COUNTFLAG is set
 * 0 -> COUNTFLAG is not set
 */
u8 MSYSTICK_u8GetFlag(void);


/*
 * Set SysTick callback function
 */
void MSYSTICK_voidSetCallback(void (*Copy_pvCallBackFunc)(void));


#endif /* MSYSTICK_INTERFACE_H_ */