#include "Std_type.h"
#include "MRCC_interface.h"
#include "MEXTI_private.h"
#include "MEXTI_interface.h"

static void (*Local_pvEXTI0Callback)(void) = 0;

void MEXTI_voidSetInterruptPort(EXTI_Line Copy_uddtLine, EXTI_PORT Copy_uddtPort)
{
    u8 Local_u8Register = (u8)Copy_uddtLine / EXTI_LINES_PER_REGISTER;
    u8 Local_u8Shift = ((u8)Copy_uddtLine % EXTI_LINES_PER_REGISTER) * 4U;

    if ((u8)Copy_uddtLine <= 15U && (u8)Copy_uddtPort <= EXTI_PORTH)
    {
        MRCC_voidEnablePeipheralClock(APB2_BUS, APB2_SYSCFG);
        SYSCFG->EXTICR[Local_u8Register] &=
            ~(EXTI_PORT_FIELD_MASK << Local_u8Shift);
        SYSCFG->EXTICR[Local_u8Register] |=
            ((u32)Copy_uddtPort << Local_u8Shift);
    }
}

void MEXTI_voidEnable(EXTI_Line Copy_uddtLine)
{
    if ((u8)Copy_uddtLine <= 15U)
        EXTI->IMR |= (1UL << (u8)Copy_uddtLine);
}

void MEXTI_voidDisable(EXTI_Line Copy_uddtLine)
{
    if ((u8)Copy_uddtLine <= 15U)
        EXTI->IMR &= ~(1UL << (u8)Copy_uddtLine);
}

void MEXTI_voidSetTrigger(EXTI_Line Copy_uddtLine, EXTI_TriggerMode Copy_uddtMode)
{
    u32 Local_u32Bit;

    if ((u8)Copy_uddtLine > 15U)
        return;

    Local_u32Bit = 1UL << (u8)Copy_uddtLine;

    if (Copy_uddtMode == EXTI_Rising)
    {
        EXTI->RTSR |= Local_u32Bit;
        EXTI->FTSR &= ~Local_u32Bit;
    }
    else if (Copy_uddtMode == EXTI_Falling)
    {
        EXTI->RTSR &= ~Local_u32Bit;
        EXTI->FTSR |= Local_u32Bit;
    }
    else if (Copy_uddtMode == EXTI_OnChange)
    {
        EXTI->RTSR |= Local_u32Bit;
        EXTI->FTSR |= Local_u32Bit;
    }
}

void MEXTI0_voidCallBackFunction(void (*Copy_pvCallback)(void))
{
    Local_pvEXTI0Callback = Copy_pvCallback;
}

void EXTI0_IRQHandler(void)
{
    EXTI->PR = (1UL << 0);

    if (Local_pvEXTI0Callback != 0)
        Local_pvEXTI0Callback();
}
