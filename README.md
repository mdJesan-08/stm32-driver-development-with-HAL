# STM32 Driver Development with HAL

A growing collection of embedded systems projects and hardware drivers for STM32 microcontrollers. Each project folder contains an independent STM32CubeIDE project and its own hardware and usage documentation.

## Projects

| Project | Description | Hardware |
| --- | --- | --- |
| [Seven-segment display driver](seven-segment-display-driver/README.md) | GPIO-based digit lookup table and single-display example; see the project README for current limitations. | Black Pill, STM32F411CEU6 |

## Repository structure

```text
.
├── README.md
├── .gitignore
└── seven-segment-display-driver/
    ├── README.md
    ├── Core/           # Application, driver, and startup code
    ├── Drivers/        # ST HAL and CMSIS dependencies
    ├── .project
    ├── .cproject
    ├── *.ioc           # CubeMX configuration
    └── *.ld            # Linker scripts
```

New projects live alongside `seven-segment-display-driver/`, each with a README describing its purpose, hardware, wiring, usage, and build instructions. Git is initialized only at the collection root.

## Opening a project

In STM32CubeIDE, use **File > Import > General > Existing Projects into Workspace** and select the desired project directory. If it already appears in Project Explorer, open it there instead. Follow that project's README for setup and build details.

## Source and generated files

Application code, startup code, linker scripts, CubeMX configuration, CubeIDE project files, and bundled HAL/CMSIS dependencies are included. Generated `Debug/` and `Release/` directories, workspace `.metadata/`, and local debug launch files are ignored.

ST and Arm components retain their original copyright notices and license files. See the individual projects for the location of custom driver code. No repository-wide license has been selected for the original code yet.
