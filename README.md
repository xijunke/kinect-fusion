# kinect-fusion
A reimplementation of the original KinectFusion paper

# Local development
Clone the project and initialize submodules:

```sh
git clone --recurse-submodules git@github.com:shalen-original/kinect-fusion.git
```

**(Linux)** Install OpenCL for your graphics card (Google for instructions). After this, the libraries should be located in a location dependent on the OpenCL provider (for Intel it is `/usr/lib/x86_64-linux-gnu`). `clinfo` is a nice command line tool to check which OpenCL devices are detected. Install OpenGL with `apt install libglew-dev libglfw3-dev libfreeimage-dev`.

**(Mac)** For OpenCL there shouldn't be anything to do, maybe a couple of tweaks to the CMakeLists.txt. For OpenGL needs to install `libglew-dev` and `libglfw3-dev`. For FreeImage needs to install `libfreeimage-dev`.

**(Windows)** Need to install the OpenCL driver. For OpenGL, needs GLEW and GLFW, they have setup for Windows. Install Freeimage. TODO add more details here.

Before building and running the project, download the [test dataset](https://vision.in.tum.de/data/datasets/rgbd-dataset) in the `data` folder. After this step, the folder tree should look like:

```
.
├── src
├── submodules
├── data
│   └── rgbd_dataset_freiburg1_xyz
├── CMakeLists.txt
├── LICENSE
└── README.md
```

Move to the project folder and build the project:

```sh
cmake -S . -B build
cd build
make
```

The executable is in the build folder.
