ATmega32 Room Entry/Exit Counter Project
Overview
This project is a room occupancy counter system built using the ATmega32 microcontroller. It tracks the number of people entering and exiting a room using two ultrasonic sensors (HC-SR04) to detect direction. The system displays real-time counts (total entries, current people inside, total exits) on a TFT display and plays audio messages ("Welcome" for entry, "Thank You" for exit) via a DFMini MP3 module.
A web-based simulator (built with HTML, CSS, and JavaScript) is included to demonstrate how the embedded system works without hardware. The simulator mimics sensor triggers, updates the display, and simulates audio playback.
This is ideal for educational purposes, IoT prototypes, or smart room management.
Features

Direction Detection: Uses two ultrasonic sensors to determine entry (Sensor 1 → Sensor 2) or exit (Sensor 2 → Sensor 1).
Counters: Tracks total entries, total exits, and current occupancy.
Display: TFT screen shows counts and status messages.
Audio Feedback: Plays welcome/thank you messages using DFMini MP3.
Reset Functionality: Button to reset all counters.
Simulator: Interactive web app to test the logic virtually.
Error Handling: Prevents negative occupancy and shows alerts.

Hardware Requirements

Microcontroller: ATmega32 (8-bit AVR).
Sensors: 2x HC-SR04 Ultrasonic Sensors (for entry/exit detection).

Trigger and Echo pins connected to PORTD (e.g., PD0/PD1 for Sensor 1, PD2/PD3 for Sensor 2).


Display: TFT LCD (e.g., ILI9341 driver, connected via SPI).
Audio Module: DFMini MP3 Player (connected via UART for serial commands).
Reset Button: Connected to an interrupt pin (e.g., INT0 on PD2).
Power Supply: 5V DC.
Other: Buzzer (optional for alerts), LEDs for sensor indicators.

Software Requirements

IDE: AVR Studio or Atmel Studio (for compiling C code).
Compiler: AVR-GCC.
Programmer: USBasp or similar for flashing code to ATmega32.
Libraries:

AVR libc (standard).
Custom libraries for TFT (e.g., Adafruit GFX) and UART for DFMini.


Simulator: Any modern web browser to run index.html.

Installation and Setup

Clone the Repository:
textgit clone https://github.com/yourusername/room-counter-simulator.git

Hardware Setup:

Wire ultrasonic sensors: Sensor 1 (Entry) - Trigger: PD0, Echo: PD1; Sensor 2 (Exit) - Trigger: PD2, Echo: PD3.
Connect TFT to SPI pins (e.g., MOSI: PB5, MISO: PB6, SCK: PB7).
DFMini MP3: TX/RX to UART pins (PD0/PD1, but adjust if conflicting).
Flash the ATmega32 code using AVR programmer.


Simulator Usage:

Open index.html in a browser.
Click "Person Entry" to simulate entry.
Click "Person Exit" to simulate exit.
Click "Reset System" to reset counters.
The display updates in real-time, with simulated audio status.



How It Works

Sensor Logic: Ultrasonic sensors measure distance. If distance < threshold (e.g., 50cm), a person is detected.

Entry: Sensor 1 triggers first, then Sensor 2.
Exit: Sensor 2 triggers first, then Sensor 1.


Display Update: TFT shows counters and status (e.g., "Person Entered").
Audio: Sends serial commands to DFMini to play MP3 files (file 001 for Welcome, 002 for Thank You).
Edge Cases: If no one is inside, exit is blocked. Max occupancy can be set if needed.

ATmega32 Code
Below is the C code for ATmega32. Save it as main.c and compile/flash it. This assumes basic libraries; you may need to include TFT and UART libraries (e.g., from AVR examples). Adjust pin definitions as per your wiring.
Notes

TFT and Audio Libraries: The code has stubs. Use libraries like Adafruit_ILI9341 for TFT (port to AVR) and ensure DFMini commands match your module.
Direction Detection: The code uses simple flags; enhance with timers for accurate sequence detection (e.g., using Timer1).
Testing: Test simulator first, then hardware. Adjust thresholds and delays.
Improvements: Add max occupancy limit, LCD instead of TFT, or IoT integration.
