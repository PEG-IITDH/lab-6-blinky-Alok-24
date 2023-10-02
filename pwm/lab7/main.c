#include<stdio.h>
#include<stdint.h>
#include"tm4c123gh6pm.h"
#define STCTRL *((volatile long *) 0xE000E010)   //Control and Status Register
#define STRELOAD *((volatile long *) 0xE000E014) //SysTick Reload Value Register
#define STCURRENT *((volatile long *) 0xE000E018) //SysTick Current Value Register
#define Sw_Bits 0x11

void SYS_config()
{
    SYSCTL_RCGCPWM_R |=(1<<1);    // Enable and provide a clock to PWM module 1
    SYSCTL_RCGCGPIO_R |= (1<<5);   // Enable and provide a clock to GPIO Port F
    SYSCTL_RCC_R &= ~(1<<20);
    GPIO_PORTF_DIR_R = 0x0E;   // Define PortF LEDs as output and switches as input
    GPIO_PORTF_DEN_R = 0x1F;  // Enable all pins on Port F
    GPIO_PORTF_PUR_R = 0x11;  // Pull-up for user switches
    GPIO_PORTF_AFSEL_R |= 1<<1;  // Enable Alternate function for PF2
    GPIO_PORTF_PCTL_R |= 0x50;  // Selecting PWM function for PF2
}
void PWM_config()
{   PWM1_2_CTL_R &= ~(1<<0);
    PWM1_2_CTL_R &= ~(1<<1);
    PWM1_2_LOAD_R |= 160;
    PWM1_2_CMPB_R = 80;
    PWM1_2_GENB_R = (1<<3) | (1<<2) | (1<<11);
    PWM1_2_CTL_R |= (1<<0);
    PWM1_ENABLE_R |= (1<<5);
}
void Interrupt_config()
{
    GPIO_PORTF_IM_R &= ~(0x11);
    GPIO_PORTF_IS_R &= ~(0x11);
    GPIO_PORTF_IBE_R &= (0x11);

    GPIO_PORTF_IM_R |= 0x11;


    NVIC_PRI7_R = (NVIC_PRI7_R & 0xFF1FFFFF) |(1<<21);
    NVIC_EN0_R |=(1<<30);
}
void Systick_Handler()
{
    STCTRL = 0x00;
        if((GPIO_PORTF_DATA_R) & 0x10)  // Long Press
            {
                if(PWM1_2_CMPB_R > 8)  //Check for 5% of Duty cycle
                {
                    PWM1_2_CMPB_R -=8;  // Decrease duty
                }
            }
        else // Short Press
            {
                if(PWM1_2_CMPB_R < 152) // Check for 95% of Duty cycle
                {
                    PWM1_2_CMPB_R +=8;  // Increase duty
                }
            }
        GPIO_PORTF_ICR_R = Sw_Bits;
        GPIO_PORTF_IM_R |= Sw_Bits;
}
void PORTF_Handler()

{
    GPIO_PORTF_IM_R &= ~Sw_Bits;
int i;

    for(i = 0; i <1600*1000/4; i++)
    {
        // Debounce Delay of 0.25 seconds
    }

    if((~GPIO_PORTF_DATA_R)&0x1)
    {
    STCURRENT = 0;   // Reinitialize Systick Counter to Zero
    STRELOAD = (8*1000000);  // 0.5 Second countdown
    STCTRL |=0x07;  // Enable Systick, Enable Interrupt Generation, Enable the system clock (80MHz) as the source
    }


}


void main()
 {
    SYS_config();
    PWM_config();
    Interrupt_config();
    while(1)
    {
        // do nothing
    }

}


