#include <stdint.h>
#include <stdbool.h>
#include <tm4c123gh6pm.h>

#define LED_RED   (1U << 1)
#define SW1       (1U << 4)  // SW1 is connected to GPIO pin 4

int main(void)
{
    int state;
    SYSCTL_RCGC2_R |= 0x00000020; // Enable clock for GPIOF
    GPIO_PORTF_LOCK_R = 0x4C4F434B; // Unlock commit register
    GPIO_PORTF_CR_R = 0x01; // Enable changes to the CR register
    GPIO_PORTF_DIR_R |= 0x0E;           /* set PORTF3+PORTF2+PORTF1 pin as output (LED) pin */
    GPIO_PORTF_DIR_R &= ~SW1;    // Set switch pin as input
    GPIO_PORTF_DEN_R = 0x1F; // Digital enable LED and switch pins
    GPIO_PORTF_PUR_R = SW1; // Enable pull-up resistor for switch

    while (1)
        {
        state = GPIO_PORTF_DATA_R & SW1;
            if (state == 0) // Check if the switch is pressed
            {
                GPIO_PORTF_DATA_R |= LED_RED; // Turn on RED
            }
            else
            {
                GPIO_PORTF_DATA_R &= ~LED_RED; // Turn off RED
            }
        }
}
