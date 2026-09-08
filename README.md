# TM4C123GH6PM Automotive Proximity Detection System

About The Project:

Embedded car backup proximity sensor built on TI's TM4C123GH6PM ARM Cortex-M4 microcontroller in combination with an ultrasonic sensor to calculate distances with objects in proximity and a buzzer to provide real-time feedback. The system measures echo pulses using 32 bit wide timer interrupts to calculate distances. Uses a bit-banged 7-segment display to show distance (cm or in) and a PWM audio warning that scales pitch and duty cycle as object approaches vehicle (or vice-versa).

Built With:
* TI Tiva C Series TM4C123GH6PM Launchpad (ARM Cortex-M4F)
* Grove Ultrasonic Sensor
* TM1637 4-digit 7-segment display
* Piezoelectric Buzzer

Technical Stack:
Language: C
Execution With: Event-Driven Background Scheduler (WFI)
Protocols/Interfaces: Custom Bit-Banged Protocol (I2C-like), PWM, Interrupts (Time Capture)


Notices:

This codebase was imported from a private repo used during development. For comprehensive portfolio organization, the project history was encapsulated into a one-commit release.

I (Julises Mercado) devised the application logic, event driven scheduling, PWM, Ultrasonic drivers, and display, however, the core MCU board definitions and initial lab structures are courtesy of the UIC Department of Electrical and Computer Engineering. To comply with academic distribution policies, proprietary board support packages and startup vectors are omitted from this public archive.
