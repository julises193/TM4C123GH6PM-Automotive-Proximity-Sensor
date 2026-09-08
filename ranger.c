/*
 * ranger.c
 */

#include "ranger.h"


typedef struct {
    uint32_t rising_edge_time;
    uint32_t falling_edge_time;
    enum {
        IDLE,
        EXPECT_START_EVENT_1,
        EXPECT_START_EVENT_2,
        EXPECT_DATA_EVENT_1,
        EXPECT_DATA_EVENT_2
    } state;
    bool new_data_ready;
} RangerState;

volatile RangerState ranger;

void RangerISR() {
    uint32_t current_time = TimerValueGet(WTIMER2_BASE, TIMER_B);

    switch (ranger.state) {
    case EXPECT_START_EVENT_1:
        ranger.state = EXPECT_START_EVENT_2;
        break;

    case EXPECT_START_EVENT_2:
        // Part 2: Reconfigure to Time Capture Mode
        TimerDisable(WTIMER2_BASE, TIMER_B);
        TimerConfigure(WTIMER2_BASE, TIMER_CFG_SPLIT_PAIR | TIMER_CFG_B_CAP_TIME);
        TimerLoadSet(WTIMER2_BASE, TIMER_B, 0xFFFFFFFF);    //set load to max
        TimerEnable(WTIMER2_BASE, TIMER_B);
        ranger.state = EXPECT_DATA_EVENT_1;
        break;

    case EXPECT_DATA_EVENT_1:
        ranger.rising_edge_time = current_time; //record start time for rising edge of echo pulse
        ranger.state = EXPECT_DATA_EVENT_2;
        break;

    case EXPECT_DATA_EVENT_2:
        ranger.falling_edge_time = current_time; //record end time for falling edge of echo pulse
        ranger.new_data_ready = true; //data is now ready
        ranger.state = IDLE;
        break;
    }
    TimerIntClear(WTIMER2_BASE, TIMER_CAPB_EVENT);
}

void RangerInit() {
    ranger.new_data_ready = false;
    ranger.state = IDLE;

    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_WTIMER2);

    GPIOPinTypeTimer(GPIO_PORTD_BASE, GPIO_PIN_1);
    GPIOPinConfigure(GPIO_PD1_WT2CCP1);

    TimerControlLevel(WTIMER2_BASE, TIMER_B, true);
    TimerControlEvent(WTIMER2_BASE, TIMER_B, TIMER_EVENT_BOTH_EDGES);

    TimerIntRegister(WTIMER2_BASE, TIMER_B, RangerISR);
    TimerIntEnable(WTIMER2_BASE, TIMER_CAPB_EVENT);
}

void RangerTriggerReading() {
    ranger.new_data_ready = false;
    ranger.state = EXPECT_START_EVENT_1;

    TimerDisable(WTIMER2_BASE, TIMER_B);
    TimerConfigure(WTIMER2_BASE, TIMER_CFG_SPLIT_PAIR | TIMER_CFG_B_PWM);
    TimerLoadSet(WTIMER2_BASE, TIMER_B, 1000); // 20us total period
    TimerMatchSet(WTIMER2_BASE, TIMER_B, 500);  // 10us pulse width
    TimerIntClear(WTIMER2_BASE, TIMER_CAPB_EVENT);
    TimerEnable(WTIMER2_BASE, TIMER_B);
}

bool RangerDataReady() {
    return ranger.new_data_ready;
}

uint32_t RangerGetData() {
    // Timer counts DOWN in Time Capture mode by default
    return (ranger.rising_edge_time - ranger.falling_edge_time);
}

