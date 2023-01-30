// *** Hardwarespecific defines ***
#include <avr/io.h>
#define cbi(reg, bitmask) reg &= ~_BV(bitmask)
#define sbi(reg, bitmask) reg |= _BV(bitmask)
#define rbi(reg, bitmask) ((reg) & _BV(bitmask))

#define pulse_high(reg, bitmask) sbi(reg, bitmask); cbi(reg, bitmask);
#define pulse_low(reg, bitmask) cbi(reg, bitmask); sbi(reg, bitmask);

#define swap(type, i, j) {type t = i; i = j; j = t;}

#define regtype volatile uint8_t
#define regsize uint8_t
