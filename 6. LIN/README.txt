## LIN (Local Interconnect Network)

### Structure
- Master
- Slave

### Working Principle
- Single-wire communication
- Synchronization
- Signal frame: sync break, sync field, identifier, data, checksum

### Data Frame
[Sync Break] [Sync Field] [Identifier] [Data] [Checksum]
Notes
- Ensure proper synchronization between master and slaves.
- Use appropriate checksum calculation for data integrity.

### Example Code

#### Arduino (simulated)
```cpp
#include <SoftwareSerial.h>

SoftwareSerial linSerial(10, 11); // RX, TX

void setup() {
  linSerial.begin(9600);
}

void loop() {
  linSerial.write(0x55); // Sync field
  linSerial.write(0x01); // Identifier
  linSerial.write(0xFF); // Data
  linSerial.write(0xFE); // Checksum
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

  uint8_t linData[] = {0x55, 0x01, 0xFF, 0xFE};
  while (1) {
    HAL_UART_Transmit(&huart2, linData, sizeof(linData), HAL_MAX_DELAY);
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
