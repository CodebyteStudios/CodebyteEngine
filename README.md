# Codebyte Engine

The Codebyte Engine is a simple, yet fast, cross-platform game engine being written from scratch in C++ and OpenGL.
Currently the Codebyte Engine only runs on Linux, but will work on Mac OS very soon (Windows will come in the future).

### Running on Linux:

First, make sure you have the standard X11 and OpenGL development libraries installed, the ones needed for our engine can  be installed in one command using:

```
sudo apt-get install libx11-dev mesa-common-dev libglu1-mesa-dev libxrandr-dev libxi-dev
```

Now go ahead and clone the Codebyte Engine repo, once cloned, cd into the "tools" directory. Here you'll find two shell scripts. You'll need to run the script titled ***installDependenciesUnix.sh***. This will install all the dependencies required to compile the engine. Before running this you'll need to install the following tools:

* Make
* CMake
* GCC 4.8
* G++ 4.8
* Curl
* Git

NOTE: Also make sure to run the ***installDependenciesUnix.sh*** script from within a terminal, and that you're current working directory is tools! Unexpected results may occur if you don't.

Once all of thats done you should now be able to compile the engine! cd into the root of the repo and run "make", this should start the compilation process, resulting in an executable being created in the bin directory. You'll need to run the ***getGameResources.sh*** script in the tools directory before running the engine though (this script will download test resources for the engine to use)
