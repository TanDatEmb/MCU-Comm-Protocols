## USB (Universal Serial Bus)

### Structure
- Host
- Device
- Hubs

### Working Principle
- Tiered-star topology
- Polling-based communication
- Endpoints

### Data Frame
[Sync] [Packet ID] [Address] [Data] [CRC] [End of Packet]
Notes
- Ensure proper USB descriptors are configured.
- Handle USB power management correctly.

### Example Code

#### Arduino (using USB host shield)
```cpp
#include <USB.h>

USB usb;

void setup() {
  if (usb.Init() == -1) {
    while (1); // Initialization error
  }
}

void loop() {
  usb.Task();
}
#### STM32
// Include necessary headers
#include "stm32f4xx_hal.h"
#include "usbd_core.h"

USBD_HandleTypeDef hUsbDeviceFS;

void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USB_DEVICE_Init(void);

int main(void) {
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_USB_DEVICE_Init();

  while (1) {
    // USB tasks handled in interrupt context
  }
}

void SystemClock_Config(void) { /* ... */ }
static void MX_GPIO_Init(void) { /* ... */ }
static void MX_USB_DEVICE_Init(void) {
  USBD_Init(&hUsbDeviceFS, &FS_Desc, DEVICE_FS);
  USBD_RegisterClass(&hUsbDeviceFS, &USBD_CDC);
  USBD_CDC_RegisterInterface(&hUsbDeviceFS, &USBD_Interface_fops_FS);
  USBD_Start(&hUsbDeviceFS);
}
