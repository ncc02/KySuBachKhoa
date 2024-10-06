#include "hal_data.h"
#include "stdio.h"
#include "string.h"

// Khai báo các hàm và biến
void R_BSP_WarmStart(bsp_warm_start_event_t event);
void user_uart_callback(uart_callback_args_t *p_args);
void console_write(const char *buffer);
void console_read(char *buffer, uint32_t length);

static volatile bool is_transfer_complete = false;
extern bsp_leds_t g_bsp_leds;

void hal_entry (void)
{
#if BSP_TZ_SECURE_BUILD
    R_BSP_NonSecureEnter();
#endif

    // Mở UART
    R_SCI_UART_Open(&g_uart0_ctrl, &g_uart0_cfg);

    // Lấy thông tin về LED
    bsp_leds_t leds = g_bsp_leds;
    if (0 == leds.led_count)
    {
        while (1) { ; }
    }

    char write_buffer[200] = {};
    char input_buffer[2] = {}; // Buffer để nhận ký tự từ người dùng

    while(1)
    {
        // Yêu cầu nhập ký tự điều khiển LED
        sprintf(write_buffer, "\n\rNhap vao ky tu (R, G, B) de dieu khien LED: ");
        console_write(write_buffer);

        // Đọc ký tự từ người dùng
        console_read(input_buffer, 1);
        input_buffer[1] = '\0';

        // Hiển thị kí tự nhận được
        sprintf(write_buffer, "Da nhan: %s \n\r", input_buffer);
        console_write(write_buffer);

        // Kiểm tra ký tự và điều khiển LED tương ứng
        if (strcmp(input_buffer, "R") == 0)
        {
            // Bật đèn đỏ
            R_IOPORT_PinWrite(&g_ioport_ctrl, leds.p_leds[0], BSP_IO_LEVEL_HIGH); // LED đỏ
            R_IOPORT_PinWrite(&g_ioport_ctrl, leds.p_leds[1], BSP_IO_LEVEL_LOW);  // Tắt LED xanh lá
            R_IOPORT_PinWrite(&g_ioport_ctrl, leds.p_leds[2], BSP_IO_LEVEL_LOW);  // Tắt LED xanh dương
        }
        else if (strcmp(input_buffer, "G") == 0)
        {
            // Bật đèn xanh lá cây
            R_IOPORT_PinWrite(&g_ioport_ctrl, leds.p_leds[0], BSP_IO_LEVEL_LOW);  // Tắt LED đỏ
            R_IOPORT_PinWrite(&g_ioport_ctrl, leds.p_leds[1], BSP_IO_LEVEL_HIGH); // LED xanh lá
            R_IOPORT_PinWrite(&g_ioport_ctrl, leds.p_leds[2], BSP_IO_LEVEL_LOW);  // Tắt LED xanh dương
        }
        else if (strcmp(input_buffer, "B") == 0)
        {
            // Bật đèn xanh dương
            R_IOPORT_PinWrite(&g_ioport_ctrl, leds.p_leds[0], BSP_IO_LEVEL_LOW);  // Tắt LED đỏ
            R_IOPORT_PinWrite(&g_ioport_ctrl, leds.p_leds[1], BSP_IO_LEVEL_LOW);  // Tắt LED xanh lá
            R_IOPORT_PinWrite(&g_ioport_ctrl, leds.p_leds[2], BSP_IO_LEVEL_HIGH); // LED xanh dương
        }
        else
        {
            // Xử lý ký tự không hợp lệ
            sprintf(write_buffer, "Ky tu khong hop le! Hay nhap lai (R, G, B).\n\r");
            console_write(write_buffer);
        }

        R_BSP_SoftwareDelay(1000, BSP_DELAY_UNITS_MILLISECONDS); // Đợi một khoảng thời gian trước khi tiếp tục
    }
}

// Hàm khởi động
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

// Callback cho UART
void user_uart_callback(uart_callback_args_t *p_args)
{
    switch (p_args->event){
        case UART_EVENT_TX_COMPLETE:
        case UART_EVENT_RX_COMPLETE:
        {
            is_transfer_complete = true;
            break;
        }
        default:
        {
        }
    }
}

// Hàm ghi dữ liệu ra console
void console_write(const char *buffer){
    is_transfer_complete = false;
    R_SCI_UART_Write(&g_uart0_ctrl, (uint8_t *) buffer, strlen(buffer));
    while (!is_transfer_complete){
        R_BSP_SoftwareDelay(1, BSP_DELAY_UNITS_MICROSECONDS);
    }
}

// Hàm đọc dữ liệu từ console
void console_read(char *buffer, uint32_t length){
    is_transfer_complete = false;
    R_SCI_UART_Read(&g_uart0_ctrl, (uint8_t *) buffer, length);
    while (!is_transfer_complete){
        R_BSP_SoftwareDelay(1, BSP_DELAY_UNITS_MICROSECONDS);
    }
}
