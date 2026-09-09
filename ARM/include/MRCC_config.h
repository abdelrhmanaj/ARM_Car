/*
 * MRCC_config.h
 *
 *  Created on: Aug 20, 2026
 *      Author: Abdelrahman
 */

#ifndef MRCC_CONFIG_H_
#define MRCC_CONFIG_H_


/*
 * Choose between:
 *
 * 1- HSI_CLOCK
 * 2- HSE_CLOCK
 */

#define SYSTEM_CLOCK HSI_CLOCK


/*
 * Choose between:
 *
 * 1- RC_CLOCK
 * 2- MECHANICAL_CLOCK
 */

#define HSE_OSC MECHANICAL_CLOCK


#endif /* MRCC_CONFIG_H_ */
