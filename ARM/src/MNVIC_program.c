#include "Std_type.h"        /* u8/u32 and common data types. */
#include "MNVIC_interface.h" /* Public function declarations. */
#include "MNVIC_private.h"   /* NVIC/SCB register definitions. */

/* The NVIC exposes eight 32-bit banks: positions 0 through 239. */
#define MNVIC_MAX_POSITION 239U

static u8 MNVIC_u8ValidPosition(u8 Copy_u8Position)
{
    /* Protect the register arrays from an invalid IRQ position. */
    return (Copy_u8Position <= MNVIC_MAX_POSITION);
}

void MNVIC_voidEnable(u8 Copy_u8Position)
{
    /* position / 32 selects the bank; position % 32 selects its bit. */
    if (MNVIC_u8ValidPosition(Copy_u8Position))
        NVIC->ISER[Copy_u8Position / 32U] = 1UL << (Copy_u8Position % 32U);
}

void MNVIC_voidDisable(u8 Copy_u8Position)
{
    /* Writing 1 to ICER disables the selected interrupt. */
    if (MNVIC_u8ValidPosition(Copy_u8Position))
        NVIC->ICER[Copy_u8Position / 32U] = 1UL << (Copy_u8Position % 32U);
}

void MNVIC_voidSetPendingFlag(u8 Copy_u8Position)
{
    /* Writing 1 to ISPR requests the selected interrupt in software. */
    if (MNVIC_u8ValidPosition(Copy_u8Position))
        NVIC->ISPR[Copy_u8Position / 32U] = 1UL << (Copy_u8Position % 32U);
}

void MNVIC_voidClearPendingFlag(u8 Copy_u8Position)
{
    /* Writing 1 to ICPR removes the pending request. */
    if (MNVIC_u8ValidPosition(Copy_u8Position))
        NVIC->ICPR[Copy_u8Position / 32U] = 1UL << (Copy_u8Position % 32U);
}

u8 MNVIC_u8ReadingActiveState(u8 Copy_u8Position)
{
    /* IABR is read-only status: 1 means its handler is running. */
    if (MNVIC_u8ValidPosition(Copy_u8Position))
        return (u8)((NVIC->IABR[Copy_u8Position / 32U] >> (Copy_u8Position % 32U)) & 1UL);

    return 0U;
}

void MNVIC_voidSetGroupMode(NVIC_Group_t Copy_uddtGroupMode)
{
    if ((Copy_uddtGroupMode >= Group16Sub0) &&
        (Copy_uddtGroupMode <= Group0Sub16))
    {
        /* AIRCR requires VECTKEY on every write. */
        SCB_AIRCR = (SCB_AIRCR_VECTKEY << 16) |
                    ((u32)Copy_uddtGroupMode << SCB_AIRCR_PRIGROUP_POS);
    }
}

void MNVIC_voidSetInterruptPriority(u8 Copy_u8Position,
                                    u8 Copy_u8Group,
                                    u8 Copy_u8SubGroup)
{
    u8 Local_u8SubBits;       /* Number of sub-priority bits. */
    u8 Local_u8GroupBits;     /* Number of pre-emption priority bits. */
    u8 Local_u8Priority;      /* Combined four-bit priority. */
    u32 Local_u32GroupMode;   /* PRIGROUP value read from AIRCR. */

    if (!MNVIC_u8ValidPosition(Copy_u8Position))
        return;

    /* Read the current group/subgroup split before validating the inputs. */
    Local_u32GroupMode = (SCB_AIRCR & SCB_AIRCR_PRIGROUP_MASK) >>
                         SCB_AIRCR_PRIGROUP_POS;

    if (Local_u32GroupMode < Group16Sub0 || Local_u32GroupMode > Group0Sub16)
        Local_u32GroupMode = Group16Sub0;

    /* PRIGROUP 3..7 maps to 0..4 subgroup bits. */
    Local_u8SubBits = (u8)(Local_u32GroupMode - Group16Sub0);
    Local_u8GroupBits = (u8)(4U - Local_u8SubBits);

    /* Reject values that cannot fit in the selected priority split. */
    if ((Copy_u8Group >= (1U << Local_u8GroupBits)) ||
        (Copy_u8SubGroup >= (1U << Local_u8SubBits)))
        return;

    /* Put group bits above subgroup bits to form the IPR priority value. */
    Local_u8Priority = (u8)((Copy_u8Group << Local_u8SubBits) |
                            Copy_u8SubGroup);
    /* ARM stores the four implemented priority bits in IPR[7:4]. */
    NVIC->IPR[Copy_u8Position] = (u8)(Local_u8Priority << 4);
}
