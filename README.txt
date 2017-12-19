Welcome to the RoadXML project
http://www.road-xml.org

RoadXML depends on Xercesc 2.8.0.
Files can be found at http://xerces.apache.org/xerces-c/download.cgi
Please download and install binaries before trying to compile RoadXML

How to build RoadXML
===============================

The RoadXML project uses the CMake build system to generate a 
platform-specific build environment.  CMake reads the CMakeLists.txt 
files that you'll find throughout the RoadXML directories, 
checks for installed dependencies and then generates the appropriate 
build system.

If you don't already have CMake installed on your system you can grab 
it from http://www.cmake.org, use version 3.1.1 or later.  


Under Unices
===============================

Under unices (i.e. Linux, IRIX, Solaris, Free-BSD, HP-Ux, AIX, OSX) 
use the cmake or ccmake command-line utils, or use the included tiny 
configure script that'll run cmake for you.  The configure script 
simply runs 'cmake . -DCMAKE_BUILD_TYPE=Release' to ensure that you 
get the best performance from your final libraries/applications.
 
    cd RoadXML
    ./configure
    make
  
Alternatively, you can create an out-of-source build directory and run 
cmake or ccmake from there. The advantage to this approach is that the 
temporary files created by CMake won't clutter the RoadXML 
source directory, and also makes it possible to have multiple 
independent build targets by creating multiple build directories. In a 
directory alongside the RoadXML use:

    mkdir build
    cd build
    cmake ../RoadXML -DCMAKE_BUILD_TYPE=Release
    make

Under Windows
==============================	

Under Windows use the GUI tool CMakeSetup to build your VisualStudio 
files. In CMake user interface, set the "Where is the source code" item
to the RoadXML root. For example "C:/roadxml/trunk". Set the build 
folder to the one of your choice. 
Then clic Configure and follow the instructions to select your IDE version 
target and other options.
Finally clic Generate. Your Project Files are now ready. Open them in 
your usual IDE (i.e Visual Studio) and Compile the project.

