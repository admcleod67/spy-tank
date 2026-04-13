# iSpy Tank Remote (C++ Qt)

A modern C++ implementation of a desktop controller for the iSpy Tank (also known as the Instant Spy Tank). This project provides a responsive GUI built with Qt 6 to drive the tank and tilt its camera using keyboard controls.

## Features
- **TCP/IP Communication**: Handles the tank's custom protocol over port `8150`.
- **Real-time Video**: Displays an MJPEG stream from the tank's camera (port `8196`).
- **Asynchronous Networking**: Uses `QTcpSocket` and `QNetworkAccessManager` to ensure the GUI remains responsive.
- **Keyboard Driving**: Intuitive controls using WASD or Arrow keys.
- **Camera Control**: Tilt the camera up and down using the Q and E keys.
- **Modern C++**: Developed with C++20 and Qt 6.

## Requirements
- **Qt 6**: (Core, Gui, Widgets, Network modules)
- **CMake 4.2+**
- **C++20** compatible compiler (GCC, Clang, or MSVC)

## Building the Application
1. Ensure Qt 6 is installed and in your PATH.
2. Clone the repository and navigate to the project directory.
3. Build using CMake:
   ```bash
   mkdir build && cd build
   cmake ..
   cmake --build .
   ```

## Usage
1. Power on your iSpy Tank and connect your computer to the tank's Wi-Fi network (usually named `iSpyTank` or similar).
2. Launch the application.
3. Enter the tank's IP (default: `10.10.1.1`) and Port (default: `8150`).
4. Click **Connect**.
5. Once connected, use the following keys to drive:
   - **Forward/Back**: `W` / `S` or `Up` / `Down` Arrow
   - **Steering**: `A` / `D` or `Left` / `Right` Arrow
   - **Camera Tilt**: `Q` (Up) / `E` (Down)

## Technical Details
The tank uses a simple 2-character command protocol after an initial `t1` handshake. 
- Motors are identified as `1` (Left), `2` (Right), and `3` (Camera).
- Directions are `1` (Forward), `2` (Back), and `0` (Stop).
Example: Sending `11` moves the left motor forward.