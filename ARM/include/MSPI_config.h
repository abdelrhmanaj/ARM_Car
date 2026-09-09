#ifndef MSPI_CONFIG_H_
#define MSPI_CONFIG_H_

/* SPI1 alternate-function pins on STM32F401: AF5. */
#define MSPI_PORT              PORTA
#define MSPI_SCK_PIN           PIN5
#define MSPI_MISO_PIN          PIN6
#define MSPI_MOSI_PIN          PIN7
#define MSPI_ALTERNATE_FUNC    AF5

/* SPI_CR1 configuration values. */
#define MSPI_BAUD_RATE         SPI_BAUD_FPCLK_DIV_8
#define MSPI_CLOCK_POLARITY    SPI_CPOL_LOW
#define MSPI_CLOCK_PHASE       SPI_CPHA_FIRST_EDGE
#define MSPI_FRAME_FORMAT      SPI_MSB_FIRST
#define MSPI_SOFTWARE_NSS      1U

#endif /* MSPI_CONFIG_H_ */
