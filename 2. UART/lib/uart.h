#ifndef UART_H
#define UART_H

#include <stdint.h>
#include <stdbool.h>

// Cấu trúc dữ liệu để cấu hình UART
typedef struct {
    uint32_t baud_rate;   // Tốc độ truyền dữ liệu
    uint8_t data_bits;   // Số bit dữ liệu (5-9)
    bool parity;        // Parity (0: None, 1: Even)
    uint8_t stop_bits;   // Số bit dừng (1 hoặc 2)
    uint32_t base_addr;  // Địa chỉ cơ sở của UART (USART1, USART2, ...)
} UART_Config;

// Các trạng thái của UART
typedef enum {
    UART_OK,
    UART_ERROR
} UART_Status;

// Khai báo các hàm thư viện
void UART_Init(UART_Config *config);
UART_Status UART_Transmit(uint8_t *data, uint16_t size);
UART_Status UART_Receive(uint8_t *data, uint16_t size);

#endif // UART_H
