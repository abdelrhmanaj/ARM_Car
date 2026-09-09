/*
 * MRCC_interface.h
 *
 *  Created on: Aug 20, 2026
 *      Author: Abdelrahman
 */

#ifndef MRCC_INTERFACE_H_
#define MRCC_INTERFACE_H_


/************* AHB1 ****************/

#define AHB1_GPIOA     0
#define AHB1_GPIOB     1
#define AHB1_GPIOC     2
#define AHB1_GPIOD     3
#define AHB1_GPIOE     4
#define AHB1_GPIOH     7

#define AHB1_DMA1      21
#define AHB1_DMA2      22


/************* AHB2 ****************/

#define AHB2_USB_OTG_FS    7


/************* APB1 ****************/

#define APB1_TIM2      0
#define APB1_TIM3      1
#define APB1_TIM4      2
#define APB1_TIM5      3

#define APB1_WWDG      11

#define APB1_SPI2      14
#define APB1_SPI3      15

#define APB1_USART2    17

#define APB1_I2C1      21
#define APB1_I2C2      22
#define APB1_I2C3      23


/************* APB2 ****************/

#define APB2_TIM1      0
#define APB2_USART1    4
#define APB2_USART6    5

#define APB2_ADC1      8

#define APB2_SPI1      12
#define APB2_SPI4      13

#define APB2_SYSCFG    14

#define APB2_TIM9      16
#define APB2_TIM10     17
#define APB2_TIM11     18


/************* BUS TYPES ****************/

typedef enum
{
    SYSTEM_BUS_AHB1 = 0,
    SYSTEM_BUS_AHB2,
    SYSTEM_BUS_APB1,
    SYSTEM_BUS_APB2

} System_Bus;

/* Named constants are macros so CDT resolves them reliably. */
#define AHB1_BUS    SYSTEM_BUS_AHB1
#define AHB2_BUS    SYSTEM_BUS_AHB2
#define APB1_BUS    SYSTEM_BUS_APB1
#define APB2_BUS    SYSTEM_BUS_APB2


/************* FUNCTIONS ****************/

void MRCC_voidInit(void);


void MRCC_voidEnablePeipheralClock(
        System_Bus Copy_uddtBus,
        u8 Copy_u8Peripheral
);


void MRCC_voidDisablePeipheralClock(
        System_Bus Copy_uddtBus,
        u8 Copy_u8Peripheral
);


#endif /* MRCC_INTERFACE_H_ */
