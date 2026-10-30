# Custom Callback: Sensor Drift Detector

## What it does
The three required callbacks only check whether a reading is inside its
allowed range. That misses a problem I wanted to catch: a sensor that jumps
a long way between two readings can still be "in range" both times. A jump
like that usually means something is wrong, such as loose wiring or a failing
sensor. The drift detector compares each reading with the one before it and
raises an alert when the change is too big.

## How it runs
The function is `sensor_drift_detector(struct Device *)`. `process_device()`
calls it through a function pointer, right after the device's own type
callback (temperature, pressure or battery). A device can therefore get two
alerts from the same reading, one from each callback.

It only works once a device has a previous processed reading. The first
reading of every device is skipped because there is nothing to compare it to.

## Algorithm
1. Read the current value from the union member that matches the device type.
2. Work out the drift: `drift = |current - previous|`.
3. Pick the limit for that type:
   - Temperature: 40 C
   - Pressure: 40 kPa
   - Battery voltage: 2.5 V
4. If the drift is bigger than the limit, set the device status to ALERT and
   log the alert with the reason.
5. Update the counters used in the summary: checks done, drift alerts raised,
   and the largest change seen (with the device it came from).

## Example
A temperature sensor reads 25.0 C and then 70.0 C. The drift is 45.0 C, which
is over the 40 C limit, so an alert is raised. The temperature monitor itself
would not complain here if 70.0 C is within its own limits, which is the case
this callback is for.

## About the limits
The readings are random, so big jumps happen a lot. I set the limits fairly
low so that drift alerts show up within a few rounds of the demo. A real
sensor would need much tighter limits.