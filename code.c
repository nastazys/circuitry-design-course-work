#define F_CPU 4000000UL

#include <avr/io.h> // ����/�����
#include <util/delay.h>	// ��������� ��������
#include <avr/interrupt.h> // ����������

// ����� ������������� ����������

void clear_led(void)
{
	PORTB = 0b00110000;
	PORTC = 0xFF;
}

int blink_led(int num)
{
	clear_led();
	_delay_ms(100);
	PORTB |= (1<<num);
	_delay_ms(100);
	clear_led();
	_delay_ms(100);
	PORTB |= (1<<num);
	_delay_ms(100);
	clear_led();
	_delay_ms(100);
	PORTB |= (1<<num);
	_delay_ms(100);
	clear_led();
	_delay_ms(100);
	PORTB |= (1<<num);
	_delay_ms(100);
	clear_led();
	_delay_ms(100);
	PORTB |= (1<<num);
	_delay_ms(100);
	return 0;
}

int beep(int k, int j) 		//������� ����
{
	int i = 0;
	while(i<k)
	{
		PORTD = 0b11111111; // 	����� �� ������ ������: ������ ������� ������� �� �����
		_delay_us(j);		//	���� ��������� �����
		PORTD = 0b01111111;	//	������ ������ �������
		_delay_us(j);		//	����� ����. ��� � ������ ������������������ ������������� ���������
		i++;
	}
	PORTD = 0b01111111;		// ��������� ��� �� ����� ������� :)
	return 0;
} 

void reset_wait(void)
{
	short t = 0;

	while(t==0)
	{
		if(!(PIND & (1<<PIND4)))
		{
			_delay_ms(5);
			if(!(PIND & (1<<PIND4)))
			{
				t = 1;
				clear_scr();
				clear_led();
			}
		}
	}
}

void indicate(int n)
{
	int t = 300;
	int tone = 100;
	p = beep(t, tone);
	clear_led();
	p = blink_led(n);
	reset_wait();
}


int observer(void)	//	������� ��� ������������ ������� ������
{
	short p = 0;

	while(i==0) 			//	���� ������������ ������� ������
	{ 						
		if(!(PIND & (1<<PIND0)) || !(PIND & (1<<PIND1)) || !(PIND & (1<<PIND2)) || !(PIND & (1<<PIND3)) || !(PIND & (1<<PIND6)))  
		//���� ���� �� ���� �� ������ ������� ������
		{ 					
			_delay_ms(5); 	//	������ �� ��������
			if(!(PIND & (1<<PIND0))) 	//	�������� ������� ������ ������ 1
			{ 					
				indicate(0);
			} 				
			else
			{
				if(!(PIND & (1<<PIND1))) //	�������� ������� ������ ������ 2
				{
					indicate(1);
				}
				else
				{
					if(!(PIND & (1<<PIND2))) //	�������� ������� ������ ������ 3
					{
						indicate(2);
					}
					else
					{
						if(!(PIND & (1<<PIND3))) //	�������� ������� ������ ������ 4
						{
							indicate(3);
						}
					}
				} 
			} 
		} 		
	} 			
	return i;
}


int main(void) 			
{  					
	DDRC = 0xFF; 	//	���� � �� �����
	PORTC = 0xFF; 	//	������������� ��������� ���. �� ���� �
	DDRD = 0b10000000; 	//	���� � �� ����, ����� 8� �����, ��� �� ����� � ���, ������� ���
	PORTD = 0b01111111; 	//	������������� ��������� �� ���� ���� �, ������ 8�� ����, ��� �� ��� ������� � ���.
	DDRB = 0xFF; 		//	���� � �� �����
	PORTB = 0b00110000; 	//	������������� ��������� �� 5 � 6 ���� ����� �

	short i = 0;
	while(1) 			
	{ 			
		i = observer();
	} 				
	return 0;
}