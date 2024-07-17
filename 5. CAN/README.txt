## CAN (Controller Area Network)

### Structure
- Nodes
- CAN bus (differential pair)

### Working Principle
- Differential signaling
- Message-based protocol
- Arbitration

### Data Frame
[Arbitration Field] [Control Field] [Data Field] [CRC Field] [ACK Field]
Notes
- Ensure proper termination resistors on CAN bus.
- Use appropriate bit timing settings for the network.

### Example Code

#### Arduino (using MCP2515)
```cpp
#include <SPI.h>
#include <mcp2515.h>

struct can_frame canMsg;
MCP2515 mcp2515(10);

void setup() {
  SPI.begin();
  mcp2515.reset();
  mcp2515.setBitrate(CAN_500KBPS);
  mcp2515.setNormalMode();
}

void loop() {
  canMsg.can_id  = 0x036;
  canMsg.can_dlc = 2;
  canMsg.data[0] = 0xFF;
  canMsg.data[1] = 0x00;
  mcp2515.sendMessage(&canMsg);
  delay(1000);
}
#### STM32
// Include necessary headers
#include "stm32f4xx_hal.h"

CAN_HandleTypeDef hcan1;

void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_CAN1_Init(void);

int main(void) {
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_CAN1_Init();

  CAN_TxHeaderTypeDef TxHeader;
  uint8_t TxData[8] = {0xFF, 0x00};
  uint32_t TxMailbox;

  TxHeader.StdId = 0x036;
  TxHeader.RTR = CAN_RTR_DATA;
  TxHeader.IDE = CAN_ID_STD;
  TxHeader.DLC = 2;
  TxHeader.TransmitGlobalTime = DISABLE;

  while (1) {
    if (HAL_CAN_AddTxMessage(&hcan1, &TxHeader, TxData, &TxMailbox) != HAL_OK) {
      // Transmission error
    }
    HAL_Delay(1000);
  }
}

void SystemClock_Config(void) { /* ... */ }
static void MX_GPIO_Init(void) { /* ... */ }
static void MX_CAN1_Init(void) {
  hcan1.Instance = CAN1;
  hcan1.Init.Prescaler = 16;
  hcan1.Init.Mode = CAN_MODE_NORMAL;
  hcan1.Init.SyncJumpWidth = CAN_SJW_1TQ;
  hcan1.Init.TimeSeg1 = CAN_BS1_1TQ;
  hcan1.Init.TimeSeg2 = CAN_BS2_1TQ;
  hcan1.Init.TimeTriggeredMode = DISABLE;
  hcan1.Init.AutoBusOff = DISABLE;
  hcan1.Init.AutoWakeUp = DISABLE;
  hcan1.Init.AutoRetransmission = DISABLE;
  hcan1.Init.ReceiveFifoLocked = DISABLE;
  hcan1.Init.TransmitFifoPriority = DISABLE;
  HAL_CAN_Init(&hcan1);
}
