#include <avr/io.h>
#include <util/delay.h>

#define LED_PIN 5

int main() {
	DDRB |= 1 << LED_PIN;	
	while(1) {
		PORTB |= 1 << LED_PIN;
		_delay_ms(1000);
		PORTB &= ~(1 << LED_PIN);
		_delay_ms(1000);
	}
	return 0;
}
