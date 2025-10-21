#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <string.h>  // For string handling

// Pin Definitions
#define TRIGGER1 PD0  // Sensor 1 Trigger (Entry)
#define ECHO1    PD1  // Sensor 1 Echo
#define TRIGGER2 PD2  // Sensor 2 Trigger (Exit)
#define ECHO2    PD3  // Sensor 2 Echo
#define RESET_PIN PD4 // Reset Button

// UART for DFMini MP3 (Baud rate 9600)
#define BAUD 9600
#define MYUBRR F_CPU/16/BAUD-1

// TFT Display Functions (Stub - Replace with actual library)
void tft_init();
void tft_display(char* message, uint16_t x, uint16_t y);
void tft_update_counters(uint16_t entries, uint16_t current, uint16_t exits);

// Ultrasonic Distance Measurement
uint16_t get_distance(uint8_t trigger, uint8_t echo) {
    PORTD |= (1 << trigger);  // Trigger high
    _delay_us(10);
    PORTD &= ~(1 << trigger); // Trigger low
    
    uint16_t duration = 0;
    while (!(PIND & (1 << echo)));  // Wait for echo high
    while (PIND & (1 << echo)) {    // Measure duration
        duration++;
        _delay_us(1);
    }
    return (duration / 58);  // Distance in cm (speed of sound adjustment)
}

// UART Functions for DFMini
void uart_init(unsigned int ubrr) {
    UBRRH = (unsigned char)(ubrr >> 8);
    UBRRL = (unsigned char)ubrr;
    UCSRB = (1 << RXEN) | (1 << TXEN);
    UCSRC = (1 << URSEL) | (1 << UCSZ1) | (1 << UCSZ0);
}

void uart_transmit(unsigned char data) {
    while (!(UCSRA & (1 << UDRE)));
    UDR = data;
}

void play_audio(uint8_t file) {
    // DFMini command example: 7E FF 06 03 00 00 01 EF for file 001
    unsigned char cmd[] = {0x7E, 0xFF, 0x06, 0x03, 0x00, 0x00, file, 0xEF};
    for (int i = 0; i < 8; i++) {
        uart_transmit(cmd[i]);
    }
}

// Global Variables
volatile uint16_t entryCount = 0;
volatile uint16_t exitCount = 0;
volatile uint16_t currentPeople = 0;

// Interrupt for Reset
ISR(INT0_vect) {
    entryCount = 0;
    exitCount = 0;
    currentPeople = 0;
    tft_display("System Reset", 10, 10);
    _delay_ms(2000);
    tft_display("System Ready", 10, 10);
}

int main(void) {
    // Initialize Ports
    DDRD = (1 << TRIGGER1) | (1 << TRIGGER2);  // Outputs for triggers
    PORTD = 0;  // Low initially
    
    // Enable Interrupt for Reset
    GICR |= (1 << INT0);  // Enable INT0
    MCUCR |= (1 << ISC01); // Falling edge
    sei();  // Global interrupts
    
    // Init Peripherals
    uart_init(MYUBRR);
    tft_init();
    tft_display("System Ready", 10, 10);
    
    uint8_t sensor1_triggered = 0;
    uint8_t sensor2_triggered = 0;
    
    while (1) {
        uint16_t dist1 = get_distance(TRIGGER1, ECHO1);
        uint16_t dist2 = get_distance(TRIGGER2, ECHO2);
        
        const uint16_t THRESHOLD = 50;  // cm
        
        if (dist1 < THRESHOLD && !sensor1_triggered) {
            sensor1_triggered = 1;
        }
        if (dist2 < THRESHOLD && !sensor2_triggered) {
            sensor2_triggered = 1;
        }
        
        // Detect Entry: Sensor1 first, then Sensor2
        if (sensor1_triggered && sensor2_triggered && sensor1_triggered_first()) {  // Custom function to check order
            entryCount++;
            currentPeople++;
            tft_display("Person Entered", 10, 50);
            play_audio(1);  // Welcome (file 001)
            _delay_ms(2000);
            tft_display("System Ready", 10, 50);
            reset_triggers(&sensor1_triggered, &sensor2_triggered);
        }
        
        // Detect Exit: Sensor2 first, then Sensor1
        else if (sensor2_triggered && sensor1_triggered && sensor2_triggered_first()) {
            if (currentPeople > 0) {
                exitCount++;
                currentPeople--;
                tft_display("Person Exited", 10, 50);
                play_audio(2);  // Thank You (file 002)
                _delay_ms(2000);
                tft_display("System Ready", 10, 50);
            } else {
                tft_display("No one inside!", 10, 50);
                _delay_ms(2000);
                tft_display("System Ready", 10, 50);
            }
            reset_triggers(&sensor1_triggered, &sensor2_triggered);
        }
        
        // Update Display
        tft_update_counters(entryCount, currentPeople, exitCount);
        
        _delay_ms(100);  // Polling delay
    }
}

// Stub Functions - Implement based on your TFT library
void tft_init() {
    // Initialize SPI and TFT
}

void tft_display(char* message, uint16_t x, uint16_t y) {
    // Draw text on TFT
}

void tft_update_counters(uint16_t entries, uint16_t current, uint16_t exits) {
    // Update counter values on screen
}

// Helper Functions
void reset_triggers(uint8_t* s1, uint8_t* s2) {
    *s1 = 0;
    *s2 = 0;
}

// Assume you implement timing to check which triggered first
uint8_t sensor1_triggered_first() {
    return 1;  // Placeholder - Use timers
}

uint8_t sensor2_triggered_first() {
    return 1;  // Placeholder - Use timers
}
