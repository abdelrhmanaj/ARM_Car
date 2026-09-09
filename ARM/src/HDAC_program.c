#include "Std_type.h"
#include "MRCC_interface.h"
#include "MGPIO_interface.h"
#include "HDAC_interface.h"

void HDAC_voidInit(void)
{
    u8 Local_u8Pin;

    MRCC_voidEnablePeipheralClock(AHB1_BUS, AHB1_GPIOA);

    for (Local_u8Pin = 0U; Local_u8Pin < 8U; Local_u8Pin++)
    {
        MGPIO_voidSetMode(PORTA, (PINS)Local_u8Pin, OUTPUT);
        MGPIO_voidSetOutputConfig(PORTA, (PINS)Local_u8Pin,
                                  PUSH_PULL, LOW_SPEED);
        MGPIO_voidSetInputConfig(PORTA, (PINS)Local_u8Pin, NO_PULL);
        MGPIO_voidSetPinValue(PORTA, (PINS)Local_u8Pin, LOW);
    }
}

void HDAC_voidSendSample(const u8 *Copy_pu8Samples, u32 Copy_u32Index)
{
    u8 Local_u8Pin;
    u8 Local_u8Sample;

    if (Copy_pu8Samples == 0)
        return;

    Local_u8Sample = Copy_pu8Samples[Copy_u32Index];

    for (Local_u8Pin = 0U; Local_u8Pin < 8U; Local_u8Pin++)
    {
        MGPIO_voidSetPinValue(PORTA, (PINS)Local_u8Pin,
                              ((Local_u8Sample >> Local_u8Pin) & 1U) ? HIGH : LOW);
    }
}
