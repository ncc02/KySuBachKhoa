#include "hal_data.h"

extern bsp_leds_t g_bsp_leds;

void hal_entry (void)
{
    /* Define the units to be used with the software delay function */
    const bsp_delay_units_t bsp_delay_units = BSP_DELAY_UNITS_MILLISECONDS;

    /* Initial delay time (start slow) */
    uint32_t delay = 1000;  // 1 second

    /* LED structure */
    bsp_leds_t leds = g_bsp_leds;

    /* If no LEDs available, halt execution */
    if (0 == leds.led_count)
    {
        while (1) { ; }
    }

    while (1)
    {
        R_BSP_PinAccessEnable(); // Enable access to the PFS registers

        /* Cycle through each LED with decreasing delay */
        //for (uint32_t i = 0; i < leds.led_count-3; i++)
        uint32_t i = 5;
        {
            uint32_t pin = leds.p_leds[i];

            /* Turn on the LED */
            R_BSP_PinWrite((bsp_io_port_pin_t) pin, BSP_IO_LEVEL_HIGH);
            //R_BSP_PinWrite((bsp_io_port_pin_t) pin, BSP_IO_LEVEL_LOW);


        }


        R_BSP_PinAccessDisable(); // Protect PFS registers
    }
}
