/*
 * MGPIO_program.c
 *
 *  Created on: Aug 20, 2026
 *      Author: Abdelrahman
 */

#include "Std_type.h"
#include "MGPIO_private.h"
#include "MGPIO_config.h"
#include "MGPIO_interface.h"


/****************************************************************/
/*                     Private Function                         */
/****************************************************************/

static volatile GPIO_t* MGPIO_GetPort(PORTS Copy_uddtPort)
{
    volatile GPIO_t* Local_pGPIO = 0;

    switch (Copy_uddtPort)
    {
    case PORTA:
        Local_pGPIO = GPIOA;
        break;

    case PORTB:
        Local_pGPIO = GPIOB;
        break;

    case PORTC:
        Local_pGPIO = GPIOC;
        break;

    case PORTD:
        Local_pGPIO = GPIOD;
        break;

    case PORTE:
        Local_pGPIO = GPIOE;
        break;

    case PORTH:
        Local_pGPIO = GPIOH;
        break;

    default:
        break;
    }

    return Local_pGPIO;
}


/****************************************************************/
/*                    Set GPIO Mode                             */
/****************************************************************/

void MGPIO_voidSetMode(
        PORTS Copy_uddtPort,
        PINS Copy_uddtPin,
        MODES Copy_uddtMode)
{
    volatile GPIO_t* Local_pGPIO;

    Local_pGPIO = MGPIO_GetPort(Copy_uddtPort);

    if (Local_pGPIO != 0)
    {
        /*
         * Clear 2 bits
         */

        Local_pGPIO->MODER &=
                ~(3 << (Copy_uddtPin * 2));

        /*
         * Set Mode
         */

        Local_pGPIO->MODER |=
                (Copy_uddtMode << (Copy_uddtPin * 2));
    }
}


/****************************************************************/
/*                 Set Output Configuration                     */
/****************************************************************/

void MGPIO_voidSetOutputConfig(
        PORTS Copy_uddtPort,
        PINS Copy_uddtPin,
        OUT_TYPE Copy_uddtOutputType,
        SPEED Copy_uddtSpeed)
{
    volatile GPIO_t* Local_pGPIO;

    Local_pGPIO = MGPIO_GetPort(Copy_uddtPort);

    if (Local_pGPIO != 0)
    {
        /*
         * Configure Output Type
         *
         * OTYPER:
         * 0 -> Push Pull
         * 1 -> Open Drain
         */

        Local_pGPIO->OTYPER &=
                ~(1 << Copy_uddtPin);

        Local_pGPIO->OTYPER |=
                (Copy_uddtOutputType << Copy_uddtPin);


        /*
         * Configure Speed
         *
         * OSPEEDR:
         * 00 -> Low
         * 01 -> Medium
         * 10 -> High
         * 11 -> Very High
         */

        Local_pGPIO->OSPEEDR &=
                ~(3 << (Copy_uddtPin * 2));

        Local_pGPIO->OSPEEDR |=
                (Copy_uddtSpeed << (Copy_uddtPin * 2));
    }
}


/****************************************************************/
/*                    Set Pull Type                             */
/****************************************************************/

void MGPIO_voidSetInputConfig(
        PORTS Copy_uddtPort,
        PINS Copy_uddtPin,
        INPUT_MODE Copy_uddtPullType)
{
    volatile GPIO_t* Local_pGPIO;

    Local_pGPIO = MGPIO_GetPort(Copy_uddtPort);

    if (Local_pGPIO != 0)
    {
        /*
         * Clear PUPDR bits
         */

        Local_pGPIO->PUPDR &=
                ~(3 << (Copy_uddtPin * 2));

        /*
         * Set Pull Type
         */

        Local_pGPIO->PUPDR |=
                (Copy_uddtPullType << (Copy_uddtPin * 2));
    }
}


/****************************************************************/
/*                    Set Pin Value                             */
/****************************************************************/

void MGPIO_voidSetPinValue(
        PORTS Copy_uddtPort,
        PINS Copy_uddtPin,
        Value Copy_uddtValue)
{
    volatile GPIO_t* Local_pGPIO;

    Local_pGPIO = MGPIO_GetPort(Copy_uddtPort);

    if (Local_pGPIO != 0)
    {
        if (Copy_uddtValue == PIN_HIGH)
        {
            /* Set the output latch, matching the reference implementation. */
            Local_pGPIO->ODR |=
                    (1U << Copy_uddtPin);
        }
        else
        {
            /* Clear the output latch. */
            Local_pGPIO->ODR &=
                    ~(1U << Copy_uddtPin);
        }
    }
}


/****************************************************************/
/*                    Toggle Pin Value                          */
/****************************************************************/

void MGPIO_voidTogglePin(
        PORTS Copy_uddtPort,
        PINS Copy_uddtPin)
{
    volatile GPIO_t* Local_pGPIO;

    Local_pGPIO = MGPIO_GetPort(Copy_uddtPort);

    if (Local_pGPIO != 0)
    {
        /* BSRR performs an atomic set/reset operation. */
        if ((Local_pGPIO->ODR & (1 << Copy_uddtPin)) != 0)
        {
            Local_pGPIO->BSRR = (1 << (Copy_uddtPin + 16));
        }
        else
        {
            Local_pGPIO->BSRR = (1 << Copy_uddtPin);
        }
    }
}


/****************************************************************/
/*                    Get Pin Value                             */
/****************************************************************/

u8 MGPIO_u8GetPinValue(
        PORTS Copy_uddtPort,
        PINS Copy_uddtPin)
{
    volatile GPIO_t* Local_pGPIO;
    u8 Local_u8Value;

    Local_pGPIO = MGPIO_GetPort(Copy_uddtPort);

    if (Local_pGPIO != 0)
    {
        Local_u8Value =
                (Local_pGPIO->IDR >> Copy_uddtPin) & 1;
    }
    else
    {
        Local_u8Value = PIN_LOW;
    }

    return Local_u8Value;
}


/****************************************************************/
/*              Set Alternative Function                        */
/****************************************************************/

void MGPIO_voidSetAlternativeConfig(
        PORTS Copy_uddtPort,
        PINS Copy_uddtPin,
        ALT_FUNC Copy_uddtAF)
{
    volatile GPIO_t* Local_pGPIO;

    Local_pGPIO = MGPIO_GetPort(Copy_uddtPort);

    if (Local_pGPIO != 0)
    {
        if (Copy_uddtPin <= PIN7)
        {
            /*
             * AFRL
             */

            Local_pGPIO->AFRL &=
                ~(0xF << (Copy_uddtPin * 4));

            Local_pGPIO->AFRL |=
                    (Copy_uddtAF << (Copy_uddtPin * 4));
        }
        else
        {
            /*
             * AFRH
             */

            Local_pGPIO->AFRH &=
                ~(0xF << ((Copy_uddtPin - 8) * 4));

            Local_pGPIO->AFRH |=
                    (Copy_uddtAF <<
                     ((Copy_uddtPin - 8) * 4));
        }
    }
}
