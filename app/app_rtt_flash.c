/*
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-10-22     Tibbers       the first version
 */
#include "app_user.h"
#include "drv_spi.h"
#include "spi_flash_sfud.h"

static int rt_hw_spi_flash_init(void)
{
    __HAL_RCC_GPIOB_CLK_ENABLE();
    rt_hw_spi_device_attach("spi1", "spi10", GPIOB, GPIO_PIN_14);

    if (RT_NULL == rt_sfud_flash_probe("W25Q64", "spi10"))
    {
        return -RT_ERROR;
    }
    return RT_EOK;
}
INIT_COMPONENT_EXPORT(rt_hw_spi_flash_init);

#define W25Q_SPI_DEVICE_NAME     "spi10"

static void spi_w25q_sample(int argc, char *argv[])
{
    struct rt_spi_device *spi_dev_w25q;
    char name[RT_NAME_MAX];
    rt_uint32_t w25x_read_id = 0x90;  //0x00000090
    rt_uint32_t w25x_read_fast = 0x03;//0x00000003
    rt_uint32_t addr = 0x10;          //0x00001003
    rt_uint8_t id[5] = {0};
    rt_uint8_t data1[50] = {0};
    rt_uint8_t data2[50] = {0};

    if (argc == 2)
    {
        rt_strncpy(name, argv[1], RT_NAME_MAX);
    }
    else
    {
        rt_strncpy(name, W25Q_SPI_DEVICE_NAME, RT_NAME_MAX);
    }

    /* 查找 spi 设备获取设备句柄 */
    spi_dev_w25q = (struct rt_spi_device *)rt_device_find(name);
    if (!spi_dev_w25q)
    {
        rt_kprintf("spi sample run failed! can't find %s device!\n", name);
    }
    else
    {
        /* 配置SPI*/
        struct rt_spi_configuration cfg;
        cfg.data_width = 8;
        cfg.mode = RT_SPI_MASTER | RT_SPI_MODE_3 | RT_SPI_MSB;
        cfg.max_hz = 20 * 1000 *1000;                           /* 20M */
        if(rt_spi_configure(spi_dev_w25q, &cfg) != RT_EOK)
        {
            rt_kprintf("spi config failed!\n");
        }
        else
        {
            /* 方式1：使用 rt_spi_send_then_recv()发送命令读取ID */
            rt_spi_send_then_recv(spi_dev_w25q, &w25x_read_id, 4, id, 2);
            rt_kprintf("use rt_spi_send_then_recv() read w25q ID is:%x%x\n",id[0],id[1]);

            rt_spi_send_then_recv(spi_dev_w25q, &w25x_read_fast, 4, data1, 11);
            rt_kprintf("use rt_spi_send_then_recv() read w25q is:%s\n",data1);

            /* 方式2：使用 rt_spi_transfer_message()发送命令读取 */

            struct rt_spi_message msg1, msg2, msg3;

            msg1.send_buf   = &w25x_read_fast;
            msg1.recv_buf   = RT_NULL;
            msg1.length     = 1;
            msg1.cs_take    = 1;
            msg1.cs_release = 0;
            msg1.next       = &msg2;

            msg2.send_buf   = &addr;
            msg2.recv_buf   = RT_NULL;
            msg2.length     = 3;
            msg2.cs_take    = 1;
            msg2.cs_release = 0;
            msg2.next       = &msg3;

            msg3.send_buf   = RT_NULL;
            msg3.recv_buf   = data2;
            msg3.length     = 11;
            msg3.cs_take    = 0;
            msg3.cs_release = 1;
            msg3.next       = RT_NULL;

            rt_spi_transfer_message(spi_dev_w25q, &msg1);
            rt_kprintf("use rt_spi_transfer_message() read w25q is:%s\n", data2);
        }
    }
}
/* 导出到 msh 命令列表中 */
MSH_CMD_EXPORT(spi_w25q_sample, spi w25q sample);
