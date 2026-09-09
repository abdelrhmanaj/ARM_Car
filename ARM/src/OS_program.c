#include "Std_type.h"
#include "MSYSTICK_config.h"
#include "MSYSTICK_interface.h"
#include "OS_private.h"
#include "OS_config.h"
#include "OS_interface.h"

/* Static task table: priority is also the table index. */
static TASK Local_astrSystemTasks[NO_OF_TASKS] = { { 0 } };
static u16 Local_au16TaskTiming[NO_OF_TASKS] = { 0 };

u8 OS_u8CreateTask(void (*Copy_pvTask)(void),
                   u16 Copy_u16Periodicity,
                   u8 Copy_u8Priority,
                   u8 Copy_u8FirstDelay)
{
    if (Copy_u8Priority >= NO_OF_TASKS)
        return 1U;

    if (Copy_pvTask == 0)
        return 2U;

    if (Local_astrSystemTasks[Copy_u8Priority].TASK_PTR != 0)
        return 3U;

    Local_astrSystemTasks[Copy_u8Priority].TASK_PTR = Copy_pvTask;
    Local_astrSystemTasks[Copy_u8Priority].TASK_Periodicity = Copy_u16Periodicity;
    Local_au16TaskTiming[Copy_u8Priority] = Copy_u8FirstDelay;

    return 0U;
}

void OS_Scheduler(void)
{
    u8 Local_u8TaskIndex;

    for (Local_u8TaskIndex = 0U;
         Local_u8TaskIndex < NO_OF_TASKS;
         Local_u8TaskIndex++)
    {
        if (Local_astrSystemTasks[Local_u8TaskIndex].TASK_PTR != 0)
        {
            if (Local_au16TaskTiming[Local_u8TaskIndex] == 0U)
            {
                Local_astrSystemTasks[Local_u8TaskIndex].TASK_PTR();
                Local_au16TaskTiming[Local_u8TaskIndex] =
                    Local_astrSystemTasks[Local_u8TaskIndex].TASK_Periodicity;
            }
            else
            {
                Local_au16TaskTiming[Local_u8TaskIndex]--;
            }
        }
    }
}

void OS_Start(u32 Copy_u32TickTimeUs)
{
    u32 Local_u32TickFrequency;
    u32 Local_u32ReloadValue;

#if MSYSTICK_CLOCK_SOURCE == MSYSTICK_AHB_DIV_8
    Local_u32TickFrequency = MSYSTICK_CLOCK_FREQUENCY / 8UL;
#else
    Local_u32TickFrequency = MSYSTICK_CLOCK_FREQUENCY;
#endif

    Local_u32ReloadValue =
        (Local_u32TickFrequency / 1000000UL) * Copy_u32TickTimeUs;

    if (Local_u32ReloadValue > 0UL)
        Local_u32ReloadValue--;

    MSYSTICK_voidInit();
    MSYSTICK_voidSetReloadValue(Local_u32ReloadValue);
    MSYSTICK_voidSetCallback(OS_Scheduler);
    MSYSTICK_voidStart();
}
