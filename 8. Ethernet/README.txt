## Ethernet

### Cấu Trúc
- **MAC (Media Access Control)**: Kiểm soát truy cập vào phương tiện truyền thông.
- **PHY (Physical Layer)**: Xử lý tín hiệu vật lý.

### Nguyên Lý Hoạt Động
- **Giao tiếp nối tiếp (Serial Communication)**
- **Giao tiếp theo gói tin (Packet Communication)**: Dữ liệu được chia thành các gói tin và truyền qua mạng.
- **CSMA/CD (Carrier Sense Multiple Access/Collision Detection)**: Phát hiện và xử lý va chạm trên mạng.

### Cấu Trúc Khung Dữ Liệu Ethernet
Một khung dữ liệu Ethernet thông thường bao gồm:

- **Preamble**: 7 byte dùng để đồng bộ hóa.
- **Start Frame Delimiter (SFD)**: 1 byte báo hiệu bắt đầu khung.
- **Destination MAC Address**: 6 byte địa chỉ MAC của thiết bị nhận.
- **Source MAC Address**: 6 byte địa chỉ MAC của thiết bị gửi.
- **Ethertype/Length**: 2 byte xác định loại giao thức hoặc độ dài khung.
- **Payload**: Dữ liệu, có độ dài từ 46 đến 1500 byte.
- **Frame Check Sequence (FCS)**: 4 byte kiểm tra lỗi.

### Cấu Trúc Của Một Frame Ethernet
Một khung dữ liệu Ethernet thông thường bao gồm:


[Preamble][SFD][Destination MAC][Source MAC][Ethertype/Length][Payload][FCS]


### Các Loại Khung Ethernet
1. **Ethernet II**: Sử dụng Ethertype để xác định giao thức lớp trên.
2. **IEEE 802.3**: Sử dụng Length để xác định độ dài của payload.

### Chế Độ Hoạt Động
1. **Half-Duplex**: Chỉ truyền hoặc nhận dữ liệu tại một thời điểm.
2. **Full-Duplex**: Có thể truyền và nhận dữ liệu đồng thời.

### Thanh Ghi Ethernet Trong STM32
1. **Ethernet MAC Configuration Register (MACCR)**
    - **MACCR_RE (Receiver Enable)**: Bật bộ nhận.
    - **MACCR_TE (Transmitter Enable)**: Bật bộ truyền.
    - **MACCR_DM (Duplex Mode)**: Chọn chế độ half-duplex hoặc full-duplex.
    - **MACCR_FES (Fast Ethernet Speed)**: Chọn tốc độ Ethernet 10 Mbps hoặc 100 Mbps.

2. **Ethernet MAC Frame Filter Register (MACFFR)**
    - **MACFFR_PM (Promiscuous Mode)**: Chế độ thu thập tất cả các khung dữ liệu.
    - **MACFFR_HPF (Hash or Perfect Filter)**: Chọn chế độ lọc.

3. **Ethernet MAC Address High/Low Register (MACA0HR, MACA0LR)**
    - **MACA0HR (MAC Address High Register)**: Địa chỉ MAC cao.
    - **MACA0LR (MAC Address Low Register)**: Địa chỉ MAC thấp.

4. **Ethernet DMA Operation Mode Register (DMAOMR)**
    - **DMAOMR_ST (Start/Stop Transmission Command)**: Bắt đầu hoặc dừng truyền.
    - **DMAOMR_SR (Start/Stop Receive Command)**: Bắt đầu hoặc dừng nhận.

5. **Ethernet DMA Status Register (DMASR)**
    - **DMASR_TS (Transmit Status)**: Trạng thái truyền.
    - **DMASR_RS (Receive Status)**: Trạng thái nhận.

### Ví Dụ Code

#### Arduino với ENC28J60 Ethernet Module

#include <UIPEthernet.h>

// Địa chỉ MAC của thiết bị
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

void setup() {
  Ethernet.begin(mac);
  Serial.begin(9600);

  // Kiểm tra kết nối Ethernet
  if (Ethernet.hardwareStatus() == EthernetNoHardware) {
    Serial.println("ENC28J60 không được tìm thấy.");
    while (true) {
      delay(1);
    }
  }
  Serial.println("ENC28J60 được tìm thấy.");
}

void loop() {
  // Kiểm tra trạng thái kết nối
  EthernetClient client = server.available();
  if (client) {
    Serial.println("Client kết nối.");
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
      }
    }
    client.stop();
    Serial.println("Client ngắt kết nối.");
  }
}


#### STM32 với LAN8720 Ethernet Module

// Include necessary headers
#include "stm32f4xx_hal.h"
#include "ethernetif.h"
#include "lwip.h"

void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_ETH_Init(void);

int main(void) {
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_ETH_Init();
  MX_LWIP_Init();

  while (1) {
    MX_LWIP_Process();
  }
}

void SystemClock_Config(void) { /* ... */ }
static void MX_GPIO_Init(void) { /* ... */ }
static void MX_ETH_Init(void) {
  /* Configure Ethernet peripheral */
  EthHandle.Instance = ETH;
  EthHandle.Init.AutoNegotiation = ETH_AUTONEGOTIATION_ENABLE;
  EthHandle.Init.PhyAddress = LAN8720_PHY_ADDRESS;
  EthHandle.Init.MACAddr = (uint8_t*)macaddress;
  EthHandle.Init.RxMode = ETH_RXINTERRUPT_MODE;
  EthHandle.Init.ChecksumMode = ETH_CHECKSUM_BY_HARDWARE;
  EthHandle.Init.MediaInterface = ETH_MEDIA_INTERFACE_RMII;

  if (HAL_ETH_Init(&EthHandle) != HAL_OK) {
    /* Initialization Error */
    Error_Handler();
  }

  /* Initialize Tx Descriptors list: Chain Mode */
  HAL_ETH_DMATxDescListInit(&EthHandle, DMATxDscrTab, Tx_Buff, ETH_TXBUFNB);

  /* Initialize Rx Descriptors list: Chain Mode */
  HAL_ETH_DMARxDescListInit(&EthHandle, DMARxDscrTab, Rx_Buff, ETH_RXBUFNB);
}

static void MX_LWIP_Init(void) {
  /* Initialize the LwIP stack */
  lwip_init();
  /* Start the network interface */
  netif_set_up(&gnetif);
  /* Start DHCP client */
  dhcp_start(&gnetif);
}
