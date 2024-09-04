# Cross-compilation
Building on Raspberry Pi is sloooow, so it's better to cross-compile on a faster machine, especially if you're going to play with the code.
I followed this tutorial in order to set up cross-compilation environment: https://tttapa.github.io/Pages/Raspberry-Pi/C++-Development-RPiOS/index.html

## Set up the toolchain (Raspberry Pi 0)

### Create opt in home di
```bash
mkdir -p ~/opt
```

### Download the toolchain and extract it to ~/opt
```bash
wget -qO- https://github.com/tttapa/docker-arm-cross-toolchain/releases/download/0.2.0/x-tools-armv6-rpi-linux-gnueabihf-gcc14.tar.xz | tar xJ -C ~/opt;
```
Note that the release can be deleted, try to find the latest release on the release page

### Add the toolchain to the PATH
```bash
echo 'export PATH="$HOME/opt/x-tools/armv6-rpi-linux-gnueabihf/bin:$PATH"' >>~/.zshrc # or ~/.bashrc
source ~/.zshrc # or ~/.bashrc
```

### Test the toolchain
```bash
armv6-rpi-linux-gnueabihf-g++ --version
```