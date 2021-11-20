#include "MKL46Z4.h"

// LED (RG)
// LED_GREEN = PTD5 (pin 98)
// LED_RED = PTE29 (pin 26)

// SWICHES
// LEFT (SW1) = PTC3 (pin 73)
// RIGHT (SW2) = PTC12 (pin 88)

//volatile int was_sw1_pressed = 0;
//volatile int was_sw2_pressed = 0;
// LEFT_SWITCH (SW1) = PTC3
void sw1_ini()
{
  SIM->COPC = 0;
  SIM->SCGC5 |= SIM_SCGC5_PORTC_MASK;
  PORTC->PCR[3] |= PORT_PCR_MUX(1) | PORT_PCR_PE(1);
  GPIOC->PDDR &= ~(1 << 3);
}

//********************************************

 

 
 // hay que poner el ISF a 1 


//*******************************************

// RIGHT_SWITCH (SW2) = PTC12
void sw2_ini()
{
  SIM->COPC = 0;
  SIM->SCGC5 |= SIM_SCGC5_PORTC_MASK;
  PORTC->PCR[12] |= PORT_PCR_MUX(1) | PORT_PCR_PE(1);
  GPIOC->PDDR &= ~(1 << 12);
}

int sw1_check()
{
  return( !(GPIOC->PDIR & (1 << 3)) );
}

int sw2_check()
{
  return( !(GPIOC->PDIR & (1 << 12)) );
}

// LED_GREEN = PTD5
void led_green_init()
{
  SIM->COPC = 0;
  SIM->SCGC5 |= SIM_SCGC5_PORTD_MASK;
  PORTD->PCR[5] = PORT_PCR_MUX(1);
  GPIOD->PDDR |= (1 << 5);
  GPIOD->PSOR = (1 << 5);
}

void led_green_toggle()
{
  GPIOD->PTOR = (1 << 5);
}

// LED_RED = PTE29
void led_red_init()
{
  SIM->COPC = 0;
  SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK;
  PORTE->PCR[29] = PORT_PCR_MUX(1);
  GPIOE->PDDR |= (1 << 29);
  GPIOE->PSOR = (1 << 29);
}



void led_red_toggle(void)
{
  GPIOE->PTOR = (1 << 29);
}

// LED_RED = PTE29
// LED_GREEN = PTD5
void leds_init(void)
{
  SIM->COPC = 0;
  SIM->SCGC5 |= SIM_SCGC5_PORTD_MASK | SIM_SCGC5_PORTE_MASK;
  PORTD->PCR[5] = PORT_PCR_MUX(1);
  PORTE->PCR[29] = PORT_PCR_MUX(1);
  GPIOD->PDDR |= (1 << 5);
  GPIOE->PDDR |= (1 << 29);
  // both LEDS off after init
  GPIOD->PSOR = (1 << 5);
  GPIOE->PSOR = (1 << 29);
}


int main(void)
{
  leds_init();
  sw1_ini();
  sw2_ini();
  
  int p1 = 0;
  int p2 = 0;
  int state =0;
  //encendido el led verde solo
  led_green_toggle();
  
  while(1){
  	switch(state){
  		case 0:
  		//las dos puertas cerradas. led verde encendido, led rojo apagado
  			if (sw1_check()){
  				if(sw2_check()){
  					//se abren las dos a la vez
  					state = 3;
  					p1 = !p1;
  					p2 = !p2;
  					//encender led rojo
  					led_red_toggle();
  					//apagar verde
  					led_green_toggle();
  					while (sw2_check());
  				}else{
  					//se abre solo la 1
  					state = 2;
  					p1 = !p1;
  					//encender rojo
  					led_red_toggle();
  					//apagar verde
  					led_green_toggle();
  				};
  				while (sw1_check());
  			}else {
  				if (sw2_check()){
  					//se abre solo la 2
  					state = 1;
  					p2 = !p2;
  					//encender rojo
  					led_red_toggle();
  					//apagar verde
  					led_green_toggle();
  					while (sw2_check());
  				};
  				//no se abre ninguna, no se pulso nada no hace nada
  			}
  		case 1:
  		//primera puerta cerrada, segunda abierta. led verde apagado, led rojo encendido
  			if (sw1_check()){
  				if(sw2_check()){
  					//se abre la primera y se cierra la segunda
  					state = 2;
  					p1 = !p1;
  					p2 = !p2;
  					while (sw2_check());
  				}else{
  					//se abre la 1, las dos abiertas
  					state = 3;
  					p1 = !p1;
  				};
  				while (sw1_check());
  			}else {
  				if (sw2_check()){
  					//se cierra la segunda, las dos estan cerradas
  					state = 0;
  					p2 = !p2;
  					//encender verde
  					led_green_toggle();
  					//apagar rojo
  					led_red_toggle();
  					while (sw2_check());
  				};
  				//no se pulso nada no hace nada
  			}
  			
  			
  		case 2:
  		//primera puerta abierta, segunda cerrada. led verde apagado, led rojo encendido
  			if (sw1_check()){
  				if(sw2_check()){
  					//se abre habre la segunda y se cierra la primera
  					state = 1;
  					p1 = !p1;
  					p2 = !p2;
  					while (sw2_check());
  				}else{
  					//se cierra la 1, las dos cerradas
  					state = 0;
  					p1 = !p1;
  					//encender verde
  					led_green_toggle();
  					//apagar rojo
  					led_red_toggle();
  				};
  				while (sw1_check());
  			}else {
  				if (sw2_check()){
  					//se abre la segunda, las dos estan abiertas
  					state = 3;
  					p2 = !p2;
  					while (sw2_check());
  				};
  				//no se pulso nada no hace nada
  			}
  			
  		case 3:
  		//dos puertas abiertas, led rojo encendido
  			if (sw1_check()){
  				if(sw2_check()){
  					//se cierran las dos
  					state = 0;
  					p1 = !p1;
  					p2 = !p2;
  					//encender led verde
  					led_green_toggle();
  					//apagar rojo
  					led_red_toggle();
  					while (sw2_check());
  				}else{
  					//se cierra la 1, la segunda abierta
  					state = 1;
  					p1 = !p1;
  				};
  				while (sw1_check());
  			}else {
  				if (sw2_check()){
  					//se cierra la segunda, la primera abierta
  					state = 2;
  					p2 = !p2;
  					while (sw2_check());
  				};
  				//no se pulso nada no hace nada
  			}
  			
  			
  	}

  }
  return 0;
}
