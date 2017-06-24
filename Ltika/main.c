#include <avr/io.h>
#include <avr/delay.h>

#define F_CPU (16000000UL)    // 16MHz

void wait(void)
{
	for( int16_t i = 0; i < 1000; ++i ){
		_delay_ms(2);
	}
}


int main(void)
{
	// CPU動作周波数 16Mhz Clock Prescaler 0
	CLKPR = (1 << CLKPCE);
	CLKPR = 0x00;

	// PD2 に1を出力
	PORTD |= _BV(PORTD2);
	// PD2 を出力ピンに設定
	DDRD |= _BV(DDD2);

	while (1)
	{
		// PD2 に1を出力
		PORTD |= _BV(PORTD2);
		wait();
		// PD2 をクリア
		PORTD &= ~_BV(PORTD2);
		wait();
	}
}

