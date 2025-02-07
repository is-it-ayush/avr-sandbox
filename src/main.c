#ifndef F_CPU
#define F_CPU 16000000
#endif

#include <avr/eeprom.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

//  The code is private to the simulator. It doesn't
//  exist in the compiled firmware.
#include "simavr/simavr/sim/avr/avr_mcu_section.h"
AVR_MCU(F_CPU, "atmega8a");
AVR_MCU_VCD_FILE("debug.vcd", 1000);
const struct avr_mmcu_vcd_trace_t _mytrace[] _MMCU_ = {
    // name of the trace, what variable to track, and what bit to track
    {AVR_MCU_VCD_SYMBOL("PB3"), .what = (void*)&PORTB, .mask = (1 << PB3)},
    {AVR_MCU_VCD_SYMBOL("TCNT2"), .what = (void*)&TCNT2},
    {AVR_MCU_VCD_SYMBOL("OCR2"), .what = (void*)&OCR2},
};

// This function toggles the LED on and off every 500ms.
// ---------------------------------------------------------
// void toggle_led() {
//   DDRB |= (1 << PB0); // set PB0 as output
//   while(1) {
//     PORTB ^= (1 << PB0); // toggle PB0
//     _delay_ms(500);
//   }
// }

// This function activates the Timer0 in Normal Mode (0b00) with
// a 1024 prescaler. It generates an overflow every 16.384ms
// or 61.03Hz which toggles the LED at 0.2Hz or every 5s.
// ---------------------------------------------------------
// Timer Frequency:
//  Max Possible: (1024 * 256) / 16000000 = 0.016384s or 16.38ms or 61.03Hz
//  Generated:
//  - 5s = 1/5s = 0.2Hz
//  - 61.03Hz / 0.2Hz = 305.15 overflows in 5s
// ---------------------------------------------------------
// ISR(TIMER0_OVF_vect) {
//   static uint16_t timer0_overflow_count = 0;
//   timer0_overflow_count++;
//   if(timer0_overflow_count >= 305) {
//     PORTB ^= (1 << PB0);
//     timer0_overflow_count = 0;
//   }
// }
// void timer0_overflow_interrupt() {
//   DDRB |= (1 << PB0); // set PB0 as output; LED
//
//   TCCR0 |= (1 << CS02) | (1 << CS00); // 1024 prescaler
//   TIMSK |= (1 << TOIE0);              // enable timer0 overflow interrupt
// }

// This function activates the Timer2 in Phase Correct PWM mode
// (0b01) with a 1024 prescaler. It generates a square wave
// on PB3 (OC2) at 30.51Hz with a 50% duty cycle.
// ---------------------------------------------------------
// Timer Frequency: (1024 * 512) / 16000000 = 0.032768s or 32ms or 30.51Hz
// PWM Frequency: Same as Timer Frequency = 30.51Hz
//---------------------------------------------------------
// void generate_square() {
//   DDRB |= (1 << PB3);
//   // phase correct PWM mode; clear OC2 on compare match; 1024 prescaler
//   TCCR2 |=
//       (1 << WGM20) | (1 << COM21) | (1 << CS22) | (1 << CS21) | (1 << CS20);
//   OCR2 = 128; // 50% duty cycle
// }

// This function activates the Timer2 in Phase Correct PWM mode
// (0b01) with a no prescaler. It generates a triangle wave
// on PB3 (OC2) at 1Hz.
// ---------------------------------------------------------
// Timer Frequency: (1 * 512) / 16000000 = 0.000032s or 32us or 31.25kHz
// PWM Frequency:
//  Max Possible: 31.25kHz / 512 = 61.03Hz
//  Generated:
//  - 1s = 1Hz
//  - 1s / 512 = 0.001953125s or 1.95ms or 512Hz
//  - 31.25kHz / 512Hz = 61.03 overflows in 1s
// ---------------------------------------------------------
// ISR(TIMER2_OVF_vect) {
//   static uint16_t timer2_ovf_count = 0;
//   timer2_ovf_count++;
//
//   static int8_t duty_direction = 1;
//   if(timer2_ovf_count >= 61) {
//     OCR2 += duty_direction;
//     timer2_ovf_count = 0;
//     // at start/half duty cycle, change direction
//     if(OCR2 == 0 || OCR2 == 255) {
//       duty_direction *= -1;
//     }
//   }
// }
// void generate_triangle() {
//   DDRB |= (1 << PB3);
//   // phase correct PWM mode; clear OC2 on compare match; no prescaler
//   TCCR2 |= (1 << WGM20) | (1 << COM21) | (1 << CS20);
//   TIMSK |= (1 << TOIE2);
//   OCR2 = 0;
// }

// This function activates activates Timer2 in Fast PWM mode
// (0b11) with no prescaler. It generates a sawtooth waveform
// on PB3 (OC2) at 100Hz or every 0.01s.
// ---------------------------------------------------------
// Timer Frequency: (1*256) / 16000000 = 0.000016s or 16us or 62.5kHz
// PWM Frequency:
//  Max Possible: 62.5kHz / 256 = 244.14Hz
//  Generated:
//  - 0.01s = 1/0.01s = 100Hz (8ms or 125Hz = 1 sawtooth)
//  - 0.01 / 256 = 0.0000390625s or 39.06us or 25.6kHz (32us or 31.25kHz)
//  - 62.5kHz / 25.6kHz = 2.44140625 or 2.44 overflows in 0.01s (2 overflows)
// ---------------------------------------------------------
// ISR(TIMER2_OVF_vect) {
//   static uint16_t timer2_ovf_count = 0;
//   timer2_ovf_count++;
//
//   if(timer2_ovf_count >= 2) {
//     OCR2++;
//     timer2_ovf_count = 0;
//   }
// }
// void generate_sawtooth() {
//   DDRB |= (1 << PB3);
//   // fast PWM mode; clear OC2 on compare match; no prescaler
//   TCCR2 |= (1 << WGM21) | (1 << WGM20) | (1 << COM21) | (1 << CS20);
//   TIMSK |= (1 << TOIE2);
//   OCR2 = 0;
// }

// This function activates Timer2 in Fast PWM mode (0b11) with
// no prescaler. It generates a sine wave on PB3 (OC2) at 100Hz
// ---------------------------------------------------------
// Timer Frequency: (1*256) / 16000000 = 0.000016s or 16us or 62.5kHz
// PWM Frequency:
// - Max Possible: 62.5kHz / 256 = 244.14Hz
// - Generated:
//   - 0.01s = 1/0.01s = 100Hz (8ms or 125Hz = 1 sine)
//   - 0.01 / 256 = 0.0000390625s or 39.06us or 25.6kHz (32us or 31.25kHz)
//   - 62.5kHz / 25.6kHz = 2.44140625 or 2.44 overflows in 0.01s (2 overflows)
// ---------------------------------------------------------
// The table is generated from the formula:
// 127 + 127 * sin(2 * pi * x / 256) where 0 <= x < 256
// const uint8_t sine_table[] PROGMEM = {
//     127, 130, 133, 136, 139, 142, 145, 148, 151, 154, 157, 160, 163, 166, 169,
//     172, 175, 178, 181, 184, 186, 189, 192, 194, 197, 200, 202, 205, 207, 209,
//     212, 214, 216, 218, 221, 223, 225, 227, 229, 230, 232, 234, 235, 237, 239,
//     240, 241, 243, 244, 245, 246, 247, 248, 249, 250, 250, 251, 252, 252, 253,
//     253, 253, 253, 253, 254, 253, 253, 253, 253, 253, 252, 252, 251, 250, 250,
//     249, 248, 247, 246, 245, 244, 243, 241, 240, 239, 237, 235, 234, 232, 230,
//     229, 227, 225, 223, 221, 218, 216, 214, 212, 209, 207, 205, 202, 200, 197,
//     194, 192, 189, 186, 184, 181, 178, 175, 172, 169, 166, 163, 160, 157, 154,
//     151, 148, 145, 142, 139, 136, 133, 130, 126, 123, 120, 117, 114, 111, 108,
//     105, 102, 99,  96,  93,  90,  87,  84,  81,  78,  75,  72,  69,  67,  64,
//     61,  59,  56,  53,  51,  48,  46,  44,  41,  39,  37,  35,  32,  30,  28,
//     26,  24,  23,  21,  19,  18,  16,  14,  13,  12,  10,  9,   8,   7,   6,
//     5,   4,   3,   3,   2,   1,   1,   0,   0,   0,   0,   0,   0,   0,   0,
//     0,   0,   0,   1,   1,   2,   3,   3,   4,   5,   6,   7,   8,   9,   10,
//     12,  13,  14,  16,  18,  19,  21,  23,  24,  26,  28,  30,  32,  35,  37,
//     39,  41,  44,  46,  48,  51,  53,  56,  59,  61,  64,  67,  69,  72,  75,
//     78,  81,  84,  87,  90,  93,  96,  99,  102, 105, 108, 111, 114, 117, 120,
//     123
// };
//
// ISR(TIMER2_OVF_vect) {
//   static uint16_t timer2_comp_count = 0;
//   timer2_comp_count++;
//
//   static uint8_t sine_index = 0;
//   if(timer2_comp_count >= 2) {
//     OCR2 = pgm_read_byte(&sine_table[sine_index]);
//     sine_index++;
//     if(sine_index >= 256) {
//       sine_index = 0;
//     }
//     timer2_comp_count = 0;
//   }
// }
// void generate_sine() {
//   DDRB |= (1 << PB3);
//   // fast PWM mode; clear OC2 on compare match; no prescaler
//   TCCR2 |= (1 << WGM21) | (1 << WGM20) | (1 << COM21) | (1 << CS20);
//   TIMSK |= (1 << TOIE2);
//   OCR2 = 0;
// }

// This function activates the ADC in single conversion mode
// with an AVCC reference and a 128 prescaler. It reads the
// ADC value from the ADC register and returns it.
// ---------------------------------------------------------
// ADC Frequency: 16000000 / 128 = 125kHz
// ---------------------------------------------------------
// void adc_init_sc() {
//   ADMUX = (1 << REFS0); // AVCC with external capacitor at AREF pin
//   ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) |
//            (1 << ADPS0); // enable, prescaler 128
// }
// uint16_t adc_read() {
//   ADCSRA |= (1 << ADSC); // start conversion
//   while(ADCSRA & (1 << ADSC))
//     ; // wait for conversion to complete
//   uint16_t adc_value = ADC;
//   return adc_value;
// }

int main(void) {
  sei(); // enable global interrupts
  while(1) {
  }
}
