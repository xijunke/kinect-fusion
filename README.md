# kinect-fusion
A reimplementation of the original KinectFusion paper

# Local development
Clone the project and initialize submodules:

```sh
git clone --recurse-submodules git@github.com:shalen-original/kinect-fusion.git
```

**(Linux)** Install OpenCL for your graphics card (Google for instructions). After this, the libraries should be located in a location dependent on the OpenCL provider (for Intel it is `/usr/lib/x86_64-linux-gnu`). `clinfo` is a nice command line tool to check which OpenCL devices are detected.

**(Mac)** Everything should already be ready.

**(Windows)** TODO.

Move to the project folder and build the project:

```sh
cmake -S . -B build
cd build
make
```

The executable is in the build folder.
