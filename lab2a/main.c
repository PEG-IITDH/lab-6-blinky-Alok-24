#include <stdint.h>
#include <stdbool.h>
#include <tm4c123gh6pm.h>

#define LED_RED   (1U << 1)
#define LED_GREEN (1U << 3)
#define LED_BLUE  (1U << 2)
#define SW1       (1U << 4)  // SW1 is connected to GPIO pin 4


int main(void)
{
    SYSCTL_RCGC2_R |= 0x00000020; // Enable clock for GPIOF
    GPIO_PORTF_LOCK_R = 0x4C4F434B; // Unlock commit register
    GPIO_PORTF_CR_R = 0x01; // Enable changes to the CR register
    GPIO_PORTF_DIR_R |= 0x0E;           /* set PORTF3+PORTF2+PORTF1 pin as output (LED) pin */
    GPIO_PORTF_DIR_R &= ~SW1;    // Set switch pin as input
    GPIO_PORTF_DEN_R = 0x1F; // Digital enable LED and switch pins
    GPIO_PORTF_PUR_R = SW1; // Enable pull-up resistor for switch

   int i, color = -1;

    while (1)
    {
        if (!(GPIO_PORTF_DATA_R & SW1)) // Check if the switch is pressed
        {
            for (i = 0; i < 10000; i++)
                {
                    ;//Debouncing delay
                }

                color = (color + 1) % 3; // 0: Red, 1: Green, 2: Blue

                GPIO_PORTF_DATA_R &= ~0x0E;

                if (color == 0)
                {
                    GPIO_PORTF_DATA_R |= LED_RED;
                }
                else if (color == 1)
                {
                    GPIO_PORTF_DATA_R |= LED_GREEN;
                }
                else if (color == 2)
                {
                    GPIO_PORTF_DATA_R |= LED_BLUE;
                }

                while (!(GPIO_PORTF_DATA_R & SW1))
                {
                   ; // Wait
                }
            }
        }
    }

