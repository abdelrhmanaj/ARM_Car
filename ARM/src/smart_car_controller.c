#ifdef SMART_CAR_IMPLEMENTATION
#include <stdio.h>
#include <string.h>
#include "Std_type.h"
#include "MRCC_interface.h"
#include "MGPIO_interface.h"
#include "MSYSTICK_interface.h"
#include "MNVIC_interface.h"
#include "stm32f4xx.h"

/* STM32F401CCU6 smart 4WD controller. Pin map: */
/* H-bridge: PB0/PB1/PA6 left, PB2/PB3/PA7 right. */
/* Ultrasonic (single): PA0 TRIG, PA1 ECHO. */
/* Headlight (كشاف): PA4. */
/* Reverse Red LED (لمبة الرجوع للخلف): PA5. */
/* Buzzer (بازر): PB5. */
/* ESP32 UART2 link: PA2 TX, PA3 RX, 115200 baud. */
#define L1_PORT PORTB
#define L1_PIN PIN0
#define L2_PORT PORTB
#define L2_PIN PIN1
#define R1_PORT PORTB
#define R1_PIN PIN2
#define R2_PORT PORTB
#define R2_PIN PIN3

#define TRIG_PORT PORTA
#define TRIG_PIN PIN0
#define ECHO_PORT PORTA
#define ECHO_PIN PIN1

#define HEADLIGHT_PORT PORTA
#define HEADLIGHT_PIN PIN4

#define REVERSE_LED_PORT PORTA
#define REVERSE_LED_PIN PIN5

#define BUZZER_PORT PORTB
#define BUZZER_PIN PIN5

#define ONBOARD_LED_PORT PORTC
#define ONBOARD_LED_PIN PIN13

#define PWM_TOP 999U
#define COMMAND_TIMEOUT_MS 600U
#define UART_RX_BUF_SIZE 128U

static volatile char rxBuffer[UART_RX_BUF_SIZE];
static volatile u8 rxHead = 0U;
static volatile u8 rxTail = 0U;

void USART2_IRQHandler(void) {
    const u32 sr = USART2->SR;
    if ((sr & (USART_SR_RXNE | USART_SR_ORE | USART_SR_FE | USART_SR_NE | USART_SR_PE)) != 0U) {
        const char c = (char)(USART2->DR & 0xFFU);
        if ((sr & USART_SR_RXNE) != 0U) {
            const u8 next = (u8)((rxHead + 1U) % UART_RX_BUF_SIZE);
            if (next != rxTail) {
                rxBuffer[rxHead] = c;
                rxHead = next;
            }
        }
    }
}

static u32 systemMs = 0U;
static u32 lastCommandMs = 0U;
static u8 speedPercent = 100U;

static void delayUs(u32 us) {
    for (volatile u32 i = 0; i < (us * 3U); i++) {
        __asm volatile("nop");
    }
}

static void outputPin(PORTS port, PINS pin) {
    MGPIO_voidSetMode(port, pin, OUTPUT);
    MGPIO_voidSetOutputConfig(port, pin, PUSH_PULL, HIGH_SPEED);
    MGPIO_voidSetInputConfig(port, pin, NO_PULL);
}
static void inputPin(PORTS port, PINS pin) {
    MGPIO_voidSetMode(port, pin, INPUT);
    MGPIO_voidSetInputConfig(port, pin, NO_PULL);
}
static void peripheralsInit(void) {
    MRCC_voidEnablePeipheralClock(AHB1_BUS, AHB1_GPIOA);
    MRCC_voidEnablePeipheralClock(AHB1_BUS, AHB1_GPIOB);
    MRCC_voidEnablePeipheralClock(AHB1_BUS, AHB1_GPIOC);
    MRCC_voidEnablePeipheralClock(APB1_BUS, APB1_TIM3);
    MRCC_voidEnablePeipheralClock(APB1_BUS, APB1_USART2);

    MGPIO_voidSetMode(PORTA, PIN2, ALTERNATIVE_FUNC);
    MGPIO_voidSetMode(PORTA, PIN3, ALTERNATIVE_FUNC);
    MGPIO_voidSetMode(PORTA, PIN6, ALTERNATIVE_FUNC);
    MGPIO_voidSetMode(PORTA, PIN7, ALTERNATIVE_FUNC);
    MGPIO_voidSetOutputConfig(PORTA, PIN2, PUSH_PULL, VERY_HIGH_SPEED);
    MGPIO_voidSetOutputConfig(PORTA, PIN3, PUSH_PULL, VERY_HIGH_SPEED);
    MGPIO_voidSetOutputConfig(PORTA, PIN6, PUSH_PULL, HIGH_SPEED);
    MGPIO_voidSetOutputConfig(PORTA, PIN7, PUSH_PULL, HIGH_SPEED);
    MGPIO_voidSetAlternativeConfig(PORTA, PIN2, AF7);
    MGPIO_voidSetAlternativeConfig(PORTA, PIN3, AF7);
    MGPIO_voidSetInputConfig(PORTA, PIN3, PULL_UP);
    MGPIO_voidSetAlternativeConfig(PORTA, PIN6, AF2);
    MGPIO_voidSetAlternativeConfig(PORTA, PIN7, AF2);

    /* SYSTEM_CLOCK is HSI (16 MHz), APB1 prescaler is 1: 16 MHz / 115200. */
    USART2->BRR = 0x8BU;
    USART2->CR2 = 0U;
    USART2->CR3 = 0U;
    USART2->CR1 = USART_CR1_TE | USART_CR1_RE | USART_CR1_UE | USART_CR1_RXNEIE;

    MNVIC_voidEnable(38U);

    TIM3->PSC = 7U; TIM3->ARR = PWM_TOP;
    /* OC1M is bits 6:4 and OC2M is bits 14:12 on STM32F4. */
    TIM3->CCMR1 = (6U << 4U) | TIM_CCMR1_OC1PE | (6U << 12U) | TIM_CCMR1_OC2PE;
    TIM3->CCER = TIM_CCER_CC1E | TIM_CCER_CC2E; TIM3->CR1 = TIM_CR1_ARPE | TIM_CR1_CEN;
}
static void pwmSet(u8 percent) {
    if (percent > 100U) percent = 100U;
    const u16 value = (u16)(((u32)percent * PWM_TOP) / 100U);
    TIM3->CCR1 = value; TIM3->CCR2 = value;
}
static void stopMotors(void) {
    /* Always extinguish reverse LED on stop */
    MGPIO_voidSetPinValue(REVERSE_LED_PORT, REVERSE_LED_PIN, LOW);
    MGPIO_voidSetPinValue(L1_PORT, L1_PIN, LOW); MGPIO_voidSetPinValue(L2_PORT, L2_PIN, LOW);
    MGPIO_voidSetPinValue(R1_PORT, R1_PIN, LOW); MGPIO_voidSetPinValue(R2_PORT, R2_PIN, LOW);
    pwmSet(0U);
}
/* direction: 1 forward, 2 backward, 3 left, 4 right, 0 stop */
static void drive(u8 direction) {
    if (direction == 0U) {
        stopMotors();
        return;
    }
    if (direction == 1U) {
        /* FORWARD: Reverse LED OFF */
        MGPIO_voidSetPinValue(REVERSE_LED_PORT, REVERSE_LED_PIN, LOW);
        MGPIO_voidSetPinValue(L1_PORT, L1_PIN, LOW);
        MGPIO_voidSetPinValue(L2_PORT, L2_PIN, HIGH);
        MGPIO_voidSetPinValue(R1_PORT, R1_PIN, LOW);
        MGPIO_voidSetPinValue(R2_PORT, R2_PIN, HIGH);
    } else if (direction == 2U) {
        /* BACKWARD: Turn ON Red Reverse LED! */
        MGPIO_voidSetPinValue(REVERSE_LED_PORT, REVERSE_LED_PIN, HIGH);
        MGPIO_voidSetPinValue(L1_PORT, L1_PIN, HIGH);
        MGPIO_voidSetPinValue(L2_PORT, L2_PIN, LOW);
        MGPIO_voidSetPinValue(R1_PORT, R1_PIN, HIGH);
        MGPIO_voidSetPinValue(R2_PORT, R2_PIN, LOW);
    } else if (direction == 3U) {
        /* LEFT: Spin in place, Reverse LED OFF */
        MGPIO_voidSetPinValue(REVERSE_LED_PORT, REVERSE_LED_PIN, LOW);
        MGPIO_voidSetPinValue(L1_PORT, L1_PIN, HIGH);
        MGPIO_voidSetPinValue(L2_PORT, L2_PIN, LOW);
        MGPIO_voidSetPinValue(R1_PORT, R1_PIN, LOW);
        MGPIO_voidSetPinValue(R2_PORT, R2_PIN, HIGH);
    } else if (direction == 4U) {
        /* RIGHT: Spin in place, Reverse LED OFF */
        MGPIO_voidSetPinValue(REVERSE_LED_PORT, REVERSE_LED_PIN, LOW);
        MGPIO_voidSetPinValue(L1_PORT, L1_PIN, LOW);
        MGPIO_voidSetPinValue(L2_PORT, L2_PIN, HIGH);
        MGPIO_voidSetPinValue(R1_PORT, R1_PIN, HIGH);
        MGPIO_voidSetPinValue(R2_PORT, R2_PIN, LOW);
    }
    pwmSet(100U);
}
static void sendChar(char c) { while ((USART2->SR & USART_SR_TXE) == 0U) { } USART2->DR = (u16)c; }
static void sendText(const char *text) { while (*text != '\0') sendChar(*text++); }
static u8 readChar(char *out) {
    if (rxHead == rxTail) {
        return 0U;
    }
    *out = rxBuffer[rxTail];
    rxTail = (u8)((rxTail + 1U) % UART_RX_BUF_SIZE);
    return 1U;
}
static u16 readDistanceCm(PORTS trigPort, PINS trigPin, PORTS echoPort, PINS echoPin) {
    u32 timeout = 30000U, pulse = 0U;
    MGPIO_voidSetPinValue(trigPort, trigPin, LOW); delayUs(2U);
    MGPIO_voidSetPinValue(trigPort, trigPin, HIGH); delayUs(12U); MGPIO_voidSetPinValue(trigPort, trigPin, LOW);
    while (MGPIO_u8GetPinValue(echoPort, echoPin) == 0U && timeout-- > 0U) delayUs(1U);
    if (timeout == 0U) return 0U;
    timeout = 30000U;
    while (MGPIO_u8GetPinValue(echoPort, echoPin) != 0U && timeout-- > 0U) { delayUs(1U); pulse++; }
    return (u16)(pulse / 58U);
}
static void handleCommand(char *line) {
    u16 value = 0U;
    if (strncmp(line, "CMD STOP", 8U) == 0) { stopMotors(); lastCommandMs = systemMs; sendText("ACK STOP\n"); return; }
    if (strncmp(line, "CMD LIGHT ON", 12U) == 0) { MGPIO_voidSetPinValue(HEADLIGHT_PORT, HEADLIGHT_PIN, HIGH); sendText("ACK LIGHT\n"); return; }
    if (strncmp(line, "CMD LIGHT OFF", 13U) == 0) { MGPIO_voidSetPinValue(HEADLIGHT_PORT, HEADLIGHT_PIN, LOW); sendText("ACK LIGHT\n"); return; }
    if (strncmp(line, "CMD HORN ON", 11U) == 0) { MGPIO_voidSetPinValue(BUZZER_PORT, BUZZER_PIN, HIGH); sendText("ACK HORN\n"); return; }
    if (strncmp(line, "CMD HORN OFF", 12U) == 0) { MGPIO_voidSetPinValue(BUZZER_PORT, BUZZER_PIN, LOW); sendText("ACK HORN\n"); return; }
    if (sscanf(line, "CMD SPEED %hu", &value) == 1) { speedPercent = value > 100U ? 100U : (u8)value; lastCommandMs = systemMs; sendText("ACK SPEED\n"); return; }
    if (strstr(line, "CMD MOVE FORWARD") != 0) drive(1U);
    else if (strstr(line, "CMD MOVE BACKWARD") != 0) drive(2U);
    else if (strstr(line, "CMD MOVE LEFT") != 0) drive(3U);
    else if (strstr(line, "CMD MOVE RIGHT") != 0) drive(4U);
    else if (strstr(line, "CMD MOVE STOP") != 0) { stopMotors(); lastCommandMs = systemMs; sendText("ACK STOP\n"); return; }
    else { sendText("NACK UNKNOWN\n"); return; }
    lastCommandMs = systemMs;
    sendText("ACK MOVE\n");
}
static void sendTelemetry(void) {
    char buffer[80];
    const u16 dist = readDistanceCm(TRIG_PORT, TRIG_PIN, ECHO_PORT, ECHO_PIN);
    sprintf(buffer, "TEL LEFT %hu RIGHT %hu DIST %hu MODE AUTO_SCAN\n", dist, dist, dist);
    sendText(buffer);
}
int smartCarMain(void) {
    char line[64]; u8 length = 0U; u32 lastTelemetry = 0U;
    MRCC_voidInit(); MSYSTICK_voidInit();
    peripheralsInit();
    outputPin(L1_PORT, L1_PIN); outputPin(L2_PORT, L2_PIN); outputPin(R1_PORT, R1_PIN); outputPin(R2_PORT, R2_PIN);
    outputPin(TRIG_PORT, TRIG_PIN); inputPin(ECHO_PORT, ECHO_PIN);
    outputPin(HEADLIGHT_PORT, HEADLIGHT_PIN);
    outputPin(REVERSE_LED_PORT, REVERSE_LED_PIN);
    outputPin(BUZZER_PORT, BUZZER_PIN);
    outputPin(ONBOARD_LED_PORT, ONBOARD_LED_PIN);

    MGPIO_voidSetPinValue(HEADLIGHT_PORT, HEADLIGHT_PIN, LOW);
    MGPIO_voidSetPinValue(REVERSE_LED_PORT, REVERSE_LED_PIN, LOW);
    MGPIO_voidSetPinValue(BUZZER_PORT, BUZZER_PIN, LOW);
    /* Turn ON BlackPill onboard Blue LED (active LOW) to indicate board is ALIVE */
    MGPIO_voidSetPinValue(ONBOARD_LED_PORT, ONBOARD_LED_PIN, LOW);

    stopMotors(); sendText("STM READY\n");
    while (1) {
        char received;
        while (readChar(&received) != 0U) {
            if (received == '\n' || received == '\r') { line[length] = '\0'; if (length > 0U) handleCommand(line); length = 0U; }
            else if (length < sizeof(line) - 1U) line[length++] = received;
        }
        systemMs++;
        if ((systemMs - lastCommandMs) > COMMAND_TIMEOUT_MS) stopMotors();
        if ((systemMs - lastTelemetry) >= 300U) {
            lastTelemetry = systemMs;
            sendTelemetry();
            /* Toggle blue LED with telemetry (Heartbeat) */
            static u8 hb = 0U;
            hb = !hb;
            MGPIO_voidSetPinValue(ONBOARD_LED_PORT, ONBOARD_LED_PIN, hb ? LOW : HIGH);
        }
        MSYSTICK_voidDelay_ms(1U);
    }
}
#endif
