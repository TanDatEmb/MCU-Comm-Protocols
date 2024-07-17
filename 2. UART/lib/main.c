#include "uart.h"

int main(void) {
    UART_Config uart1_config = {
        .baud_rate = 9600,
        .data_bits = 8,
        .parity = false,
        .stop_bits = 1,
        .base_addr = USART1
    };

    UART_Init(&uart1_config);

    uint8_t tx_data[] = "Hello UART";
    UART_Transmit(tx_data, sizeof(tx_data) - 1);

    uint8_t rx_data[10];
    UART_Receive(rx_data, 10);

    return 0;
}
