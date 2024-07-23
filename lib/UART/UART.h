#ifndef UART_H
#define UART_H

#include <Arduino.h>
#include "driver/uart.h"
#include <../pin_out.h>


#define UART_NUM UART_NUM_2

#define carriage_return "\r"
#define CR carriage_return

const uart_config_t uart_config = {
    .baud_rate = 9600,
    .data_bits = UART_DATA_8_BITS,
    .parity = UART_PARITY_EVEN,
    .stop_bits = UART_STOP_BITS_1,
    .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
    .source_clk = UART_SCLK_APB,
};

class UART_2
{
private:
public:
    UART_2(/* args */);
    void init();
    void write(const char *test_str);
    void writepH(const char *pH);
    char read();
};

#endif