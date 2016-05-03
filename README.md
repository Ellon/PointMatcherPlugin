PointMatcherPlugin
==================

#### **WARNING: This is a WIP, thus there is no stable release yet.**

PointMatcherPlugin is a plugin that allows to configure libpointmatcher's ICP
pipeline from ParaView interface, while allowing a direct visualization of the
data transformed by each element of the pipeline. 

## Build Instructions

To proper build this plugin it is necessary to follow three steps:
- Build ParaView
- Build and install libpointmatcher
- Build PointMatcherPlugin

### Build ParaView

ParaView source code can be downloaded from [here](https://github.com/Kitware/ParaView) using:
```
cd ~/  #or any other directory to keep the sources
git clone --recursive git@github.com:Kitware/ParaView.git
```
The `--recursive` option will make sure the sources of external projects
needed to build paraview (e.g. VTK) are also downloaded.

Now select a version of ParaView and VTK to build. The plugin was developed using ParaView 5.0.1 and VTK 7.0.0, so using these versions should be good.
```
cd ~/ParaView
git checkout v5.0.1
cd ~/ParaView/VTK
git checkout v7.0.0
```

Now build Paraview. If you want you can use `ccmake .` to configure some build
flags, like `CMAKE_INSTALL_PREFIX` and `PARAVIEW_ENABLE_PYTHON`. *Note that
the build process may take a while.*
```
cd ~/ParaView
mkdir build && cd build
cmake ../
ccmake .  #optional
make
```

If you want to install it, configure the `CMAKE_INSTALL_PREFIX` flag and install with: 
```
ccmake .      #to configure CMAKE_INSTALL_PREFIX if not done already
make install  #or sudo make install
```

### Build libpointmatcher

You can download libpointmatcher using:
```
cd ~/
git clone git@github.com:ethz-asl/libpointmatcher.git
cd libpointmatcher
```

To build and install it you should follow the instructions on [libpointmatcher
repository page](https://github.com/ethz-asl/libpointmatcher).

### Build PointMatcherPlugin

To build the plugin, you should inform the build directory of
ParaView. This can be done with:
```
cd ~/
git clone git@github.com:Ellon/PointMatcherPlugin.git
cd PointMatcherPlugin
mkdir build && cd build
cmake -DParaView_DIR=~/ParaView/build ../
```

Now you should have a shared library in the build directory
(`libvtkPMFilters.so` on Linux). This is the file that should be loaded on
ParaView to use this plugin.

## Using the plugin

PointMatcherPlugin can be loaded through `Tools > Manage Plugins...` menu. On
the plugins window click on `Load New...` button and select the shared library
generated when you built the plugin. You might also want to enable Auto Load
so that the plugin gets automatically loaded.

The libpointmatcher filters are available from `Filters > Point Matcher` menu.
You should follow the normal ParaView workflow to build the pipeline. If you
don't know how to use ParaView yet, the [ParaView Guide](http://www.paraview.org/paraview-guide/) is a good place to start.

## Bug reporting

Please use [github's issue
tracker](https://github.com/Ellon/PointMatcherPlugin/issues) to report bugs.