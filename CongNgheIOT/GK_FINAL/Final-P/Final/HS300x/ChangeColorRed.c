#include "hal_data.h"
#include "stdio.h"
#include "string.h"

void R_BSP_WarmStart(bsp_warm_start_event_t event);

void user_uart_callback(uart_callback_args_t *p_args);
void console_write(const char *buffer);
void console_read(char *buffer, uint32_t length);

static volatile bool is_transfer_complete = false;
extern bsp_leds_t g_bsp_leds;

void hs300x_callback(rm_hs300x_callback_args_t * p_args);
static volatile bool hs300x_complete = false;

void hal_entry (void)
{
#if BSP_TZ_SECURE_BUILD
    R_BSP_NonSecureEnter();
#endif

    const bsp_delay_units_t bsp_delay_units = BSP_DELAY_UNITS_MILLISECONDS;
    const uint32_t freq_in_hz = 1;
    const uint32_t delay = bsp_delay_units * 2 / freq_in_hz;
    bsp_leds_t leds = g_bsp_leds;

    if (0 == leds.led_count)
    {
        while (1) { ; }
    }

    char write_buffer[200] = {};
    uint8_t humidity_threshold = 58;  // Default threshold

    fsp_err_t            err = FSP_SUCCESS;
    rm_hs300x_raw_data_t hs300x_raw_data;
    rm_hs300x_data_t     hs300x_data;
    uint8_t              calculated_flag = 0;

    R_SCI_UART_Open(&g_uart0_ctrl, &g_uart0_cfg);
    rm_comms_i2c_bus_extended_cfg_t * p_extend_hs300x =
    (rm_comms_i2c_bus_extended_cfg_t *)g_hs300x_sensor0_cfg.p_instance->p_cfg->p_extend;
    i2c_master_instance_t * p_driver_instance_hs300x = (i2c_master_instance_t *) p_extend_hs300x->p_driver_instance;
    p_driver_instance_hs300x->p_api->open(p_driver_instance_hs300x->p_ctrl, p_driver_instance_hs300x->p_cfg);

    RM_HS300X_Open(&g_hs300x_sensor0_ctrl, &g_hs300x_sensor0_cfg);

    R_BSP_PinAccessEnable();
    while(1)
    {
        sprintf(write_buffer, "\x1b[H");
        console_write(write_buffer);

        hs300x_complete = false;
        RM_HS300X_MeasurementStart(&g_hs300x_sensor0_ctrl);
        while (false == hs300x_complete) { ; }
        do
        {
            hs300x_complete = false;
            RM_HS300X_Read(&g_hs300x_sensor0_ctrl, &hs300x_raw_data);
            while (false == hs300x_complete) { ; }
            err = RM_HS300X_DataCalculate(&g_hs300x_sensor0_ctrl, &hs300x_raw_data, &hs300x_data);
            if (FSP_SUCCESS == err)
            {
                calculated_flag = 1;
            }
            else if (FSP_ERR_SENSOR_INVALID_DATA == err)
            {
                calculated_flag = 0;
            }
        } while (0 == calculated_flag);

        sprintf(write_buffer, "Humidity value: %d.%d \n\r",hs300x_data.humidity.integer_part, hs300x_data.humidity.decimal_part);
        console_write(write_buffer);

        sprintf(write_buffer, "Temperature value: %d.%d \n\r",hs300x_data.temperature.integer_part, hs300x_data.temperature.decimal_part);
        console_write(write_buffer);

        if (hs300x_data.humidity.integer_part >= humidity_threshold)
        {
            R_IOPORT_PinWrite(&g_ioport_ctrl, leds.p_leds[0], BSP_IO_LEVEL_HIGH); // Assuming red LED is on p_leds[0]
        }
        else
        {
            R_IOPORT_PinWrite(&g_ioport_ctrl, leds.p_leds[0], BSP_IO_LEVEL_LOW);
        }

        R_BSP_SoftwareDelay(delay, bsp_delay_units);
    }

    R_BSP_PinAccessDisable();
}

void R_BSP_WarmStart (bsp_warm_start_event_t event)
{
    if (BSP_WARM_START_RESET == event)
    {
#if BSP_FEATURE_FLASH_LP_VERSION != 0
        R_FACI_LP->DFLCTL = 1U;
#endif
    }

    if (BSP_WARM_START_POST_C == event)
    {
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

void console_read(char *buffer, uint32_t length){
    is_transfer_complete = false;
    R_SCI_UART_Read(&g_uart0_ctrl, (uint8_t *) buffer, length);
    while (!is_transfer_complete){
        R_BSP_SoftwareDelay(1, BSP_DELAY_UNITS_MICROSECONDS);
    }
}

void hs300x_callback(rm_hs300x_callback_args_t * p_args){
    switch (p_args->event){
        case RM_HS300X_EVENT_SUCCESS:
        {
            hs300x_complete = true;
            break;
        }
        default:
        {
        }
    }
}
