/*
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-09-09     Tibbers       the first version
 */
#ifndef _DRV_W25Q16_H_
#define _DRV_W25Q16_H_

#include "stm32f4xx_hal.h"

//Command Table
#define W25X_WriteEnable                0x06    //写使能
#define W25X_WriteDisable               0x04    //写禁能
#define W25X_ReadStatusReg_1            0x05    //读状态寄存器1
#define W25X_ReadStatusReg_2            0x35    //读状态寄存器2
#define W25X_WriteStatusReg             0x01    //写状态寄存器
#define W25X_ReadData                   0x03    //读数据
#define W25X_FastReadData               0x0B    //快读数据
#define W25X_PageProgram                0x02    //页编程
#define W25X_BlockErase                 0xD8    //块擦除(64KB)
#define W25X_SectorErase                0x20    //扇擦除(4KB)
#define W25X_ChipErase                  0xC7    //芯片擦除
#define W25X_PowerDown                  0xB9    //掉电
#define W25X_ReleasePowerDown           0xAB    //释放掉电
#define W25X_ManufactDeviceID           0x90    //制造商ID
#define W25X_JedecDeviceID              0x9F    //JEDEC ID

void MX_SPI1_Init(void);
void MX_GPIO_Init(void);

void Flash_Write_Enable(void);
void Flash_Write_Disable(void);
uint8_t Flash_ReadSR_1(void);
uint8_t Flash_ReadSR_2(void);
void Flash_WriteSR(uint8_t sr1, uint8_t sr2);
void Flash_Read(uint32_t addr, uint8_t *pData, uint8_t numBytes);
void Flash_FastRead(uint32_t addr, uint8_t *pData, uint8_t numBytes);
void Flash_PageProgram(uint32_t addr, uint8_t *pData, uint8_t numBytes);
void Flash_SectorErase(uint32_t addr);
void Flash_BlockErase(uint32_t addr);
void Flash_ChipErase(void);
void Flash_Powerdown(void);
void Flash_ReleasePowerDown(void);
uint16_t Flash_ReadID(void);
int Flash_CheckBusy(void);

#endif
