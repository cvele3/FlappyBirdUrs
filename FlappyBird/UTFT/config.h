/*
 * config.h
 *
 * Created: 2/19/2013 10:41:04 AM
 */ 


#ifndef __CONFIG_H__
#define __CONFIG_H__


#include <avr/io.h>

	
/* Configure your hardware pins here */
#define WR_PORT PORTC
#define WR_PIN	7
#define WR_PORT_DDR DDRC

#define DATA_PORT_LOW PORTA
#define DATA_PORT_HIGH PORTD
#define DATA_PORT_LOW_DDR DDRA
#define DATA_PORT_HIGH_DDR DDRD

#define RS_PORT PORTC
#define RS_PIN 6
#define RS_PORT_DDR DDRC

#define CS_PORT PORTC
#define CS_PIN 1
#define CS_PORT_DDR DDRC

#define RD_PORT PORTB
#define RD_PIN 5
#define RD_PORT_DDR DDRB

#define RESET_PORT PORTC
#define RESET_PIN 0
#define RESET_PORT_DDR DDRC

#define BACK_LIGHT_PORT PORTC
#define BACK_LIGHT_PIN 1
#define BACK_LIGHT_DDR DDRC

//#define F_CPU 12000000
#define F_CPU 7372800UL

#define HAVE_STATUS_LED 0	
#define STATUS_LED_PORT PORTB
#define STATUS_LED_PIN PORTB0
#define STATUS_LED_DDR DDRB
	
	
	
/* No touch zone follows */	
#define DPLIO _SFR_IO_ADDR(DATA_PORT_LOW)
#define DPHIO _SFR_IO_ADDR(DATA_PORT_HIGH)

#ifdef HAVE_STATUS_LED

#define status_led_on()\
STATUS_LED_PORT&=~_BV(STATUS_LED_PIN);

#define status_led_off()\
STATUS_LED_PORT|=_BV(STATUS_LED_PIN);

#define status_led_toggle()\
STATUS_LED_PORT^=_BV(STATUS_LED_PIN);

#define status_led_init()\
STATUS_LED_DDR|=_BV(STATUS_LED_PIN);

#else

#define status_led_on() {}
#define status_led_off() {}
#define status_led_toggle() {}
#define status_led_init() {}
	
#endif


#endif /* __CONFIG_H__ */