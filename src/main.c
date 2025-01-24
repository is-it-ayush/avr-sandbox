#ifndef F_CPU
#define F_CPU 16000000
#endif

#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>

//  The code is private to the simulator. It doesn't
//  exist in the compiled firmware.
#include "simavr/simavr/sim/avr/avr_mcu_section.h"
AVR_MCU(F_CPU, "atmega8a");
AVR_MCU_VCD_FILE("debug_trace.vcd", 1000);
const struct avr_mmcu_vcd_trace_t _mytrace[] _MMCU_ = {
    // name of the trace, what variable to track, and what bit to track
    {AVR_MCU_VCD_SYMBOL("PB0"), .what = (void*)&PORTB, .mask = (1 << PB0)},
};

// This function toggles the LED on and off every 500ms.
//
// DDRx: Data Direction Register x where x is the port name
// from A to D. It is used to set the direction of the pins
// or a ping in the port as input or output. 1 means output,
// 0 means input.
// --
// PORTx: Port Output Register x where x is the port name
// from A to D. It is used to put pins or pin either
// high or low. 1 means high, 0 means low.
void toggle_led() {
  DDRB |= (1 << PB0); // set PB0 as output
  while(1) {
    PORTB ^= (1 << PB0); // toggle PB0
    _delay_ms(500);
  }
}

// This function enables the timer0 overflow interrupt and
// toggles the LED on and off every 5 seconds using the
// timer0 overflow interrupt.
//
// ATMega8: 3 timers,
//  timer0 : 8-bit timer;
//  timer1 : 16-bit timer;
//  timer2 : 8-bit timer
//
//
// TCCRnx: Timer/Counter Control Register n where n is the timer
// number and x is the channel if applicable.
// --
// TIMSK: Timer/Counter Interrupt Mask Register. Used to enable
// timer interrupts.
//
// timer0 overflow time => (prescaler * 256) / F_CPU
// (1024 * 256) / 16000000 = 0.016384s
// timer0 frequency = 1 / 0.016384 = 61.04Hz
// Thus it'll overflow 61.04 times per second
//
// We can thus use this to create a precise delay, say
// we want to do an action every 5 seconds. thus
// 5 * 61.04 = 305.2 overflows
ISR(TIMER0_OVF_vect) {
  static uint16_t timer0_overflow_count = 0;
  timer0_overflow_count++;
  if(timer0_overflow_count >= 305) { // 5 seconds: 305 overflows
    PORTB ^= (1 << PB0);             // toggle PB0
    timer0_overflow_count = 0;
  }
}
void timer0_overflow_interrupt() {
  DDRB |= (1 << PB0); // set PB0 as output; LED

  TCCR0 |= (1 << CS02) | (1 << CS00); // 1024 prescaler
  TIMSK |= (1 << TOIE0);              // enable timer0 overflow interrupt
  sei();                              // enable global interrupts
}

int main(void) {
  while(1) {
  }
}
