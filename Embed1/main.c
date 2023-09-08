#include <stdint.h>
#include <stdbool.h>
#include "tm4c123gh6pm.h"

int main(void)
{
    int i;
    SYSCTL_RCGC2_R |= 0x00000020;       /* enable clock to GPIOF */
    GPIO_PORTF_LOCK_R = 0x4C4F434B;     /* unlock commit register */
    GPIO_PORTF_CR_R = 0x01;             /* make PORTF0 configurable */
    GPIO_PORTF_DIR_R |= 0x0E;           /* set PORTF3+PORTF2+PORTF1 pin as output (LED) pin */
    GPIO_PORTF_DEN_R |= 0x1F;           /* set PORTF pins 4-3-2-1 as digital pins */
    GPIO_PORTF_PUR_R = 0x11;            /* enable pull up for pin 4 and 0 */

while(1)
{
    GPIO_PORTF_DATA_R = 0x02; // PF1 on
    for(i=0;i<729;i++); // delay for 1ms
    GPIO_PORTF_DATA_R = 0X00; // PF1 off
    for(i=0;i<729;i++); // delay for 1ms
 }
}
