# STM32F446RE Bare-Metal Drivers

Bare-Metal Embedded C drivers and practical examples for the **STM32F446RE** microcontroller based on the ARM Cortex-M4 core.

This repository documents my hands-on work with STM32 peripherals, register-level programming, and low-level driver development. It includes custom GPIO and SPI driver implementations, along with standalone examples that demonstrate peripheral configuration and interaction with external hardware.

The project is developed as part of my Embedded Systems learning path, with an emphasis on understanding how MCU hardware, peripheral registers, and Embedded C firmware work together.

## Project Goals

- Understand STM32 peripheral operation at the register level.
- Develop reusable Bare-Metal drivers in Embedded C.
- Configure and control GPIO without relying on the STM32 HAL.
- Explore SPI configuration and data transmission.
- Connect peripheral behavior to the STM32 Reference Manual.
- Validate firmware behavior through practical hardware experiments.
- Build a structured Embedded C portfolio.

## Target Platform

| Component | Details |
|---|---|
| Microcontroller | STM32F446RE |
| MCU family | STM32F4 |
| CPU core | ARM Cortex-M4 |
| Programming language | Embedded C |
| Development environment | STM32CubeIDE |
| Build toolchain | GNU Arm Embedded Toolchain |
| Programming approach | Bare-Metal, register-level peripheral control |

## Implemented Drivers

### GPIO Driver

Source files:

- `drivers/Inc/stm32f446xx_gpio_driver.h`
- `drivers/Src/stm32f446xx_gpio_driver.c`

The GPIO driver provides a foundation for working with General-Purpose Input/Output peripherals through a custom driver interface.

The associated examples explore LED control, button input, and interrupt-driven GPIO operation.

Topics covered include:

- GPIO peripheral clock configuration.
- GPIO pin configuration and operating modes.
- Input and output operations.
- Pull-up and pull-down configuration.
- External interrupt-related experiments.

### SPI Driver

Source files:

- `drivers/Inc/stm32f446xx_spi_driver.h`
- `drivers/Src/stm32f446xx_spi_driver.c`

The SPI driver provides a foundation for configuring and using the STM32 SPI peripheral.

The examples explore SPI initialization, data transmission, and communication with an external SPI device.

Topics covered by the project include:

- SPI peripheral configuration.
- Master-mode transmission experiments.
- SPI communication with an Arduino Uno.
- Command-response communication experiments.
- Practical investigation of SPI signals and transfer behavior.

The exact feature set and implementation status of individual driver functions should be verified against the current source code.

## Repository Structure

```text
stm32f446xx_drivers/
├── drivers/
│   ├── Inc/
│   │   ├── stm32f446xx.h
│   │   ├── stm32f446xx_gpio_driver.h
│   │   └── stm32f446xx_spi_driver.h
│   └── Src/
│       ├── stm32f446xx_gpio_driver.c
│       └── stm32f446xx_spi_driver.c
├── Src/
│   ├── 000_TXS0108E_testing.c
│   ├── 001led_toggle.c
│   ├── 002Led_btn.c
│   ├── 003led_btn_ext.c
│   ├── 004led_btn_interrupt.c
│   ├── 005spi_tx_testing.c
│   ├── 006spi_tx_only_arduino.c
│   └── 007spi_cmd_response_arduino.c
├── Startup/
│   └── startup_stm32f446retx.s
├── STM32F446RETX_FLASH.ld
├── STM32F446RETX_RAM.ld
└── stm32f446xx_gpio_driver_MCU_1.launch
```

### Directory Overview

- **`drivers/Inc/`** — driver and MCU-related header files.
- **`drivers/Src/`** — driver implementation files.
- **`Src/`** — standalone firmware examples and hardware experiments.
- **`Startup/`** — startup assembly code.
- **Linker scripts** — memory layout definitions for FLASH and RAM.
- **STM32CubeIDE project files** — development environment configuration.

## Examples

| Source file | Purpose |
|---|---|
| `000_TXS0108E_testing.c` | Logic-level converter testing |
| `001led_toggle.c` | LED control and GPIO output |
| `002Led_btn.c` | LED and button interaction |
| `003led_btn_ext.c` | GPIO and external-interrupt-related experiment |
| `004led_btn_interrupt.c` | Interrupt-driven button and LED experiment |
| `005spi_tx_testing.c` | SPI transmission experiment |
| `006spi_tx_only_arduino.c` | SPI Master transmission to Arduino |
| `007spi_cmd_response_arduino.c` | SPI command-response communication experiment |

These files represent separate experiments rather than a single application. The active example and its build configuration determine which firmware is compiled.

## Hardware

The project targets the STM32F446RE microcontroller. Practical experiments may also use:

- Nucleo-F446RE development board.
- Arduino Uno configured as an SPI Slave.
- TXS0108E logic-level translator.
- LEDs, push buttons, resistors, and jumper wires.
- Logic analyzer for inspecting digital signals.

For the STM32-to-Arduino SPI experiments, the intended setup uses the STM32 as SPI Master and the Arduino Uno as SPI Slave. The electrical interface and signal wiring must match the configuration used by the corresponding firmware.

## Development Environment

The project is maintained in STM32CubeIDE and uses the GNU Arm toolchain. STM32CubeIDE provides the build, debug, and flashing workflow; the peripheral driver code is developed to explore direct hardware control rather than hiding peripheral configuration behind HAL APIs.

## Build and Run

1. Clone or download the repository.
2. Open the project in STM32CubeIDE.
3. Select the standalone example you want to build.
4. Check the source files included in the build configuration, especially if multiple examples define `main()`.
5. Build the project.
6. Connect the STM32F446RE board and flash the firmware.
7. Connect any external hardware according to the selected example.
8. Observe the behavior using the board, a multimeter, or a logic analyzer as appropriate.

Before running an example, verify its pin assignments, clock configuration, peripheral settings, and external wiring. Do not assume that every example uses the same hardware connections.

## Documentation and Learning Resources

The primary technical references for this project are:

- **STM32F446RE Reference Manual (RM0390)** — peripheral architecture, registers, and operating modes.
- **STM32F446RE Datasheet (DS10693)** — electrical characteristics, pin assignments, and device specifications.
- **ARM Cortex-M4 Programming Manual (PM0214)** — core programming model and processor-specific behavior.
- **STM32 Nucleo-64 Boards User Manual (UM1724)** — development board information.

The driver implementations and examples are intended to be studied alongside the relevant reference-manual sections.

## Current Status

This is an evolving Embedded C learning and portfolio project. Driver functionality, examples, documentation, and validation will be expanded incrementally.

The presence of a driver or example in the repository does not by itself imply complete peripheral coverage, exhaustive testing, or production readiness.

