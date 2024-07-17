## UART (Universal Asynchronous Receiver-Transmitter)

### Cấu Trúc
- **Transmitter (Bộ phát)**
- **Receiver (Bộ thu)**

### Nguyên Lý Hoạt Động
- **Giao tiếp không đồng bộ (Asynchronous communication)**
- **Tốc độ truyền dữ liệu (Baud rate)**
- **Đóng gói dữ liệu (Data framing)**: bit bắt đầu (start bit), các bit dữ liệu (data bits), bit kiểm tra chẵn lẻ (parity bit), bit dừng (stop bit)

### Khung Dữ Liệu (Data Frame)

[Start Bit] [Data Bits] [Parity Bit] [Stop Bit]

- **Lưu ý**
  - Đảm bảo tốc độ truyền dữ liệu (baud rate) giữa bộ phát và bộ thu phải khớp nhau.
  - Sử dụng bộ chuyển mức điện áp phù hợp để đảm bảo tương thích điện áp.

### Cấu Trúc Của Một Frame UART
Một khung dữ liệu UART thông thường bao gồm:

- **Start Bit**: 1 bit bắt đầu, luôn là 0, báo hiệu bắt đầu của một khung dữ liệu.
- **Data Bits**: 5-9 bits dữ liệu, tùy thuộc vào cấu hình (thường là 8 bits).
- **Parity Bit (tùy chọn)**: 1 bit kiểm tra chẵn/lẻ để phát hiện lỗi.
- **Stop Bit(s)**: 1 hoặc 2 bits dừng, báo hiệu kết thúc của một khung dữ liệu.

### Hàm UART_Transmit
- Gửi từng byte một từ mảng `tx_data`.
- Kiểm tra bit **TXE** trong thanh ghi trạng thái (SR) để biết bộ phát có rỗng hay không. Nếu bộ phát rỗng, byte tiếp theo sẽ được gửi.
- Ghi byte dữ liệu vào thanh ghi dữ liệu (DR) để bắt đầu truyền.
- Lặp lại cho đến khi tất cả các byte dữ liệu đã được gửi.
- Cuối cùng, kiểm tra bit **TC** để đảm bảo rằng tất cả dữ liệu đã được truyền xong.

### Thông Số Cấu Hình
- **Số bit dữ liệu**: Thường dùng 8 bit, nhưng có thể là 7 hoặc 9 bit tùy vào yêu cầu cụ thể của thiết bị hoặc giao thức.
- **Parity bit**: Dùng để phát hiện lỗi, có thể là parity chẵn, parity lẻ hoặc không dùng.
- **Số bit dừng**: Thường dùng 1 bit, nhưng có thể là 1.5 hoặc 2 bit để tăng độ ổn định trong một số ứng dụng đặc biệt.

### Thanh Ghi UART Trong STM32
1. **USART Status Register (USART_SR)**
    - **USART_SR_PE (Parity Error)**: Bit này được đặt nếu có lỗi parity trong quá trình nhận dữ liệu.
    - **USART_SR_FE (Framing Error)**: Bit này được đặt nếu không có stop bit hoặc stop bit không đúng vị trí.
    - **USART_SR_NE (Noise Error)**: Bit này được đặt nếu có nhiễu trong quá trình nhận dữ liệu.
    - **USART_SR_ORE (Overrun Error)**: Bit này được đặt nếu bộ đệm nhận đầy trước khi dữ liệu cũ được đọc ra.
    - **USART_SR_RXNE (Read Data Register Not Empty)**: Bit này được đặt khi có dữ liệu mới trong thanh ghi dữ liệu nhận (USART_DR).
    - **USART_SR_TC (Transmission Complete)**: Bit này được đặt khi quá trình truyền dữ liệu hoàn tất.
    - **USART_SR_TXE (Transmit Data Register Empty)**: Bit này được đặt khi bộ đệm truyền rỗng và sẵn sàng nhận dữ liệu mới để truyền.

2. **USART Data Register (USART_DR)**
    - Thanh ghi này được sử dụng để đọc dữ liệu nhận hoặc ghi dữ liệu truyền.
    - Dữ liệu nhận hoặc truyền sẽ bao gồm cả bit dữ liệu và bit parity (nếu có).

3. **USART Baud Rate Register (USART_BRR)**
    - Thanh ghi này được sử dụng để thiết lập tốc độ truyền dữ liệu (baud rate).

4. **USART Control Register 1 (USART_CR1)**
    - **USART_CR1_M (Word Length)**: Thiết lập độ dài từ (8 hoặc 9 bit).
    - **USART_CR1_PCE (Parity Control Enable)**: Bật/tắt kiểm tra parity.
    - **USART_CR1_PS (Parity Selection)**: Chọn loại parity (0: chẵn, 1: lẻ).
    - **USART_CR1_TE (Transmitter Enable)**: Bật bộ phát.
    - **USART_CR1_RE (Receiver Enable)**: Bật bộ thu.
    - **USART_CR1_UE (USART Enable)**: Bật UART.

5. **USART Control Register 2 (USART_CR2)**
    - **USART_CR2_STOP (STOP bits)**: Thiết lập số lượng bit dừng (1, 0.5, 2, 1.5).

### UART Đồng Bộ (Synchronous UART)
- Là một phương thức truyền thông nối tiếp, trong đó dữ liệu được truyền với một tín hiệu xung nhịp (clock signal) đồng bộ hóa giữa thiết bị gửi và thiết bị nhận.
- Khác với UART không đồng bộ (Asynchronous UART), nơi không có tín hiệu xung nhịp chung và các thiết bị phải đồng bộ hóa dựa trên các bit bắt đầu và bit dừng.

### Example Code

#### Arduino

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
