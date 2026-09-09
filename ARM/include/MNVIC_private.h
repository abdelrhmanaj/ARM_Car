/*
 * MNVIC_private.h
 *
 *  Created on: Aug 19, 2026
 *      Author: Abdelrhmanaj
 */

#ifndef MNVIC_PRIVATE_H_
#define MNVIC_PRIVATE_H_


/****************************************************************/
/*                    NVIC Base Address                         */
/****************************************************************/

#define NVIC_BASE_ADDRESS      0xE000E100


/****************************************************************/
/*                    NVIC Register Structure                   */
/****************************************************************/

typedef struct
{
    u32 ISER[8];        /* Interrupt Set Enable */
    u32 Reserved1[24];

    u32 ICER[8];        /* Interrupt Clear Enable */
    u32 Reserved2[24];

    u32 ISPR[8];        /* Interrupt Set Pending */
    u32 Reserved3[24];

    u32 ICPR[8];        /* Interrupt Clear Pending */
    u32 Reserved4[24];

    u32 IABR[8];        /* Interrupt Active Bit */
    u32 Reserved5[56];

    u8  IPR[240];      /* Interrupt Priority */

} NVIC_t;


#define NVIC    ((volatile NVIC_t*)NVIC_BASE_ADDRESS)

#define SCB_AIRCR_ADDRESS       0xE000ED0C
#define SCB_AIRCR               (*(volatile u32*)SCB_AIRCR_ADDRESS)
#define SCB_AIRCR_VECTKEY       0x5FAUL
#define SCB_AIRCR_PRIGROUP_POS  8
#define SCB_AIRCR_PRIGROUP_MASK (7UL << SCB_AIRCR_PRIGROUP_POS)


/****************************************************************/
/*                    Priority Configuration                    */
/****************************************************************/

/*
 * STM32F401 uses 4 implemented priority bits.
 */

#define NVIC_PRIORITY_MASK      0x0F

#define NVIC_PRIORITY_BITS      4


#endif /* MNVIC_PRIVATE_H_ */
