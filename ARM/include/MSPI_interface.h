#ifndef MSPI_INTERFACE_H_
#define MSPI_INTERFACE_H_

#include "Std_type.h"

/* SPI1 master, 8-bit, polling driver. */
void MSPI_voidInit(void);

/* Enable or disable SPI1. */
void MSPI_voidEnable(void);
void MSPI_voidDisable(void);

/* Transmit one byte and return the byte received simultaneously. */
u8 MSPI_u8Transceive(u8 Copy_u8Data);

/* Return the SPI status register value. */
u16 MSPI_u16GetStatus(void);

#endif /* MSPI_INTERFACE_H_ */
