/*
 * 
 *
 * pwmled.c: PWM LED functions
 *
 * Base Framework: Zhao Zhang
 * Author / Modifications: Julises Mercado
 */

#include <pwm_buzzer.h>

/*
 * Initialize the timer PWM functions connected to the buzzer
 *    Buzzer: 		PC5, T2CCP1, Timer 2, sub-timer B
 */
void PwmBuzzerInit()
{
    // Enable Timer 2 and TGPIO Port C
    SysCtlPeripheralEnable(SYSCTL_PERIPH_WTIMER0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);

    // Connect pin to the timer
    GPIOPinTypeTimer(GPIO_PORTC_BASE, GPIO_PIN_5);
    GPIOPinConfigure(GPIO_PC5_WT0CCP1);

    // Select PWM for Timer 2 sub-Timer B
    TimerConfigure(WTIMER0_BASE, (TIMER_CFG_SPLIT_PAIR | TIMER_CFG_B_PWM));


    // Invert the PWM waveform, so that the Match register value is the pulse width.
    // Otherwise, the pulse width will be (Load value) - (Match value).
    // The inversion is done by enabling output inversion on the PWM pins.
    TimerControlLevel(WTIMER0_BASE, TIMER_B, true /* output inversion */);

    // Enable the Timer 2's TimerB
    TimerEnable(WTIMER0_BASE, TIMER_B);

}

/*
 * Set color of the on-board LED by setting the intensity of the three primary colors
 */
void PwmBuzzerSet(int pulse_period, int pulse_width)
{
    // Set the PWM parameters for the buzzer
    TimerLoadSet(WTIMER0_BASE, TIMER_B, pulse_period - 1);

    // Set the PWM pulse width
    TimerMatchSet(WTIMER0_BASE, TIMER_B, pulse_width - 1);

}





