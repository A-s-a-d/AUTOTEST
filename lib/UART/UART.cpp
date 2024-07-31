#include "UART.h"

UART_2::UART_2(/* args */)
{
}

void UART_2::init()
{
    // Configure UART parameters
    uart_param_config(UART_NUM, &uart_config);
    // Set UART pins (TX, RX, RTS, CTS)
    uart_set_pin(UART_NUM, TXD_PIN, RXD_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    // Install UART driver, and get the queue.
    uart_driver_install(UART_NUM, 1024 * 2, 0, 0, NULL, 0);
    // Invert TX and RX signals
    // uart_set_line_inverse(UART_NUM, UART_SIGNAL_TXD_INV | UART_SIGNAL_RXD_INV);
}

void UART_2::write(const char *test_str)
{
    uart_write_bytes(UART_NUM, test_str, strlen(test_str));
    delay(50);
    uart_write_bytes(UART_NUM, CR, strlen(CR));
}

char UART_2::read()
{
    char *read_str;
    uart_read_bytes(UART_NUM, read_str, 50, 100);
    return *read_str;
}
