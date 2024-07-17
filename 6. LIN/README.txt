## LIN (Local Interconnect Network)

### Cấu Trúc
- **Master (Thiết bị chủ)**
- **Slave (Thiết bị phụ)**

### Nguyên Lý Hoạt Động
- **Giao tiếp nối tiếp (Serial Communication)**
- **Master/Slave Communication**: Thiết bị chủ điều khiển truyền thông và thiết bị phụ phản hồi.
- **Low-Cost**: Giải pháp truyền thông chi phí thấp cho các ứng dụng trong xe hơi.
- **Single-Wire Communication**: Chỉ cần một dây để truyền và nhận dữ liệu.

### Khung Dữ Liệu (Data Frame)

[Break Field] [Sync Field] [Identifier Field] [Data Field] [Checksum Field]

- **Lưu ý**
  - Thiết bị chủ gửi khung điều khiển, bao gồm Break Field, Sync Field và Identifier Field.
  - Thiết bị phụ phản hồi bằng cách gửi dữ liệu và checksum.

### Cấu Trúc Của Một Frame LIN
Một khung dữ liệu LIN thông thường bao gồm:

- **Break Field**: Chuỗi bit kéo dài, báo hiệu bắt đầu khung.
- **Sync Field**: Byte đồng bộ, giá trị cố định là 0x55 để đồng bộ tốc độ baud.
- **Identifier Field**: Byte định danh, xác định loại khung và thiết bị phụ nào cần phản hồi.
- **Data Field**: Dữ liệu cần truyền, có độ dài từ 2 đến 8 byte.
- **Checksum Field**: Byte kiểm tra lỗi, tính toán từ dữ liệu và định danh.

### Thanh Ghi LIN Trong STM32
1. **USART Control Register (USART_CR1)**
    - **USART_CR1_UE (USART Enable)**: Bật USART.
    - **USART_CR1_M (Word Length)**: Thiết lập độ dài từ.
    - **USART_CR1_TE (Transmitter Enable)**: Bật bộ truyền.
    - **USART_CR1_RE (Receiver Enable)**: Bật bộ nhận.

2. **USART Control Register (USART_CR2)**
    - **USART_CR2_LINEN (LIN Mode Enable)**: Bật chế độ LIN.
    - **USART_CR2_LBDIE (LIN Break Detection Interrupt Enable)**: Bật ngắt phát hiện break.
    - **USART_CR2_LBDL (LIN Break Detection Length)**: Thiết lập độ dài break.

3. **USART Status Register (USART_SR)**
    - **USART_SR_LBD (LIN Break Detection Flag)**: Cờ phát hiện break.
    - **USART_SR_RXNE (Read Data Register Not Empty)**: Bộ đệm nhận không rỗng.
    - **USART_SR_TXE (Transmit Data Register Empty)**: Bộ đệm truyền rỗng.

4. **USART Data Register (USART_DR)**
    - Thanh ghi này được sử dụng để đọc dữ liệu nhận hoặc ghi dữ liệu truyền.

### Chế Độ Hoạt Động
1. **Chế độ chủ (Master Mode)**
    - Thiết bị chủ khởi tạo và kiểm soát giao tiếp trên bus.
    - Thiết bị chủ gửi Break Field, Sync Field và Identifier Field.

2. **Chế độ phụ (Slave Mode)**
    - Thiết bị phụ chờ các lệnh và dữ liệu từ thiết bị chủ.
    - Thiết bị phụ phản hồi lại dữ liệu và checksum theo yêu cầu của thiết bị chủ.

### Ví Dụ Code

#### Arduino

#include <SoftwareSerial.h>

SoftwareSerial LIN(10, 11); // RX, TX

void setup() {
  LIN.begin(9600); // Khởi tạo LIN với baud rate 9600
  Serial.begin(9600); // Khởi tạo UART với baud rate 9600
}

void loop() {
  byte syncField = 0x55;
  byte identifierField = 0x01;
  byte dataField[2] = {0x10, 0x20};
  byte checksumField = syncField + identifierField + dataField[0] + dataField[1];

  LIN.write(syncField);
  LIN.write(identifierField);
  LIN.write(dataField, 2);
  LIN.write(checksumField);

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

  uint8_t syncField = 0x55;
  uint8_t identifierField = 0x01;
  uint8_t dataField[2] = {0x10, 0x20};
  uint8_t checksumField = syncField + identifierField + dataField[0] + dataField[1];

  while (1) {
    HAL_UART_Transmit(&huart2, &syncField, 1, HAL_MAX_DELAY);
    HAL_UART_Transmit(&huart2, &identifierField, 1, HAL_MAX_DELAY);
    HAL_UART_Transmit(&huart2, dataField, 2, HAL_MAX_DELAY);
    HAL_UART_Transmit(&huart2, &checksumField, 1, HAL_MAX_DELAY);

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
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  HAL_UART_Init(&huart2);
}
