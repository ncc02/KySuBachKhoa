/* generated vector source file - do not edit */
#include "bsp_api.h"
/* Do not build these data structures if no interrupts are currently allocated because IAR will have build errors. */
#if VECTOR_DATA_IRQ_COUNT > 0
        BSP_DONT_REMOVE const fsp_vector_t g_vector_table[BSP_ICU_VECTOR_MAX_ENTRIES] BSP_PLACE_IN_SECTION(BSP_SECTION_APPLICATION_VECTORS) =
        {
                        [0] = sci_uart_rxi_isr, /* SCI5 RXI (Receive data full) */
            [1] = sci_uart_txi_isr, /* SCI5 TXI (Transmit data empty) */
            [2] = sci_uart_tei_isr, /* SCI5 TEI (Transmit end) */
            [3] = sci_uart_eri_isr, /* SCI5 ERI (Receive error) */
            [4] = iic_master_rxi_isr, /* IIC0 RXI (Receive data full) */
            [5] = iic_master_txi_isr, /* IIC0 TXI (Transmit data empty) */
            [6] = iic_master_tei_isr, /* IIC0 TEI (Transmit end) */
            [7] = iic_master_eri_isr, /* IIC0 ERI (Transfer error) */
        };
        #if BSP_FEATURE_ICU_HAS_IELSR
        const bsp_interrupt_event_t g_interrupt_event_link_select[BSP_ICU_VECTOR_MAX_ENTRIES] =
        {
            [0] = BSP_PRV_VECT_ENUM(EVENT_SCI5_RXI,GROUP0), /* SCI5 RXI (Receive data full) */
            [1] = BSP_PRV_VECT_ENUM(EVENT_SCI5_TXI,GROUP1), /* SCI5 TXI (Transmit data empty) */
            [2] = BSP_PRV_VECT_ENUM(EVENT_SCI5_TEI,GROUP2), /* SCI5 TEI (Transmit end) */
            [3] = BSP_PRV_VECT_ENUM(EVENT_SCI5_ERI,GROUP3), /* SCI5 ERI (Receive error) */
            [4] = BSP_PRV_VECT_ENUM(EVENT_IIC0_RXI,GROUP4), /* IIC0 RXI (Receive data full) */
            [5] = BSP_PRV_VECT_ENUM(EVENT_IIC0_TXI,GROUP5), /* IIC0 TXI (Transmit data empty) */
            [6] = BSP_PRV_VECT_ENUM(EVENT_IIC0_TEI,GROUP6), /* IIC0 TEI (Transmit end) */
            [7] = BSP_PRV_VECT_ENUM(EVENT_IIC0_ERI,GROUP7), /* IIC0 ERI (Transfer error) */
        };
        #endif
        #endif
