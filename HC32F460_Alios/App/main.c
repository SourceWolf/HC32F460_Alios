//#include "hc32_ddl.h"
#include "sys_init.h"
#include <k_api.h>
#include "sys_init.h"
#include "cut.h"
#include "System_Clk.h"
//#include "User_Task.h"
//#include "Uart_DMA.h"
#include "FPU_Enable.h"
//#include "System_PowerDown.h"
//USB_OTG_CORE_HANDLE  USB_OTG_dev;
//stc_clk_freq_t Clkdata;
stc_clk_freq_t Clkdata;
extern void test_certificate(void);
int main(void)
{
#if defined (__CC_ARM) && defined (__TARGET_FPU_VFP)
    FPU_Enable();
#endif
    system_clk_init();
    Ddl_UartInit();
    printf("system_init\r\n");
    krhino_init();
    CLK_GetClockFreq(&Clkdata);
    SysTick_Config(Clkdata.hclkFreq/1000);
    NVIC_EnableIRQ(SysTick_IRQn);
    test_certificate();
    krhino_start();
    while(1)
    {
        ;
    }
}
