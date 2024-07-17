## UART (Universal Asynchronous Receiver-Transmitter)

### Structure
- Transmitter
- Receiver

### Working Principle
- Asynchronous communication
- Baud rate
- Data framing: start bit, data bits, parity bit, stop bit

### Data Frame
[Start Bit] [Data Bits] [Parity Bit] [Stop Bit]
Notes
- Ensure matching baud rates between transmitter and receiver.
- Use proper level shifters for voltage compatibility.

### Example Code

#### Arduino
```cpp
void setup() {
  Serial.begin(9600); // Initialize UART with 9600 baud rate
}

void loop() {
  Serial.println("Hello, UART!"); // Send data
  delay(1000);
}

#### STM32
// Include necessary headers
#include "stm32f4xx_hal.h"

UART_HandleTypeDef huart2;

void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);

int main(void) {
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_USART2_UART_Init();

  char msg[] = "Hello, UART!\r\n";
  while (1) {
    HAL_UART_Transmit(&huart2, (uint8_t*)msg, sizeof(msg)-1, HAL_MAX_DELAY);
    HAL_Delay(1000);
  }
}

void SystemClock_Config(void) { /* ... */ }
static void MX_GPIO_Init(void) { /* ... */ }
static void MX_USART2_UART_Init(void) {
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 9600;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  HAL_UART_Init(&huart2);
}
