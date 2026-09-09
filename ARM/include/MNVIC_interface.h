#ifndef MNVIC_INTERFACE_H_
#define MNVIC_INTERFACE_H_

/* Public NVIC API. Position is the IRQ number from the vector table. */
typedef enum
{
    Group16Sub0 = 3,
    Group8Sub2,
    Group4Sub4,
    Group2Sub8,
    Group0Sub16
} NVIC_Group_t;

/* Enable or disable one external interrupt. */
void MNVIC_voidEnable(u8 Copy_u8Position);
void MNVIC_voidDisable(u8 Copy_u8Position);
/* Set or clear the software pending flag. */
void MNVIC_voidSetPendingFlag(u8 Copy_u8Position);
void MNVIC_voidClearPendingFlag(u8 Copy_u8Position);
/* Return 1 while the interrupt is currently active. */
u8 MNVIC_u8ReadingActiveState(u8 Copy_u8Position);
/* Select the group/sub-group priority split. */
void MNVIC_voidSetGroupMode(NVIC_Group_t Copy_uddtGroupMode);
void MNVIC_voidSetInterruptPriority(u8 Copy_u8Position,
                                    u8 Copy_u8Group,
                                    u8 Copy_u8SubGroup);

#endif /* MNVIC_INTERFACE_H_ */
