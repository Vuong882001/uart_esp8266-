#include "Arduino.h"
#include "coredecls.h"
#include <pgmspace.h>
#include "gdb_hooks.h"
#include "esp8266_peri.h"
#include "user_interface.h"
#include "uart_register.h"

#define UART0   0

void uart_init() {
    // Cấu hình UART0 với tốc độ baud là 9600
    WRITE_PERI_REG(UART_CLKDIV(UART0), UART_CLK_FREQ / 9600);
    // Tắt chế độ chẳng lẻ và chẳng chẵn (parity) trong thanh ghi UART_CONF0
    CLEAR_PERI_REG_MASK(UART_CONF0(UART0), UART_PARITY_EN | UART_PARITY | UART_BIT_NUM | UART_STOP_BIT_NUM);
    // Reset thanh ghi RX và TX
    SET_PERI_REG_MASK(UART_CONF0(UART0), UART_RXFIFO_RST | UART_TXFIFO_RST); // set len 1 de xoa bo nho fifo
    CLEAR_PERI_REG_MASK(UART_CONF0(UART0), UART_RXFIFO_RST | UART_TXFIFO_RST);// tra ve 0 mac dinh
}
void uart_send_char(char c) {
    // Đợi đến khi bộ đệm TX UART trống
     char fifo_cnt  = (READ_PERI_REG(UART_STATUS(UART0)) >> UART_TXFIFO_CNT_S) & UART_TXFIFO_CNT;
    // Check bộ nhớ có vượt quá 126 bytes không
      if(fifo_cnt < 126)
      {
        WRITE_PERI_REG(UART_FIFO(UART0), c);
      }
}
char uart_receive() {
  // kiem tra dữ liệu có trong rx fifo không
  char data;
  int fifo_cnt = (READ_PERI_REG(UART_STATUS(UART0))>>UART_RXFIFO_CNT_S) & UART_RXFIFO_CNT;
  if ( fifo_cnt != 0 ) {
    // Đọc dữ liệu từ bộ đệm RX UART0
     data = READ_PERI_REG(UART_FIFO(UART0)) & 0xFF;
  }
  return data;
}
void setup() {
    pinMode(BUILTIN_LED,OUTPUT);
    digitalWrite(BUILTIN_LED,HIGH);
    Serial.begin(9600); // Cài đặt tốc độ baud cho Serial Monitor
    uart_init();
}
void loop() {
      char data = uart_receive();
      digitalWrite(BUILTIN_LED,HIGH);
      delay(1000);
      digitalWrite(BUILTIN_LED,LOW);
      delay(1000);
      if(data == 'A'){
        Serial.print(data);
        Serial.print("->OK");
      }else {Serial.print("SAI! ");}
      
}