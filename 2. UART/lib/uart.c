#include "uart.h"
#include "stm32f4xx.h"  // Thay đổi theo vi điều khiển của bạn

static UART_Config uart_config;

void UART_Init(UART_Config *config) {
    uart_config = *config;

    // Kích hoạt đồng hồ cho UART
    if (config->base_addr == USART1) {
        RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
    } else if (config->base_addr == USART2) {
        RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
    }

    // Cấu hình tốc độ baud rate
    uint32_t uart_div = SystemCoreClock / (16 * config->baud_rate);
    config->base_addr->BRR = uart_div;

    // Cấu hình data bits, parity, stop bits
    config->base_addr->CR1 = 0;
    config->base_addr->CR1 |= (config->data_bits << USART_CR1_M_Pos);
    if (config->parity) {
        config->base_addr->CR1 |= USART_CR1_PCE;  // Enable parity
        if (config->parity == 1) {
            config->base_addr->CR1 |= USART_CR1_PPS;
        }
    }
    if (config->stop_bits == 2) {
        config->base_addr->CR2 |= USART_CR2_STOP_1;
    }

    // Cấu hình chế độ truyền và nhận
    config->base_addr->CR1 |= USART_CR1_TE | USART_CR1_RE;

    // Bật UART
    config->base_addr->CR1 |= USART_CR1_UE;
}

UART_Status UART_Transmit(uint8_t *data, uint16_t size) {
    for (int i = 0; i < size; i++) {
        while (!(USART1->SR & USART_SR_TXE)) {}
        USART1->DR = data[i];
    }
    while (!(USART1->SR & USART_SR_TC)) {}
    return UART_OK;
}

UART_Status UART_Receive(uint8_t *data, uint16_t size) {
    for (int i = 0; i < size; i++) {
        while (!(USART1->SR & USART_SR_RXNE)) {}
        data[i] = USART1->DR;
    }
    return UART_OK;
}

// Cấu hình Interrupt

void UART1_IRQHandler(void) {
    if (USART1->SR & USART_SR_RXNE) {
        // Xử lý dữ liệu nhận
    }
    if (USART1->SR & USART_SR_TXE) {
        // Xử lý dữ liệu truyền
    }
}

// Cấu hình DMA cho UART

void UART_Transmit_DMA(uint8_t *data, uint16_t size) {
    // Cấu hình DMA
    DMA1_Stream6->PAR = (uint32_t)&USART1->DR;
    DMA1_Stream6->M0AR = (uint32_t)data;
    DMA1_Stream6->NDTR = size;
    DMA1_Stream6->CR |= DMA_SxCR_EN;
    
    // Kích hoạt DMA
    USART1->CR3 |= USART_CR3_DMAT;
}

void UART_Receive_DMA(uint8_t *data, uint16_t size) {
    // Cấu hình DMA
    DMA1_Stream1->PAR = (uint32_t)&USART1->DR;
    DMA1_Stream1->M0AR = (uint32_t)data;
    DMA1_Stream1->NDTR = size;
    DMA1_Stream1->CR |= DMA_SxCR_EN;
    
    // Kích hoạt DMA
    USART1->CR3 |= USART_CR3_DMAR;
}

// Cấu hình Error

// uart.c

UART_Status UART_CheckError(void) {
    if (USART1->SR & (USART_SR_ORE | USART_SR_FE | USART_SR_NE)) {
        // Xử lý lỗi
        return UART_ERROR;
    }
    return UART_OK;
}
