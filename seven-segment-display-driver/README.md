# Seven-segment display driver

A GPIO-based seven-segment display driver and STM32CubeIDE example for the Black Pill STM32F411CEU6 board. A lookup table encodes digits 0–9, and a seven-element pin array maps segments `a` through `g` to GPIO outputs.

The current example repeatedly requests digit `0` with `COMMON_CATHODE` in `main.c`. The project author identifies the physical display as a 5011BS common-anode display, powered from the Black Pill’s 3.3 V supply. The selected enum does not match that hardware type; correct the polarity implementation and select `COMMON_ANNODE` together.

## Current status

The source currently has two known limitations:

- The polarity branches are reversed for direct GPIO drive: `COMMON_CATHODE` drives illuminated segments low, while `COMMON_ANNODE` drives them high. Common-cathode requires active-high outputs and common-anode requires active-low outputs for direct drive.
- The driver indexes the lookup table without checking the digit. Callers must pass only `0` through `9` and a valid array of seven pin entries.

The existing enum spells anode `COMMON_ANNODE`; examples preserve the actual API spelling. Decimal-point control and multi-digit multiplexing are not implemented. This repository setup has not independently verified a build or operation on hardware.

## Hardware

| Item | Details |
| --- | --- |
| Board | Black Pill |
| MCU | STM32F411CEU6 |
| Display | 5011BS single seven-segment display |
| Physical display type | Common-anode (identified by the project author) |
| Display supply | Black Pill 3.3 V |
| Current limiting | A shared 220 Ω resistor on the common-anode supply has been proposed by the author; installation is not confirmed. Separate segment resistors are recommended for consistent brightness. |
| Programming/debug connection | Compatible SWD probe; actual probe to be documented |

Confirm the display's physical pinout and common connection from its datasheet before using the wiring table. The table below records the mapping in the code, not a verified physical display pinout.

## Segment mapping

| Segment | MCU GPIO |
| --- | --- |
| a | PA1 |
| b | PA2 |
| c | PA3 |
| d | PA4 |
| e | PA5 |
| f | PA6 |
| g | PA7 |

The example configures PA1–PA7 as push-pull GPIO outputs. The decimal point is not connected by this driver.

## Source files

- [Core/Inc/seven_segment.h](Core/Inc/seven_segment.h): pin structure, display-type enum, and public function declaration.
- [Core/Src/seven_segment.c](Core/Src/seven_segment.c): digit lookup table and GPIO writes.
- [Core/Src/main.c](Core/Src/main.c): pin mapping, MCU initialization, and example call.
- `Drivers/`: bundled ST HAL and CMSIS dependencies, with their existing license files.

## API usage

Include the header and define the pins in segment order. Call the function after HAL, clock, and GPIO initialization:

```c
#include "seven_segment.h"

const seven_seg_pin pins[7] = {
    {GPIOA, GPIO_PIN_1},  // a
    {GPIOA, GPIO_PIN_2},  // b
    {GPIOA, GPIO_PIN_3},  // c
    {GPIOA, GPIO_PIN_4},  // d
    {GPIOA, GPIO_PIN_5},  // e
    {GPIOA, GPIO_PIN_6},  // f
    {GPIOA, GPIO_PIN_7}   // g
};

/* Current example call; see the polarity limitation above. */
seven_segment_set_digit(pins, 0, COMMON_CATHODE);
```

## Build and flash

1. Open STM32CubeIDE. Use **File > Import > General > Existing Projects into Workspace** and select this directory. If the project is already in Project Explorer, use that existing project.
2. Select the project and use **Project > Build Project**. The HAL/CMSIS sources are included, so regenerating them is not a prerequisite for the initial build.
3. Connect your board through a compatible SWD probe and configure its connection in CubeIDE's debug configuration.
4. Launch the debugger to program the board, then resume execution. Resolve the polarity limitation before expecting correct digit output on a directly connected display.

The workspace directory is named `workspace_1.19.0`; confirm the actual IDE version under **Help > About STM32CubeIDE**. The project’s `.ioc` records STM32CubeMX **6.15.0** and STM32CubeF4 firmware package **V1.28.3**.

The `.ioc` is retained for configuration changes. Use a compatible CubeMX/CubeIDE configuration workflow when regeneration is needed, then review the resulting changes. Opening the `.ioc` alone is not a build or a guarantee of dependency restoration.

## Licensing

The bundled vendor components retain their own licenses:

- [CMSIS license](Drivers/CMSIS/LICENSE.txt)
- [STM32F4 HAL license](Drivers/STM32F4xx_HAL_Driver/LICENSE.txt)

No license has yet been selected for the original seven-segment driver code.
