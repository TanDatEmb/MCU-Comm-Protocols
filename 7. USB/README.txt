## USB (Universal Serial Bus)

### Cấu Trúc
- **Host (Thiết bị chủ)**: Máy tính hoặc thiết bị có khả năng điều khiển giao tiếp USB.
- **Device (Thiết bị)**: Các thiết bị ngoại vi kết nối với Host qua USB.
- **Hub (Bộ chia)**: Thiết bị mở rộng số lượng cổng USB.

### Nguyên Lý Hoạt Động
- **Giao tiếp nối tiếp (Serial Communication)**
- **Chế độ giao tiếp đa điểm (Multiple Communication Modes)**: USB hỗ trợ nhiều chế độ truyền thông như điều khiển (Control), gián đoạn (Interrupt), đồng bộ (Isochronous), và khối lượng lớn (Bulk).
- **Plug and Play**: Khả năng tự động nhận diện và cấu hình thiết bị khi kết nối.
- **Hot Swapping**: Khả năng kết nối và ngắt kết nối thiết bị mà không cần tắt nguồn.

### Cấu Trúc Khung Dữ Liệu USB
Một khung dữ liệu USB thông thường bao gồm:

- **Token Packet**
  - **Sync**: 8 bit đồng bộ.
  - **PID (Packet Identifier)**: Xác định loại gói tin.
  - **Address**: Địa chỉ thiết bị.
  - **Endpoint**: Điểm cuối (Endpoint) của thiết bị.
  - **CRC**: Kiểm tra lỗi.

- **Data Packet**
  - **Sync**: 8 bit đồng bộ.
  - **PID**: Xác định loại gói tin.
  - **Data**: Dữ liệu.
  - **CRC**: Kiểm tra lỗi.

- **Handshake Packet**
  - **Sync**: 8 bit đồng bộ.
  - **PID**: Xác định loại gói tin.

### Các Loại Gói Tin (Packet) trong USB
- **Token Packet**: Điều khiển luồng dữ liệu giữa Host và Device.
- **Data Packet**: Chứa dữ liệu truyền tải giữa Host và Device.
- **Handshake Packet**: Xác nhận tình trạng của quá trình truyền dữ liệu.
- **Start-of-Frame Packet**: Đồng bộ hóa các thiết bị USB theo khung thời gian.

### Chế Độ Giao Tiếp
1. **Chế độ điều khiển (Control Transfer)**
   - Được sử dụng để cấu hình thiết bị khi kết nối lần đầu.
   - Bao gồm các giai đoạn: Setup, Data, và Status.

2. **Chế độ gián đoạn (Interrupt Transfer)**
   - Được sử dụng cho các thiết bị cần truyền dữ liệu không thường xuyên.
   - Ví dụ: Chuột, bàn phím.

3. **Chế độ đồng bộ (Isochronous Transfer)**
   - Được sử dụng cho các thiết bị yêu cầu băng thông cố định và độ trễ thấp.
   - Ví dụ: Âm thanh, video.

4. **Chế độ khối lượng lớn (Bulk Transfer)**
   - Được sử dụng cho các thiết bị truyền dữ liệu lớn nhưng không yêu cầu thời gian thực.
   - Ví dụ: Ổ cứng, máy in.

### Thanh Ghi USB Trong STM32
1. **USB Control Register (USB_CNTR)**
   - **USB_CNTR_FRES (Force Reset)**: Khởi động lại USB.
   - **USB_CNTR_PDWN (Power Down)**: Tiết kiệm năng lượng.
   - **USB_CNTR_LPMODE (Low-Power Mode)**: Chế độ tiết kiệm năng lượng thấp.
   - **USB_CNTR_FSUSP (Force Suspend)**: Tạm dừng hoạt động USB.
   - **USB_CNTR_RESUME (Resume Request)**: Yêu cầu tiếp tục hoạt động.

2. **USB Interrupt Status Register (USB_ISTR)**
   - **USB_ISTR_CTR (Correct Transfer)**: Truyền dữ liệu đúng.
   - **USB_ISTR_RESET (USB Reset)**: USB được khởi động lại.
   - **USB_ISTR_SUSP (Suspend Mode Request)**: Yêu cầu tạm dừng.

3. **USB Frame Number Register (USB_FNR)**
   - **USB_FNR_FN (Frame Number)**: Số khung hiện tại.
   - **USB_FNR_LSOF (Lost SOF)**: Mất đồng bộ khung.

4. **USB Device Address Register (USB_DADDR)**
   - **USB_DADDR_ADD (Device Address)**: Địa chỉ thiết bị.
   - **USB_DADDR_EF (Enable Function)**: Kích hoạt chức năng.

### Ví Dụ Code

#### Arduino với USB Host Shield

#include <Usb.h>
#include <usbhub.h>

USB Usb;
USBHub Hub(&Usb);

void setup() {
  Serial.begin(115200);

  if (Usb.Init() == -1) {
    Serial.println("USB Host Shield không khởi tạo được.");
    while (1); // Dừng chương trình nếu không khởi tạo được
  }
  Serial.println("USB Host Shield khởi tạo thành công.");
}

void loop() {
  Usb.Task();
}


#### STM32

// Include necessary headers
#include "usb_device.h"
#include "usbd_cdc_if.h"

void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USB_DEVICE_Init(void);

int main(void) {
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_USB_DEVICE_Init();

  while (1) {
    CDC_Transmit_FS((uint8_t *)"Hello USB", strlen("Hello USB"));
    HAL_Delay(1000);
  }
}

void SystemClock_Config(void) { /* ... */ }
static void MX_GPIO_Init(void) { /* ... */ }
static void MX_USB_DEVICE_Init(void) {
  /* Init USB device Library, add supported class and start the library. */
  MX_USB_DEVICE_Init();
}
