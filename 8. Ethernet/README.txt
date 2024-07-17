## Ethernet

### Structure
- Nodes
- Switches
- Routers

### Working Principle
- CSMA/CD (Carrier Sense Multiple Access with Collision Detection)
- Frame-based communication

### Data Frame
[Preamble] [Destination MAC] [Source MAC] [Type] [Data] [CRC]
Notes
- Ensure proper network configuration (IP, MAC addresses).
- Handle collisions and retransmissions appropriately.

### Example Code

#### Arduino (using Ethernet shield)
```cpp
#include <SPI.h>
#include <Ethernet.h>

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192, 168, 1, 177);

void setup() {
  Ethernet.begin(mac, ip);
}

void loop() {
  EthernetClient client = server.available();
  if (client) {
    client.println("Hello, Ethernet!");
    delay(1000);
  }
}
#### STM32
// Include necessary headers
#include "stm32f4xx_hal.h"
#include "lwip.h"

void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_LWIP_Init(void);

int main(void) {
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_LWIP_Init();

  while (1) {
    // Network tasks handled in interrupt context
  }
}

void SystemClock_Config(void) { /* ... */ }
static void MX_GPIO_Init(void) { /* ... */ }
static void MX_LWIP_Init(void) {
  // Initialization code for LwIP stack
  MX_LWIP_Init();
}
