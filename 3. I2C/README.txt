## I2C (Inter-Integrated Circuit)

### Cấu Trúc
- **Master (Thiết bị chủ)**
- **Slave (Thiết bị phụ)**

### Nguyên Lý Hoạt Động
- **Giao tiếp nối tiếp (Serial Communication)**
- **Địa chỉ thiết bị (Device Addressing)**
- **Tốc độ truyền dữ liệu (Data Transfer Rate)**
- **Dữ liệu truyền (Data Transfer)**: bit khởi đầu (start bit), địa chỉ thiết bị (address bits), dữ liệu (data bits), bit dừng (stop bit)

### Khung Dữ Liệu (Data Frame)

[Start Bit] [Address Bits] [Read/Write Bit] [ACK/NACK Bit] [Data Bits] [ACK/NACK Bit] [Stop Bit]

- **Lưu ý**
  - Đảm bảo rằng tất cả các thiết bị trên cùng một bus phải có địa chỉ duy nhất.
  - Sử dụng điện trở kéo (pull-up resistors) để đảm bảo mức logic đúng trên các đường SDA và SCL.

### Cấu Trúc Của Một Frame I2C
Một khung dữ liệu I2C thông thường bao gồm:

- **Start Bit**: Bit bắt đầu, báo hiệu bắt đầu của quá trình truyền dữ liệu.
- **Address Bits**: 7 hoặc 10 bits địa chỉ của thiết bị.
- **Read/Write Bit**: 1 bit xác định hoạt động đọc (1) hoặc ghi (0).
- **ACK/NACK Bit**: 1 bit xác nhận (ACK) hoặc không xác nhận (NACK) từ thiết bị nhận.
- **Data Bits**: 8 bits dữ liệu.
- **Stop Bit**: Bit dừng, báo hiệu kết thúc của quá trình truyền dữ liệu.

### Thanh Ghi I2C Trong STM32
1. **I2C Control Register 1 (I2C_CR1)**
    - **I2C_CR1_PE (Peripheral Enable)**: Bật/tắt I2C.
    - **I2C_CR1_START (Start Generation)**: Tạo tín hiệu bắt đầu.
    - **I2C_CR1_STOP (Stop Generation)**: Tạo tín hiệu dừng.
    - **I2C_CR1_ACK (Acknowledge Enable)**: Bật/tắt tín hiệu xác nhận.
    - **I2C_CR1_POS (Acknowledge/PEC Position)**: Thiết lập vị trí bit ACK.

2. **I2C Control Register 2 (I2C_CR2)**
    - **I2C_CR2_FREQ (Peripheral Clock Frequency)**: Thiết lập tần số xung nhịp của thiết bị.
    - **I2C_CR2_ITERREN (Error Interrupt Enable)**: Bật/tắt ngắt lỗi.
    - **I2C_CR2_ITEVTEN (Event Interrupt Enable)**: Bật/tắt ngắt sự kiện.
    - **I2C_CR2_ITBUFEN (Buffer Interrupt Enable)**: Bật/tắt ngắt bộ đệm.

3. **I2C Status Register 1 (I2C_SR1)**
    - **I2C_SR1_SB (Start Bit)**: Bit này được đặt khi đã tạo ra tín hiệu bắt đầu.
    - **I2C_SR1_ADDR (Address Sent/Matched)**: Bit này được đặt khi đã gửi địa chỉ hoặc địa chỉ phù hợp.
    - **I2C_SR1_BTF (Byte Transfer Finished)**: Bit này được đặt khi truyền byte dữ liệu xong.
    - **I2C_SR1_STOPF (Stop Detection)**: Bit này được đặt khi đã phát hiện tín hiệu dừng.
    - **I2C_SR1_RXNE (Receive Buffer Not Empty)**: Bit này được đặt khi bộ đệm nhận không rỗng.
    - **I2C_SR1_TXE (Transmit Buffer Empty)**: Bit này được đặt khi bộ đệm truyền rỗng.

4. **I2C Status Register 2 (I2C_SR2)**
    - **I2C_SR2_MSL (Master/Slave)**: Cho biết thiết bị đang ở chế độ chủ hay chế độ phụ.
    - **I2C_SR2_BUSY (Bus Busy)**: Cho biết bus đang bận hay không.
    - **I2C_SR2_TRA (Transmitter/Receiver)**: Cho biết thiết bị đang ở chế độ phát hay chế độ nhận.

### Chế Độ Hoạt Động
1. **Chế độ chủ (Master Mode)**
    - Thiết bị chủ khởi tạo và kiểm soát giao tiếp trên bus.
    - Thiết bị chủ tạo ra các tín hiệu bắt đầu (start) và dừng (stop).

2. **Chế độ phụ (Slave Mode)**
    - Thiết bị phụ chờ các lệnh và địa chỉ từ thiết bị chủ.
    - Thiết bị phụ phản hồi với tín hiệu xác nhận (ACK) hoặc không xác nhận (NACK).

### Ví Dụ Code

#### Arduino

#include <Wire.h>

void setup() {
  Wire.begin(); // Khởi tạo I2C ở chế độ master
  Serial.begin(9600); // Khởi tạo UART với baud rate 9600
}

void loop() {
  Wire.beginTransmission(8); // Bắt đầu truyền dữ liệu tới thiết bị với địa chỉ 8
  Wire.write("Hello, I2C!"); // Gửi dữ liệu
  Wire.endTransmission(); // Kết thúc truyền dữ liệu

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

  uint8_t msg[] = "Hello, I2C!";
  while (1) {
    HAL_I2C_Master_Transmit(&hi2c1, (uint16_t)0x08<<1, msg, sizeof(msg)-1, HAL_MAX_DELAY);
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
