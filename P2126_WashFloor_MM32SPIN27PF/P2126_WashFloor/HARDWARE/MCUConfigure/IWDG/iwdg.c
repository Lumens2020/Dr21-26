#include <stdio.h>
#include "HAL_conf.h"
#include "HAL_device.h"

/*******************************************************************************
* @name   : PVU_CheckStatus
* @brief  : Check IWDG prescaler status
* @param  : None
* @retval : void
*******************************************************************************/
void PVU_CheckStatus(void)
{
    while(1)
    {
        //Check the prescaler bit status. Only RESET can change the prescaler value.
        if(IWDG_GetFlagStatus(IWDG_FLAG_PVU) == RESET)
        {
            break;
        }
    }
}

/*******************************************************************************
* @name   : RVU_CheckStatus
* @brief  : Check IWDG reload status
* @param  : None
* @retval : void
*******************************************************************************/
void RVU_CheckStatus(void)
{
    while(1)
    {
        //Check overload flag status
        if(IWDG_GetFlagStatus(IWDG_FLAG_RVU) == RESET)
        {
            break;
        }
    }
}
void DelayLoop(u32 uldelay)
{
    u32 i, j;
    for (i = 0; i < uldelay; i++)
    {
        for (j = 0; j < 3000; j++)
        {

        }
    }
}

/*******************************************************************************
* @name                 : Write_Iwdg_PR
* @brief                : Start IWDG
* @param IWDG_Prescaler : IWDG_Prescaler_X, X is 4,8,16,32,64,128,256, corresponding to the same value as the X value
* @param Reload         : Counter reload value
* @retval               : void
*******************************************************************************/
void Write_Iwdg_ON(unsigned short int IWDG_Prescaler, unsigned short int Reload)
{
    /*Start the internal low-speed clock and wait for the clock to be ready*/
    RCC_LSICmd(ENABLE);
    while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET);

    /*Set clock prescaler*/
    PVU_CheckStatus();
    IWDG_WriteAccessCmd(0x5555);
    IWDG_SetPrescaler(IWDG_Prescaler);
    PVU_CheckStatus();
    /*Set the overload register value*/
    RVU_CheckStatus();
    IWDG_WriteAccessCmd(0x5555);
    IWDG_SetReload(Reload & 0xfff);
    RVU_CheckStatus();
    /*Load and enable counters*/
    IWDG_ReloadCounter();
    IWDG_Enable();
    DelayLoop(20);
}

/*******************************************************************************
* @name   : Write_Iwdg_RL
* @brief  : Reload function
* @param  : None
* @retval : void
*******************************************************************************/
void Write_Iwdg_RL(void)
{
    IWDG_ReloadCounter();
}



