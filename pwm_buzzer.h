/*
 * Starter code for ECE 266 Lab 7, Sound Player (PWM), spring 2026
 *
 * pwmled.c: PWM BUZZER functions
 *
 * Base Framework: Zhao Zhang
 * Author / Modifications: Julises Mercado
 */

#ifndef PWM_BUZZER_H_
#define PWM_BUZZER_H_

#include <stdbool.h>
#include <stdint.h>
#include <inc/hw_memmap.h>
#include <inc/hw_timer.h>
#include <driverlib/sysctl.h>
#include <driverlib/gpio.h>
#include <driverlib/pin_map.h>
#include <driverlib/timer.h>

/*
 * Initialize the timer PWM functions connected to the Buzzer.
 */
void PwmBuzzerInit();

/*
 * Set the PWM parameters for the buzzer.
 */
void PwmBuzzerSet(int pulsePeriod, int pulseWidth);

#endif /* PWDBUZZER_H_ */
