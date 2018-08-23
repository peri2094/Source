#include "delay.h"
#include "usart.h"
#include "key.h"
#include "led.h"
#include "motor.h"
#include "timer.h"
#include "sudoku.h"
#include "stm32f10x_it.h"

int prob[9][9] = {
{ 5,3,0,0,7,0,0,0,0 },
{ 6,0,0,0,9,5,0,0,0 },
{ 0,9,8,0,0,0,0,6,0 },
{ 8,0,0,0,6,0,0,0,3 },
{ 4,0,0,0,0,3,0,0,0 },
{ 7,0,0,0,2,0,0,0,6 },
{ 0,6,0,0,0,0,2,8,0 },
{ 0,0,0,4,0,9,0,0,5 },
{ 0,0,0,0,8,0,0,7,9 }
};

int flag[9][9];
int locx = 0, locy = 0;
int size = 20;

int main()
{ 
  int ch, count = 0;
  
  // Main System Clock and Port I/O Setting //
  RCC_ClocksTypeDef rcc_clocks;
  RCC_Configuration();
  RCC_GetClocksFreq(&rcc_clocks);
  Delay_Active(&rcc_clocks);
  
  NVIC_Configuration();
  EXTI_Configuration();
  Motor_Configuration();
  TIM_Configuration();
  USART3_Configuration();
  KEY_Configuration();
  LED_Configuration();
  
  simple_move(400, -400);
  
  printf("System Ready!\n");

  while(1)
  {
    ch = USART_GetChar(USART3);
   
    // Vertical Line search //
    if(ch == 'a')
    {
      prev_stop();
      Axis_Control(0, 215);
      Axis_Init();
      Change_Speed();
      simple_move(-320, 0);
    }   
    
    // Vertical Move //
    else if(ch == 's')  {prev_stop(); simple_move(0, 100);}       
    
    // Horizontal Move //
    else if(ch == 'd')  {prev_stop(); simple_move(-400, 0);}
    
    // Line tracing stop//
    else if(ch == 'f')  {prev_stop(); Change_Speed();}
        
    // Move Accurately to find starting point//
    else if(ch == '(') { MotorX_PositionControl(-7); USART_SendData(USART3, 'e');}
    else if(ch == ')') { MotorX_PositionControl(7); USART_SendData(USART3, 'e');}
    else if(ch == '+') { MotorY_PositionControl(7); USART_SendData(USART3, 'e');}
    else if(ch == '-') { MotorY_PositionControl(-7); USART_SendData(USART3, 'e');}
  
    // Image processing Ready //
    else if(ch == 'g') 
    {
      //size = stepx_cnt / 180;
      //size1 = stepx_cnt / 1080;
      //printf("stepx_cnt : %d ,size1 : %d\n", stepx_cnt ,size1);
      prev_stop();
      Change_Speed();
      simple_move(10, -10);
      USART_SendData(USART3, 'q');
    }
    
    // Solution Writing and System Exit//
    else if(ch == 'o') { writing_sudoku(prob, flag, size); break;}
    
    // Giving a hint //
    else if(ch == 'h') { hint(prob, flag, size); }
    
    
    else if((ch >= '0')||(ch <= '9')) 
    {
      if((count / 9) % 2 == 0)
      { 
        prob[count / 9][count % 9] = ch;
        count++;
        
        if(count == 81)
        {
          USART_SendData(USART3, 'w');
          Axis_Control(corr_x, corr_y); 
   
          // Sudoku solving //
          for (locx = 0; locx < 9; locx++)
          {
            for (locy = 0; locy < 9; locy++) {solve(locx, locy, prob, flag);}
          }
          
          print_sudoku(prob);
          LED_On_ALL();
        }

        else 
        {
          if((count % 9) == 0) {Axis_Control(size * (8 - (count % 9)), - (size * (count / 9)));}
          else{Axis_Control(size * (count % 9), - (size * (count / 9)));}
          USART_SendData(USART3, 'q');
        }
        
      }
      
      else 
      {
        prob[count / 9][8 - (count % 9)] = ch;
        count++;
        if((count % 9) == 0) {Axis_Control(size * (count % 9), - (size * (count / 9)));}
        else {Axis_Control(size * (8 - (count % 9)), - (size * (count / 9)));}
        USART_SendData(USART3, 'q');
      }
    } 
    
    
    
    /*
    // Number recognization //
    else if((ch >= '0')||(ch <= '9')) 
    {
      //printf("(%d, %d) inputed number : %d\n", locx, locy, ch);//
      prob[locx][locy++] = ch;
      
      if (locy == 9) { locx++; locy = 0; }
      if (locx == 9) 
      {
        USART_SendData(USART3,'w');
        Axis_Control(corr_x, corr_y); 
   
        // Sudoku solving //
        for (locx = 0; locx < 9; locx++)
        {
          for (locy = 0; locy < 9; locy++) {solve(locx, locy, prob, flag);}
        }
        
        print_sudoku(prob);
        LED_On_ALL();
        
      }
      else {Axis_Control(size * locy, - (size * locx)); USART_SendData(USART3, 'q');}
    } 
    */

    delay(1);
    
  }
  return 0;
}
















/*


else if((ch >= '0')||(ch <= '9')) 
    {
      if((count / 9) % 2 == 0)
      { 
        prob[count / 9][count % 9] = ch;
        Axis_Control(size * (count % 9), - (size * (count / 9)));
        
        if(count == 80)
        {
          USART_SendData(USART3,'w');
          Axis_Control(corr_x, corr_y); 
   
          // Sudoku solving //
          for (locx = 0; locx < 9; locx++)
          {
            for (locy = 0; locy < 9; locy++) {solve(locx, locy, prob, flag);}
          }
          
          print_sudoku(prob);
          LED_On_ALL();
  
        }
        else {USART_SendData(USART3, 'q');}
        
      }
      
      else 
      {
        prob[count / 9][8 - (count % 9)] = ch;
        Axis_Control(size * (8 - (count % 9)), - (size * (count / 9)));
        USART_SendData(USART3, 'q');
      }
      count++;
    } 
*/











/*
else if((ch >= '0')||(ch <= '9')) 
    {
      //printf("(%d, %d) inputed number : %d\n", locx, locy, ch);//
      prob[locx][locy++] = ch;
      
      if (locy == 9) { locx++; locy = 0; }
      if (locx == 9) 
      {
        Axis_Control(corr_x, corr_y); 
   
        // Sudoku solving //
        for (locx = 0; locx < 9; locx++)
        {
          for (locy = 0; locy < 9; locy++) {solve(locx, locy, prob, flag);}
        }
        
        print_sudoku(prob);
        USART_SendData(USART3,'w');
        
      }
      else {Axis_Control(size * locy, - (size * locx)); USART_SendData(USART3, 'q');}
    } 

*/




/*
// wasd 이동시 사용 //
int main()
{
  int ch, a = 0, b = 0;
  int size = 20;
  
  
  //--------Debugging mode--------//
  //--------Main System Clock and Port I/O Setting--------//
  RCC_ClocksTypeDef rcc_clocks;
  RCC_Configuration();
  RCC_GetClocksFreq(&rcc_clocks);
  Delay_Active(&rcc_clocks);
  
  Motor_Configuration();
  TIM_Configuration();
  USART1_Configuration();
  
  NVIC_Configuration();
  EXTI_Configuration();
  
  
  for (locx = 0; locx < 9; locx++)
      {
        for (locy = 0; locy < 9; locy++)
        {
         solve(locx, locy, prob, flag);
        }
      }
   //Solution Printing //
   print_sudoku(prob);
  
  while(1)
  { 
    ch = USART_GetChar(USART1);
    
    if(ch == 'd') {a += size; Axis_Control(a, b);}
    else if(ch == 'w'){b += size; Axis_Control(a, b);}                                  
    else if(ch == 's') {b -= size; Axis_Control(a, b);}                          
    else if(ch == 'a') {a -= size; Axis_Control(a, b);}
    
    else if(ch == '1') {Print_Num(1, size);}
    else if(ch == '2') {Print_Num(2, size);}
    else if(ch == '3') {Print_Num(3, size);}
    else if(ch == '4') {Print_Num(4, size);}
    else if(ch == '5') {Print_Num(5, size);}
    else if(ch == '6') {Print_Num(6, size);}
    else if(ch == '7') {Print_Num(7, size);}
    else if(ch == '8') {Print_Num(8, size);}
    else if(ch == '9') {Print_Num(9, size);}
    else if(ch == 'h') {hint(prob, flag, size);}
    else if(ch == 'c') {Counter_OnOff();}
    delay(1);
  }
  
  return 0;
}
*/


