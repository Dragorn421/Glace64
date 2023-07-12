# Glace64

The repo for The Scoop Slingers' entry in the N64Brew Game jam for Summer 2023

(Glace64 is only a temporary name)

# Install

Clone the repo and its submodules:

```
git clone --recurse-submodules ...
```

Or if you already cloned:

```
git clone ...
cd ...
git submodule update --init --recursive
```

Build and install libdragon.

Libdragon is installed into the repository folder. If you don't have the N64_GCCPREFIX environment variable set, it will default to N64_INST and N64_INST will be set to the local install path. The Makefile works similarly.

```
./build_libdragon_install.sh -j7
```

Build the rom:

```
make -j7
```
