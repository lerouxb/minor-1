/**
 * \file
 *
 * \brief Driver ISR.
 *
 (c) 2020 Microchip Technology Inc. and its subsidiaries.

    Subject to your compliance with these terms,you may use this software and
    any derivatives exclusively with Microchip products.It is your responsibility
    to comply with third party license terms applicable to your use of third party
    software (including open source software) that may accompany Microchip software.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 */

/*
 * Code generated by START.
 *
 * This file will be overwritten when reconfiguring your START project.
 * Please copy examples or other code you want to keep to a separate file
 * to avoid losing it when reconfiguring.
 */

#include <driver_init.h>
#include <compiler.h>

extern int16_t note;

unsigned int acc;
unsigned int pulse = 0;
int pulse_amount = 1;

#define MIN_PULSE 6553
#define MAX_PULSE 58983


ISR(TCA0_OVF_vect) {
    // TODO: should the pulse change even when we're not pressing a note or only when pressing a note?

    //printf("%d\r\n", note);

/*
    // forwards/backwards
    if (pulse_amount > 0) {
        if (pulse + pulse_amount > MAX_PULSE) {
            pulse_amount = -pulse_amount;
        }
    }
    else  {
        if (pulse + pulse_amount < MIN_PULSE) {
            pulse_amount = -pulse_amount;
        }
    }
    pulse += pulse_amount; // assuming PulseAmount == 1 and we're going end to end (which we're not): 65556 / 20000 == ~3.25hz
    */

    if (note == -1) {
        // middle-value == "off"
        DAC_0_set_output(127);
    }
    else {
        acc = acc + note;

        // funny how we're using a DAC and right now we're only using it to make a square wave :)
        //if (acc < pulse) {
        if (acc < 32768) {
            DAC_0_set_output(255);
        }
        else {
            DAC_0_set_output(0);
        }
    }

    /* The interrupt flag has to be cleared manually */
    TCA0.SINGLE.INTFLAGS = TCA_SINGLE_OVF_bm;
}
