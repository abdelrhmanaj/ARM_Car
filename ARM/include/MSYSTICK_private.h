/*
 * MSYSTICK_private.h
 *
 *  Created on: Aug 19, 2026
 *      Author: Abdelrhmanaj
 */

#ifndef MSYSTICK_PRIVATE_H_
#define MSYSTICK_PRIVATE_H_


/****************************************************************/
/*                    SysTick Base Address                      */
/****************************************************************/

#define SYSTICK_BASE_ADDRESS       0xE000E010


/****************************************************************/
/*                    SysTick Register Structure                */
/****************************************************************/

typedef struct
{
    u32 CTRL;       /* Offset 0x00 */
    u32 LOAD;       /* Offset 0x04 */
    u32 VAL;        /* Offset 0x08 */
    u32 CALIB;      /* Offset 0x0C */

} SysTick_t;


/****************************************************************/
/*                    SysTick Pointer                           */
/****************************************************************/

#define SYSTICK    ((volatile SysTick_t*)SYSTICK_BASE_ADDRESS)

void MSYSTICK_voidHandler(void);
/****************************************************************/
/*                    CTRL Register Bits                        */
/****************************************************************/

/*
 * CTRL Register
 *
 * Bit 0  -> ENABLE
 * Bit 1  -> TICKINT
 * Bit 2  -> CLKSOURCE
 * Bit 16 -> COUNTFLAG
 */

#define ENABLE_BIT       0
#define TICKINT_BIT      1
#define CLKSOURCE_BIT    2
#define COUNTFLAG_BIT    16


/****************************************************************/
/*                    Masks                                     */
/****************************************************************/

#define SYSTICK_CTRL_MASK        0x07

#define SYSTICK_MAX_RELOAD       0x00FFFFFF


#endif /* MSYSTICK_PRIVATE_H_ */