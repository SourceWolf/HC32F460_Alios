#ifndef SYS_INIT_H
#define SYS_INIT_H
#include "hc32_ddl.h"


void HAL_IncTick(void);	 
uint32_t HAL_GetTick(void);	 
void HAL_Delay(volatile uint32_t Delay);	 
uint32_t HAL_GetHalVersion(void);


#endif



