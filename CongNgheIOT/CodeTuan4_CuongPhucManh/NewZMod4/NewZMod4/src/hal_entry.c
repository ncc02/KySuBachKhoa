/*
* Copyright (c) 2020 - 2024 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

#include "hal_data.h"

void R_BSP_WarmStart(bsp_warm_start_event_t event);

extern bsp_leds_t g_bsp_leds;


/* TODO: Enable if you want to open I2C bus */
void g_comms_i2c_bus0_quick_setup(void);

/* Quick setup for g_comms_i2c_bus0. */
void g_comms_i2c_bus0_quick_setup(void)
{
    fsp_err_t err;
    i2c_master_instance_t * p_driver_instance = (i2c_master_instance_t *) g_comms_i2c_bus0_extended_cfg.p_driver_instance;

    /* Open I2C driver, this must be done before calling any COMMS API */
    err = p_driver_instance->p_api->open(p_driver_instance->p_ctrl, p_driver_instance->p_cfg);
    assert(FSP_SUCCESS == err);

#if BSP_CFG_RTOS
    /* Create a semaphore for blocking if a semaphore is not NULL */
    if (NULL != g_comms_i2c_bus0_extended_cfg.p_blocking_semaphore)
    {
#if BSP_CFG_RTOS == 1 // AzureOS
        tx_semaphore_create(g_comms_i2c_bus0_extended_cfg.p_blocking_semaphore->p_semaphore_handle,
                            g_comms_i2c_bus0_extended_cfg.p_blocking_semaphore->p_semaphore_name,
                            (ULONG) 0);
#elif BSP_CFG_RTOS == 2 // FreeRTOS
        *(g_comms_i2c_bus0_extended_cfg.p_blocking_semaphore->p_semaphore_handle)
            = xSemaphoreCreateCountingStatic((UBaseType_t) 1, (UBaseType_t) 0, g_comms_i2c_bus0_extended_cfg.p_blocking_semaphore->p_semaphore_memory);
#endif
    }

    /* Create a recursive mutex for bus lock if a recursive mutex is not NULL */
    if (NULL != g_comms_i2c_bus0_extended_cfg.p_bus_recursive_mutex)
    {
#if BSP_CFG_RTOS == 1 // AzureOS
        tx_mutex_create(g_comms_i2c_bus0_extended_cfg.p_bus_recursive_mutex->p_mutex_handle,
                        g_comms_i2c_bus0_extended_cfg.p_bus_recursive_mutex->p_mutex_name,
                        TX_INHERIT);
#elif BSP_CFG_RTOS == 2 // FreeRTOS
        *(g_comms_i2c_bus0_extended_cfg.p_bus_recursive_mutex->p_mutex_handle)
            = xSemaphoreCreateRecursiveMutexStatic(g_comms_i2c_bus0_extended_cfg.p_bus_recursive_mutex->p_mutex_memory);
#endif
    }
#endif
}


/* TODO: Enable if you want to open ZMOD4XXX */
#define G_ZMOD4XXX_SENSOR0_NON_BLOCKING (1)
#define G_ZMOD4XXX_SENSOR0_IRQ_ENABLE   (1)

#if G_ZMOD4XXX_SENSOR0_NON_BLOCKING
volatile bool g_zmod4xxx_i2c_completed = false;
volatile rm_zmod4xxx_event_t g_zmod4xxx_i2c_callback_event;
#endif
#if G_ZMOD4XXX_SENSOR0_IRQ_ENABLE
volatile bool g_zmod4xxx_irq_completed = false;
#endif

/* TODO: Enable if you want to use a I2C callback */
#define G_ZMOD4XXX_SENSOR0_I2C_CALLBACK_ENABLE (1)
#if G_ZMOD4XXX_SENSOR0_I2C_CALLBACK_ENABLE
void zmod4xxx_comms_i2c_callback(rm_zmod4xxx_callback_args_t * p_args)
{
#if G_ZMOD4XXX_SENSOR0_NON_BLOCKING
    g_zmod4xxx_i2c_callback_event = p_args->event;

    if (RM_ZMOD4XXX_EVENT_ERROR != p_args->event)
    {
        g_zmod4xxx_i2c_completed = true;
    }
#else
    FSP_PARAMETER_NOT_USED(p_args);
#endif
}
#endif

/* TODO: Enable if you want to use a IRQ callback */
#define G_ZMOD4XXX_SENSOR0_IRQ_CALLBACK_ENABLE (1)
#if G_ZMOD4XXX_SENSOR0_IRQ_CALLBACK_ENABLE
void zmod4xxx_irq_callback(rm_zmod4xxx_callback_args_t * p_args)
{
#if G_ZMOD4XXX_SENSOR0_IRQ_ENABLE
    if (RM_ZMOD4XXX_EVENT_MEASUREMENT_COMPLETE == p_args->event)
    {
        g_zmod4xxx_irq_completed = true;
    }
#else
    FSP_PARAMETER_NOT_USED(p_args);
#endif
}
#endif

/* Delay */
#define G_ZMOD4XXX_SENSOR0_DELAY_50     (50)
#define G_ZMOD4XXX_SENSOR0_DELAY_5475   (5475)
#define G_ZMOD4XXX_SENSOR0_DELAY_1990   (1990)
#define G_ZMOD4XXX_SENSOR0_DELAY_1010   (1010)
#define G_ZMOD4XXX_SENSOR0_DELAY_90000  (90000)
#define G_ZMOD4XXX_SENSOR0_DELAY_1500   (1500)
#define G_ZMOD4XXX_SENSOR0_DELAY_2000   (2000)
#define G_ZMOD4XXX_SENSOR0_DELAY_3000   (3000)
#define G_ZMOD4XXX_SENSOR0_DELAY_5000   (5000)
#define G_ZMOD4XXX_SENSOR0_DELAY_MS     (1000)

/* Quick setup for g_zmod4xxx_sensor0.
 * - g_comms_i2c_bus0 must be setup before calling this function
 *     (See Developer Assistance -> g_zmod4xxx_sensor0 -> ZMOD4xxx ***** on rm_zmod4xxx -> g_comms_i2c_device0 -> g_comms_i2c_bus0 -> Quick Setup).
 */
void g_zmod4xxx_sensor0_quick_setup(void);

/* Quick setup for g_zmod4xxx_sensor0. */
void g_zmod4xxx_sensor0_quick_setup(void)
{
    fsp_err_t err;

    /* Open ZMOD4XXX sensor instance, this must be done before calling any ZMOD4XXX API */
    err = g_zmod4xxx_sensor0.p_api->open(g_zmod4xxx_sensor0.p_ctrl, g_zmod4xxx_sensor0.p_cfg);
    assert(FSP_SUCCESS == err);
}

/* Quick getting IAQ 1st Gen. Continuous mode values for g_zmod4xxx_sensor0.
 * - g_zmod4xxx_sensor0 must be setup before calling this function.
 */
bool g_zmod4xxx_sensor0_quick_getting_iaq_1st_gen_continuous_mode_data(rm_zmod4xxx_iaq_1st_data_t * p_gas_data);

/* Quick getting gas data for g_zmod4xxx_sensor0. */
bool g_zmod4xxx_sensor0_quick_getting_iaq_1st_gen_continuous_mode_data(rm_zmod4xxx_iaq_1st_data_t * p_gas_data)
{
    fsp_err_t            err;
    rm_zmod4xxx_raw_data_t zmod4xxx_raw_data;
    bool stabilization_complete = false;

    /* Clear callback flags */
#if G_ZMOD4XXX_SENSOR0_IRQ_ENABLE
    g_zmod4xxx_irq_completed = false;
#endif
#if G_ZMOD4XXX_SENSOR0_NON_BLOCKING
    g_zmod4xxx_i2c_completed = false;
#endif

    /* Start the measurement */
    /* If the MeasurementStart API is called once, a second call is not required. */
    err = g_zmod4xxx_sensor0.p_api->measurementStart(g_zmod4xxx_sensor0.p_ctrl);
    assert(FSP_SUCCESS == err);
#if G_ZMOD4XXX_SENSOR0_NON_BLOCKING
    while (!g_zmod4xxx_i2c_completed)
    {
        ;
    }
    g_zmod4xxx_i2c_completed = false;
#endif

    do
    {
        /* Wait for the measurement to complete */
#if G_ZMOD4XXX_SENSOR0_IRQ_ENABLE
        while (!g_zmod4xxx_irq_completed)
        {
            ;
        }
        g_zmod4xxx_irq_completed = false;
#else
        err = g_zmod4xxx_sensor0.p_api->statusCheck(g_zmod4xxx_sensor0.p_ctrl);
        assert(FSP_SUCCESS == err);
#if G_ZMOD4XXX_SENSOR0_NON_BLOCKING
        while (!g_zmod4xxx_i2c_completed)
        {
            ;
        }
        g_zmod4xxx_i2c_completed = false;
#endif
#endif
        /* Read ADC data from ZMOD4xxx sensor */
        err = g_zmod4xxx_sensor0.p_api->read(g_zmod4xxx_sensor0.p_ctrl, &zmod4xxx_raw_data);
        if (err == FSP_ERR_SENSOR_MEASUREMENT_NOT_FINISHED)
        {
#if BSP_CFG_RTOS == 1 // AzureOS
            tx_thread_sleep(G_ZMOD4XXX_SENSOR0_DELAY_50 * TX_TIMER_TICKS_PER_SECOND / G_ZMOD4XXX_SENSOR0_DELAY_MS);
#elif BSP_CFG_RTOS == 2 // FreeRTOS
            vTaskDelay(G_ZMOD4XXX_SENSOR0_DELAY_50 * configTICK_RATE_HZ / G_ZMOD4XXX_SENSOR0_DELAY_MS);
#else // Bare Metal
            R_BSP_SoftwareDelay(G_ZMOD4XXX_SENSOR0_DELAY_50, BSP_DELAY_UNITS_MILLISECONDS);
#endif
        }
    }
    while (err == FSP_ERR_SENSOR_MEASUREMENT_NOT_FINISHED);
    assert(FSP_SUCCESS == err);

#if G_ZMOD4XXX_SENSOR0_NON_BLOCKING
    while (!g_zmod4xxx_i2c_completed)
    {
        ;
    }
    g_zmod4xxx_i2c_completed = false;
#endif

    /* Calculate IAQ 1st Gen. values from ZMOD4xxx ADC data */
    err = g_zmod4xxx_sensor0.p_api->iaq1stGenDataCalculate(g_zmod4xxx_sensor0.p_ctrl, &zmod4xxx_raw_data, p_gas_data);
    if (err == FSP_SUCCESS)
    {
        stabilization_complete = true;
    }
    else if(err == FSP_ERR_SENSOR_IN_STABILIZATION)
    {
        stabilization_complete = false;
    }
    else
    {
        assert(false);
    }

    return stabilization_complete;
}

/*******************************************************************************************************************//**
 * @brief  Blinky example application
 *
 * Blinks all leds at a rate of 1 second using the software delay function provided by the BSP.
 *
 **********************************************************************************************************************/
volatile rm_zmod4xxx_iaq_1st_data_t zmod_1st_data;

void hal_entry (void)
{
    g_comms_i2c_bus0_quick_setup();
    g_zmod4xxx_sensor0_quick_setup();

    while (1) {
        g_zmod4xxx_sensor0_quick_getting_iaq_1st_gen_continuous_mode_data((rm_zmod4xxx_iaq_1st_data_t*)& zmod_1st_data);
    }

#if BSP_TZ_SECURE_BUILD

    /* Enter non-secure code */
    R_BSP_NonSecureEnter();
#endif

    /* Define the units to be used with the software delay function */
    const bsp_delay_units_t bsp_delay_units = BSP_DELAY_UNITS_MILLISECONDS;

    /* Set the blink frequency (must be <= bsp_delay_units */
    const uint32_t freq_in_hz = 2;

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
    bsp_io_level_t pin_level = BSP_IO_LEVEL_LOW;

    while (1)
    {
        /* Enable access to the PFS registers. If using r_ioport module then register protection is automatically
         * handled. This code uses BSP IO functions to show how it is used.
         */
        R_BSP_PinAccessEnable();

        /* Update all board LEDs */
        for (uint32_t i = 0; i < leds.led_count; i++)
        {
            /* Get pin to toggle */
            uint32_t pin = leds.p_leds[i];

            /* Write to this pin */
            R_BSP_PinWrite((bsp_io_port_pin_t) pin, pin_level);
        }

        /* Protect PFS registers */
        R_BSP_PinAccessDisable();

        /* Toggle level for next write */
        if (BSP_IO_LEVEL_LOW == pin_level)
        {
            pin_level = BSP_IO_LEVEL_HIGH;
        }
        else
        {
            pin_level = BSP_IO_LEVEL_LOW;
        }

        /* Delay */
        R_BSP_SoftwareDelay(delay, bsp_delay_units);
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
