#define F_CPU 4000000UL

#include <avr/io.h> // Ввод/вывод
#include <util/delay.h>	// временные задержки
#include <avr/interrupt.h> // прерывания

// Гасим засветившиеся светодиоды

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

int beep(int k, int j) 		//функция бипа
{
	int i = 0;
	while(i<k)
	{
		PORTD = 0b11111111; // 	Здесь мы делаем меандр: ставим высокий уровень на ножке
		_delay_us(j);		//	Ждем некоторое время
		PORTD = 0b01111111;	//	Ставим низкий уровень
		_delay_us(j);		//	Снова ждем. Вот и готова последовательность прямоугольных импульсов
		i++;
	}
	PORTD = 0b01111111;		// Завершить все же лучше тишиной :)
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


int observer(void)	//	Функция для отслеживания нажатия клавиш
{
	short p = 0;

	while(i==0) 			//	Цикл отслеживания нажатия кнопок
	{ 						
		if(!(PIND & (1<<PIND0)) || !(PIND & (1<<PIND1)) || !(PIND & (1<<PIND2)) || !(PIND & (1<<PIND3)) || !(PIND & (1<<PIND6)))  
		//Если хотя бы одна из кнопок игроков нажата
		{ 					
			_delay_ms(5); 	//	Защита от дребезга
			if(!(PIND & (1<<PIND0))) 	//	Проверка нажатия кнопки игрока 1
			{ 					
				indicate(0);
			} 				
			else
			{
				if(!(PIND & (1<<PIND1))) //	Проверка нажатия кнопки игрока 2
				{
					indicate(1);
				}
				else
				{
					if(!(PIND & (1<<PIND2))) //	Проверка нажатия кнопки игрока 3
					{
						indicate(2);
					}
					else
					{
						if(!(PIND & (1<<PIND3))) //	Проверка нажатия кнопки игрока 4
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
	DDRC = 0xFF; 	//	Порт С на выход
	PORTC = 0xFF; 	//	Подтягивающие резисторы вкл. на порт С
	DDRD = 0b10000000; 	//	порт Д на вход, кроме 8й ножки, она на выход у нас, пищалка там
	PORTD = 0b01111111; 	//	подтягивающие резисторы на весь порт Д, окромя 8го бита, ибо на нем динамик у нас.
	DDRB = 0xFF; 		//	Порт Б на выход
	PORTB = 0b00110000; 	//	Подтягивающие резисторы на 5 и 6 биты порта Б

	short i = 0;
	while(1) 			
	{ 			
		i = observer();
	} 				
	return 0;
}