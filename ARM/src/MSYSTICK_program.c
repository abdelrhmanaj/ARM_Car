/*
 * MSYSTICK_program.c
 *
 *  Created on: Aug 19, 2026
 *      Author: Abdelrhmanaj
 */

#include "Std_type.h"
#include "MSYSTICK_interface.h"
#include "MSYSTICK_private.h"
#include "MSYSTICK_config.h"


/****************************************************************/
/*                    Global Variables                          */
/****************************************************************/

static void (*MSYSTICK_pvCallBack)(void) = 0;


/****************************************************************/
/*                    MSYSTICK_voidInit                         */
/****************************************************************/

void MSYSTICK_voidInit(void)
{
    /*
     * Stop SysTick
     */

    SYSTICK->CTRL = 0;


    /*
     * Clear current value
     */

    SYSTICK->VAL = 0;
    SYSTICK->LOAD = 0;


    /*
     * Select Clock Source
     */

#if MSYSTICK_CLOCK_SOURCE == MSYSTICK_AHB_DIV_8

    SYSTICK->CTRL &= ~(1 << CLKSOURCE_BIT);

#elif MSYSTICK_CLOCK_SOURCE == MSYSTICK_AHB

    SYSTICK->CTRL |= (1 << CLKSOURCE_BIT);

#endif


    /*
     * Configure Interrupt
     */

#if MSYSTICK_INTERRUPT_ENABLE == 1

    SYSTICK->CTRL |= (1 << TICKINT_BIT);

#else

    SYSTICK->CTRL &= ~(1 << TICKINT_BIT);

#endif
}


/****************************************************************/
/*                 MSYSTICK_voidSetReloadValue                  */
/****************************************************************/

void MSYSTICK_voidSetReloadValue(u32 Copy_u32ReloadValue)
{
    /*
     * Reload register is 24-bit
     */

    if (Copy_u32ReloadValue <= SYSTICK_MAX_RELOAD)
    {
        SYSTICK->LOAD = Copy_u32ReloadValue;
    }
}


/****************************************************************/
/*                    SysTick Interrupt Handler                  */
/****************************************************************/

void SysTick_Handler(void)
{
    if (MSYSTICK_pvCallBack != 0)
    {
        MSYSTICK_pvCallBack();
    }
}


/****************************************************************/
/*                    MSYSTICK_voidStart                        */
/****************************************************************/

void MSYSTICK_voidStart(void)
{
    SYSTICK->CTRL |= (1 << ENABLE_BIT);
}


/****************************************************************/
/*                    MSYSTICK_voidStop                         */
/****************************************************************/

void MSYSTICK_voidStop(void)
{
    SYSTICK->CTRL &= ~(1 << ENABLE_BIT);
}


/****************************************************************/
/*              MSYSTICK_u32GetCurrentValue                     */
/****************************************************************/

u32 MSYSTICK_u32GetCurrentValue(void)
{
    return SYSTICK->VAL;
}


/****************************************************************/
/*                  MSYSTICK_u8GetFlag                          */
/****************************************************************/

u8 MSYSTICK_u8GetFlag(void)
{
    u8 Local_u8Flag;

    Local_u8Flag =
            (SYSTICK->CTRL >> COUNTFLAG_BIT) & 1;

    return Local_u8Flag;
}


/****************************************************************/
/*                 MSYSTICK_voidDelay_us                        */
/****************************************************************/

void MSYSTICK_voidDelay_us(u32 Copy_u32Time)
{
    u32 Local_u32Ticks;


#if MSYSTICK_CLOCK_SOURCE == MSYSTICK_AHB_DIV_8

    /*
     * Clock = System Clock / 8
     */

    Local_u32Ticks =
            (MSYSTICK_CLOCK_FREQUENCY / 8) / 1000000UL;

#elif MSYSTICK_CLOCK_SOURCE == MSYSTICK_AHB

    /*
     * Clock = System Clock
     */

    Local_u32Ticks =
            MSYSTICK_CLOCK_FREQUENCY / 1000000UL;

#endif


    /*
     * Number of ticks required
     */

    Local_u32Ticks *= Copy_u32Time;


    /*
     * Reload value = ticks - 1
     */

    if (Local_u32Ticks > 0)
    {
        Local_u32Ticks--;

        /*
         * Set reload
         */

        SYSTICK->LOAD = Local_u32Ticks;


        /*
         * Clear current value
         */

        SYSTICK->VAL = 0;


        /*
         * Start with interrupt disabled so hardware won't clear COUNTFLAG
         */
        SYSTICK->CTRL &= ~(1 << TICKINT_BIT);
        MSYSTICK_voidStart();


        /*
         * Wait until COUNTFLAG becomes 1
         */

        while (!(SYSTICK->CTRL &
                 (1 << COUNTFLAG_BIT)));


        /*
         * Stop
         */

        MSYSTICK_voidStop();
    }
}


/****************************************************************/
/*                 MSYSTICK_voidDelay_ms                        */
/****************************************************************/

void MSYSTICK_voidDelay_ms(u32 Copy_u32Time)
{
    /*
     * Convert milliseconds to microseconds
     */

    MSYSTICK_voidDelay_us(
            Copy_u32Time * 1000UL
    );
}


/****************************************************************/
/*              MSYSTICK_voidSetCallback                       */
/****************************************************************/

void MSYSTICK_voidSetCallback(void (*Copy_pvCallBackFunc)(void))
{
    MSYSTICK_pvCallBack = Copy_pvCallBackFunc;
}
