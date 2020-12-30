# sequence-miner
Compact tool for mining frequent sequential patterns using SPADE and PrefixSpan algorithms. Project for Data Mining Methods course.

# prerequisites
- GCC >= 9.3
- CMake >= 3.10
- [TBB](https://software.intel.com/content/www/us/en/develop/tools/oneapi/components/onetbb.html)

# installation
1. clone repository: `git@github.com:adamnapieralski/sequence-miner.git`
1. `cd sequence-miner`
1. update submodules: `git submodule update --init --recursive`

# build
build with ninja on Linux
```
mkdir build && cd build
cmake -GNinja ..
ninja
```
build with make on Linux
```
mkdir build && cd build
cmake ..
make
```

