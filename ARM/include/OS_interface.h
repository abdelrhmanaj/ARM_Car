#ifndef OS_INTERFACE_H_
#define OS_INTERFACE_H_

#include "Std_type.h"

/* Return 0 on success, 1 invalid priority, 2 null callback, 3 occupied slot. */
u8 OS_u8CreateTask(void (*Copy_pvTask)(void),
                   u16 Copy_u16Periodicity,
                   u8 Copy_u8Priority,
                   u8 Copy_u8FirstDelay);

/* Execute tasks whose delay has expired. */
void OS_Scheduler(void);

/* Start the scheduler with a SysTick period expressed in microseconds. */
void OS_Start(u32 Copy_u32TickTimeUs);

#endif /* OS_INTERFACE_H_ */
