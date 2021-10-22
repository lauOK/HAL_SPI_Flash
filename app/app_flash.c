/*
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-10-19     Tibbers       the first version
 */
#include <drv_spi_flash.h>
#include "drv_W25Q16.h"

int Task_Flash(void)
{
    int i = 0;
    uint16_t id;

    uint8_t pchar[11] = "hascovision";
    uint8_t rchar[11];

    MX_SPI1_Init();
    MX_GPIO_Init();

    id = Flash_ReadID();
    printf("0x%x\n",id);

    //Flash_ChipErase();
    Flash_BlockErase(0x000000);

    Flash_PageProgram(0x000000, pchar, 11);

    Flash_Read(0x000000, rchar, 11);

    for (i = 0; i < 11; ++i)
    {
        printf("%c",rchar[i]);
    }
    printf("\n");

    return 0;
}
