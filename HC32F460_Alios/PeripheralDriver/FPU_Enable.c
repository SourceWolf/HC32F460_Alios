#include "FPU_Enable.h"
void FPU_Enable( void )
{
#if (__FPU_PRESENT == 1)&&(__FPU_USED == 1)//#if defined (__CC_ARM) && defined (__TARGET_FPU_VFP)
    SCB->CPACR |= 0x00F00000;
#endif
}
//__asm void FPU_Enable( void )
//{
// PRESERVE8

// /* The FPU enable bits are in the CPACR. */
// ldr.w r0, =0xE000ED88
// ldr r1, [r0]

// /* Enable CP10 and CP11 coprocessors, then save back. */
// orr r1, r1, #( 0xf << 20 )
// str r1, [r0]
// bx r14
// nop
//}

