/*
 * MRCC_program.c
 *
 *  Created on: Aug 20, 2026
 *      Author: Abdelrahman
 */

#include "Std_type.h"
#include "MRCC_private.h"
#include "MRCC_config.h"
#include "MRCC_interface.h"


void MRCC_voidInit(void)
{

#if(SYSTEM_CLOCK == HSI_CLOCK)

    /* HSI ON */
    SET_BIT(RCC->CR, HSION);

    /* Wait till HSI ready */
    while(GET_BIT(RCC->CR, HSIRDY) == 0);


    /* Select HSI as System Clock */

    RCC->CFGR &= ~(SW_MUSK_2BIT << SW0);

    /*
     * SW = 00
     * HSI selected
     */


    /* Wait until HSI becomes System Clock */

    while((RCC->CFGR & (SWS_MUSK_2BIT << SWS0)) !=
          (0 << SWS0));


#elif(SYSTEM_CLOCK == HSE_CLOCK)


#if(HSE_OSC == MECHANICAL_CLOCK)

    /*
     * HSE Crystal / Ceramic Resonator
     *
     * HSEBYP = 0
     */

    CLR_BIT(RCC->CR, HSEBYP);


#elif(HSE_OSC == RC_CLOCK)

    /*
     * HSE External Clock
     *
     * HSEBYP = 1
     */

    SET_BIT(RCC->CR, HSEBYP);

#endif


    /* HSE ON */

    SET_BIT(RCC->CR, HSEON);


    /* Wait for HSE, but never block forever if the crystal is unavailable. */
    {
        u32 Local_u32Timeout = 1000000U;

        while ((GET_BIT(RCC->CR, HSERDY) == 0) &&
               (Local_u32Timeout > 0U))
        {
            Local_u32Timeout--;
        }

        if (GET_BIT(RCC->CR, HSERDY) == 0)
        {
            /* Fall back to HSI so the application can still run. */
            SET_BIT(RCC->CR, HSION);
            while (GET_BIT(RCC->CR, HSIRDY) == 0);

            RCC->CFGR &= ~(SW_MUSK_2BIT << SW0);
            while ((RCC->CFGR & (SWS_MUSK_2BIT << SWS0)) != 0U);
        }
        else
        {


    /*
     * Select HSE as System Clock
     *
     * SW = 01
     */

    RCC->CFGR &= ~(SW_MUSK_2BIT << SW0);

    RCC->CFGR |= (1 << SW0);


    /*
     * Wait until HSE becomes System Clock
     *
     * SWS = 01
     */

    while((RCC->CFGR & (SWS_MUSK_2BIT << SWS0)) !=
          (1 << SWS0));


        }
    }

#endif
}


/****************************************************************/
/*              Enable Peripheral Clock                         */
/****************************************************************/

void MRCC_voidEnablePeipheralClock(
        System_Bus Copy_uddtBus,
        u8 Copy_u8Peripheral)
{
    switch(Copy_uddtBus)
    {

    case AHB1_BUS:

        SET_BIT(
                RCC->AHB1ENR,
                Copy_u8Peripheral
        );

        break;


    case AHB2_BUS:

        SET_BIT(
                RCC->AHB2ENR,
                Copy_u8Peripheral
        );

        break;


    case APB1_BUS:

        SET_BIT(
                RCC->APB1ENR,
                Copy_u8Peripheral
        );

        break;


    case APB2_BUS:

        SET_BIT(
                RCC->APB2ENR,
                Copy_u8Peripheral
        );

        break;


    default:

        break;
    }
}


/****************************************************************/
/*              Disable Peripheral Clock                        */
/****************************************************************/

void MRCC_voidDisablePeipheralClock(
        System_Bus Copy_uddtBus,
        u8 Copy_u8Peripheral)
{
    switch(Copy_uddtBus)
    {

    case AHB1_BUS:

        CLR_BIT(
                RCC->AHB1ENR,
                Copy_u8Peripheral
        );

        break;


    case AHB2_BUS:

        CLR_BIT(
                RCC->AHB2ENR,
                Copy_u8Peripheral
        );

        break;


    case APB1_BUS:

        CLR_BIT(
                RCC->APB1ENR,
                Copy_u8Peripheral
        );

        break;


    case APB2_BUS:

        CLR_BIT(
                RCC->APB2ENR,
                Copy_u8Peripheral
        );

        break;


    default:

        break;
    }
}
