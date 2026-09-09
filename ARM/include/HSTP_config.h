#ifndef HSTP_CONFIG_H_
#define HSTP_CONFIG_H_

/* HAL configuration: 74HC595 control pins connected to MCAL GPIO pins. */
#define HSTP_SER_PORT       PORTA
#define HSTP_SER_PIN        PIN1
#define HSTP_SRCLK_PORT     PORTA
#define HSTP_SRCLK_PIN      PIN2
#define HSTP_RCLK_PORT      PORTA
#define HSTP_RCLK_PIN       PIN3

/* OE and MR are active LOW according to the 74HC595 datasheet. */
#define HSTP_OE_PORT        PORTA
#define HSTP_OE_PIN         PIN4
#define HSTP_MR_PORT        PORTA
#define HSTP_MR_PIN         PIN5
#define HSTP_LSB_FIRST      0U

#endif /* HSTP_CONFIG_H_ */
