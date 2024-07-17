## CAN (Controller Area Network)

### Cấu Trúc
- **Nodes (Nút mạng)**: Các thiết bị kết nối trên cùng một bus CAN.

### Nguyên Lý Hoạt Động
- **Giao tiếp nối tiếp (Serial Communication)**
- **Broadcast Communication**: Dữ liệu được gửi đến tất cả các thiết bị trên bus.
- **Arbitration**: Điều khiển ưu tiên truyền dữ liệu.
- **Error Detection**: Phát hiện lỗi trong quá trình truyền dữ liệu.

### Khung Dữ Liệu (Data Frame)

[Start of Frame] [Identifier] [Control Field] [Data Field] [CRC Field] [ACK Field] [End of Frame]

- **Lưu ý**
  - Tất cả các thiết bị trên bus có thể nhận tất cả các khung dữ liệu nhưng chỉ xử lý những khung có định danh phù hợp.
  - CAN sử dụng phương pháp Non-Destructive Bitwise Arbitration để quyết định thiết bị nào được quyền truyền dữ liệu trước.

### Cấu Trúc Của Một Frame CAN
Một khung dữ liệu CAN thông thường bao gồm:

- **Start of Frame**: Bit bắt đầu, báo hiệu bắt đầu của quá trình truyền dữ liệu.
- **Identifier**: Định danh của khung dữ liệu, quyết định mức độ ưu tiên trong quá trình arbitration.
- **Control Field**: Thông tin điều khiển, bao gồm DLC (Data Length Code).
- **Data Field**: Dữ liệu cần truyền, có độ dài từ 0 đến 8 byte.
- **CRC Field**: Kiểm tra lỗi, gồm 15 bit CRC và 1 bit CRC delimiter.
- **ACK Field**: Bit xác nhận, gồm 1 bit ACK và 1 bit ACK delimiter.
- **End of Frame**: Bit kết thúc, báo hiệu kết thúc của quá trình truyền dữ liệu.

### Thanh Ghi CAN Trong STM32
1. **CAN Control Register (CAN_MCR)**
    - **CAN_MCR_INRQ (Initialization Request)**: Yêu cầu khởi tạo.
    - **CAN_MCR_SLEEP (Sleep Mode Request)**: Yêu cầu chế độ ngủ.
    - **CAN_MCR_TXFP (Transmit FIFO Priority)**: Thiết lập ưu tiên FIFO truyền.

2. **CAN Status Register (CAN_MSR)**
    - **CAN_MSR_INAK (Initialization Acknowledge)**: Xác nhận khởi tạo.
    - **CAN_MSR_SLAK (Sleep Acknowledge)**: Xác nhận chế độ ngủ.

3. **CAN Transmit Status Register (CAN_TSR)**
    - **CAN_TSR_RQCP0/1/2 (Request Completed Mailbox 0/1/2)**: Hoàn thành yêu cầu hộp thư 0/1/2.
    - **CAN_TSR_TXOK0/1/2 (Transmission OK of Mailbox 0/1/2)**: Truyền thành công hộp thư 0/1/2.
    - **CAN_TSR_TME0/1/2 (Transmit Mailbox Empty 0/1/2)**: Hộp thư truyền 0/1/2 rỗng.

4. **CAN Receive FIFO 0/1 Register (CAN_RF0R/CAN_RF1R)**
    - **CAN_RF0R/CAN_RF1R_FMP (FIFO 0/1 Message Pending)**: Số lượng tin nhắn chờ trong FIFO 0/1.
    - **CAN_RF0R/CAN_RF1R_FULL (FIFO 0/1 Full)**: FIFO 0/1 đầy.
    - **CAN_RF0R/CAN_RF1R_FOVR (FIFO 0/1 Overrun)**: Tràn FIFO 0/1.

5. **CAN Filter Master Register (CAN_FMR)**
    - **CAN_FMR_FINIT (Filter Initialization Mode)**: Chế độ khởi tạo bộ lọc.

### Chế Độ Hoạt Động
1. **Chế độ bình thường (Normal Mode)**
    - Thiết bị hoạt động bình thường, truyền và nhận dữ liệu trên bus.

2. **Chế độ khởi tạo (Initialization Mode)**
    - Thiết bị vào chế độ khởi tạo để cấu hình các tham số trước khi hoạt động.

3. **Chế độ ngủ (Sleep Mode)**
    - Thiết bị vào chế độ ngủ để tiết kiệm năng lượng khi không cần truyền/nhận dữ liệu.

### Ví Dụ Code

#### Arduino với MCP2515 CAN Module

#include <SPI.h>
#include <mcp_can.h>

const int SPI_CS_PIN = 10;
MCP_CAN CAN(SPI_CS_PIN);

void setup() {
  Serial.begin(115200);
  while (CAN_OK != CAN.begin(CAN_500KBPS)) {
    Serial.println("CAN init fail, retrying...");
    delay(100);
  }
  Serial.println("CAN init ok!");
}

void loop() {
  byte data[8] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07};
  CAN.sendMsgBuf(0x100, 0, 8, data);
  delay(1000);
}


#### STM32

// Include necessary headers
#include "stm32f4xx_hal.h"

CAN_HandleTypeDef hcan;

void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_CAN_Init(void);

int main(void) {
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_CAN_Init();

  CAN_TxHeaderTypeDef TxHeader;
  uint8_t TxData[8] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07};
  uint32_t TxMailbox;

  TxHeader.DLC = 8;
  TxHeader.IDE = CAN_ID_STD;
  TxHeader.RTR = CAN_RTR_DATA;
  TxHeader.StdId = 0x100;

  while (1) {
    if (HAL_CAN_AddTxMessage(&hcan, &TxHeader, TxData, &TxMailbox) != HAL_OK) {
      Error_Handler();
    }
    HAL_Delay(1000);
  }
}

void SystemClock_Config(void) { /* ... */ }
static void MX_GPIO_Init(void) { /* ... */ }
static void MX_CAN_Init(void) {
  hcan.Instance = CAN1;
  hcan.Init.Prescaler = 16;
  hcan.Init.Mode = CAN_MODE_NORMAL;
  hcan.Init.SyncJumpWidth = CAN_SJW_1TQ;
  hcan.Init.TimeSeg1 = CAN_BS1_1TQ;
  hcan.Init.TimeSeg2 = CAN_BS2_1TQ;
  hcan.Init.TimeTriggeredMode = DISABLE;
  hcan.Init.AutoBusOff = DISABLE;
  hcan.Init.AutoWakeUp = DISABLE;
  hcan.Init.AutoRetransmission = ENABLE;
  hcan.Init.ReceiveFifoLocked = DISABLE;
  hcan.Init.TransmitFifoPriority = DISABLE;
  if (HAL_CAN_Init(&hcan) != HAL_OK) {
    Error_Handler();
  }
}
