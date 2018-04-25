#include "FreeRTOS.h"
#include "task.h"
#include "irq.h"
#include "uart.h"
#include "bcm2835_intc.h"
#include "gpio.h"

#define peribase								( (unsigned long ) 0x20000000 )
#define uartbase								( (unsigned long ) peribase + 0x201000 )
#define UART0_IMSC_RXIM							( (unsigned long ) (1 << 4))
#define UART0_MIS_RXMIS							( (unsigned long ) (1 << 4))
#define UART0_FR_RXFE							( (unsigned long ) (1 << 4))
#define UART0_ICRC_RXIC							( (unsigned long ) (1 << 4))



//------------------------------------------------------------------------
inline void PUT32 ( unsigned int addr, unsigned int val ){
    *(volatile unsigned int *)addr = val;
}
//------------------------------------------------------------------------
inline unsigned int GET32 ( unsigned int addr ){
    return *(volatile unsigned int *)addr;
}

void uart_puts(const char * str){
  while(* str){
		uart_putc(* str++);
	}
}

//------------------------------------------------------------------------
void uart_putc ( unsigned int c )
{
    while(1)
    {
        if((GET32(UART0_FR)&0x20)==0) break;
    }
    PUT32(UART0_DR,c);
}
//------------------------------------------------------------------------
unsigned int uart_getc ( void )
{
    while(1)
    {
        if((GET32(UART0_FR)&0x10)==0) break;
    }
    return(GET32(UART0_DR));
}
//------------------------------------------------------------------------
void uart_init ( void )
{
    volatile unsigned int ra;

    PUT32(UART0_CR,0);

    ra=GET32(GPFSEL1);
    ra&=~(7<<12); //gpio14
    ra|=4<<12;    //alt0
    ra&=~(7<<15); //gpio15
    ra|=4<<15;    //alt0
    PUT32(GPFSEL1,ra);

    PUT32(GPPUD,0);
    for(ra=0;ra<150;ra++);
    PUT32(GPPUDCLK0,(1<<14)|(1<<15));
    for(ra=0;ra<150;ra++);
    PUT32(GPPUDCLK0,0);

    PUT32(UART0_ICR,0x7FF);
    PUT32(UART0_IBRD,1);
    PUT32(UART0_FBRD,40); 		// Set baud rate: 115200
    PUT32(UART0_LCRH,0x60);
    PUT32(UART0_CR,0x301);
}
//
__attribute__((no_instrument_function))
void vUartISR(unsigned int nIRQ, void *pParam)
{
//	portSAVE_CONTEXT();
  // DisableInterrupts();
	unsigned int mis = GET32(UART0_MIS);
	if((mis & (1<<4))){
		externalInput = uart_getc()-48;
    // char buf[10];
    // snprintf_473(buf,sizeof(buf),"\n%d",value);
    // const char* str = buf;
    // uart_puts(str);
	}
	PUT32(UART0_ICR , 0x7FF);
}

__attribute__((no_instrument_function))
void setupUARTInterrupt(void)
{
	irqBlock();
  irqDisable(57);
	SetGpioFunction(4,1);
	volatile unsigned int ra;
	PUT32(UART0_CR,0);

	ra=GET32(GPFSEL1);
  ra&=~(7<<12); //gpio14
  ra|=4<<12;    //alt0
  ra&=~(7<<15); //gpio15
  ra|=4<<15;    //alt0
  PUT32(GPFSEL1,ra);

  PUT32(GPPUD,0);
  for(ra=0;ra<150;ra++);
  PUT32(GPPUDCLK0,(1<<14)|(1<<15));
  for(ra=0;ra<150;ra++);
  PUT32(GPPUDCLK0,0);

  PUT32(UART0_ICR , 0x7FF);
  PUT32(UART0_IMSC, (1<<4));
  PUT32(UART0_IBRD,1);
  PUT32(UART0_FBRD,40); 		// Set baud rate: 115200
  PUT32(UART0_LCRH,0x60);
  PUT32(UART0_CR,0x301);

  // EnableGpioDetect(14,DETECT_HIGH);
  // pdTASK_CODE test1 = vUartISR;
  // pdTASK_CODE test2 = uart_init;
  // char bof[28];
  // snprintf_473(bof, sizeof(bof), "\nISR:0x%08x\nInit:0x%08x", test1,NULL);
  // const char * strs = bof;
  // uart_puts(strs);

	irqRegister(57,vUartISR, NULL);
  irqRegister(83,vUartISR, NULL);
	irqEnable(57);
  irqEnable(83);
	irqUnblock();
}
// //------------------------------------------------------------------------
// void hexstrings ( unsigned int d )
// {
//     //unsigned int ra;
//     unsigned int rb;
//     unsigned int rc;

//     rb=32;
//     while(1)
//     {
//         rb-=4;
//         rc=(d>>rb)&0xF;
//         if(rc>9) rc+=0x37; else rc+=0x30;
//         uart_putc(rc);
//         if(rb==0) break;
//     }
//     uart_putc(0x20);
// }
// //------------------------------------------------------------------------
// void hexstring ( unsigned int d )
// {
//     hexstrings(d);
//     uart_putc(0x0D);
//     uart_putc(0x0A);
// }
// //------------------------------------------------------------------------
// int notmain ( unsigned int earlypc )
// {
//     unsigned int ra;

//     uart_init();
//     hexstring(0x12345678);
//     hexstring(earlypc);

//     for(ra=0;ra<30000;ra++)
//     {
//         uart_putc(0x30|(ra&7));
//     }

//     for(ra=0;ra<100;ra++) uart_putc(0x55);

//     //probably a better way to flush the rx fifo.  depending on if and
//     //which bootloader you used you might have some stuff show up in the
//     //rx fifo.
//     while(1)
//     {
//         if(GET32(UART0_FR)&0x10) break;
//         GET32(UART0_DR);
//     }

//     while(1)
//     {
//         ra=uart_getc();
//         if(ra==0x0D) uart_putc(0x0A);
//         uart_putc(ra);
//     }


//     return(0);
// }
//-------------------------------------------------------------------------
//-------------------------------------------------------------------------


//-------------------------------------------------------------------------
//
// Copyright (c) 2012 David Welch dwelch@dwelch.com
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//
//-------------------------------------------------------------------------
