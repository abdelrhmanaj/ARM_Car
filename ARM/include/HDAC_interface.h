#ifndef HDAC_INTERFACE_H_
#define HDAC_INTERFACE_H_

#include "Std_type.h"

/* Initialize the eight-bit parallel DAC bus on GPIOA[7:0]. */
void HDAC_voidInit(void);

/* Send one sample from Copy_pu8Samples[Copy_u32Index] to GPIOA[7:0]. */
void HDAC_voidSendSample(const u8 *Copy_pu8Samples, u32 Copy_u32Index);

#endif /* HDAC_INTERFACE_H_ */
