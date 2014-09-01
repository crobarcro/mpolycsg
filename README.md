mpolycsg
========

Matlab/Octave m-code interface to computational solid geometry libraries based on pyPolyCSG

A C++ wrapper for the Carve Constructive Solid Geometry (CSG) library, and more experimentally, the CGAL library, intended to simplify performing CSG operations on meshes. The wrapper depends on the pyPolyCSG project by James Gregson. Currently a forked version of the pyPolyCSG project is required (available: https://github.com/crobarcro/pyPolyCSG), but it is likely the changes in the forked version of the project will be merged back into the main project in the future.

Building
--------

You will need to grab and build my fork of pyPolyCSG, although we don't actually need the entire project, just the libpolyhcsg subproject


    git clone https://github.com/crobarcro/pyPolyCSG.git pyPolyCSG-code
    cd pyPolyCSG-code/libpolyhcsg
    cmake -DCMAKE_BUILD_TYPE=Release .
    make
    sudo make install

Next get the mpolycsg project

    git clone https://github.com/crobarcro/mpolycsg.git mpolycsg

Then start Matlab or Octave, change directory to the top level directory of mpolyhcsg, and run:

    mpolyhcsgsetup
    
Look at the file test_csg.m for some example uses.


