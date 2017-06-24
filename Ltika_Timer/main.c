#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

void Init_Timer0A(void)
{
	// Timer Setting
	// 0.1 ms 毎に割り込みが来る

	// CTC Mode
	TCCR0A |= _BV(WGM01);
	// Clock select = (Prescaler / 8)
	TCCR0B |= _BV(CS01);
	// 200 timer clock (0-199)でコンペア
	OCR0A = 200-1;
	// Timer0 Compare Match A interrupt Enable
	TIMSK0 |= _BV(OCIE0A);
}

volatile static uint32_t gTimer0A_Counter = 0;
ISR(TIMER0_COMPA_vect)
{
	++gTimer0A_Counter;
}

void wait_ms( uint32_t ms )
{
	gTimer0A_Counter = 0;
	while( gTimer0A_Counter < ms*10 ){
		sleep_mode();
	}
}

int main(void)
{
	// CPU動作周波数 16Mhz Clock Prescaler 0
	CLKPR = (1 << CLKPCE);
	CLKPR = 0x00;

	// スリープモードの設定
	set_sleep_mode(SLEEP_MODE_IDLE);
	// 割り込み有効
	Init_Timer0A();
	sei();

	// PD2 を出力ピンに設定
	DDRD |= _BV(DDD2);

	while (1)
	{
		wait_ms(1000);
		// PD2 を反転
		PORTD ^= _BV(PORTD2);
	}
}

