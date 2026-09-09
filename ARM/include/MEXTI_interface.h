#ifndef MEXTI_INTERFACE_H_
#define MEXTI_INTERFACE_H_

#include "Std_type.h"

typedef enum
{
    EXTI_LineZero = 0,
    EXTI_LineOne,
    EXTI_LineTwo,
    EXTI_LineThree,
    EXTI_LineFour,
    EXTI_LineFive,
    EXTI_LineSix,
    EXTI_LineSeven,
    EXTI_LineEight,
    EXTI_LineNine,
    EXTI_LineTen,
    EXTI_LineEleven,
    EXTI_LineTwelve,
    EXTI_LineThirteen,
    EXTI_LineFourteen,
    EXTI_LineFifteen
} EXTI_Line;

typedef enum
{
    EXTI_Rising,
    EXTI_Falling,
    EXTI_OnChange
} EXTI_TriggerMode;

typedef enum
{
    EXTI_PORTA = 0,
    EXTI_PORTB,
    EXTI_PORTC,
    EXTI_PORTD,
    EXTI_PORTE,
    EXTI_PORTH
} EXTI_PORT;

void MEXTI_voidSetInterruptPort(EXTI_Line Copy_uddtLine, EXTI_PORT Copy_uddtPort);
void MEXTI_voidEnable(EXTI_Line Copy_uddtLine);
void MEXTI_voidDisable(EXTI_Line Copy_uddtLine);
void MEXTI_voidSetTrigger(EXTI_Line Copy_uddtLine, EXTI_TriggerMode Copy_uddtMode);
void MEXTI0_voidCallBackFunction(void (*Copy_pvCallback)(void));

#endif /* MEXTI_INTERFACE_H_ */
