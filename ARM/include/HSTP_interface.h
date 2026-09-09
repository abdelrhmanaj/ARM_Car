#ifndef HSTP_INTERFACE_H_
#define HSTP_INTERFACE_H_

#include "Std_type.h"

void HSTP_voidInit(void);
void HSTP_voidSendByte(u8 Copy_u8Data);
void HSTP_voidLatch(void);
void HSTP_voidSendByteAndLatch(u8 Copy_u8Data);
void HSTP_voidSetOutputEnable(u8 Copy_u8Enable);
void HSTP_voidClear(void);

#endif /* HSTP_INTERFACE_H_ */
