#include "Std_type.h"
#include "MRCC_interface.h"       /* MCAL RCC dependency. */
#include "MGPIO_interface.h"      /* MCAL GPIO dependency. */
#include "HSTP_config.h"
#include "HSTP_interface.h"

static void HSTP_voidConfigurePin(PORTS Copy_uddtPort, PINS Copy_uddtPin)
{
    MGPIO_voidSetMode(Copy_uddtPort, Copy_uddtPin, OUTPUT);
    MGPIO_voidSetOutputConfig(Copy_uddtPort, Copy_uddtPin, PUSH_PULL, HIGH_SPEED);
    MGPIO_voidSetInputConfig(Copy_uddtPort, Copy_uddtPin, NO_PULL);
    MGPIO_voidSetPinValue(Copy_uddtPort, Copy_uddtPin, LOW);
}

static void HSTP_voidEnablePortClock(PORTS Copy_uddtPort)
{
    switch (Copy_uddtPort)
    {
    case PORTA: MRCC_voidEnablePeipheralClock(AHB1_BUS, AHB1_GPIOA); break;
    case PORTB: MRCC_voidEnablePeipheralClock(AHB1_BUS, AHB1_GPIOB); break;
    case PORTC: MRCC_voidEnablePeipheralClock(AHB1_BUS, AHB1_GPIOC); break;
    case PORTD: MRCC_voidEnablePeipheralClock(AHB1_BUS, AHB1_GPIOD); break;
    case PORTE: MRCC_voidEnablePeipheralClock(AHB1_BUS, AHB1_GPIOE); break;
    case PORTH: MRCC_voidEnablePeipheralClock(AHB1_BUS, AHB1_GPIOH); break;
    default: break;
    }
}

void HSTP_voidInit(void)
{
    HSTP_voidEnablePortClock(HSTP_SER_PORT);
    HSTP_voidEnablePortClock(HSTP_SRCLK_PORT);
    HSTP_voidEnablePortClock(HSTP_RCLK_PORT);
    HSTP_voidEnablePortClock(HSTP_OE_PORT);
    HSTP_voidEnablePortClock(HSTP_MR_PORT);

    HSTP_voidConfigurePin(HSTP_SER_PORT, HSTP_SER_PIN);
    HSTP_voidConfigurePin(HSTP_SRCLK_PORT, HSTP_SRCLK_PIN);
    HSTP_voidConfigurePin(HSTP_RCLK_PORT, HSTP_RCLK_PIN);
    HSTP_voidConfigurePin(HSTP_OE_PORT, HSTP_OE_PIN);
    HSTP_voidConfigurePin(HSTP_MR_PORT, HSTP_MR_PIN);

    /* OE LOW enables outputs; MR HIGH releases the internal reset. */
    MGPIO_voidSetPinValue(HSTP_OE_PORT, HSTP_OE_PIN, LOW);
    MGPIO_voidSetPinValue(HSTP_MR_PORT, HSTP_MR_PIN, HIGH);
}

void HSTP_voidSendByte(u8 Copy_u8Data)
{
    u8 Local_u8BitIndex;
    u8 Local_u8BitValue;

    for (Local_u8BitIndex = 0U; Local_u8BitIndex < 8U; Local_u8BitIndex++)
    {
#if HSTP_LSB_FIRST == 1U
        Local_u8BitValue = (u8)((Copy_u8Data >> Local_u8BitIndex) & 1U);
#else
        Local_u8BitValue = (u8)((Copy_u8Data >> (7U - Local_u8BitIndex)) & 1U);
#endif
        MGPIO_voidSetPinValue(HSTP_SER_PORT, HSTP_SER_PIN,
                              Local_u8BitValue ? HIGH : LOW);
        MGPIO_voidSetPinValue(HSTP_SRCLK_PORT, HSTP_SRCLK_PIN, HIGH);
        MGPIO_voidSetPinValue(HSTP_SRCLK_PORT, HSTP_SRCLK_PIN, LOW);
    }
}

void HSTP_voidLatch(void)
{
    /* RCLK rising edge transfers shift data to QA..QH. */
    MGPIO_voidSetPinValue(HSTP_RCLK_PORT, HSTP_RCLK_PIN, LOW);
    MGPIO_voidSetPinValue(HSTP_RCLK_PORT, HSTP_RCLK_PIN, HIGH);
    MGPIO_voidSetPinValue(HSTP_RCLK_PORT, HSTP_RCLK_PIN, LOW);
}

void HSTP_voidSendByteAndLatch(u8 Copy_u8Data)
{
    HSTP_voidSendByte(Copy_u8Data);
    HSTP_voidLatch();
}

void HSTP_voidSetOutputEnable(u8 Copy_u8Enable)
{
    /* OE is active LOW. */
    MGPIO_voidSetPinValue(HSTP_OE_PORT, HSTP_OE_PIN,
                          Copy_u8Enable ? LOW : HIGH);
}

void HSTP_voidClear(void)
{
    /* MR is active LOW and clears the shift register asynchronously. */
    MGPIO_voidSetPinValue(HSTP_MR_PORT, HSTP_MR_PIN, LOW);
    MGPIO_voidSetPinValue(HSTP_MR_PORT, HSTP_MR_PIN, HIGH);
}
