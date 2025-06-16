# reki
reki is a GUI application capable of capturing and displaying network traffic.

# Building
## Dependencies (Ubuntu 24.04)
```bash
sudo apt-get install build-essential git make \
pkg-config cmake ninja-build gnome-desktop-testing libasound2-dev libpulse-dev \
libaudio-dev libjack-dev libsndio-dev libx11-dev libxext-dev \
libxrandr-dev libxcursor-dev libxfixes-dev libxi-dev libxss-dev libxtst-dev \
libxkbcommon-dev libdrm-dev libgbm-dev libgl1-mesa-dev libgles2-mesa-dev \
libegl1-mesa-dev libdbus-1-dev libibus-1.0-dev libudev-dev \
libfreetype-dev libcap-dev
```

## Submodules
If the repository was not cloned with `--recurse-submodules`, you can initialize them after the fact 
using `git submodule update --init`.

## Build steps
```bash
cmake -B build
cmake --build build -j$(nproc)
```
