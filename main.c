#include <avr/io.h>

#define BAUD 9600
#define UBRR_VAL ((F_CPU / (16UL * BAUD)) - 1)

void uart_init(void) {
    UBRR0H = (uint8_t)(UBRR_VAL >> 8);
    UBRR0L = (uint8_t)UBRR_VAL;

    UCSR0B = (1 << RXEN0) | (1 << TXEN0);   // Enable RX + TX
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); // 8N1
}

void uart_send(char c) {
    while (!(UCSR0A & (1 << UDRE0)));
    UDR0 = c;
}

char uart_recv(void) {
    while (!(UCSR0A & (1 << RXC0)));
    return UDR0;
}

void uart_send_string(const char *s) {
    while (*s) uart_send(*s++);
}

// int main(void) {
//     uart_init();

//     uart_send_string("READY\n");

//     char buf[32];
//     uint8_t idx = 0;

//     while (1) {
//         char c = uart_recv();

//         if (c == '\n' || idx >= sizeof(buf) - 1) {
//             buf[idx] = 0;
//             uart_send_string("ATmega: ");
//             uart_send_string(buf);
//             uart_send('\n');
//             idx = 0;
//         } else {
//             buf[idx++] = c;
//         }
//     }
// }

int main(void) {
    uart_init();

    while (1) {
        uart_send_string("HELLO FROM ATMEGA\r\n");
        for (volatile uint32_t i = 0; i < 800000UL; i++); // ~100ms @ 8MHz
    }
}