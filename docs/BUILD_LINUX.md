# Building on Linux

This guide covers building spy-tank on Debian-based Linux distributions, including **Raspberry Pi OS**. The steps assume a desktop environment for running the GUI.

## Requirements

- **CMake** 3.16 or newer
- **C++20** compiler (GCC 10+ or Clang 10+)
- **Qt 6** development packages: Core, Gui, Widgets, Network, Test, Quick, and Multimedia

All Qt modules are required at configure time because the project builds both the desktop and mobile targets. On a Raspberry Pi you will typically run the desktop app (`spy_tank`).

## Raspberry Pi OS

Use **Raspberry Pi OS (64-bit) with Bookworm or newer**. Qt 6 is available from the system package manager on Bookworm; older releases may not have all required Qt 6 packages.

### Install dependencies

```bash
sudo apt update
sudo apt install -y \
  build-essential \
  cmake \
  qt6-base-dev \
  qt6-declarative-dev \
  qt6-multimedia-dev
```

`qt6-base-dev` provides Core, Gui, Widgets, Network, and Test. `qt6-declarative-dev` provides Quick (QML). `qt6-multimedia-dev` provides Multimedia.

### Build

From the project root:

```bash
mkdir -p build && cd build
cmake ..
cmake --build . -j"$(nproc)"
```

On Pis with limited RAM (1 GB or less), reduce parallelism to avoid out-of-memory errors:

```bash
cmake --build . -j2
```

### Run the desktop app

```bash
./src/desktop/spy_tank
```

### Build and run tests

```bash
cmake --build . --target test_spy_tank test_video_receiver
./tests/test_spy_tank
./tests/test_video_receiver
```

## Other Debian-based distributions

Ubuntu, Debian, and Linux Mint follow the same package names and build steps as Raspberry Pi OS above.

## Troubleshooting

### `Could NOT find Qt6`

Install the Qt 6 development packages listed above. If CMake still cannot find Qt, point it at your Qt installation:

```bash
cmake .. -DCMAKE_PREFIX_PATH=/usr/lib/aarch64-linux-gnu/cmake/Qt6
```

Adjust the path for your architecture (for example `/usr/lib/x86_64-linux-gnu/cmake/Qt6` on amd64).

### CMake version too old

This project requires CMake 3.16+. Check your version with `cmake --version`. On older systems, install a newer `cmake` package or use the [Kitware APT repository](https://apt.kitware.com/).

### Compiler does not support C++20

Ensure `build-essential` is installed and your GCC version is 10 or newer (`g++ --version`). On very old systems you may need a newer compiler from backports.

### Build runs out of memory

Lower the parallel job count (for example `-j1` or `-j2`) or add swap space before building on memory-constrained Pis.

### GUI does not appear

The desktop app requires a running display server (Wayland or X11). Running over SSH without display forwarding, or on a headless Pi without a desktop session, will not show the window.
