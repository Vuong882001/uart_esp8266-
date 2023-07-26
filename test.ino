#include "Arduino.h"
#include "coredecls.h"
#include <pgmspace.h>
#include "gdb_hooks.h"
#include "esp8266_peri.h"
#include "user_interface.h"
#include "uart_register.h"

#define UART0   0
uint8_t i = 21;
void uart_init() {
    // Cấu hình UART0 với tốc độ baud là 115200
    WRITE_PERI_REG(UART_CLKDIV(UART0), UART_CLK_FREQ / 9600);
    // Tắt chế độ chẳng lẻ và chẳng chẵn (parity) trong thanh ghi UART_CONF0
    CLEAR_PERI_REG_MASK(UART_CONF0(UART0), UART_PARITY_EN | UART_PARITY | UART_BIT_NUM | UART_STOP_BIT_NUM);
    // Reset thanh ghi RX và TX
    SET_PERI_REG_MASK(UART_CONF0(UART0), UART_RXFIFO_RST | UART_TXFIFO_RST); // set len 1 de xoa bo nho fifo
    CLEAR_PERI_REG_MASK(UART_CONF0(UART0), UART_RXFIFO_RST | UART_TXFIFO_RST);// tra ve 0 mac dinh
}
void uart_send_char(uint8_t c) {
    WRITE_PERI_REG(UART_FIFO(UART0), c);
}
void setup() {
    Serial.begin(9600); // Cài đặt tốc độ baud cho Serial Monitor
    uart_init();
}
void loop() {
    delay(10000);
    while(i < 148)
    {
      uart_send_char(i);
      i++;
    }
}