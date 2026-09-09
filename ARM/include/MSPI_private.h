#ifndef MSPI_PRIVATE_H_
#define MSPI_PRIVATE_H_

#include "Std_type.h"

#define SPI1_BASE_ADDRESS 0x40013000UL

typedef struct
{
    u32 CR1;
    u32 CR2;
    u32 SR;
    u32 DR;
    u32 CRCPR;
    u32 RXCRCR;
    u32 TXCRCR;
    u32 I2SCFGR;
    u32 I2SPR;
} MSPI_t;

#define SPI1 ((volatile MSPI_t *)SPI1_BASE_ADDRESS)

/* SPI_CR1 bit positions. */
#define MSPI_CR1_CPHA       0U
#define MSPI_CR1_CPOL       1U
#define MSPI_CR1_MSTR       2U
#define MSPI_CR1_BR_POS     3U
#define MSPI_CR1_SPE        6U
#define MSPI_CR1_LSBFIRST   7U
#define MSPI_CR1_SSI        8U
#define MSPI_CR1_SSM        9U
#define MSPI_CR1_DFF        11U

/* SPI_SR bit positions. */
#define MSPI_SR_RXNE        0U
#define MSPI_SR_TXE         1U
#define MSPI_SR_BSY         7U

#define MSPI_BIT(BIT)       (1UL << (BIT))

#endif /* MSPI_PRIVATE_H_ */
