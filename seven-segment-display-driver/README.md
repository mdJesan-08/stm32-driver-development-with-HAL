<div align="center">

# Seven-Segment Display Driver

**A breadboard prototype that turns a decimal digit into seven GPIO outputs.**

STM32F411 Black Pill · 5011BS common-anode display · C · STM32 HAL

![MCU: STM32F411CEU6](https://img.shields.io/badge/MCU-STM32F411CEU6-03234B?style=flat-square)
![Language: C](https://img.shields.io/badge/language-C-00599C?style=flat-square)
![Interface: GPIO](https://img.shields.io/badge/interface-GPIO-28786A?style=flat-square)
![Stage: breadboard prototype](https://img.shields.io/badge/stage-breadboard_prototype-D99A28?style=flat-square)

[Prototype](#the-prototype) · [Start here](#what-i-wish-i-had-understood-before-wiring-it) · [Wiring](#wiring) · [How it works](#how-it-works) · [Usage](#usage) · [Build](#build-and-flash) · [Current limitations](#current-limitations)

</div>

<p align="center">
  <img src="../docs/images/breadboard-prototype.jpg" alt="Black Pill STM32 board wired to a red seven-segment display showing digit 0 on a breadboard, with an ST-LINK probe alongside it." width="480">
  <br>
  <sub>Working breadboard prototype: Black Pill STM32F411CEU6 and 5011BS common-anode display showing digit 0.</sub>
</p>

---

## The prototype

This project implements a single-digit seven-segment display on a breadboard using a **Black Pill STM32F411CEU6** and a **5011BS common-anode display**. The display is powered from the board's **3.3 V** supply. Seven GPIO outputs control the segments individually through STM32 HAL.

The driver separates digit patterns from the board's pin assignments: a lookup table describes which segments form each number, while an array of port/pin pairs describes where those segments are connected.

**Hardware status:** the prototype photograph shows the display illuminated as digit **0**, matching the digit requested by the checked-in example. A full 0–9 hardware test and measured segment currents have not yet been documented.

> **Implementation note:** the current polarity enum is inverted. The example selects `COMMON_CATHODE`, but that branch produces the active-low outputs used by this common-anode prototype. See [Current limitations](#current-limitations) before changing the display type.

## What I wish I had understood before wiring it

When I first started this project, I struggled with the documentation. I wanted to get a number on the display, but first I needed to understand what the pins meant and how the display type affected my code. I am keeping those basics here so you can follow the project without having to piece everything together yourself.

### 1. Read the segment letters before connecting wires

<p align="center">
  <img src="../docs/images/7-segment-display-pin-diagr_0.png" alt="Illustrative ten-pin seven-segment display: segments a through g, decimal point, and two common pins." width="460">
  <br>
  <sub>Reference pin diagram supplied for this guide. Check your own display's datasheet before using its physical pin numbers.</sub>
</p>

There are three different labels to keep track of: the **segment letter**, the **display's physical pin number**, and the **STM32 GPIO name**. They are not interchangeable. In my code, `a` connects to `PA1`; that does not mean segment `a` is physical pin 1 on the display.

Read the picture in the orientation shown. It labels the top bar `a`, the middle bar `g`, and the small dot `DP`. The two `com` labels identify the shared connection. This is a reference drawing, not a verified package-pin map for my particular 5011BS.

### 2. Common anode: the type used in my prototype

<p align="center">
  <img src="../docs/images/Common-Cathode-7-Segment-Di.png" alt="Common-anode display diagram showing the LED anodes joined to a shared positive connection." width="640">
  <br>
  <sub>Common-anode internal connections. The drawing is common anode despite the downloaded filename.</sub>
</p>

My display is a **5011BS common-anode** part. In the picture, the joined line is the shared positive side of the LEDs. For my 3.3 V setup, pulling a segment's other end LOW through a current-limited path allows that segment to light.

The useful rule is: **common anode → LOW turns a segment on**. A `1` in my lookup table means “this segment belongs to the digit”; it does not automatically mean “write HIGH to the GPIO.” The driver has to translate the pattern into the right electrical level.

### 3. Common cathode: why some examples look reversed

<p align="center">
  <img src="../docs/images/common-cathode-7-segment.png" alt="Common-cathode display diagram showing the LED cathodes joined to a shared ground connection." width="640">
  <br>
  <sub>Common-cathode internal connections. One schematic label reads “h”; this project's seven segments are a–g, with DP separate.</sub>
</p>

A common-cathode display shares the negative side instead. With that common connection at ground, a HIGH output supplies current to a segment through its resistor. So the rule becomes **common cathode → HIGH turns a segment on**.

These two circuit drawings explain the internal connections; they omit the external current-limiting resistors. They are not complete breadboard wiring instructions.

### 4. Connect the pictures to my breadboard

In the [prototype photo above](../docs/images/breadboard-prototype.jpg), you can see the Black Pill, the jumper wires, the illuminated display, and an ST-LINK probe beside the breadboard. The display is showing **0**: the outside segments are on and the middle segment is off. That is the same pattern stored in the first row of my lookup table.

Use the [wiring table below](#wiring) to follow each segment to its GPIO. Wire colours in the photo are just a visual aid; the segment-to-pin mapping is what matters.

### 5. Check one segment before trying a full digit

With power disconnected, check the display's orientation and common pins against its own datasheet. For a common-anode display, wire the common connection to the supply and one segment through a suitable resistor to a controlled LOW output. Test the segments individually, then connect all seven using the table below.

The 3.3 V supply does not replace current limiting. A resistor on each segment makes brightness more consistent than one resistor shared by the entire display. Also check the [known polarity issue](#current-limitations) in this version before selecting the display-type enum.

**Reference and diagram credit:** [Components101 — 7-Segment Display Pinout, Working and Datasheet](https://components101.com/displays/7-segment-display-pinout-working-datasheet). The three reference diagrams above were supplied for this documentation; they are separate from my own prototype photograph. Use the guide for background and your component's manufacturer datasheet for exact pinout and electrical ratings.

## Hardware at a glance

| Component | Prototype details |
| --- | --- |
| Development board | Black Pill |
| Microcontroller | STM32F411CEU6 |
| Display | 5011BS, single digit |
| Display type | Common-anode, identified by the author |
| Display supply | 3.3 V from the Black Pill |
| Assembly | Breadboard and jumper wires |
| Segment interface | PA1–PA7, configured as push-pull GPIO outputs |
| Current limiting | A shared 220 Ω resistor was proposed; final installed arrangement is not yet confirmed |

For repeatable brightness, use a separate current-limiting resistor for each segment. A resistor shared at the common anode limits total display current, so the brightness of each segment depends on how many segments are lit. The final resistor selection should account for the display's forward voltage and the MCU's current limits.

## Wiring

The segment names follow this layout:

```text
      a
     ───
  f │   │ b
     ───  g
  e │   │ c
     ───    · dp
      d
```

| Segment | GPIO | Pin-array index |
| :---: | :---: | :---: |
| a | PA1 | 0 |
| b | PA2 | 1 |
| c | PA3 | 2 |
| d | PA4 | 3 |
| e | PA5 | 4 |
| f | PA6 | 5 |
| g | PA7 | 6 |

The common-anode connection goes to the 3.3 V supply, with current limiting included in the circuit. For direct GPIO drive, a **LOW** segment output turns that segment **on**; a **HIGH** output turns it **off**. The decimal point is not controlled by this driver.

This table describes the mapping in [`main.c`](Core/Src/main.c). It does not assign physical display package-pin numbers; verify those against the datasheet for the actual component.

## How it works

```text
Decimal digit       Segment pattern       Board pin mapping       GPIO outputs
    0–9       →     a b c d e f g     →    seven_seg_pin[7]    →   HAL_GPIO_WritePin
```

1. **Look up the digit.** A constant `10 × 7` table stores the patterns for digits 0–9. Each `1` means that a segment belongs to the digit.
2. **Select the electrical level.** The display type determines the output polarity. The intended common-anode/common-cathode behavior is currently reversed in the implementation.
3. **Write the pins.** The driver loops through seven entries and calls `HAL_GPIO_WritePin()` for each segment.

For digit **0**, the pattern is `{1, 1, 1, 1, 1, 1, 0}`: segments **a–f** illuminate and **g** remains off.

The pin array allows segments to be assigned to different GPIO ports without changing the lookup table. The caller is responsible for enabling the GPIO clocks and configuring the selected pins as outputs. The driver writes one digit per call and contains no delay or multiplexing scheduler.

## Usage

Public API, declared in [`seven_segment.h`](Core/Inc/seven_segment.h):

```c
void seven_segment_set_digit(const seven_seg_pin *display,
                             uint8_t digit,
                             disp_type type);
```

| Argument | Requirement |
| --- | --- |
| `display` | Valid array of seven port/pin entries, in `a, b, c, d, e, f, g` order |
| `digit` | A value from `0` through `9`; currently not checked by the driver |
| `type` | `COMMON_CATHODE` or `COMMON_ANNODE`; see the polarity caveat below |

The existing example uses this mapping:

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

/* Call after HAL, clock, and GPIO initialization.
 * The current implementation uses this enum for active-low output.
 * It does not describe the physical display type correctly yet.
 */
seven_segment_set_digit(pins, 0, COMMON_CATHODE);
```

After correcting the polarity branches, the common-anode example should select `COMMON_ANNODE`. Changing the enum argument alone in the present implementation would invert the segment pattern.

## Build and flash

1. Clone the collection:

   ```bash
   git clone https://github.com/mdJesan-08/stm32-driver-development-with-HAL.git
   ```

2. In STM32CubeIDE, select **File → Import → General → Existing Projects into Workspace**. Choose the `seven-segment-display-driver` directory inside the clone. If this project is already open in your workspace, use that existing project.
3. Select the project and choose **Project → Build Project**. The repository includes the HAL/CMSIS sources, startup assembly, and linker scripts.
4. Connect a compatible SWD probe to the board and configure the probe in CubeIDE's debug settings.
5. Launch the debugger to program the board, then resume execution. With the documented common-anode wiring and present active-low example, the program requests digit **0** continuously.

<details>
<summary><strong>Configuration and tool versions</strong></summary>

The project's [`seven-segment-display-driver.ioc`](seven-segment-display-driver.ioc) records:

| Setting | Recorded value |
| --- | --- |
| Target MCU | STM32F411CEUx |
| STM32CubeMX | 6.15.0 |
| STM32CubeF4 firmware package | V1.28.3 |
| Target IDE | STM32CubeIDE |

The exact CubeIDE version has not been confirmed; the local workspace folder name alone is not evidence of the installed version. No fresh build was performed as part of this documentation update.

Use the `.ioc` for peripheral-configuration changes through a compatible CubeMX/CubeIDE workflow. Review generated changes before committing. Regeneration is not required simply to restore HAL/CMSIS sources because those dependencies are included.

</details>

## Source map

| File or folder | Responsibility |
| --- | --- |
| [`Core/Inc/seven_segment.h`](Core/Inc/seven_segment.h) | Driver interface, pin structure, and display-type enum |
| [`Core/Src/seven_segment.c`](Core/Src/seven_segment.c) | Digit patterns and segment output logic |
| [`Core/Src/main.c`](Core/Src/main.c) | Board initialization, pin mapping, and example call |
| [`Core/Startup/`](Core/Startup/) | MCU startup assembly |
| [`Drivers/`](Drivers/) | Bundled ST HAL and CMSIS dependencies |
| [`seven-segment-display-driver.ioc`](seven-segment-display-driver.ioc) | CubeMX configuration |
| `STM32F411CEUX_*.ld` | MCU memory layout and linker configuration |

The custom display driver is contained in `seven_segment.h` and `seven_segment.c`. The remaining project files provide the STM32 application and build environment.

## Current limitations

| Area | Current behavior | Follow-up |
| --- | --- | --- |
| Display polarity | Enum names select the opposite of their intended electrical behavior | Correct the branches and update the common-anode example together |
| Digit validation | Values above `9` index outside the lookup table | Add a defined out-of-range policy |
| Input pointers | The driver assumes a valid seven-entry pin array | Document or add input validation |
| API spelling | Common anode is spelled `COMMON_ANNODE` | Rename consistently when updating the interface |
| Display scope | One digit, segments a–g | Decimal point and multiplexing are not implemented |
| Hardware evidence | Prototype photograph showing digit 0 | Add a 0–9 demonstration and current measurements |

## Author and licensing

Created by **[MD. JESAN](https://github.com/mdJesan-08)** as part of [STM32 Driver Development with HAL](../README.md).

A license has not yet been selected for the original driver code. Bundled dependencies retain their existing notices and licenses: [CMSIS](Drivers/CMSIS/LICENSE.txt), [STM32 device support](Drivers/CMSIS/Device/ST/STM32F4xx/LICENSE.txt), and [STM32F4 HAL](Drivers/STM32F4xx_HAL_Driver/LICENSE.txt).
