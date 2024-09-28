/*
 * Copyright 2024, Cypress Semiconductor Corporation (an Infineon company) or
 * an affiliate of Cypress Semiconductor Corporation.  All rights reserved.
 *
 * This software, including source code, documentation and related
 * materials ("Software") is owned by Cypress Semiconductor Corporation
 * or one of its affiliates ("Cypress") and is protected by and subject to
 * worldwide patent protection (United States and foreign),
 * United States copyright laws and international treaty provisions.
 * Therefore, you may use this Software only as provided in the license
 * agreement accompanying the software package from which you
 * obtained this Software ("EULA").
 * If no EULA applies, Cypress hereby grants you a personal, non-exclusive,
 * non-transferable license to copy, modify, and compile the Software
 * source code solely for use in connection with Cypress's
 * integrated circuit products.  Any reproduction, modification, translation,
 * compilation, or representation of this Software except as specified
 * above is prohibited without the express written permission of Cypress.
 *
 * Disclaimer: THIS SOFTWARE IS PROVIDED AS-IS, WITH NO WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, NONINFRINGEMENT, IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE. Cypress
 * reserves the right to make changes to the Software without notice. Cypress
 * does not assume any liability arising out of the application or use of the
 * Software or any product or circuit described in the Software. Cypress does
 * not authorize its products for use in any products where a malfunction or
 * failure of the Cypress product may reasonably be expected to result in
 * significant property damage, injury or death ("High Risk Product"). By
 * including Cypress's product in a High Risk Product, the manufacturer
 * of such system or application assumes all risk of such use and in doing
 * so agrees to indemnify Cypress against all liability.
 */

#include <cyhal.h>
#include <cybsp.h>
#include <lwip/tcpip.h>
#include <lwip/api.h>
#include <cy_retarget_io.h>
#include <cybsp_wifi.h>
#include <FreeRTOS.h>
#include <task.h>
#include <stdio.h>
#include "command_console.h"
#include "wifi_app_intf.h"

#if defined(__GNUC__) && !(defined(__ICCARM__) || defined(__clang__) || defined(__CC_ARM))
void optimized_wordsize_memcpy(void *dst, const void *src, size_t len);
#endif

/*******************************************************************************
* Macros
********************************************************************************/
/*
 * Command console buffers
 */
#define CONSOLE_COMMAND_MAX_PARAMS     (32)
#define CONSOLE_COMMAND_MAX_LENGTH     (1024)
#define CONSOLE_COMMAND_HISTORY_LENGTH (10)
#define CONSOLE_THREAD_PRIORITY        (5)

/*
 * CPU CLOCK FREQUENCY
 */
#ifdef SDIO_UHS_OPTIMIZATION
#define CPU_CLOCK_FREQUENCY 150000000
#else
#define CPU_CLOCK_FREQUENCY 144000000
#endif

/* Peripheral clock dividier */
#define CPU_PERI_CLOCK_DIV (2)

#define CHECK_SIGMA_APP_RETURN(expr)  { \
        cy_rslt_t rslt = (expr); \
        if (rslt != CY_RSLT_SUCCESS) \
        { \
           return rslt; \
        } \
}

/*******************************************************************************
* Global Variables
********************************************************************************/
const char* console_delimiter_string = ",";

static char command_buffer[CONSOLE_COMMAND_MAX_LENGTH];
static char command_history_buffer[CONSOLE_COMMAND_MAX_LENGTH * CONSOLE_COMMAND_HISTORY_LENGTH];

extern cyhal_uart_t cy_retarget_io_uart_obj;

/* This enables RTOS aware debugging */
volatile int uxTopUsedPriority ;
#if defined(__GNUC__) && !(defined(__ICCARM__) || defined(__clang__) || defined(__CC_ARM))
extern volatile uint32_t __HeapBase;
extern volatile uint32_t __HeapLimit;
uint8_t* base = (uint8_t *)&__HeapBase;
uint8_t* end  = (uint8_t *)&__HeapLimit;
#endif

#ifndef COMPONENT_4390X
/* set CPU clock frequency */
#if (CYHAL_API_VERSION >= 2)
cy_rslt_t set_cpu_clock_v2 ( uint32_t freq );
#else // HAL API version 1
cy_rslt_t set_cpu_clock ( uint32_t freq );
#endif
#endif
/*
 * This is called first after the initialization of FreeRTOS.  It basically connects to WiFi and then starts the
 * task that waits for DHCP.  No tasks will actually run until this function returns.
 */
void vApplicationDaemonTaskStartupHook()
{
    cy_command_console_cfg_t console_cfg;
    cy_rslt_t result;

    console_cfg.serial             = (void *)&cy_retarget_io_uart_obj;
    console_cfg.line_len           = sizeof(command_buffer);
    console_cfg.buffer             = command_buffer;
    console_cfg.history_len        = CONSOLE_COMMAND_HISTORY_LENGTH;
    console_cfg.history_buffer_ptr = command_history_buffer;
    console_cfg.delimiter_string   = console_delimiter_string;
    console_cfg.params_num         = CONSOLE_COMMAND_MAX_PARAMS;
    console_cfg.thread_priority    = (cy_thread_priority_t)CONSOLE_THREAD_PRIORITY;

    /* Initialize command console library */
    result = cy_command_console_init(&console_cfg);

    /* Initialize SigmaDUT library */
    result = cywifi_init_sigmadut();

    if ( result != CY_RSLT_SUCCESS )
    {
        printf("Sigma Init failed res:%u\n", (unsigned int)result );
    }
#ifndef COMPONENT_4390X
    printf("CY_SRAM_SIZE     : %lu\n", CY_SRAM_SIZE);
#endif
#if defined(__GNUC__) && !(defined(__ICCARM__) || defined(__clang__) || defined(__CC_ARM))
    printf("Heap Base        : %p\n", base);
    printf("Heap size        : %d\n", (end - base));
#else
    printf("Heap size        : %d\n", configTOTAL_HEAP_SIZE);
#endif
    printf("SystemCoreClock  : %u\n", (unsigned int)configCPU_CLOCK_HZ);

#ifdef SDIO_UHS_OPTIMIZATION
    printf("UHS optimization : %s\n", "enabled");
#else
    printf("UHS optimization : %s\n", "disabled");
#endif

}

/************************************************************************************
* Main initializes the hardware and low power support, and starts the task scheduler
*************************************************************************************/
int main()
{
    cy_rslt_t result ;

    /* This enables RTOS aware debugging in OpenOCD */
    uxTopUsedPriority = configMAX_PRIORITIES - 1 ;

    /* Initialize the board support package */
    result = cybsp_init() ;
    CY_ASSERT(result == CY_RSLT_SUCCESS) ;

    /* Enable global interrupts */
    __enable_irq();

    /* Enable High Speed CPU clock change to 144Mhz/150Mhz only for PSOC62M devices
     * and use the default CPU clock for PSOC6 1M devices
     */
#ifndef COMPONENT_4390X
#if !defined(CYHAL_UDB_SDIO)
#if (CYHAL_API_VERSION >= 2)
    /* set CPU clock to CPU_CLOCK_FREQUENCY */
    result = set_cpu_clock_v2(CPU_CLOCK_FREQUENCY);
#else
    /* set CPU clock to CPU_CLOCK_FREQUENCY */
    result = set_cpu_clock(CPU_CLOCK_FREQUENCY);
#endif
    CY_ASSERT(result == CY_RSLT_SUCCESS) ;
#endif
#endif

    /* Initialize retarget-io to use the debug UART port */
    cy_retarget_io_init(CYBSP_DEBUG_UART_TX, CYBSP_DEBUG_UART_RX, CY_RETARGET_IO_BAUDRATE);

    /* clear the screen */
    printf("\x1b[2J\x1b[;H");

    /* Start the FreeRTOS scheduler */
    vTaskStartScheduler() ;

    /* Should never get here */
    CY_ASSERT(0) ;

    return result;
}
#ifndef COMPONENT_4390X
#if (CYHAL_API_VERSION >= 2)
cy_rslt_t set_cpu_clock_v2 ( uint32_t freq )
{
    cy_rslt_t ret = CY_RSLT_SUCCESS;
    uint32_t old_freq;
    cyhal_clock_t clock_pll, clock_hf0 , clock_peri;

    old_freq = cyhal_clock_get_frequency(&CYHAL_CLOCK_HF[0]);
    if ( freq != old_freq )
    {
        /* Initialize, take ownership of, the PLL instance */
        CHECK_SIGMA_APP_RETURN(cyhal_clock_reserve(&clock_pll, &CYHAL_CLOCK_PLL[0]));

        /* Set CPU clock to freq */
        CHECK_SIGMA_APP_RETURN(cyhal_clock_set_frequency(&clock_pll, freq, NULL));

        /* If the PLL is not already enabled, enable it */
        if (!cyhal_clock_is_enabled(&clock_pll))
        {
            CHECK_SIGMA_APP_RETURN(cyhal_clock_set_enabled(&clock_pll, true, true));
        }
        /* Initialize, take ownership of, the PERI instance */
        CHECK_SIGMA_APP_RETURN(cyhal_clock_reserve(&clock_peri, &CYHAL_CLOCK_PERI));

        /* Initialize, take ownership of, the HF0 instance */
        CHECK_SIGMA_APP_RETURN(cyhal_clock_reserve(&clock_hf0, &CYHAL_CLOCK_HF[0]));

        /* Set peri clock divider */
        CHECK_SIGMA_APP_RETURN(cyhal_clock_set_divider(&clock_peri, CPU_PERI_CLOCK_DIV));

        /* set HF0 Clock source to PLL */
        CHECK_SIGMA_APP_RETURN(cyhal_clock_set_source(&clock_hf0, &clock_pll));
    }
    return ret;
}
#else // HAL API version 1
cy_rslt_t set_cpu_clock ( uint32_t freq )
{
    cy_rslt_t ret = CY_RSLT_SUCCESS;
    uint32_t old_freq;
    cyhal_clock_t clock_pll, clock_hf0 , clock_peri;

    /* Get the HF0 resource */
    CHECK_SIGMA_APP_RETURN(cyhal_clock_get(&clock_hf0 , (const cyhal_resource_inst_t *)&CYHAL_CLOCK_HF));

    old_freq = cyhal_clock_get_frequency(&clock_hf0);
    if ( freq != old_freq )
    {
        /* Get the PLL and PERI resource */
        CHECK_SIGMA_APP_RETURN(cyhal_clock_get(&clock_pll,  (const cyhal_resource_inst_t *)&CYHAL_CLOCK_PLL));
        CHECK_SIGMA_APP_RETURN(cyhal_clock_get(&clock_peri, (const cyhal_resource_inst_t *)&CYHAL_CLOCK_PERI));

        /* Initialize, take ownership of, the PLL instance */
        CHECK_SIGMA_APP_RETURN(cyhal_clock_init(&clock_pll));

        /* Set CPU clock to freq */
        CHECK_SIGMA_APP_RETURN(cyhal_clock_set_frequency(&clock_pll, freq, NULL));

        /* If the PLL is not already enabled, enable it */
        if (!cyhal_clock_is_enabled(&clock_pll))
        {
            CHECK_SIGMA_APP_RETURN(cyhal_clock_set_enabled(&clock_pll, true, true));
        }

        /* Initialize, take ownership of, the PERI instance */
        CHECK_SIGMA_APP_RETURN(cyhal_clock_init(&clock_peri));

        /* Initialize, take ownership of, the HF0 instance */
        CHECK_SIGMA_APP_RETURN(cyhal_clock_init(&clock_hf0));

        /* Set peri clock divider */
        CHECK_SIGMA_APP_RETURN(cyhal_clock_set_divider(&clock_peri, CPU_PERI_CLOCK_DIV));

        /* set HF0 Clock source to PLL */
        CHECK_SIGMA_APP_RETURN(cyhal_clock_set_source(&clock_hf0, &clock_pll));
    }
    return ret;
}
#endif
#endif

#if defined(__GNUC__) && !(defined(__ICCARM__) || defined(__clang__) || defined(__CC_ARM))
void optimized_wordsize_memcpy(void *dst, const void *src, size_t len)
{
    volatile unsigned int *pdest1 = (unsigned int *)dst;
    volatile unsigned int *psrc1  = (unsigned int*)src;
    volatile unsigned char *psrc;
    volatile unsigned char *pdest;
    int index;

    if ( (len != 0 ) && ( len > 4 ))
    {
        do
        {
            *pdest1 = *psrc1;
            pdest1++;
            psrc1++;
            len -= 4;
        } while ( len >= 4 );
   }
   psrc  = (unsigned char *)psrc1;
   pdest = (unsigned char *)pdest1;

   for ( index = 0; index < len; index++)
   {
       *pdest = *psrc;
       pdest++;
       psrc++;
   }
}
#endif
