/*
 * lab8_main.c: Combined Project File
 * Organizes Security System states, Ranger measurements, and RAS sensing.
 */

#include <stdint.h>
#include <stdbool.h>
#include "launchpad.h"
#include "seg7.h"
#include "ranger.h"
#include "music.h"
#include <pwm_buzzer.h>

// --- Structures and State ---

typedef enum { Millimeter, Inch } DisplayMode;

typedef struct {
    // Security/System States
    bool activated;
    bool alerted;
    bool buzzer_on;

    //Buzzer States
    int pitch;
    int volume;

    bool in_range;
    int beep_delay;

    // Display States
    DisplayMode display_mode;
} SysState;

// Single initialization of the system state
static SysState sys = {
    false,          // not activated
    false,          // not alerted
    false,          // buzzer off
    0,              //pitch
    0,              //volume
    Millimeter      // default display mode
};

// --- Event Declarations ---

// Security & UI Events
Event buzzer_play_event;
Event check_push_button_event;
Event check_motion_event;
Event play_sound_event;

// Sensor Events
Event trigger_ranger_reading_event;




// --- Callback Functions ---

void TriggerRangerReading(Event *event) {
    RangerTriggerReading();
    EventSchedule(event, event->time + 500);
}

void PlaySound(Event *event)
{

    if (sys.in_range) { //beeping effect w/delay
        // Turn on the buzzer with the current PWM setting
        if (sys.buzzer_on) {
            MusicTurnOffBuzzer();
            sys.buzzer_on = false;
        }
        else {
            MusicSetBuzzer(sys.pitch, sys.volume); //added
            sys.buzzer_on = true;
        }
        EventSchedule(&play_sound_event, event->time + sys.beep_delay);
    }
    else
    { //object is far, turn off buzzer
        // Turn off the buzzer
        if (sys.buzzer_on) {
            MusicTurnOffBuzzer(); //added
            sys.buzzer_on = false;
        }

    // Increment the position and schedule the next callback
        EventSchedule(&play_sound_event, event->time + 200);
    }
}

// --- Main Program ---

void main(void) {
    // Hardware Initialization
    LaunchPadInit();
    RangerInit();
    Seg7Init();
    MusicInitBuzzer();


    // Event Initialization
    EventInit(&trigger_ranger_reading_event, TriggerRangerReading);
    EventInit(&play_sound_event, PlaySound);

    // Schedule initial triggers
    EventSchedule(&trigger_ranger_reading_event, 100);

    // Schedule time events
    EventSchedule(&play_sound_event, 100);


    while (true) {
        asm(" wfi");

        // UI: Switch modes using buttons
        int button = PushButtonRead();

        if (button == 1 && sys.display_mode == Millimeter) {
            sys.display_mode = Inch;
        }
        else if (button == 1 && sys.display_mode == Inch) {
            sys.display_mode = Millimeter;
        }



        // Ranger Processing
        if (RangerDataReady()) {

            uint32_t cycles = RangerGetData();
            uint32_t distance = 0;
            Seg7Display disp = {{10, 10, 10, 10}, false};

            uint32_t distanceMM = (uint32_t)(cycles * 0.0034);
            if (sys.display_mode == Millimeter) {
                distance = distanceMM;
            } else {
                distance = (uint32_t)(cycles * 0.00013385);
            }

            if (distanceMM >= 0 && distanceMM < 50) { //in mm, figure out how to make universal
                sys.beep_delay = 80;
                sys.in_range = true;
                sys.pitch = 6;
                sys.volume = 2;
            }
            else if (distanceMM >= 50 && distanceMM < 115 ) {
                sys.beep_delay = 110;
                sys.in_range = true;
                sys.pitch = 6;
                sys.volume = 1;
            }
            else if (distanceMM >= 115 && distanceMM < 250 ) {
                sys.beep_delay = 140;
                sys.in_range = true;
                sys.pitch = 6;
                sys.volume = 0;
            }
            else{
                sys.in_range = false;
            }


            // Update 4-digit display
            disp.digit[0] = distance % 10;
            disp.digit[1] = (distance / 10) % 10;
            disp.digit[2] = (distance / 100) % 10;
            disp.digit[3] = (distance / 1000) % 10;

            // Suppress leading zeros
            int i = 3;
            while (i > 0) {
                if (disp.digit[i] == 0) {
                    disp.digit[i] = 10;
                } else {
                    break;
                }
                i--;
            }
            Seg7Update(&disp);
        }

        EventExecute();
    }
}
