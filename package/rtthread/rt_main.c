/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2021-12-07     lyon       the first version
 */
#include <rtthread.h>
#include <pikaScript.h>

#define SAMPLE_UART_NAME       RT_CONSOLE_DEVICE_NAME    /* 串口设备名称 */
static rt_device_t serial; /* 串口设备句柄 */
static struct rt_semaphore rx_sem; /* 用于接收消息的信号量 */

#define RX_Buff_SIZE 256
char rxBuff[RX_Buff_SIZE] = { 0 };
Args *rxArgs;
uint8_t isRxOk = 0;
PikaObj *pikaMain;

void rx_single_line_handle(char *line) {
    rt_kprintf("\r\n");
    obj_run(pikaMain, line);
    rt_kprintf(">>>");
}

/* 接收数据回调函数 */
static rt_err_t uart_input_callback(rt_device_t dev, rt_size_t size) {
    /* 串口接收到数据后产生中断，调用此回调函数，然后发送接收信号量 */
    char inputChar;
    rt_device_read(serial, -1, &inputChar, 1);
    rt_device_write(serial, 0, &inputChar, 1);
    if (inputChar == '\b'){
        rt_device_write(serial, 0, " ", 1);
        rt_device_write(serial, 0, &inputChar, 1);
        uint32_t size = strGetSize(rxBuff);
        rxBuff[size - 1] = 0;
        return RT_EOK;
    }
    if (inputChar != '\r' && inputChar != '\n') {
        strAppendWithSize(rxBuff, &inputChar, 1);
    }
    if (inputChar == '\r') {
        isRxOk = 1;
    }
    return RT_EOK;
}

static void clearBuff(char *buff, uint32_t size) {
    for (int i = 0; i < size; i++) {
        buff[i] = 0;
    }
}

static void uart_init() {
    serial = rt_device_find(SAMPLE_UART_NAME);

    /* 以中断接收及轮询发送模式打开串口设备 */
    rt_device_open(serial, RT_DEVICE_FLAG_INT_RX);

    /* 初始化信号量 */
    rt_sem_init(&rx_sem, "rx_sem", 0, RT_IPC_FLAG_FIFO);

    /* 设置接收回调函数 */
    rt_device_set_rx_indicate(serial, uart_input_callback);
}

static int rt_pika_main(void)
{
    uart_init();
    pikaMain = pikaScriptInit();
    clearBuff(rxBuff, RX_Buff_SIZE);
    rt_kprintf(">>>");
    while (1) {
        if (isRxOk) {
            isRxOk = 0;
            rx_single_line_handle(rxBuff);
            clearBuff(rxBuff, RX_Buff_SIZE);
        }
    }
    return RT_EOK;
}
INIT_APP_EXPORT(rt_pika_main);
