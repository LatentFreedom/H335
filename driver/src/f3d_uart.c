// File: f3d_uart.c 
// Part of: C335 stm32 Discovery board source code
// Created by: Austin Moore (aulmoore), Brandon Burzon (brburzon)
// Created on: 2/13/2016
// Last modified by: Austin Moore (aulmoore), Nick Palumbo (npalumbo)
// Last modified on: 2/23/2016
// Description: contains the initialization basic i/o functions for UART 

#include <stm32f30x.h>
#include <f3d_uart.h>
//the initialization function to call
void f3d_uart_init(void) {
  //Set up the GPIO structure
  GPIO_InitTypeDef GPIO_InitStructure;
  //Initialize the clock
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);

  //Set up Pin 5
  GPIO_StructInit(&GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOC,&GPIO_InitStructure);

  //Set up Pin 4
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOC,&GPIO_InitStructure); 

  //Finish configuration of 4 & 5
  GPIO_PinAFConfig(GPIOC,5,GPIO_AF_7);
  GPIO_PinAFConfig(GPIOC,4,GPIO_AF_7);
  
  //More initializations
  USART_InitTypeDef USART_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

  //Even more init
  USART_StructInit(&USART_InitStructure);
  USART_InitStructure.USART_BaudRate = 9600;
  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_Init(USART1 ,&USART_InitStructure);
  USART_Cmd(USART1 , ENABLE);
}
//sends a character
int putchar(int c) {
  while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == (uint16_t)RESET);
  USART_SendData(USART1, c);
  return 0;
} 
//gets a character
int getchar(void) {
  while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == (uint16_t)RESET);
  int c = USART_ReceiveData(USART1);
  return c;
}

int getchar_nopause(void)
{
  int c = 0;
  if(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) != (uint16_t)RESET){
    c = USART_ReceiveData(USART1);
  }
  return c;
}

//sends a string
void putstring(char *s) {
  //While character at s is not null char,
  while(*s != '\0')
    {
      //Putchar the char and increment
      putchar(*(s++));
    }
}



/* f3d_uart.c ends here */