Mbed OS SPI Master / Slave
===

# Prerequisites

1. [SAM L21 Xplained Pro Evaluation Kit](https://www.microchip.com/developmenttools/ProductDetails/atsaml21-xpro-b)
2. [xDot Development Kit](https://www.multitech.com/brands/micro-xdot-devkit)

Or any other two Mbed OS compatible devices.

# Firmware

Use [PlaftormIO](https://platformio.org/) to build and upload via `cmsis-dap` protocol.

1. Go to `master` and `slave` folders (separate tabs).
1. Run `platformio run --target upload` in both tabs.
1. Run `cp .pioenvs/xdot_l151cc/firmware.bin /Volumes/XDOT` (only for slave, manually copy, since xDot DK does not support `cmsis-dap` for some reason).
1. Run `platformio device monitor`.
1. Push Reset button on slave, then on master.