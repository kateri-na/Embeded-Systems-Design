#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

//обработчик прерывания PCINT0
#if defined(__AVR_ATtiny24__) || defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny84__)
    ISR(PCINT1_vect, ISR_ALIASOF(PCINT0_vect));
    ISR(PCINT0_vect)
    {
        _delay_ms(50);//антидребезг, чтобы прерывание не сработало несколько раз при однократном нажатии
        if( (PIN_KEY_SWITCH & (1<<KEY_SWITCH) ) ==0 ){ //если нажат ключ и схема замкнута

            if( (PIN_BUTTON1 & (1<<BUTTON1) ) == 0){
                while ((PIN_BUTTON1 & (1<<BUTTON1) ) == 0) {} //ждём отпускания кнопки
                PORT_LED1 ^= (1<<LED1); //инвертируем состояние светодиода            
            }

            if( (PIN_BUTTON2 & (1<<BUTTON2) ) == 0){
                while ((PIN_BUTTON1 & (1<<BUTTON2) ) == 0) {} //ждём отпускания кнопки
                PORT_LED2 ^= (1<<LED2); //инвертируем состояние светодиода
            }

            if( (PORT_LED1 & (1<<LED1)) && (PORT_LED2 & (1<<LED2))){
                PORT_LED3 |= (1<<LED3);
            }
            else {
                PORT_LED3 &= ~(1<<LED3);
            }   

        }
        if((PIN_KEY_SWITCH & (1<<KEY_SWITCH)) != 0){
            PORT_LED1 &= ~(1<<LED1);
            PORT_LED2 &= ~(1<<LED2);
            PORT_LED3 &= ~(1<<LED3);
        }
    }

#elif defined(__AVR_ATtinny13__) || defined(__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
ISR(PCINT0_vect)
{
    _delay_ms(50);//антидребезг, чтобы прерывание не сработало несколько раз при однократном нажатии
    if( (PIN_KEY_SWITCH & (1<<KEY_SWITCH) ) ==0 ){ //если нажат ключ и схема замкнута

        if( (PIN_BUTTON1 & (1<<BUTTON1) ) == 0){
            while ((PIN_BUTTON1 & (1<<BUTTON1) ) == 0) {} //ждём отпускания кнопки
            PORT_LED1 ^= (1<<LED1); //инвертируем состояние светодиода            
        }

        if( (PIN_BUTTON2 & (1<<BUTTON2) ) == 0){
            while ((PIN_BUTTON1 & (1<<BUTTON2) ) == 0) {} //ждём отпускания кнопки
            PORT_LED2 ^= (1<<LED2); //инвертируем состояние светодиода
        }

        if( (PORT_LED1 & (1<<LED1)) && (PORT_LED2 & (1<<LED2))){
            PORT_LED3 |= (1<<LED3);
        }
        else {
            PORT_LED3 &= ~(1<<LED3);
        }   
    }
    if((PIN_KEY_SWITCH & (1<<KEY_SWITCH)) != 0){
        PORT_LED1 &= ~(1<<LED1);
        PORT_LED2 &= ~(1<<LED2);
        PORT_LED3 &= ~(1<<LED3);
    }
}
#endif

int main(void){

    //конфигурируем пины как вход
    DDR_BUTTON1 &= ~(1<<BUTTON1);
    PORT_BUTTON1 |= (1<<BUTTON1);

    DDR_BUTTON2 &= ~(1<<BUTTON2);
    PORT_BUTTON2 |= (1<<BUTTON2);

    DDR_KEY_SWITCH &= ~(1<<BUTTON2);
    PORT_KEY_SWITCH |= (1<<BUTTON2);

    DDR_LED1 |= (1<<LED1);//конфигурируем пины как выход
    PORT_LED1 &= ~(1<<LED1);//по уполчанию выключаем светодиоды
    DDR_LED2 |= (1<<LED2);
    PORT_LED2 &= ~(1<<LED2);
    DDR_LED3 |= (1<<LED3);
    PORT_LED3 &= ~(1<<LED3);

    #if defined(__AVR_ATtiny13__) || defined(__AVR_ATtiny25__) || defined(__AVR_ATtiny45__) || defined(__AVR_ATtiny85__)
        GIMSK |= (1<<PCIE); // Разрешаем внешние прерывания PCINT.
    #elif defined(__AVR_ATtiny24__) || defined(__AVR_ATtiny44__) || defined(__AVR_ATtiny84__)
        GIMSK |= (1<<PCIE1); // Разрешаем внешние прерывания PCINT1.
        GIMSK |= (1<<PCIE0); // Разрешаем внешние прерывания PCINT0.
    #endif

    // задаем при изменении состояния каких входов будет срабатывать прерывание PCINT0
    PCMSK_BUTTON1 |= (1<<BUTTON1);
    PCMSK_BUTTON2 |= (1<<BUTTON2);
    PCMSK_KEY_SWITCH |= (1<<KEY_SWITCH);

    sei(); //разрешаем прерывания глобально
    while(1) {
        //цикл пустой, чтобы никакие задачи не выполнялись в фоновом режиме и всё работало на прерываниях
    }
}