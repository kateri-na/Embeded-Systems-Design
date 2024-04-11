#define F_CPU 1200000UL
#define LED1 PA0
#define LED2 PA1
#define LED3 PA2
#define BUTTON1 PB1
#define BUTTON2 PB2
#define KEY_SWITCH PB0

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

//обработчик прерывания PCINT0
ISR(PCINT1_vect){
    _delay_ms (50);//антидребезг, чтобы прерывание не сработало несколько раз при однократном нажатии
    if( (PINB & (1<<KEY_SWITCH) ) ==0 ){ //если нажат ключ и схема замкнута

        if( (PINB & (1<<BUTTON1) ) == 0){
           while ((PINB & (1<<BUTTON1) ) == 0) {} //ждём отпускания кнопки
           PORTA ^= (1<<LED1); //инвертируем состояние светодиода            
        }

        if( (PINB & (1<<BUTTON2) ) == 0){
            while ((PINB & (1<<BUTTON2) ) == 0) {} //ждём отпускания кнопки
            PORTA ^= (1<<LED2); //инвертируем состояние светодиода
        }

        if( (PORTA & (1<<LED1)) && (PORTA & (1<<LED2))){
            PORTA |= (1<<LED3);
        }
        else {
            PORTA &= ~(1<<LED3);
        }   

    }
    if((PINB & (1<<KEY_SWITCH)) != 0){
        PORTA &= ~(1<<LED1);
        PORTA &= ~(1<<LED2);
        PORTA &= ~(1<<LED3);
    }
}

int main(void){

    //конфигурируем пины как вход
    DDRB &= ~((1<<BUTTON1)|(1<<BUTTON2)|(1<<KEY_SWITCH));
    //подтягиваем входы к питающему напряжению
    PORTB |= ((1<<BUTTON1)|(1<<BUTTON2)|(1<<KEY_SWITCH));

    DDRA |= (1<<LED1);//конфигурируем пины как выход
    PORTA &= ~(1<<LED1);//по уполчанию выключаем светодиоды
    DDRA |= (1<<LED2);
    PORTA &= ~(1<<LED2);
    DDRA |= (1<<LED3);
    PORTA &= ~(1<<LED3);

    GIMSK |= (1<<PCIE1); // разрешаем внешние прерывания PCINT0 для портов А

    // задаем при изменении состояния каких входов будет срабатывать прерывание PCINT0
    PCMSK1 |= (1<<BUTTON1);
    PCMSK1 |= (1<<BUTTON2);
    PCMSK1 |= (1<<KEY_SWITCH);

    sei(); //разрешаем прерывания глобально
    while(1) {
        //цикл пустой, чтобы никакие задачи не выполнялись в фоновом режиме и всё работало на прерываниях
    }
}
