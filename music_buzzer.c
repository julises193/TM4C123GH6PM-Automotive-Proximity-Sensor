

#include "music.h"
#include "pwm_buzzer.h"

int buzzer_period[NUM_PITCH] = //calculated by using 50,000,000/freq
{
     191110, //C4
     170265, //D4
     151685, //E4
     143172, //F4
     127551, //G4
     113636, //A4
     101239, //B4
};

float buzzer_volume[NUM_VOLUME_LEVEL] = //aka duty cycle
{
    0.05,   // 5%
    0.20,   //20%
    0.50,   //50%

};



/*d
 * Initialize the buzzer
 */
void MusicInitBuzzer()
{
    PwmBuzzerInit();
}


void MusicSetBuzzer(int pitch, int volume)
{
    int period = buzzer_period[pitch];
    int pulse_width = period * buzzer_volume[volume]; //pulseW = period*duty cycle


    PwmBuzzerSet(period, pulse_width);
}

/*
 * Turn off the Buzzer
 */
void MusicTurnOffBuzzer()
{
    PwmBuzzerSet(1000,0);
}
