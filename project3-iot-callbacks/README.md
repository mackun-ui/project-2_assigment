# Project 3: Dynamic IoT Event Monitoring and Callback Engine

## Build and run
```
gcc -Wall -Wextra -std=c99 -o iot src/*.c
./iot
```

## Files
- src/device.h: structs, union, enums, limits, prototypes
- src/main.c: header, menu, function-pointer dispatch table
- src/devices.c: dynamic device list (register, display, remove, grow/shrink)
- src/callbacks.c: process_device, temperature/pressure/battery monitors,
  Sensor Drift Detector, alert log
- src/simulation.c: rand() readings, processing, alerts, summary
- docs/custom_callback.md: explanation of the custom callback
- docs/demo_input.txt, docs/sample_output.txt: scripted demo run

## Evidence
Reproduce the sample output with: ./iot < docs/demo_input.txt
It shows 21 readings across the three device types, each handled by its own
callback; normal readings and alerts; drift alerts; duplicate-ID rejection;
registration (capacity 6 to 12) and removal (12 back to 6); all memory freed.