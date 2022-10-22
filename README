# hwmx

This is my implementation of hwmx homework task.

For license notes see LICENSE.

### Prerequirements
- C++ compiler that supports c++20 standard, g++ 12.2.0 is ok
- `cmake`, tested with 3.24.2
- `gtest` library for unit tests (optionally, if you don't need them)
- ruby (optionally, for build and test scripts)

### Build and install

Suppose you are in repo root directory.

###### Manual

``` sh
mkdir build
cd build
cmake ..
cmake --build .
ctest
DESTDIR=../install cmake --install . --prefix /
```

###### Using script

``` sh
./tools/build.rb [-v | --verbose]
```

This script will build hwmx in `build`, run unit tests and install in `install`.
`-v` or `--verbose` enables verbose output. Requires ruby.

### Testing

``` sh
./tools/runtest.rb [-v | --verbose]
```

Runs end-to-end testing. `-v` or `--verbose` enables verbose output. Requires
ruby.
