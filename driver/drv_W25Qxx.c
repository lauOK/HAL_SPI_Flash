/*
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-09-09     Tibbers       the first version
 */

#include "drv_W25Qxx.h"
#include "drv_spi_flash.h"

void Flash_Write_Enable(void)
{
    SPI_CS_0;
    User_SPI_Trans_Rec(W25X_WriteEnable);
    SPI_CS_1;
}

void Flash_Write_Disable(void)
{
    SPI_CS_0;
    User_SPI_Trans_Rec(W25X_WriteDisable);
    SPI_CS_1;
}

uint8_t Flash_ReadSR_1(void)
{
    SPI_CS_0;
    uint8_t Rxdata = 0;
    User_SPI_Trans_Rec(W25X_ReadStatusReg_1);
    Rxdata = User_SPI_Trans_Rec(0xFF);
    SPI_CS_1;

    return Rxdata;
}

uint8_t Flash_ReadSR_2(void)
{
    SPI_CS_0;
    uint8_t Rxdata = 0;
    User_SPI_Trans_Rec(W25X_ReadStatusReg_2);
    Rxdata = User_SPI_Trans_Rec(0xFF);
    SPI_CS_1;

    return Rxdata;
}

void Flash_WriteSR(uint8_t sr1, uint8_t sr2)
{
    while(Flash_CheckBusy());
    Flash_Write_Enable();
    SPI_CS_0;
    User_SPI_Trans_Rec(W25X_WriteStatusReg);
    User_SPI_Trans_Rec(sr1);
    User_SPI_Trans_Rec(sr2);
    SPI_CS_1;
}

void Flash_Read(uint32_t addr, uint8_t *pData, uint8_t numBytes)
{
    while(Flash_CheckBusy());
    SPI_CS_0;
    User_SPI_Trans_Rec(W25X_ReadData);
    User_SPI_Trans_Rec(addr >> 16);
    User_SPI_Trans_Rec(addr >> 8);
    User_SPI_Trans_Rec(addr);
    while(numBytes--)
    {
        *pData = User_SPI_Trans_Rec(0xFF);
        pData++;
    }
    SPI_CS_1;
}

void Flash_FastRead(uint32_t addr, uint8_t *pData, uint8_t numBytes)
{
    while(Flash_CheckBusy());
    SPI_CS_0;
    User_SPI_Trans_Rec(W25X_FastReadData);
    User_SPI_Trans_Rec(addr >> 16);
    User_SPI_Trans_Rec(addr >> 8);
    User_SPI_Trans_Rec(addr);
    User_SPI_Trans_Rec(0xFF);
    while(numBytes--)
    {
        *pData = User_SPI_Trans_Rec(0xFF);
        pData++;
    }
    SPI_CS_1;
}

void Flash_PageProgram(uint32_t addr, uint8_t *pData, uint8_t numBytes)
{
    while(Flash_CheckBusy());
    Flash_Write_Enable();
    SPI_CS_0;
    User_SPI_Trans_Rec(W25X_PageProgram);
    User_SPI_Trans_Rec(addr >> 16);
    User_SPI_Trans_Rec(addr >> 8);
    User_SPI_Trans_Rec(addr);
    while(numBytes--)
    {
        User_SPI_Trans_Rec(*pData);
        pData++;
    }
    SPI_CS_1;
}

void Flash_SectorErase(uint32_t addr)
{
    while(Flash_CheckBusy());
    Flash_Write_Enable();
    SPI_CS_0;
    User_SPI_Trans_Rec(W25X_SectorErase);
    User_SPI_Trans_Rec(addr >> 16);
    User_SPI_Trans_Rec(addr >> 8);
    User_SPI_Trans_Rec(addr);
    SPI_CS_1;
}

void Flash_BlockErase(uint32_t addr)
{
    while(Flash_CheckBusy());
    Flash_Write_Enable();
    SPI_CS_0;
    User_SPI_Trans_Rec(W25X_BlockErase);
    User_SPI_Trans_Rec(addr >> 16);
    User_SPI_Trans_Rec(addr >> 8);
    User_SPI_Trans_Rec(addr);
    SPI_CS_1;
}

void Flash_ChipErase(void)
{
    while(Flash_CheckBusy());
    Flash_Write_Enable();
    SPI_CS_0;
    User_SPI_Trans_Rec(W25X_ChipErase);
    SPI_CS_1;
}

void Flash_Powerdown(void)
{
    SPI_CS_0;
    User_SPI_Trans_Rec(W25X_PowerDown);
    SPI_CS_1;
}

void Flash_ReleasePowerDown(void)
{
    SPI_CS_0;
    User_SPI_Trans_Rec(W25X_ReleasePowerDown);
    SPI_CS_1;
}

uint16_t Flash_ReadID(void)
{
    uint16_t id = 0;
    while(Flash_CheckBusy());
    SPI_CS_0;
    User_SPI_Trans_Rec(W25X_ManufactDeviceID);
    User_SPI_Trans_Rec(0x00);
    User_SPI_Trans_Rec(0x00);
    User_SPI_Trans_Rec(0x00);
    id |= User_SPI_Trans_Rec(0xFF) << 8;
    id |= User_SPI_Trans_Rec(0xFF);
    SPI_CS_1;
    return id;
}

int Flash_CheckBusy(void)
{
    uint8_t sr = Flash_ReadSR_1();
    if((sr &= 0x01) == 0)
    {
        return 0;
    }
    return 1;
}
