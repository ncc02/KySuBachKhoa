/*
* Copyright (c) 2020 - 2024 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#include "hal_data.h"
#include "stdio.h"

void R_BSP_WarmStart(bsp_warm_start_event_t event);

void user_uart_callback(uart_callback_args_t *p_args);
void console_write(const char *buffer);

static volatile bool is_transfer_complete = false;
extern bsp_leds_t g_bsp_leds;

/*******************************************************************************************************************//**
 * @brief  Blinky example application
 *
 * Blinks all leds at a rate of 1 second using the software delay function provided by the BSP.
 *
 **********************************************************************************************************************/
void hal_entry (void)
{
#if BSP_TZ_SECURE_BUILD

    /* Enter non-secure code */
    R_BSP_NonSecureEnter();
#endif

    /* Define the units to be used with the software delay function */
    const bsp_delay_units_t bsp_delay_units = BSP_DELAY_UNITS_MILLISECONDS;

    /* Set the blink frequency (must be <= bsp_delay_units */
    const uint32_t freq_in_hz = 1;

    /* Calculate the delay in terms of bsp_delay_units */
    const uint32_t delay = bsp_delay_units / freq_in_hz;

    /* LED type structure */
    bsp_leds_t leds = g_bsp_leds;

    /* If this board has no LEDs then trap here */
    if (0 == leds.led_count)
    {
        while (1)
        {
            ;                          // There are no LEDs on this board
        }
    }

    /* Holds level to set for pins */
   // bsp_io_level_t pin_level = BSP_IO_LEVEL_LOW;

    char write_buffer[200] = {};
    R_SCI_UART_Open(&g_uart0_ctrl, &g_uart0_cfg);
    sprintf(write_buffer, "Hello World!\r\n");
    char red_buffer[200] = {};
    R_SCI_UART_Open(&g_uart0_ctrl, &g_uart0_cfg);
    sprintf(red_buffer, "RED\r\n");
    char blue_buffer[200] = {};
    R_SCI_UART_Open(&g_uart0_ctrl, &g_uart0_cfg);
    sprintf(blue_buffer, "BLUE\r\n");
    char green_buffer[200] = {};
    R_SCI_UART_Open(&g_uart0_ctrl, &g_uart0_cfg);
    sprintf(green_buffer, "GREEN\r\n");

    while (1)
    {
        /* Enable access to the PFS registers. If using r_ioport module then register protection is automatically
         * handled. This code uses BSP IO functions to show how it is used.
         */
        R_BSP_PinAccessEnable();

        /* Update all board LEDs */
        for (uint32_t i = 0; i < 3; i++)
        {
            /* Get pin to toggle */
            uint32_t pin = leds.p_leds[i];
            uint32_t pin_xxx = leds.p_leds[i+3];

            /* Write to this pin */
            R_BSP_PinWrite((bsp_io_port_pin_t) pin, BSP_IO_LEVEL_HIGH);
            R_BSP_PinWrite((bsp_io_port_pin_t) pin_xxx, BSP_IO_LEVEL_LOW);
//            console_write(write_buffer);
            if (i==0) {
                console_write(red_buffer);
            }
            else if (i==1) {
                console_write(green_buffer);
            }
            else if (i==2) {
                console_write(blue_buffer);
            }
            R_BSP_SoftwareDelay(delay, bsp_delay_units);

            R_BSP_PinWrite((bsp_io_port_pin_t) pin, BSP_IO_LEVEL_LOW);
            R_BSP_PinWrite((bsp_io_port_pin_t) pin_xxx, BSP_IO_LEVEL_HIGH);
        }

        /* Protect PFS registers */
        R_BSP_PinAccessDisable();

        /* Delay */
//        R_BSP_SoftwareDelay(delay, bsp_delay_units);
    }
}

/*******************************************************************************************************************//**
 * This function is called at various points during the startup process.  This implementation uses the event that is
 * called right before main() to set up the pins.
 *
 * @param[in]  event    Where at in the start up process the code is currently at
 **********************************************************************************************************************/
void R_BSP_WarmStart (bsp_warm_start_event_t event)
{
    if (BSP_WARM_START_RESET == event)
    {
#if BSP_FEATURE_FLASH_LP_VERSION != 0

        /* Enable reading from data flash. */
        R_FACI_LP->DFLCTL = 1U;

        /* Would normally have to wait tDSTOP(6us) for data flash recovery. Placing the enable here, before clock and
         * C runtime initialization, should negate the need for a delay since the initialization will typically take more than 6us. */
#endif
    }

    if (BSP_WARM_START_POST_C == event)
    {
        /* C runtime environment and system clocks are setup. */

        /* Configure pins. */
        R_IOPORT_Open(&IOPORT_CFG_CTRL, &IOPORT_CFG_NAME);
    }
}

void user_uart_callback(uart_callback_args_t *p_args)
{
    switch (p_args->event){
        case UART_EVENT_TX_COMPLETE:
        {
            is_transfer_complete = true;
            break;
        }
        default:
        {

        }
    }
}
void console_write(const char *buffer){
    is_transfer_complete = false;
    R_SCI_UART_Write(&g_uart0_ctrl, (uint8_t *) buffer, strlen(buffer));
    while (!is_transfer_complete){
        R_BSP_SoftwareDelay(1, BSP_DELAY_UNITS_MICROSECONDS);
    }
}
