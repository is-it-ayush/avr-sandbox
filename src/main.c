#ifndef F_CPU
  #define F_CPU 16000000
#endif

#include <avr/io.h>
#include <util/delay.h>

//  The code is private to the simulator. It doesn't
//  exist in the compiled firmware.
#include "simavr/simavr/sim/avr/avr_mcu_section.h"
AVR_MCU(F_CPU, "atmega8a");
AVR_MCU_VCD_FILE("debug_trace.vcd", 1000);
const struct avr_mmcu_vcd_trace_t _mytrace[]  _MMCU_ = {
    { AVR_MCU_VCD_SYMBOL("PORTB"), .what = (void*)&PORTB, .mask = (1 << PB0) },
    { AVR_MCU_VCD_SYMBOL("DDRB"), .what = (void*)&DDRB, .mask = (1 << DDB0) },
};

int main(void) {
    // Set PB0 as output
    DDRB |= (1 << PB0);

    while (1) {
        // Toggle PB0
        PORTB ^= (1 << PB0);

        // Delay
        _delay_ms(100);

    }

    return 0;
}
