/*
 * MGPIO_interface.h
 *
 *  Created on: Aug 20, 2026
 *      Author: Abdelrahman
 */

#ifndef MGPIO_INTERFACE_H_
#define MGPIO_INTERFACE_H_


/**************** PORTS ****************/

typedef enum
{
    PORTA,
    PORTB,
    PORTC,
    PORTD,
    PORTE,
    PORTH

} PORTS;


/**************** PINS ****************/

typedef enum
{
    PIN0,
    PIN1,
    PIN2,
    PIN3,
    PIN4,
    PIN5,
    PIN6,
    PIN7,
    PIN8,
    PIN9,
    PIN10,
    PIN11,
    PIN12,
    PIN13,
    PIN14,
    PIN15

} PINS;


/**************** MODES ****************/

typedef enum
{
    INPUT,
    OUTPUT,
    ALTERNATIVE_FUNC,
    ANALOG

} MODES;


/**************** OUTPUT TYPE ****************/

typedef enum
{
    PUSH_PULL,
    OPEN_DRAIN

} OUT_TYPE;


/**************** SPEED ****************/

typedef enum
{
    LOW_SPEED,
    MEDIUM_SPEED,
    HIGH_SPEED,
    VERY_HIGH_SPEED

} SPEED;


/**************** PIN VALUE ****************/

typedef enum
{
    PIN_LOW,
    PIN_HIGH

} Value;


/**************** INPUT CONFIGURATION ****************/

typedef enum
{
    NO_PULL,
    PULL_UP,
    PULL_DOWN

} INPUT_MODE;


/**************** ALTERNATIVE FUNCTION ****************/

typedef enum
{
    AF0,
    AF1,
    AF2,
    AF3,
    AF4,
    AF5,
    AF6,
    AF7,
    AF8,
    AF9,
    AF10,
    AF11,
    AF12,
    AF13,
    AF14,
    AF15

} ALT_FUNC;

/* Compatibility names used by the supplied reference files. */
typedef PORTS Port_t;
typedef PINS Pin_t;
typedef MODES Mode_t;
typedef Value Value_t;
typedef OUT_TYPE Output_Type_t;
typedef INPUT_MODE Input_Mode_t;
typedef SPEED Speed_t;
typedef ALT_FUNC Alternatice_Function_t;

#define AF       ALTERNATIVE_FUNC
#define ANALONG  ANALOG
#define LOW      PIN_LOW
#define HIGH     PIN_HIGH
#define NOPULLUP_NOPULLDOWN NO_PULL
#define PULLUP   PULL_UP
#define PULLDOWN PULL_DOWN


/**************** FUNCTIONS ****************/


void MGPIO_voidSetMode(
        PORTS Copy_uddtPort,
        PINS Copy_uddtPin,
        MODES Copy_uddtMode
);


void MGPIO_voidSetOutputConfig(
        PORTS Copy_uddtPort,
        PINS Copy_uddtPin,
        OUT_TYPE Copy_uddtOutputType,
        SPEED Copy_uddtSpeed
);


void MGPIO_voidSetInputConfig(
        PORTS Copy_uddtPort,
        PINS Copy_uddtPin,
        INPUT_MODE Copy_uddtPullType
);


void MGPIO_voidSetPinValue(
        PORTS Copy_uddtPort,
        PINS Copy_uddtPin,
        Value Copy_uddtValue
);


void MGPIO_voidSetAtomicPinValue(
        PORTS Copy_uddtPort,
        PINS Copy_uddtPin,
        Value Copy_uddtValue
);


void MGPIO_voidTogglePin(
        PORTS Copy_uddtPort,
        PINS Copy_uddtPin
);


u8 MGPIO_u8GetPinValue(
        PORTS Copy_uddtPort,
        PINS Copy_uddtPin
);


void MGPIO_voidSetAlternativeConfig(
        PORTS Copy_uddtPort,
        PINS Copy_uddtPin,
        ALT_FUNC Copy_uddtAlternativeFunction
);


#endif /* MGPIO_INTERFACE_H_ */
