#include "Std_type.h"
#include "MRCC_interface.h"
#include "MGPIO_interface.h"
#include "MSPI_config.h"
#include "MSPI_private.h"
#include "MSPI_interface.h"

/* Baud-rate values are the values of SPI_CR1 BR[2:0]. */
enum
{
    SPI_BAUD_FPCLK_DIV_2 = 0U,
    SPI_BAUD_FPCLK_DIV_4,
    SPI_BAUD_FPCLK_DIV_8,
    SPI_BAUD_FPCLK_DIV_16,
    SPI_BAUD_FPCLK_DIV_32,
    SPI_BAUD_FPCLK_DIV_64,
    SPI_BAUD_FPCLK_DIV_128,
    SPI_BAUD_FPCLK_DIV_256
};

/* Clock polarity and phase configuration values. */
enum
{
    SPI_CPOL_LOW = 0U,
    SPI_CPOL_HIGH = 1U,
    SPI_CPHA_FIRST_EDGE = 0U,
    SPI_CPHA_SECOND_EDGE = 1U,
    SPI_MSB_FIRST = 0U,
    SPI_LSB_FIRST = 1U
};

static void MSPI_voidConfigurePin(PINS Copy_uddtPin)
{
    MGPIO_voidSetMode(MSPI_PORT, Copy_uddtPin, ALTERNATIVE_FUNC);
    MGPIO_voidSetAlternativeConfig(MSPI_PORT, Copy_uddtPin,
                                   MSPI_ALTERNATE_FUNC);
    MGPIO_voidSetOutputConfig(MSPI_PORT, Copy_uddtPin,
                              PUSH_PULL, VERY_HIGH_SPEED);
    MGPIO_voidSetInputConfig(MSPI_PORT, Copy_uddtPin, NO_PULL);
}

void MSPI_voidInit(void)
{
    /* SPI1 is connected to APB2; GPIOA is needed for SCK/MISO/MOSI. */
    MRCC_voidEnablePeipheralClock(APB2_BUS, APB2_SPI1);
    MRCC_voidEnablePeipheralClock(AHB1_BUS, AHB1_GPIOA);

    MSPI_voidConfigurePin(MSPI_SCK_PIN);
    MSPI_voidConfigurePin(MSPI_MISO_PIN);
    MSPI_voidConfigurePin(MSPI_MOSI_PIN);

    /* Configure SPI while disabled, as required by the reference manual. */
    SPI1->CR1 = 0UL;
    SPI1->CR2 = 0UL;
    SPI1->I2SCFGR = 0UL; /* I2SMOD=0 selects SPI mode. */

    SPI1->CR1 |= MSPI_BIT(MSPI_CR1_MSTR);
    SPI1->CR1 |= ((u32)MSPI_BAUD_RATE & 7UL) << MSPI_CR1_BR_POS;
    SPI1->CR1 |= ((u32)MSPI_CLOCK_POLARITY & 1UL) << MSPI_CR1_CPOL;
    SPI1->CR1 |= ((u32)MSPI_CLOCK_PHASE & 1UL) << MSPI_CR1_CPHA;
    SPI1->CR1 |= ((u32)MSPI_FRAME_FORMAT & 1UL) << MSPI_CR1_LSBFIRST;

#if MSPI_SOFTWARE_NSS == 1U
    /* SSM=1 selects software NSS; SSI=1 keeps the master selected. */
    SPI1->CR1 |= MSPI_BIT(MSPI_CR1_SSM) | MSPI_BIT(MSPI_CR1_SSI);
#endif

    /* DFF=0 selects the 8-bit frame format; CRC and TI mode stay disabled. */
    SPI1->CR1 &= ~MSPI_BIT(MSPI_CR1_DFF);
    SPI1->CR2 &= ~MSPI_BIT(4U); /* FRF=0 selects Motorola SPI mode. */

    MSPI_voidEnable();
}

void MSPI_voidEnable(void)
{
    SPI1->CR1 |= MSPI_BIT(MSPI_CR1_SPE);
}

void MSPI_voidDisable(void)
{
    SPI1->CR1 &= ~MSPI_BIT(MSPI_CR1_SPE);
}

u8 MSPI_u8Transceive(u8 Copy_u8Data)
{
    /* Wait until the transmit buffer is ready before writing SPI_DR. */
    while ((SPI1->SR & MSPI_BIT(MSPI_SR_TXE)) == 0UL)
    {
    }

    SPI1->DR = (u32)Copy_u8Data;

    /* In full-duplex SPI, every transmitted byte also receives one byte. */
    while ((SPI1->SR & MSPI_BIT(MSPI_SR_RXNE)) == 0UL)
    {
    }

    Copy_u8Data = (u8)SPI1->DR;

    /* Ensure the final clock edge has completed before returning. */
    while ((SPI1->SR & MSPI_BIT(MSPI_SR_BSY)) != 0UL)
    {
    }

    return Copy_u8Data;
}

u16 MSPI_u16GetStatus(void)
{
    return (u16)SPI1->SR;
}
