﻿#include <avr/interrupt.h>
#include <avr/io.h>
#include <inttypes.h>

#include "Timer.h"
#include "UART.h"

/*	Definir el macro que calcula los ticks en base
	a al parametro de frecuencia (f). */
#define TICKS(f) 16000000/(1024/f)

const struct note *cancion;
static volatile uint8_t secFlag;
static volatile uint8_t nota;
static volatile uint8_t notaFin;
static volatile uint8_t volumen;
static char volumenChar[20];


void Timer0_Ini ( void ){
	/* 	Permanece igual, ocasionando una interrupción 
		cada 1 ms en modo CTC. */
	// 1ms: 0.001/(1/(16M/64) = 250 
	TCCR0A = (2<<WGM00);	//Iniciando CTC
	TCCR0B = (3<<CS00);		//64PS
	TCNT0  = 0;				//Iniciando contador
	OCR0A  = 250-1;			//1ms tope
	TIMSK0 = (1<<OCIE0A);	//Habilitando interrupcion
	sei();					//Habilitando interrupciones gobales
}

ISR(TIMER0_COMPA_vect){ 
	/* 	Código para actualizar bandera de segundos */
	static uint16_t mSeg;
	mSeg++;
	
	/*	Agregar las instrucciones necesarias para reproducir
		la siguiente nota en el arreglo dependiendo de la duración, 
		e insertar los silencios entre cada nota. */
	
	mSeg++;
	/*if( mSeg >= delay ){
		mSeg = 0;
		if(silencio && nota != notaFin){					//Si el indice es diferente al tam de la estructura
			silencio = 0;									//apagar band , actualizar
			Timer2_Freq_Gen(TICKS(*cancion[nota].freq));	//frec igual a notas x ticks.
			delay = *cancion[nota].delay;					//El delay es la nota x 10ms
			nota++;
		}
		else {												//Si el indice es igual al tam
			silencio = 1;
			Timer2_Freq_Gen(0);
			delay = SILENCE;
		}
	}*/
}

void Timer2_Freq_Gen(uint8_t ticks){
	/* 	Si "ticks" es mayor que 0 entonces, inicializa y habilita el Generador 
		de Frecuencia del Timer2 con el tope dado por "ticks".
		De lo contrario se requiere deshabilitar el Generador, generando de 
		esta forma el silencio (0 lógico). */
		
	if(ticks > 0)
	{
		TCCR2A = (3<<WGM20);						 //PWM OCR2A TOP
		TCCR2B = (6<<CS20)|(2<<COM2B0)|(1<<WGM22);	 //256PS, COM2B0 non inverting
		OCR2A  = ticks -1 ;							 //Tope en OCR2A
	}else
	{
		TCCR2B &= ~(6<<CS20);			//Deshabilitando PS para 0 logico
	}
}

void Timer2_Play(const struct note song[],unsigned int len)
{	
	/*	Función que establece las condiciones necesarias para que
		el generador recorra el arreglo de notas. */
	
	cancion = &song[0];
	nota = 0;				//obteniendo principio de musica
	notaFin = len;			//Obteniendo final de musica
	volumen = 50;			//Volumen a la mitad
	DDRH = (1<<DDH6);		//PB4 como salida
	
	UART0_puts(itoa(*cancion->delay));
}

void Timer2_Volume(int8_t direction){
	/* 	Ajusta el ciclo de trabajo para incrementar o decrementar  el volumen
	   	de las notas que se estan generando. */
	
	if(volumen <= 80)
	{
		volumen += 10;
	}
	else if( volumen >= 10)
	{
		volumen -= 10;	
	}
	
	OCR2B = (OCR2A*volumen)/100;
	itoa(volumenChar,OCR2B,10);
	UART0_puts("\n\r\n\rVolumen: ");
	UART0_puts(volumenChar);
}