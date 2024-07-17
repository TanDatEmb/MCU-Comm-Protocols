## I2C (Inter-Integrated Circuit)

### Structure
- Master
- Slave
- SDA (Data line)
- SCL (Clock line)

### Working Principle
- Synchronous communication
- Addressing
- Data framing: start condition, address, data, stop condition

### Data Frame
[Start Condition] [Address] [Data] [Stop Condition]
Notes
- Ensure pull-up resistors on SDA and SCL lines.
- Verify slave addresses and data integrity.

### Example Code

#### Arduino
```cpp
#include <Wire.h>

void setup() {
  Wire.begin(); // Join I2C bus as master
}

void loop() {
  Wire.beginTransmission(0x08); // Transmit to device #8
  Wire.write("Hello, I2C!");
  Wire.endTransmission();
  delay(1000);
}
#### STM32
// Include necessary headers
#include "stm32f4xx_hal.h"

I2C_HandleTypeDef hi2c1;

void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C1_Init(void);

int main(void) {
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_I2C1_Init();

  uint8_t data[] = "Hello, I2C!";
  while (1) {
    HAL_I2C_Master_Transmit(&hi2c1, 0x08 << 1, data, sizeof(data)-1, HAL_MAX_DELAY);
    HAL_Delay(1000);
  }
}

void SystemClock_Config(void) { /* ... */ }
static void MX_GPIO_Init(void) { /* ... */ }
static void MX_I2C1_Init(void) {
  hi2c1.Instance = I2C1;
  hi2c1.Init.ClockSpeed = 100000;
  hi2c1.Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  HAL_I2C_Init(&hi2c1);
}
