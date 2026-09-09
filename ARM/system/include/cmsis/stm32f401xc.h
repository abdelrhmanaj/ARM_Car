/*
 * Compatibility wrapper for this SDK, which ships the STM32F407 CMSIS
 * device header but not the STM32F401xC header. The register map used by
 * the GPIO/RCC drivers in this project is common to these STM32F4 parts.
 */
#ifndef STM32F401XC_COMPAT_H_
#define STM32F401XC_COMPAT_H_

#include "stm32f407xx.h"

#endif
