#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

void Init_Timer0A_PWM( uint8_t pwm_ratio )
{
	// Fast PWM Mode
	TCCR0A |= _BV(COM0A1) | _BV(WGM01) | _BV(WGM00);
	// Clock select = (Prescaler / 64)
	TCCR0B |= _BV(CS01) | _BV(CS00);
	// pwm_ratio までは1を出力
	OCR0A = pwm_ratio;
	// PD6 pin を出力に設定
	DDRD |= _BV(DDD6);
	// pullup 有効
	PORTD |= _BV(PORTD6);
}

void SetRatio_Timer0A_PWM( uint8_t pwm_ratio )
{
	OCR0A = pwm_ratio;
}

void Init_Timer1A(void)
{
	// Timer Setting
	// 1 ms 毎に割り込みが来る

	// CTC Mode
	// Clock select = (Prescaler / 1)
	TCCR1B |= _BV(WGM12) | _BV(CS10);
	// 16000 timer clock (0-15999)でコンペア
	OCR1A = (16*1000)-1;
	// Timer1 Compare Match A interrupt Enable
	TIMSK1 |= _BV(OCIE1A);
}

volatile static uint32_t gTimer1A_Counter = 0;
ISR(TIMER1_COMPA_vect)
{
	++gTimer1A_Counter;
}

void wait_ms( uint32_t ms )
{
	gTimer1A_Counter = 0;
	while( gTimer1A_Counter < ms ){
		sleep_mode();
	}
}

int main(void)
{
	uint8_t pwm_ratio[] = { 255/10, 255/8, 255/6, 255/4, 255/2 };
	uint8_t ratio_idx = 0;
	// CPU動作周波数 16Mhz Clock Prescaler 0
	CLKPR = (1 << CLKPCE);
	CLKPR = 0x00;

	// スリープモードの設定
	set_sleep_mode(SLEEP_MODE_IDLE);
	// PWM 有効
	Init_Timer0A_PWM( pwm_ratio[ratio_idx] );
	Init_Timer1A();
	sei();

	while (1)
	{
		wait_ms(1000);
		++ratio_idx;
		ratio_idx = ratio_idx % sizeof(pwm_ratio);
		SetRatio_Timer0A_PWM( pwm_ratio[ratio_idx] );
	}
}

