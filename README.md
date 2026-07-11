# embedDIP

Source repository: https://github.com/embeddip/embedDIP

embedDIP is a portable digital image processing library for embedded systems. It is written in C and C++ and is intended for microcontroller projects that need image data structures, image processing routines, and board-specific peripheral support.

The library is used by the example projects for the book *Embedded Digital Image Processing with Microcontrollers*.

## Contents

- `core/`: core image, error, and memory management interfaces.
- `imgproc/`: image processing modules, including filtering, thresholding, morphology, color conversion, histograms, segmentation, drawing, compression, and FFT support.
- `device/`: common camera, display, and serial interfaces.
- `board/`: board profiles for supported targets.
- `arch/`: architecture-specific implementations.
- `wrapper/`: C++ wrapper classes.
- `docs/`: documentation configuration and support files.

## Build

embedDIP is built with CMake. Select the target board, architecture, and CPU through CMake cache variables.

Example for STM32F7:

```bash
cmake -S . -B build -DEMBEDDIP_TARGET_BOARD=STM32F7 -DEMBEDDIP_ARCH=ARM -DEMBEDDIP_CPU=CORTEX_M7
cmake --build build
```

Example for ESP32:

```bash
cmake -S . -B build -DEMBEDDIP_TARGET_BOARD=ESP32 -DEMBEDDIP_ARCH=XTENSA -DEMBEDDIP_CPU=LX6
cmake --build build
```

The library can also be included from another CMake project with `add_subdirectory(embedDIP)`.

## Related Repositories

- ESP32 examples: https://github.com/embeddip/examples-esp32
- STM32 examples: https://github.com/embeddip/examples-stm32
- Python UART transfer tool: https://github.com/embeddip/PyDIPLink

## License

This project is licensed under the MIT License. See [LICENSE](LICENSE) for details.

## Author

Developed by Ozan Durgut for embedded systems and digital image processing examples.
