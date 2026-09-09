#include "Std_type.h"
#include "MRCC_interface.h"       /* MCAL RCC dependency. */
#include "MGPIO_interface.h"      /* MCAL GPIO dependency. */
#include "MSYSTICK_interface.h"   /* Multiplex row timing. */
#include "HLEDMATRIX_config.h"
#include "HLEDMATRIX_interface.h"

static const PINS Local_au8RowPins[8] =
    { PIN0, PIN1, PIN2, PIN3, PIN4, PIN5, PIN6, PIN7 };
static const PINS Local_au8ColumnPins[8] =
    { PIN0, PIN1, PIN2, PIN3, PIN4, PIN5, PIN6, PIN7 };

static void HLEDMATRIX_voidSetRow(u8 Copy_u8Row, u8 Copy_u8On)
{
    u8 Local_u8Value = Copy_u8On;
#if HLEDMATRIX_ROW_ACTIVE_HIGH == 0U
    Local_u8Value = (u8)!Local_u8Value;
#endif
    MGPIO_voidSetPinValue(HLEDMATRIX_ROW_PORT,
                          Local_au8RowPins[Copy_u8Row],
                          Local_u8Value ? HIGH : LOW);
}

static void HLEDMATRIX_voidSetColumn(u8 Copy_u8Column, u8 Copy_u8On)
{
    u8 Local_u8Value = Copy_u8On;
#if HLEDMATRIX_COLUMN_ACTIVE_HIGH == 0U
    Local_u8Value = (u8)!Local_u8Value;
#endif
    MGPIO_voidSetPinValue(HLEDMATRIX_COLUMN_PORT,
                          Local_au8ColumnPins[Copy_u8Column],
                          Local_u8Value ? HIGH : LOW);
}

static void HLEDMATRIX_voidDisableRows(void)
{
    u8 Local_u8Row;
    for (Local_u8Row = 0U; Local_u8Row < 8U; Local_u8Row++)
        HLEDMATRIX_voidSetRow(Local_u8Row, 0U);
}

static void HLEDMATRIX_voidClearColumns(void)
{
    u8 Local_u8Column;
    for (Local_u8Column = 0U; Local_u8Column < 8U; Local_u8Column++)
        HLEDMATRIX_voidSetColumn(Local_u8Column, 0U);
}

void HLEDMATRIX_voidInit(void)
{
    u8 Local_u8Pin;

    MRCC_voidEnablePeipheralClock(AHB1_BUS, AHB1_GPIOB);
    MRCC_voidEnablePeipheralClock(AHB1_BUS, AHB1_GPIOC);

    for (Local_u8Pin = 0U; Local_u8Pin < 8U; Local_u8Pin++)
    {
        MGPIO_voidSetMode(HLEDMATRIX_ROW_PORT, Local_au8RowPins[Local_u8Pin], OUTPUT);
        MGPIO_voidSetOutputConfig(HLEDMATRIX_ROW_PORT, Local_au8RowPins[Local_u8Pin], PUSH_PULL, LOW_SPEED);
        MGPIO_voidSetInputConfig(HLEDMATRIX_ROW_PORT, Local_au8RowPins[Local_u8Pin], NO_PULL);
        MGPIO_voidSetMode(HLEDMATRIX_COLUMN_PORT, Local_au8ColumnPins[Local_u8Pin], OUTPUT);
        MGPIO_voidSetOutputConfig(HLEDMATRIX_COLUMN_PORT, Local_au8ColumnPins[Local_u8Pin], PUSH_PULL, LOW_SPEED);
        MGPIO_voidSetInputConfig(HLEDMATRIX_COLUMN_PORT, Local_au8ColumnPins[Local_u8Pin], NO_PULL);
    }

    HLEDMATRIX_voidDisableRows();
    HLEDMATRIX_voidClearColumns();
}

void HLEDMATRIX_voidDisplay(u8 *Copy_Pu8Arr)
{
    u8 Local_u8Row;
    u8 Local_u8Column;

    if (Copy_Pu8Arr == 0)
        return;

    /* Scan one row at a time; call this continuously from the application. */
    for (Local_u8Row = 0U; Local_u8Row < 8U; Local_u8Row++)
    {
        HLEDMATRIX_voidDisableRows();

        for (Local_u8Column = 0U; Local_u8Column < 8U; Local_u8Column++)
            HLEDMATRIX_voidSetColumn(Local_u8Column,
                (u8)((Copy_Pu8Arr[Local_u8Row] >> Local_u8Column) & 1U));

        HLEDMATRIX_voidSetRow(Local_u8Row, 1U);
        MSYSTICK_voidDelay_ms(HLEDMATRIX_ROW_TIME_MS);
    }

    HLEDMATRIX_voidDisableRows();
}
