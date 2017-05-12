/*	Definir el macro que calcula los ticks en base
	a al parametro de frecuencia (f). */
#define TICKS(f) 0.001/(1/(f/64))

static volatile uint8_t secFlag;

void Timer0_Ini ( void ){
	/* 	Permanece igual, ocasionando una interrupción 
		cada 1 ms en modo CTC. */
	// 1ms: 0.001/(1/(16M/64) = 250 
	TCCR0A = (2<<WGM00);	//Iniciando CTC
	TCCR0B = (3<<CS00);		//64PS
	TCNT0  = 0;				//Iniciando contador
	OCR0A  = 250-1;			//1ms tope
	TIMSK0 = (1<<OCIE0A)	//Habilitando interrupcion
}

ISR(TIMER0_COMPA_vect){ 
	/* 	Código para actualizar bandera de segundos */
	static uint16_t mSeg;
	
	mSeg++;
	if(mSeg == 1000)
	{
		secFlag++;
		mSeg = 0;
	}
	

	/*	Agregar las instrucciones necesarias para reproducir
		la siguiente nota en el arreglo dependiendo de la duración, 
		e insertar los silencios entre cada nota. */
}

void Timer2_Freq_Gen(uint8_t ticks){
	/* 	Si "ticks" es mayor que 0 entonces, inicializa y habilita el Generador 
		de Frecuencia del Timer2 con el tope dado por "ticks".
		De lo contrario se requiere deshabilitar el Generador, generando de 
		esta forma el silencio (0 lógico). */
	if(ticks > 0)
	{
		DDRB = (1<<DDB4);		//PB4 como salida
		PORTB &= ~(1<<PORTB4);	//PB4 como salida
		
	}else
	{
		
	}
}

void Timer2_Play(const struct note song[],unsigned int len)
{
	
	/*	Función que establece las condiciones necesarias para que
		el generador recorra el arreglo de notas. */
	
}

void Timer2_Volume(int8_t direction){
	/* 	Ajusta el ciclo de trabajo para incrementar o decrementar  el volumen
	   	de las notas que se estan generando. */
}