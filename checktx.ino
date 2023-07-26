#include "Arduino.h"
#include "coredecls.h"
#include <pgmspace.h>
#include "gdb_hooks.h"
#include "esp8266_peri.h"
#include "user_interface.h"
#include "uart_register.h"

#define UART0   0
#define UART_RXFIFO_FULL_THRHD_VALUE 1 // Thay đổi ngưỡng thành 1 byte

void uart_init()
{
  // setup tốc độ baud
  WRITE_PERI_REG(UART_CLKDIV(UART0), UART_CLK_FREQ / 9600);
  //Setup bit chẵn lẻ/stop bit
  CLEAR_PERI_REG_MASK(UART_CONF0(UART0), UART_PARITY_EN | UART_PARITY | UART_BIT_NUM | UART_STOP_BIT_NUM);
  // Reset thanh ghi RX và TX
  SET_PERI_REG_MASK(UART_CONF0(UART0), UART_RXFIFO_RST | UART_TXFIFO_RST); // set len 1 de xoa bo nho fifo
  CLEAR_PERI_REG_MASK(UART_CONF0(UART0), UART_RXFIFO_RST | UART_TXFIFO_RST);// tra ve 0 mac dinh
  //Setup ngắt RX tràn
  SET_PERI_REG_MASK(UART_INT_ENA(UART0),UART_RXFIFO_FULL_INT_ENA);
  // Cài đặt ngưỡng ngắt của rx fifo
  WRITE_PERI_REG(UART_CONF1(UART0),(UART_RXFIFO_FULL_THRHD_VALUE & UART_RXFIFO_FULL_THRHD));
}

void uart_receive() {
  uint8_t uart_intr_status = READ_PERI_REG(UART_INT_ST(UART0));
  // kiem tra dữ liệu có trong rx fifo không
  if((UART_RXFIFO_FULL_INT_ST) == (uart_intr_status & UART_RXFIFO_FULL_INT_ST))
  {
  char data;
  int fifo_cnt = (READ_PERI_REG(UART_STATUS(UART0))>>UART_RXFIFO_CNT_S) & UART_RXFIFO_CNT;
  if ( fifo_cnt != 0 ) {
    // Đọc dữ liệu từ bộ đệm RX UART0
     data = READ_PERI_REG(UART_FIFO(UART0)) & 0xFF;
      }
      WRITE_PERI_REG(UART_INT_CLR(UART0),UART_RXFIFO_FULL_INT_CLR);
      Serial.print("Interrupt-");
      Serial.print(data);
      Serial.print(" ");
  }
}

void setup()
{
    Serial.begin(9600); // Khởi tạo Serial Monitor để hiển thị dữ liệu nhận được
    uart_init();
    ETS_UART_INTR_ENABLE(); // cho phép ngắt uart
    ETS_UART_INTR_ATTACH(uart_receive, NULL);
}

void loop()
{
 Serial.print("No-Interrup! ");
 delay(3000);
}



