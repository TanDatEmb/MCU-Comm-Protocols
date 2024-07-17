## SPI (Serial Peripheral Interface)

### Cấu Trúc
- **Master (Thiết bị chủ)**
- **Slave (Thiết bị phụ)**

### Nguyên Lý Hoạt Động
- **Giao tiếp nối tiếp (Serial Communication)**
- **Full-duplex Communication**: Có thể truyền và nhận dữ liệu đồng thời.
- **Tốc độ truyền dữ liệu (Data Transfer Rate)**: Tùy thuộc vào xung nhịp cung cấp bởi thiết bị chủ.
- **Dữ liệu truyền (Data Transfer)**: bit khởi đầu (start bit), bit dữ liệu (data bits), bit dừng (stop bit)

### Khung Dữ Liệu (Data Frame)

[MOSI (Master Out Slave In)] [MISO (Master In Slave Out)] [SCLK (Serial Clock)] [SS (Slave Select)]

- **Lưu ý**
  - Sử dụng bộ đệm dịch (shift register) để truyền và nhận dữ liệu.
  - Thiết bị chủ kiểm soát xung nhịp và chọn thiết bị phụ bằng cách kéo mức tín hiệu SS xuống thấp.

### Cấu Trúc Của Một Frame SPI
Một khung dữ liệu SPI thông thường bao gồm:

- **MOSI (Master Out Slave In)**: Dữ liệu được truyền từ thiết bị chủ đến thiết bị phụ.
- **MISO (Master In Slave Out)**: Dữ liệu được truyền từ thiết bị phụ đến thiết bị chủ.
- **SCLK (Serial Clock)**: Xung nhịp được cung cấp bởi thiết bị chủ để đồng bộ dữ liệu.
- **SS (Slave Select)**: Tín hiệu chọn thiết bị phụ, thường được kéo xuống thấp để kích hoạt thiết bị phụ.

### Thanh Ghi SPI Trong STM32
1. **SPI Control Register 1 (SPI_CR1)**
    - **SPI_CR1_CPHA (Clock Phase)**: Thiết lập pha của xung nhịp.
    - **SPI_CR1_CPOL (Clock Polarity)**: Thiết lập cực của xung nhịp.
    - **SPI_CR1_MSTR (Master Selection)**: Chọn chế độ chủ/phụ.
    - **SPI_CR1_BR (Baud Rate Control)**: Thiết lập tốc độ baud.
    - **SPI_CR1_SPE (SPI Enable)**: Bật SPI.
    - **SPI_CR1_LSBFIRST (Frame Format)**: Thiết lập thứ tự bit (LSB hoặc MSB trước).

2. **SPI Control Register 2 (SPI_CR2)**
    - **SPI_CR2_SSOE (SS Output Enable)**: Bật tín hiệu SS khi ở chế độ chủ.
    - **SPI_CR2_TXEIE (TX Buffer Empty Interrupt Enable)**: Bật ngắt khi bộ đệm truyền rỗng.
    - **SPI_CR2_RXNEIE (RX Buffer Not Empty Interrupt Enable)**: Bật ngắt khi bộ đệm nhận không rỗng.

3. **SPI Status Register (SPI_SR)**
    - **SPI_SR_RXNE (Receive Buffer Not Empty)**: Bit này được đặt khi bộ đệm nhận không rỗng.
    - **SPI_SR_TXE (Transmit Buffer Empty)**: Bit này được đặt khi bộ đệm truyền rỗng.
    - **SPI_SR_BSY (Busy)**: Bit này được đặt khi SPI đang bận.

4. **SPI Data Register (SPI_DR)**
    - Thanh ghi này được sử dụng để đọc dữ liệu nhận hoặc ghi dữ liệu truyền.

### Chế Độ Hoạt Động
1. **Chế độ chủ (Master Mode)**
    - Thiết bị chủ khởi tạo và kiểm soát giao tiếp trên bus.
    - Thiết bị chủ tạo ra xung nhịp và chọn thiết bị phụ bằng tín hiệu SS.

2. **Chế độ phụ (Slave Mode)**
    - Thiết bị phụ chờ các lệnh và dữ liệu từ thiết bị chủ.
    - Thiết bị phụ phản hồi lại dữ liệu theo yêu cầu của thiết bị chủ.

### Ví Dụ Code

#### Arduino

#include <SPI.h>

void setup() {
  SPI.begin(); // Khởi tạo SPI ở chế độ master
  Serial.begin(9600); // Khởi tạo UART với baud rate 9600
}

void loop() {
  digitalWrite(SS, LOW); // Chọn thiết bị phụ
  SPI.transfer(0x53); // Gửi dữ liệu 0x53
  digitalWrite(SS, HIGH); // Bỏ chọn thiết bị phụ

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

  uint8_t data = 0x53;
  while (1) {
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET); // Chọn thiết bị phụ
    HAL_SPI_Transmit(&hspi1, &data, 1, HAL_MAX_DELAY); // Gửi dữ liệu
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET); // Bỏ chọn thiết bị phụ

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
