/*
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-09-14     Tibbers       the first version
 */
#ifndef _DRV_FLASH_H_
#define _DRV_FLASH_H_

#include "stm32f4xx_hal.h"

extern SPI_HandleTypeDef hspi1;

#define FLASH_CS_Pin GPIO_PIN_14
#define FLASH_CS_GPIO_Port GPIOB

#define SPI_CS_0  HAL_GPIO_WritePin(FLASH_CS_GPIO_Port, FLASH_CS_Pin, GPIO_PIN_RESET)
#define SPI_CS_1  HAL_GPIO_WritePin(FLASH_CS_GPIO_Port, FLASH_CS_Pin, GPIO_PIN_SET)

uint8_t User_SPI_Trans_Rec(uint8_t Txdata);

#endif
