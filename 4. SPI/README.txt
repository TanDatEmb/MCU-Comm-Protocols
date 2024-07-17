## SPI (Serial Peripheral Interface)

### Structure
- Master
- Slave
- MOSI (Master Out Slave In)
- MISO (Master In Slave Out)
- SCK (Clock)
- SS (Slave Select)

### Working Principle
- Full-duplex communication
- Clock polarity (CPOL) and clock phase (CPHA)

### Data Frame
[SS Low] [Data Transfer] [SS High]
Notes
- Ensure proper configuration of CPOL and CPHA.
- atch data transfer speeds between master and slave.

### Example Code

#### Arduino
```cpp
#include <SPI.h>

void setup() {
  SPI.begin();
  pinMode(SS, OUTPUT);
}

void loop() {
  digitalWrite(SS, LOW);
  SPI.transfer(0x42);
  digitalWrite(SS, HIGH);
  delay(1000);
}
#### STM32
// Include necessary headers
#include "stm32f4xx_hal.h"

SPI_HandleTypeDef hspi1;

void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI1_Init(void);

int main(void) {
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_SPI1_Init();

  uint8_t data = 0x42;
  while (1) {
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
    HAL_SPI_Transmit(&hspi1, &data, 1, HAL_MAX_DELAY);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
    HAL_Delay(1000);
  }
}

void SystemClock_Config(void) { /* ... */ }
static void MX_GPIO_Init(void) { /* ... */ }
static void MX_SPI1_Init(void) {
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  HAL_SPI_Init(&hspi1);
}
